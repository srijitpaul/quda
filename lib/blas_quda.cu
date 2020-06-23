#include <stdlib.h>
#include <stdio.h>
#include <cstring> // needed for memset

#include <tune_quda.h>
#include <quda_internal.h>
#include <blas_quda.h>
#include <color_spinor_field.h>

#include <jitify_helper.cuh>
#ifndef DPCPP_DEVELOP
#include <kernels/blas_core.cuh>
#else
#include "blas_core.cuh"
#endif

namespace quda {

  namespace blas {

    unsigned long long flops;
    unsigned long long bytes;

    static qudaStream_t *blasStream;

    template <template <typename real> class Functor, typename store_t, typename y_store_t,
              int nSpin, typename coeff_t>
    class Blas : public Tunable
    {
      using real = typename mapper<y_store_t>::type;
      Functor<real> f;
      const int nParity; // for composite fields this includes the number of composites

      const coeff_t &a, &b, &c;
      ColorSpinorField &x, &y, &z, &w, &v;
      const QudaFieldLocation location;

      unsigned int sharedBytesPerThread() const { return 0; }
      unsigned int sharedBytesPerBlock(const TuneParam &param) const { return 0; }

      bool tuneSharedBytes() const { return false; }

      // for these streaming kernels, there is no need to tune the grid size, just use max
      unsigned int minGridSize() const { return maxGridSize(); }

    public:
      Blas(const coeff_t &a, const coeff_t &b, const coeff_t &c, ColorSpinorField &x,
           ColorSpinorField &y, ColorSpinorField &z, ColorSpinorField &w, ColorSpinorField &v) :
        f(a, b, c),
        nParity((x.IsComposite() ? x.CompositeDim() : 1) * x.SiteSubset()),
        a(a),
        b(b),
        c(c),
        x(x),
        y(y),
        z(z),
        w(w),
        v(v),
        location(checkLocation(x, y, z, w, v))
      {
        checkLength(x, y, z, w, v);
        auto x_prec = checkPrecision(x, z, w);
        auto y_prec = checkPrecision(y, v);
        auto x_order = checkOrder(x, z, w);
        auto y_order = checkOrder(y, v);
        if (x_prec == y_prec && x_order != y_order) errorQuda("Orders %d %d do not match", x_order, y_order);

        strcpy(aux, x.AuxString());
        if (x.Precision() != y.Precision()) {
          strcat(aux, ",");
          strcat(aux, y.AuxString());
        }
        if (location == QUDA_CPU_FIELD_LOCATION) strcat(aux, ",CPU");

#ifdef JITIFY
        ::quda::create_jitify_program("kernels/blas_core.cuh");
#endif

        apply(*blasStream);
        checkCudaError();

        blas::bytes += bytes();
        blas::flops += flops();
      }

      TuneKey tuneKey() const { return TuneKey(x.VolString(), typeid(f).name(), aux); }

      void apply(const qudaStream_t &stream)
      {
        constexpr bool site_unroll = !std::is_same<store_t, y_store_t>::value || isFixed<store_t>::value;
        if (site_unroll && (x.Ncolor() != 3 || x.Nspin() == 2))
          errorQuda("site unroll not supported for nSpin = %d nColor = %d", x.Nspin(), x.Ncolor());

        TuneParam tp = tuneLaunch(*this, getTuning(), getVerbosity());
        if (location == QUDA_CUDA_FIELD_LOCATION) {
          if (site_unroll) checkNative(x, y, z, w, v); // require native order when using site_unroll
          using device_store_t = typename device_type_mapper<store_t>::type;
          using device_y_store_t = typename device_type_mapper<y_store_t>::type;
          using device_real_t = typename mapper<device_y_store_t>::type;
          Functor<device_real_t> f_(a, b, c);

          constexpr int N = n_vector<device_store_t, true, nSpin, site_unroll>();
          constexpr int Ny = n_vector<device_y_store_t, true, nSpin, site_unroll>();
          constexpr int M = site_unroll ? (nSpin == 4 ? 24 : 6) : N; // real numbers per thread
          const int length = x.Length() / (nParity * M);

          BlasArg<device_store_t, N, device_y_store_t, Ny, decltype(f_)> arg(x, y, z, w, v, f_, length, nParity);
#ifdef JITIFY
          using namespace jitify::reflection;
          jitify_error = program->kernel("quda::blas::blasKernel")
            .instantiate(Type<device_real_t>(), M, Type<decltype(arg)>())
            .configure(tp.grid, tp.block, tp.shared_bytes, stream)
            .launch(arg);
#else
          blasKernel<device_real_t, M><<<tp.grid, tp.block, tp.shared_bytes, stream>>>(arg);
#endif
        } else {
          if (checkOrder(x, y, z, w, v) != QUDA_SPACE_SPIN_COLOR_FIELD_ORDER)
            errorQuda("CPU Blas functions expect AoS field order");

          using host_store_t = typename host_type_mapper<store_t>::type;
          using host_y_store_t = typename host_type_mapper<y_store_t>::type;
          using host_real_t = typename mapper<host_y_store_t>::type;
          Functor<host_real_t> f_(a, b, c);

          // if site unrolling then we need full AoS ordering
          constexpr int N = n_vector<host_store_t, false, nSpin, site_unroll>();
          constexpr int Ny = n_vector<host_y_store_t, false, nSpin, site_unroll>();
          constexpr int M = N;
          const int length = x.Length() / (nParity * M);

          BlasArg<host_store_t, N, host_y_store_t, Ny, decltype(f_)> arg(x, y, z, w, v, f_, length, nParity);
          blasCPU<host_real_t, M>(arg);
        }
      }

      void preTune()
      {
        if (f.write.X) x.backup();
        if (f.write.Y) y.backup();
        if (f.write.Z) z.backup();
        if (f.write.W) w.backup();
        if (f.write.V) v.backup();
      }

      void postTune()
      {
        if (f.write.X) x.restore();
        if (f.write.Y) y.restore();
        if (f.write.Z) z.restore();
        if (f.write.W) w.restore();
        if (f.write.V) v.restore();
      }

      bool advanceTuneParam(TuneParam &param) const
      {
        return location == QUDA_CPU_FIELD_LOCATION ? false : Tunable::advanceTuneParam(param);
      }

      void initTuneParam(TuneParam &param) const
      {
        Tunable::initTuneParam(param);
        param.grid.y = nParity;
      }

      void defaultTuneParam(TuneParam &param) const
      {
        Tunable::initTuneParam(param);
        param.grid.y = nParity;
      }

      long long flops() const { return f.flops() * x.Length(); }
      long long bytes() const
      {
        // the factor two here assumes we are reading and writing to the high precision vector
        // this will evaluate correctly for non-mixed kernels since the +2/-2 will cancel out
        return (f.streams() - 2) * x.Bytes() + 2 * y.Bytes();
      }
      int tuningIter() const { return 3; }
    };

    void zero(ColorSpinorField &a) {
      if (typeid(a) == typeid(cudaColorSpinorField)) {
	static_cast<cudaColorSpinorField&>(a).zero();
      } else {
	static_cast<cpuColorSpinorField&>(a).zero();
      }
    }

    void initReduce();
    void endReduce();

    void init()
    {
      blasStream = &streams[Nstream-1];
      initReduce();
    }

    void end(void)
    {
      endReduce();
    }

    qudaStream_t* getStream() { return blasStream; }

    void axpbyz(double a, ColorSpinorField &x, double b, ColorSpinorField &y, ColorSpinorField &z)
    {
      instantiate<axpbyz_, Blas, true>(a, b, 0.0, x, y, x, x, z);
    }

    void ax(double a, ColorSpinorField &x)
    {
      instantiate<ax_, Blas, false>(a, 0.0, 0.0, x, x, x, x, x);
    }

    void caxpy(const Complex &a, ColorSpinorField &x, ColorSpinorField &y)
    {
      instantiate<caxpy_, Blas, true>(a, Complex(0.0), Complex(0.0), x, y, x, x, y);
    }

    void caxpby(const Complex &a, ColorSpinorField &x, const Complex &b, ColorSpinorField &y)
    {
      instantiate<caxpby_, Blas, false>(a, b, Complex(0.0), x, y, x, x, y);
    }

    void caxpbypczw(const Complex &a, ColorSpinorField &x, const Complex &b, ColorSpinorField &y, const Complex &c,
                    ColorSpinorField &z, ColorSpinorField &w)
    {
      instantiate<caxpbypczw_, Blas, false>(a, b, c, x, y, z, w, y);
    }

    void cxpaypbz(ColorSpinorField &x, const Complex &a, ColorSpinorField &y, const Complex &b, ColorSpinorField &z)
    {
      instantiate<caxpbypczw_, Blas, false>(Complex(1.0), a, b, x, y, z, z, y);
    }

    void axpyBzpcx(double a, ColorSpinorField& x, ColorSpinorField& y, double b, ColorSpinorField& z, double c)
    {
      instantiate<axpyBzpcx_, Blas, true>(a, b, c, x, y, z, x, y);
    }

    void axpyZpbx(double a, ColorSpinorField& x, ColorSpinorField& y, ColorSpinorField& z, double b)
    {
      instantiate<axpyZpbx_, Blas, true>(a, b, 0.0, x, y, z, x, y);
    }

    void caxpyBzpx(const Complex &a, ColorSpinorField &x, ColorSpinorField &y, const Complex &b, ColorSpinorField &z)
    {
      instantiate<caxpyBzpx_, Blas, true>(a, b, Complex(0.0), x, y, z, x, y);
    }

    void caxpyBxpz(const Complex &a, ColorSpinorField &x, ColorSpinorField &y, const Complex &b, ColorSpinorField &z)
    {
      instantiate<caxpyBxpz_, Blas, true>(a, b, Complex(0.0), x, y, z, x, y);
    }

    void caxpbypzYmbw(const Complex &a, ColorSpinorField &x, const Complex &b, ColorSpinorField &y, ColorSpinorField &z, ColorSpinorField &w)
    {
      instantiate<caxpbypzYmbw_, Blas, false>(a, b, Complex(0.0), x, y, z, w, y);
    }

    void cabxpyAx(double a, const Complex &b, ColorSpinorField &x, ColorSpinorField &y)
    {
      instantiate<cabxpyAx_, Blas, false>(Complex(a), b, Complex(0.0), x, y, x, x, y);
    }

    void caxpyXmaz(const Complex &a, ColorSpinorField &x, ColorSpinorField &y, ColorSpinorField &z)
    {
      instantiate<caxpyxmaz_, Blas, false>(a, Complex(0.0), Complex(0.0), x, y, z, x, y);
    }

    void caxpyXmazMR(const double &a, ColorSpinorField &x, ColorSpinorField &y, ColorSpinorField &z)
    {
      if (!commAsyncReduction())
	errorQuda("This kernel requires asynchronous reductions to be set");
      if (x.Location() == QUDA_CPU_FIELD_LOCATION)
	errorQuda("This kernel cannot be run on CPU fields");
      instantiate<caxpyxmazMR_, Blas, false>(a, 0.0, 0.0, x, y, z, y, y);
    }

    void tripleCGUpdate(double a, double b, ColorSpinorField &x, ColorSpinorField &y, ColorSpinorField &z, ColorSpinorField &w)
    {
      instantiate<tripleCGUpdate_, Blas, true>(a, b, 0.0, x, y, z, w, y);
    }

  } // namespace blas

} // namespace quda

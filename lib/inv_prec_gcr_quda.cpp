#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <complex>

#include <quda_internal.h>
#include <blas_quda.h>
#include <dslash_quda.h>
#include <invert_quda.h>
#include <util_quda.h>

#include<face_quda.h>

#include <color_spinor_field.h>

#include <domain_decomposition.h>
#include <resize_quda.h>

#include <sys/time.h>

namespace quda {

  static struct timeval orth0, orth1, pre0, pre1, mat0, mat1, rst0, rst1;

  static double timeInterval(struct timeval start, struct timeval end) {
    long ds = end.tv_sec - start.tv_sec;
    long dus = end.tv_usec - start.tv_usec;
    return ds + 0.000001*dus;
  }

  // set the required parameters for the inner solver
  static void fillInnerInvertParam(QudaInvertParam &inner, const QudaInvertParam &outer) {
    inner.tol = outer.tol_precondition;
    inner.maxiter = outer.maxiter_precondition;
    inner.reliable_delta = 1e-20; // no reliable updates within the inner solver

    inner.cuda_prec = outer.cuda_prec_precondition; // preconditioners are uni-precision solvers
    inner.cuda_prec_sloppy = outer.cuda_prec_precondition;

    inner.verbosity = outer.verbosity_precondition;

    inner.iter = 0;
    inner.gflops = 0;
    inner.secs = 0;

    inner.inv_type_precondition = QUDA_GCR_INVERTER; // used to tell the inner solver it is an inner solver

    if (outer.inv_type == QUDA_GCR_INVERTER && outer.cuda_prec_sloppy != outer.cuda_prec_precondition) 
      inner.preserve_source = QUDA_PRESERVE_SOURCE_NO;
    else inner.preserve_source = QUDA_PRESERVE_SOURCE_YES;
  }

  static void orthoDir(quda::Complex **beta, cudaColorSpinorField *Ap[], int k) {
    gettimeofday(&orth0, NULL);

    int type = 1;

    switch (type) {
      case 0: // no kernel fusion
        for (int i=0; i<k; i++) { // 5 (k-1) memory transactions here
          beta[i][k] = cDotProductCuda(*Ap[i], *Ap[k]);
          caxpyCuda(-beta[i][k], *Ap[i], *Ap[k]);
        }
        break;
      case 1: // basic kernel fusion
        if (k==0) break;
        beta[0][k] = cDotProductCuda(*Ap[0], *Ap[k]);
        for (int i=0; i<k-1; i++) { // 4 (k-1) memory transactions here
          beta[i+1][k] = caxpyDotzyCuda(-beta[i][k], *Ap[i], *Ap[k], *Ap[i+1]);
        }
        caxpyCuda(-beta[k-1][k], *Ap[k-1], *Ap[k]);
        break;
      case 2: // 
        for (int i=0; i<k-2; i+=3) { // 5 (k-1) memory transactions here
          for (int j=i; j<i+3; j++) beta[j][k] = cDotProductCuda(*Ap[j], *Ap[k]);
          caxpbypczpwCuda(-beta[i][k], *Ap[i], -beta[i+1][k], *Ap[i+1], -beta[i+2][k], *Ap[i+2], *Ap[k]);
        }

        if (k%3 != 0) { // need to update the remainder
          if ((k - 3*(k/3)) % 2 == 0) {
            beta[k-2][k] = cDotProductCuda(*Ap[k-2], *Ap[k]);
            beta[k-1][k] = cDotProductCuda(*Ap[k-1], *Ap[k]);
            caxpbypzCuda(beta[k-2][k], *Ap[k-2], beta[k-1][k], *Ap[k-1], *Ap[k]);
          } else {
            beta[k-1][k] = cDotProductCuda(*Ap[k-1], *Ap[k]);
            caxpyCuda(beta[k-1][k], *Ap[k-1], *Ap[k]);
          }
        }

        break;
      case 3:
        for (int i=0; i<k-1; i+=2) {
          for (int j=i; j<i+2; j++) beta[j][k] = cDotProductCuda(*Ap[j], *Ap[k]);
          caxpbypzCuda(-beta[i][k], *Ap[i], -beta[i+1][k], *Ap[i+1], *Ap[k]);
        }

        if (k%2 != 0) { // need to update the remainder
          beta[k-1][k] = cDotProductCuda(*Ap[k-1], *Ap[k]);
          caxpyCuda(beta[k-1][k], *Ap[k-1], *Ap[k]);
        }
        break;
      default:
        errorQuda("Orthogonalization type not defined");
    }

    gettimeofday(&orth1, NULL);
  }   

  static void backSubs(const quda::Complex *alpha, quda::Complex** const beta, const double *gamma, quda::Complex *delta, int n) {
    for (int k=n-1; k>=0;k--) {
      delta[k] = alpha[k];
      for (int j=k+1;j<n; j++) {
        delta[k] -= beta[k][j]*delta[j];
      }
      delta[k] /= gamma[k];
    }
  }

  static void updateSolution(cudaColorSpinorField &x, const quda::Complex *alpha, quda::Complex** const beta, 
      double *gamma, int k, cudaColorSpinorField *p[]) {

    quda::Complex *delta = new quda::Complex[k];

    // Update the solution vector
    backSubs(alpha, beta, gamma, delta, k);

    //for (int i=0; i<k; i++) caxpyCuda(delta[i], *p[i], x);

    for (int i=0; i<k-2; i+=3) 
      caxpbypczpwCuda(delta[i], *p[i], delta[i+1], *p[i+1], delta[i+2], *p[i+2], x); 

    if (k%3 != 0) { // need to update the remainder
      if ((k - 3*(k/3)) % 2 == 0) caxpbypzCuda(delta[k-2], *p[k-2], delta[k-1], *p[k-1], x);
      else caxpyCuda(delta[k-1], *p[k-1], x);
    }

    delete []delta;
  }

  PreconGCR::PreconGCR(DiracMatrix &mat, DiracMatrix &matSloppy, DiracMatrix &matPrecon, QudaInvertParam &invParam,
      TimeProfile &profile) :
    Solver(invParam, profile), mat(mat), matSloppy(matSloppy), matPrecon(matPrecon), K(0)
  {

    Kparam = newQudaInvertParam();
    fillInnerInvertParam(Kparam, invParam);
    for(int dir=0; dir<4; ++dir) Kparam.domain_overlap[dir] = invParam.domain_overlap[dir];

    if(Kparam.maxiter >= 0){
      if (invParam.inv_type_precondition == QUDA_CG_INVERTER)
        K = new SimpleCG(matPrecon, Kparam, profile);
      else if (invParam.inv_type_precondition == QUDA_MR_INVERTER)
        K = new SimpleMR(matPrecon, Kparam, profile);
      else if (invParam.inv_type_precondition == QUDA_SD_INVERTER)
        K = new SD(matPrecon, Kparam, profile);
      else if (invParam.inv_type_precondition != QUDA_INVALID_INVERTER) // unknown preconditioner
        errorQuda("Unknown inner solver %d", invParam.inv_type_precondition);
    }



  }

  PreconGCR::~PreconGCR() {
    profile[QUDA_PROFILE_FREE].Start();

    if (K) delete K;

    profile[QUDA_PROFILE_FREE].Stop();
  }

  static void setPreconditionParams(ColorSpinorParam* param,
      const QudaPrecision& precision,
      const int padding,
      const int* const dims)
  {
    param->create = QUDA_ZERO_FIELD_CREATE;
    param->precision = precision;
    param->nColor = 3;
    param->nDim = 4;
    param->pad = padding;
    param->nSpin = 1;
    param->siteSubset = QUDA_PARITY_SITE_SUBSET;
    param->siteOrder = QUDA_EVEN_ODD_SITE_ORDER;
    param->fieldOrder = QUDA_FLOAT2_FIELD_ORDER;
    for(int dir=0; dir<4; ++dir) param->x[dir] = dims[dir];
    param->x[0] /= 2;

    return;
  }


  void PreconGCR::operator()(cudaColorSpinorField &x, cudaColorSpinorField &b)
  {
    profile[QUDA_PROFILE_INIT].Start();

    int Nkrylov = invParam.gcrNkrylov; // size of Krylov space

    // Find the maximum domain overlap 
    // This will determine the number of faces needed by the vector r. 
    int max_overlap = Kparam.domain_overlap[0];
    for(int dir=0; dir<4; ++dir){
      if(Kparam.domain_overlap[dir] > max_overlap){
        max_overlap = Kparam.domain_overlap[dir];
      }
    }
   
    int X[4]; // smaller sublattice dimensions
    int Y[4]; // extended subdomain dimensions 
    X[0] = b.X(0)*2;
    X[1] = b.X(1);
    X[2] = b.X(2);
    X[3] = b.X(3);
    for(int dir=0; dir<4; ++dir) Y[dir] = X[dir] + 2*Kparam.domain_overlap[dir];

    DecompParam dparam;
    initDecompParam(&dparam,X,Y);
    int domain_overlap[4];
    for(int dir=0; dir<4; ++dir) domain_overlap[dir] = invParam.domain_overlap[dir];
    
     


    ColorSpinorParam param(x);
    param.create = QUDA_ZERO_FIELD_CREATE;
    if(max_overlap) param.nFace = max_overlap;
    cudaColorSpinorField r(x, param); 
    param.nFace = b.Nface();
    cudaColorSpinorField y(x, param); // high precision accumulator

    Extender extendCuda(r); 

    // create sloppy fields used for orthogonalization
    param.setPrecision(invParam.cuda_prec_sloppy);
    cudaColorSpinorField **p = new cudaColorSpinorField*[Nkrylov];
    cudaColorSpinorField **Ap = new cudaColorSpinorField*[Nkrylov];
    for (int i=0; i<Nkrylov; i++) {
      p[i] = new cudaColorSpinorField(x, param);
      Ap[i] = new cudaColorSpinorField(x, param);
    }

    cudaColorSpinorField tmp(x, param); //temporary for sloppy mat-vec

    cudaColorSpinorField *x_sloppy, *r_sloppy;
    if (invParam.cuda_prec_sloppy != invParam.cuda_prec) {
      param.setPrecision(invParam.cuda_prec_sloppy);
      param.nFace = b.Nface();
      x_sloppy = new cudaColorSpinorField(x, param);
      if(max_overlap) param.nFace = max_overlap;
      r_sloppy = new cudaColorSpinorField(x, param);
      param.nFace = b.Nface();
    } else {
      x_sloppy = &x;
      r_sloppy = &r;
    }

    cudaColorSpinorField &xSloppy = *x_sloppy;
    cudaColorSpinorField &rSloppy = *r_sloppy;

    // these low precision fields are used by the inner solver
    bool precMatch = true;
    cudaColorSpinorField *r_pre, *p_pre;
    
    if(max_overlap){
      ColorSpinorParam prec_param(x);
      setPreconditionParams(&prec_param,
                            invParam.cuda_prec_precondition,
                            rSloppy.Pad(),
                            Y);
      p_pre = new cudaColorSpinorField(prec_param);
      r_pre = new cudaColorSpinorField(prec_param);
      precMatch = false;
    }else if (invParam.cuda_prec_precondition != invParam.cuda_prec_sloppy || invParam.precondition_cycle > 1) {
      param.setPrecision(invParam.cuda_prec_precondition);
      p_pre = new cudaColorSpinorField(x, param);
      r_pre = new cudaColorSpinorField(x, param);
      precMatch = false;
    } else {
      p_pre = NULL;
      r_pre = r_sloppy;
    }
    
    cudaColorSpinorField &rPre = *r_pre;

    quda::Complex *alpha = new quda::Complex[Nkrylov];
    quda::Complex **beta = new quda::Complex*[Nkrylov];
    for (int i=0; i<Nkrylov; i++) beta[i] = new quda::Complex[Nkrylov];
    double *gamma = new double[Nkrylov];

    double b2 = normCuda(b);

    double stop = b2*invParam.tol*invParam.tol; // stopping condition of solver

    int k = 0;

    // compute parity of the node
    int parity = 0;
    for (int i=0; i<4; i++) parity += commCoords(i);
    parity = parity % 2;

    cudaColorSpinorField rM(rSloppy);

    profile[QUDA_PROFILE_INIT].Stop();
    profile[QUDA_PROFILE_PREAMBLE].Start();

    quda::blas_flops = 0;

    // calculate initial residual
    mat(r, x, y);
    zeroCuda(y);
    double r2 = xmyNormCuda(b, r);  
    copyCuda(rSloppy, r);

    int total_iter = 0;
    int restart = 0;
    double r2_old = r2;

    double orthT = 0, matT = 0, preT = 0, resT = 0;

    if (invParam.verbosity >= QUDA_VERBOSE) 
      printfQuda("PreconGCR: %d total iterations, %d Krylov iterations, <r,r> = %e, |r|/|b| = %e\n", 
          total_iter+k, k, r2, sqrt(r2/b2));

    profile[QUDA_PROFILE_PREAMBLE].Stop();
    profile[QUDA_PROFILE_COMPUTE].Start();

    while (r2 > stop && total_iter < invParam.maxiter) {

      gettimeofday(&pre0, NULL);


      double simple_time[3] = {0., 0., 0.};
      
      

      for (int m=0; m<invParam.precondition_cycle; m++) {
        if (invParam.inv_type_precondition != QUDA_INVALID_INVERTER) {
          cudaColorSpinorField &pPre = (precMatch ? *p[k] : *p_pre);

          if (m==0) { // residual is just source
            if(max_overlap){
                extendCuda(rPre, rSloppy, dparam, domain_overlap); 
            }else{
                copyCuda(rPre, rSloppy);
            }
          } else { // compute residual
            copyCuda(rM,rSloppy);
            axpyCuda(-1.0, *Ap[k], rM);
            if(max_overlap)
              extendCuda(rPre, rM, dparam, domain_overlap);
            else
              copyCuda(rPre, rM);
          }


          // rPre needs to be extended and pPre needs to be contracted
          if(K){
            (*K)(pPre, rPre, simple_time);
          }else{
            copyCuda(pPre, rPre);
          }

          // relaxation p = omega*p + (1-omega)*r
          //if (invParam.omega!=1.0) axpbyCuda((1.0-invParam.omega), rPre, invParam.omega, pPre);

          if (m==0) { 
            if(max_overlap)
              cropCuda(*p[k], pPre, dparam);
            else
              copyCuda(*p[k], pPre); 
          }
          else { 
            if(max_overlap)
              cropCuda(tmp, pPre, dparam);
            else
              copyCuda(tmp, pPre); 
            xpyCuda(tmp, *p[k]); }

        } else { // no preconditioner
          *p[k] = rSloppy;
        } 


        gettimeofday(&pre1, NULL);
        preT += timeInterval(pre0, pre1);

        gettimeofday(&mat0, NULL);
        matSloppy(*Ap[k], *p[k], tmp);
        gettimeofday(&mat1, NULL);
        matT += timeInterval(mat0, mat1);
      }
      //    exit(0);

      orthoDir(beta, Ap, k);

      double3 Apr = cDotProductNormACuda(*Ap[k], rSloppy);

      gamma[k] = sqrt(Apr.z); // gamma[k] = Ap[k]
      if (gamma[k] == 0.0) errorQuda("PreconGCR breakdown\n");
      alpha[k] = quda::Complex(Apr.x, Apr.y) / gamma[k]; // alpha = (1/|Ap|) * (Ap, r)

      // r -= (1/|Ap|^2) * (Ap, r) r, Ap *= 1/|Ap|
      r2 = cabxpyAxNormCuda(1.0/gamma[k], -alpha[k], *Ap[k], rSloppy); 

      if (invParam.verbosity >= QUDA_DEBUG_VERBOSE) {
        double x2 = norm2(x);
        double p2 = norm2(*p[k]);
        double Ap2 = norm2(*Ap[k]);
        printfQuda("PreconGCR: alpha = (%e,%e), norm2(x) = %e, norm2(p) = %e, norm2(Ap) = %e\n", 
            real(alpha[k]), imag(alpha[k]), x2, p2, Ap2);
      }

      k++;
      total_iter++;

      if (invParam.verbosity >= QUDA_VERBOSE) 
        printfQuda("PreconGCR: %d total iterations, %d Krylov iterations, <r,r> = %e, |r|/|b| = %e\n", 
            total_iter, k, r2, sqrt(r2/b2));


      gettimeofday(&rst0, NULL);
      // update solution and residual since max Nkrylov reached, converged or reliable update required
      if (k==Nkrylov || r2 < stop || r2/r2_old < invParam.reliable_delta) { 

        // update the solution vector
        updateSolution(xSloppy, alpha, beta, gamma, k, p);

        // recalculate residual in high precision
        copyCuda(x, xSloppy);
        xpyCuda(x, y);

        double r2Sloppy = r2;

        k = 0;
        mat(r, y, x);
        r2 = xmyNormCuda(b, r);  

        if (r2 > stop) {
          restart++; // restarting if residual is still too great

          if (invParam.verbosity >= QUDA_VERBOSE) 
            printfQuda("\nPreconGCR: restart %d, iterated r2 = %e, true r2 = %e\n", restart, r2Sloppy, r2);
        }

        copyCuda(rSloppy, r);
        zeroCuda(xSloppy);

        r2_old = r2;
      }
      gettimeofday(&rst1, NULL);

      orthT += timeInterval(orth0, orth1);
      resT += timeInterval(rst0, rst1);

    }

    if (total_iter > 0) copyCuda(x, y);

    profile[QUDA_PROFILE_COMPUTE].Stop();
    profile[QUDA_PROFILE_EPILOGUE].Start();

    invParam.secs += profile[QUDA_PROFILE_COMPUTE].Last();

    double gflops = (quda::blas_flops + mat.flops() + matSloppy.flops() + matPrecon.flops())*1e-9;
    reduceDouble(gflops);

    if (k>=invParam.maxiter && invParam.verbosity >= QUDA_SUMMARIZE) 
      warningQuda("Exceeded maximum iterations %d", invParam.maxiter);

    if (invParam.verbosity >= QUDA_VERBOSE) printfQuda("PreconGCR: number of restarts = %d\n", restart);

    // Calculate the true residual
    mat(r, x);
    double true_res = xmyNormCuda(b, r);
    invParam.true_res = sqrt(true_res / b2);
#if (__COMPUTE_CAPABILITY__ >= 200)
    invParam.true_res_hq = sqrt(HeavyQuarkResidualNormCuda(x,r).z);
#else
    invParam.true_res_hq = 0.0;
#endif   

    if (invParam.verbosity >= QUDA_SUMMARIZE)
      printfQuda("gflops = %f time = %e: Preconditoner = %e, Mat-Vec = %e, orthogonolization %e restart %e\n", 
          gflops / invParam.secs, invParam.secs, preT, matT, orthT, resT);
    invParam.gflops += gflops;
    invParam.iter += total_iter;

    // reset the flops counters
    quda::blas_flops = 0;
    mat.flops();
    matSloppy.flops();
    matPrecon.flops();

    profile[QUDA_PROFILE_EPILOGUE].Stop();
    profile[QUDA_PROFILE_FREE].Start();

    if (invParam.verbosity >= QUDA_SUMMARIZE) {
      printfQuda("PreconGCR: Converged after %d iterations, relative residua: iterated = %e, true = %e\n", 
          total_iter, sqrt(r2/b2), sqrt(true_res / b2));    
    }

    if (invParam.cuda_prec_sloppy != invParam.cuda_prec) {
      delete x_sloppy;
      delete r_sloppy;
    }

    if ((invParam.cuda_prec_precondition != invParam.cuda_prec_sloppy) || max_overlap) {
      delete p_pre;
      delete r_pre;
    }

    for (int i=0; i<Nkrylov; i++) {
      delete p[i];
      delete Ap[i];
    }
    delete[] p;
    delete[] Ap;

    delete []alpha;
    for (int i=0; i<Nkrylov; i++) delete []beta[i];
    delete []beta;
    delete []gamma;

    profile[QUDA_PROFILE_FREE].Stop();

    return;
  }

} // namespace quda

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <stack>

#include <color_spinor_field.h>
#include <dslash_quda.h>

namespace quda {

  void make4DMidPointProp(ColorSpinorField &out, ColorSpinorField &in) {

    if(in.Ndim() != 5) errorQuda("Can not make 4D quark propagator from %dD field\n", in.Ndim());

    // Clone the 4D vector
    ColorSpinorParam param4D(out);

    // Extract pointers
    auto Ls = in.X()[4];
    auto vol4D = in.Volume() / Ls;
    auto spin = in.Nspin();
    auto col = in.Ncolor();
    auto data_size = 2 * in.Precision();
    
    void *VLs2m1 = (char *)(in.V()) + (Ls/2-1) * vol4D * spin * col * data_size;
    void *VLs2 = (char *)(in.V()) + (Ls/2) * vol4D * spin * col * data_size;

    // Create wrappers around the (Ls/2)-1 and Ls/2 4D fields
    std::vector<ColorSpinorField*> Ls2m1;
    param4D.v = VLs2m1;
    param4D.create = QUDA_REFERENCE_FIELD_CREATE;    
    Ls2m1.push_back(ColorSpinorField::Create(param4D));
    
    std::vector<ColorSpinorField*> Ls2;
    param4D.v = VLs2;
    param4D.create = QUDA_REFERENCE_FIELD_CREATE;    
    Ls2.push_back(ColorSpinorField::Create(param4D));

    // Ensure out is zeroed
    qudaMemsetAsync(out.V(), 0, vol4D * spin * col * data_size, 0);    

    // out(x) = P_L L0(x) + P_R Lsm1(x)
    chiralProject(out, *Ls2m1[0], 1);
    chiralProject(out, *Ls2[0], -1);
  }
  
  void make4DQuarkProp(ColorSpinorField &out, ColorSpinorField &in)
  {
    if(in.Ndim() != 5) errorQuda("Can not make 4D quark propagator from %dD field\n", in.Ndim());

    // Clone the 5D field to make 4D vector
    ColorSpinorParam param4D(out);
    
    // Extract pointers
    int Ls = in.X(4);
    int vol4D = (1.0 * in.Volume()) / Ls;
    int spin = in.Nspin();
    int col = in.Ncolor();
    int data_size = 2 * in.Precision();
    
    void *V0 = static_cast<char *>(in.V());
    void *VLsm1 = static_cast<char *>(in.V()) + (Ls-1) * vol4D * spin * col * data_size;
    
    // Create wrappers around the 0 and Ls-1 4D fields
    std::vector<ColorSpinorField*> L0;
    param4D.v = V0;
    param4D.create = QUDA_REFERENCE_FIELD_CREATE;    
    L0.push_back(ColorSpinorField::Create(param4D));

    std::vector<ColorSpinorField*> Lsm1;
    param4D.v = VLsm1;
    param4D.create = QUDA_REFERENCE_FIELD_CREATE;    
    Lsm1.push_back(ColorSpinorField::Create(param4D));

    // Ensure out is zeroed
    qudaMemsetAsync(out.V(), 0, vol4D * spin * col * data_size, 0);    

    // out(x) = P_L L0(x) + P_R Lsm1(x)
    chiralProject(out, *L0[0], -1);
    chiralProject(out, *Lsm1[0], 1);
    
    delete Lsm1[0];
    delete L0[0];
  }
}

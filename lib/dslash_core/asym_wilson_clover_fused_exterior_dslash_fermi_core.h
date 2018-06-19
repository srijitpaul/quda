// *** CUDA DSLASH ***

#define DSLASH_SHARED_FLOATS_PER_THREAD 24


#ifdef MULTI_GPU

#if ((CUDA_VERSION >= 4010) && (__COMPUTE_CAPABILITY__ >= 200)) // NVVM compiler
#define VOLATILE
#else // Open64 compiler
#define VOLATILE volatile
#endif
// input spinor
#ifdef SPINOR_DOUBLE
#define spinorFloat double
#define WRITE_SPINOR_SHARED WRITE_SPINOR_SHARED_DOUBLE2
#define READ_SPINOR_SHARED READ_SPINOR_SHARED_DOUBLE2
#define i00_re I0.x
#define i00_im I0.y
#define i01_re I1.x
#define i01_im I1.y
#define i02_re I2.x
#define i02_im I2.y
#define i10_re I3.x
#define i10_im I3.y
#define i11_re I4.x
#define i11_im I4.y
#define i12_re I5.x
#define i12_im I5.y
#define i20_re I6.x
#define i20_im I6.y
#define i21_re I7.x
#define i21_im I7.y
#define i22_re I8.x
#define i22_im I8.y
#define i30_re I9.x
#define i30_im I9.y
#define i31_re I10.x
#define i31_im I10.y
#define i32_re I11.x
#define i32_im I11.y
#define acc00_re accum0.x
#define acc00_im accum0.y
#define acc01_re accum1.x
#define acc01_im accum1.y
#define acc02_re accum2.x
#define acc02_im accum2.y
#define acc10_re accum3.x
#define acc10_im accum3.y
#define acc11_re accum4.x
#define acc11_im accum4.y
#define acc12_re accum5.x
#define acc12_im accum5.y
#define acc20_re accum6.x
#define acc20_im accum6.y
#define acc21_re accum7.x
#define acc21_im accum7.y
#define acc22_re accum8.x
#define acc22_im accum8.y
#define acc30_re accum9.x
#define acc30_im accum9.y
#define acc31_re accum10.x
#define acc31_im accum10.y
#define acc32_re accum11.x
#define acc32_im accum11.y
#else
#define spinorFloat float
#define WRITE_SPINOR_SHARED WRITE_SPINOR_SHARED_FLOAT4
#define READ_SPINOR_SHARED READ_SPINOR_SHARED_FLOAT4
#define i00_re I0.x
#define i00_im I0.y
#define i01_re I0.z
#define i01_im I0.w
#define i02_re I1.x
#define i02_im I1.y
#define i10_re I1.z
#define i10_im I1.w
#define i11_re I2.x
#define i11_im I2.y
#define i12_re I2.z
#define i12_im I2.w
#define i20_re I3.x
#define i20_im I3.y
#define i21_re I3.z
#define i21_im I3.w
#define i22_re I4.x
#define i22_im I4.y
#define i30_re I4.z
#define i30_im I4.w
#define i31_re I5.x
#define i31_im I5.y
#define i32_re I5.z
#define i32_im I5.w
#define acc00_re accum0.x
#define acc00_im accum0.y
#define acc01_re accum0.z
#define acc01_im accum0.w
#define acc02_re accum1.x
#define acc02_im accum1.y
#define acc10_re accum1.z
#define acc10_im accum1.w
#define acc11_re accum2.x
#define acc11_im accum2.y
#define acc12_re accum2.z
#define acc12_im accum2.w
#define acc20_re accum3.x
#define acc20_im accum3.y
#define acc21_re accum3.z
#define acc21_im accum3.w
#define acc22_re accum4.x
#define acc22_im accum4.y
#define acc30_re accum4.z
#define acc30_im accum4.w
#define acc31_re accum5.x
#define acc31_im accum5.y
#define acc32_re accum5.z
#define acc32_im accum5.w
#endif // SPINOR_DOUBLE

// gauge link
#ifdef GAUGE_FLOAT2
#define g00_re G0.x
#define g00_im G0.y
#define g01_re G1.x
#define g01_im G1.y
#define g02_re G2.x
#define g02_im G2.y
#define g10_re G3.x
#define g10_im G3.y
#define g11_re G4.x
#define g11_im G4.y
#define g12_re G5.x
#define g12_im G5.y
#define g20_re G6.x
#define g20_im G6.y
#define g21_re G7.x
#define g21_im G7.y
#define g22_re G8.x
#define g22_im G8.y

#else
#define g00_re G0.x
#define g00_im G0.y
#define g01_re G0.z
#define g01_im G0.w
#define g02_re G1.x
#define g02_im G1.y
#define g10_re G1.z
#define g10_im G1.w
#define g11_re G2.x
#define g11_im G2.y
#define g12_re G2.z
#define g12_im G2.w
#define g20_re G3.x
#define g20_im G3.y
#define g21_re G3.z
#define g21_im G3.w
#define g22_re G4.x
#define g22_im G4.y

#endif // GAUGE_DOUBLE

// conjugated gauge link
#define gT00_re (+g00_re)
#define gT00_im (-g00_im)
#define gT01_re (+g10_re)
#define gT01_im (-g10_im)
#define gT02_re (+g20_re)
#define gT02_im (-g20_im)
#define gT10_re (+g01_re)
#define gT10_im (-g01_im)
#define gT11_re (+g11_re)
#define gT11_im (-g11_im)
#define gT12_re (+g21_re)
#define gT12_im (-g21_im)
#define gT20_re (+g02_re)
#define gT20_im (-g02_im)
#define gT21_re (+g12_re)
#define gT21_im (-g12_im)
#define gT22_re (+g22_re)
#define gT22_im (-g22_im)

// first chiral block of inverted clover term
#ifdef CLOVER_DOUBLE
#define c00_00_re C0.x
#define c01_01_re C0.y
#define c02_02_re C1.x
#define c10_10_re C1.y
#define c11_11_re C2.x
#define c12_12_re C2.y
#define c01_00_re C3.x
#define c01_00_im C3.y
#define c02_00_re C4.x
#define c02_00_im C4.y
#define c10_00_re C5.x
#define c10_00_im C5.y
#define c11_00_re C6.x
#define c11_00_im C6.y
#define c12_00_re C7.x
#define c12_00_im C7.y
#define c02_01_re C8.x
#define c02_01_im C8.y
#define c10_01_re C9.x
#define c10_01_im C9.y
#define c11_01_re C10.x
#define c11_01_im C10.y
#define c12_01_re C11.x
#define c12_01_im C11.y
#define c10_02_re C12.x
#define c10_02_im C12.y
#define c11_02_re C13.x
#define c11_02_im C13.y
#define c12_02_re C14.x
#define c12_02_im C14.y
#define c11_10_re C15.x
#define c11_10_im C15.y
#define c12_10_re C16.x
#define c12_10_im C16.y
#define c12_11_re C17.x
#define c12_11_im C17.y
#else
#define c00_00_re C0.x
#define c01_01_re C0.y
#define c02_02_re C0.z
#define c10_10_re C0.w
#define c11_11_re C1.x
#define c12_12_re C1.y
#define c01_00_re C1.z
#define c01_00_im C1.w
#define c02_00_re C2.x
#define c02_00_im C2.y
#define c10_00_re C2.z
#define c10_00_im C2.w
#define c11_00_re C3.x
#define c11_00_im C3.y
#define c12_00_re C3.z
#define c12_00_im C3.w
#define c02_01_re C4.x
#define c02_01_im C4.y
#define c10_01_re C4.z
#define c10_01_im C4.w
#define c11_01_re C5.x
#define c11_01_im C5.y
#define c12_01_re C5.z
#define c12_01_im C5.w
#define c10_02_re C6.x
#define c10_02_im C6.y
#define c11_02_re C6.z
#define c11_02_im C6.w
#define c12_02_re C7.x
#define c12_02_im C7.y
#define c11_10_re C7.z
#define c11_10_im C7.w
#define c12_10_re C8.x
#define c12_10_im C8.y
#define c12_11_re C8.z
#define c12_11_im C8.w
#endif // CLOVER_DOUBLE

#define c00_01_re (+c01_00_re)
#define c00_01_im (-c01_00_im)
#define c00_02_re (+c02_00_re)
#define c00_02_im (-c02_00_im)
#define c01_02_re (+c02_01_re)
#define c01_02_im (-c02_01_im)
#define c00_10_re (+c10_00_re)
#define c00_10_im (-c10_00_im)
#define c01_10_re (+c10_01_re)
#define c01_10_im (-c10_01_im)
#define c02_10_re (+c10_02_re)
#define c02_10_im (-c10_02_im)
#define c00_11_re (+c11_00_re)
#define c00_11_im (-c11_00_im)
#define c01_11_re (+c11_01_re)
#define c01_11_im (-c11_01_im)
#define c02_11_re (+c11_02_re)
#define c02_11_im (-c11_02_im)
#define c10_11_re (+c11_10_re)
#define c10_11_im (-c11_10_im)
#define c00_12_re (+c12_00_re)
#define c00_12_im (-c12_00_im)
#define c01_12_re (+c12_01_re)
#define c01_12_im (-c12_01_im)
#define c02_12_re (+c12_02_re)
#define c02_12_im (-c12_02_im)
#define c10_12_re (+c12_10_re)
#define c10_12_im (-c12_10_im)
#define c11_12_re (+c12_11_re)
#define c11_12_im (-c12_11_im)

// second chiral block of inverted clover term (reuses C0,...,C9)
#define c20_20_re c00_00_re
#define c21_20_re c01_00_re
#define c21_20_im c01_00_im
#define c22_20_re c02_00_re
#define c22_20_im c02_00_im
#define c30_20_re c10_00_re
#define c30_20_im c10_00_im
#define c31_20_re c11_00_re
#define c31_20_im c11_00_im
#define c32_20_re c12_00_re
#define c32_20_im c12_00_im
#define c20_21_re c00_01_re
#define c20_21_im c00_01_im
#define c21_21_re c01_01_re
#define c22_21_re c02_01_re
#define c22_21_im c02_01_im
#define c30_21_re c10_01_re
#define c30_21_im c10_01_im
#define c31_21_re c11_01_re
#define c31_21_im c11_01_im
#define c32_21_re c12_01_re
#define c32_21_im c12_01_im
#define c20_22_re c00_02_re
#define c20_22_im c00_02_im
#define c21_22_re c01_02_re
#define c21_22_im c01_02_im
#define c22_22_re c02_02_re
#define c30_22_re c10_02_re
#define c30_22_im c10_02_im
#define c31_22_re c11_02_re
#define c31_22_im c11_02_im
#define c32_22_re c12_02_re
#define c32_22_im c12_02_im
#define c20_30_re c00_10_re
#define c20_30_im c00_10_im
#define c21_30_re c01_10_re
#define c21_30_im c01_10_im
#define c22_30_re c02_10_re
#define c22_30_im c02_10_im
#define c30_30_re c10_10_re
#define c31_30_re c11_10_re
#define c31_30_im c11_10_im
#define c32_30_re c12_10_re
#define c32_30_im c12_10_im
#define c20_31_re c00_11_re
#define c20_31_im c00_11_im
#define c21_31_re c01_11_re
#define c21_31_im c01_11_im
#define c22_31_re c02_11_re
#define c22_31_im c02_11_im
#define c30_31_re c10_11_re
#define c30_31_im c10_11_im
#define c31_31_re c11_11_re
#define c32_31_re c12_11_re
#define c32_31_im c12_11_im
#define c20_32_re c00_12_re
#define c20_32_im c00_12_im
#define c21_32_re c01_12_re
#define c21_32_im c01_12_im
#define c22_32_re c02_12_re
#define c22_32_im c02_12_im
#define c30_32_re c10_12_re
#define c30_32_im c10_12_im
#define c31_32_re c11_12_re
#define c31_32_im c11_12_im
#define c32_32_re c12_12_re

// output spinor
VOLATILE spinorFloat o00_re;
VOLATILE spinorFloat o00_im;
VOLATILE spinorFloat o01_re;
VOLATILE spinorFloat o01_im;
VOLATILE spinorFloat o02_re;
VOLATILE spinorFloat o02_im;
VOLATILE spinorFloat o10_re;
VOLATILE spinorFloat o10_im;
VOLATILE spinorFloat o11_re;
VOLATILE spinorFloat o11_im;
VOLATILE spinorFloat o12_re;
VOLATILE spinorFloat o12_im;
VOLATILE spinorFloat o20_re;
VOLATILE spinorFloat o20_im;
VOLATILE spinorFloat o21_re;
VOLATILE spinorFloat o21_im;
VOLATILE spinorFloat o22_re;
VOLATILE spinorFloat o22_im;
VOLATILE spinorFloat o30_re;
VOLATILE spinorFloat o30_im;
VOLATILE spinorFloat o31_re;
VOLATILE spinorFloat o31_im;
VOLATILE spinorFloat o32_re;
VOLATILE spinorFloat o32_im;

#ifdef SPINOR_DOUBLE
#define SHARED_STRIDE 16 // to avoid bank conflicts on Fermi
#else
#define SHARED_STRIDE 32 // to avoid bank conflicts on Fermi
#endif

#include "read_gauge.h"
#include "read_clover.h"
#include "io_spinor.h"

int coord[5];
int X;

#if (DD_PREC==2) // half precision
int sp_norm_idx;
#endif // half precision

int sid;


  sid = blockIdx.x*blockDim.x + threadIdx.x;
  if (sid >= param.threads) return;

  int dim = dimFromFaceIndex(sid, param); // sid is also modified


  const int face_volume = ((param.threadDimMapUpper[dim] - param.threadDimMapLower[dim]) >> 1);   // volume of one face
  int face_num = (sid >= face_volume);              // is this thread updating face 0 or 1
  int face_idx = sid - face_num*face_volume;        // index into the respective face

  // ghostOffset is scaled to include body (includes stride) and number of FloatN arrays (SPINOR_HOP)
  // face_idx not sid since faces are spin projected and share the same volume index (modulo UP/DOWN reading)
  //sp_idx = face_idx + param.ghostOffset[dim];

  switch(dim) {
  case 0:
    coordsFromFaceIndex<4,QUDA_4D_PC,0,1>(X, sid, coord, face_idx, face_num, param);
    break;
  case 1:
    coordsFromFaceIndex<4,QUDA_4D_PC,1,1>(X, sid, coord, face_idx, face_num, param);
    break;
  case 2:
    coordsFromFaceIndex<4,QUDA_4D_PC,2,1>(X, sid, coord, face_idx, face_num, param);
    break;
  case 3:
    coordsFromFaceIndex<4,QUDA_4D_PC,3,1>(X, sid, coord, face_idx, face_num, param);
    break;
  }


  bool active = false;
  for(int dir=0; dir<4; ++dir){
   active = active  || isActive(dim,dir,+1,coord,param.commDim,param.X);
  }
  if(!active) return;

  READ_INTERMEDIATE_SPINOR(INTERTEX, param.sp_stride, sid, sid);

  o00_re = i00_re;  o00_im = i00_im;
  o01_re = i01_re;  o01_im = i01_im;
  o02_re = i02_re;  o02_im = i02_im;
  o10_re = i10_re;  o10_im = i10_im;
  o11_re = i11_re;  o11_im = i11_im;
  o12_re = i12_re;  o12_im = i12_im;
  o20_re = i20_re;  o20_im = i20_im;
  o21_re = i21_re;  o21_im = i21_im;
  o22_re = i22_re;  o22_im = i22_im;
  o30_re = i30_re;  o30_im = i30_im;
  o31_re = i31_re;  o31_im = i31_im;
  o32_re = i32_re;  o32_im = i32_im;


if ( isActive(dim,0,+1,coord,param.commDim,param.X) && coord[0]==X1m1 )
{
  // Projector P0-
  // 1 0 0 -i 
  // 0 1 -i 0 
  // 0 i 1 0 
  // i 0 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,0,param);
  const int sp_idx = face_idx + param.ghostOffset[0][1];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[0][1];
#endif
  
  const int ga_idx = sid;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[0];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 0);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE0TEX, 0, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(0);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += g00_re * a0_re;
  A0_re -= g00_im * a0_im;
  A0_re += g01_re * a1_re;
  A0_re -= g01_im * a1_im;
  A0_re += g02_re * a2_re;
  A0_re -= g02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += g00_re * a0_im;
  A0_im += g00_im * a0_re;
  A0_im += g01_re * a1_im;
  A0_im += g01_im * a1_re;
  A0_im += g02_re * a2_im;
  A0_im += g02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += g00_re * b0_re;
  B0_re -= g00_im * b0_im;
  B0_re += g01_re * b1_re;
  B0_re -= g01_im * b1_im;
  B0_re += g02_re * b2_re;
  B0_re -= g02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += g00_re * b0_im;
  B0_im += g00_im * b0_re;
  B0_im += g01_re * b1_im;
  B0_im += g01_im * b1_re;
  B0_im += g02_re * b2_im;
  B0_im += g02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += g10_re * a0_re;
  A1_re -= g10_im * a0_im;
  A1_re += g11_re * a1_re;
  A1_re -= g11_im * a1_im;
  A1_re += g12_re * a2_re;
  A1_re -= g12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += g10_re * a0_im;
  A1_im += g10_im * a0_re;
  A1_im += g11_re * a1_im;
  A1_im += g11_im * a1_re;
  A1_im += g12_re * a2_im;
  A1_im += g12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += g10_re * b0_re;
  B1_re -= g10_im * b0_im;
  B1_re += g11_re * b1_re;
  B1_re -= g11_im * b1_im;
  B1_re += g12_re * b2_re;
  B1_re -= g12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += g10_re * b0_im;
  B1_im += g10_im * b0_re;
  B1_im += g11_re * b1_im;
  B1_im += g11_im * b1_re;
  B1_im += g12_re * b2_im;
  B1_im += g12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += g20_re * a0_re;
  A2_re -= g20_im * a0_im;
  A2_re += g21_re * a1_re;
  A2_re -= g21_im * a1_im;
  A2_re += g22_re * a2_re;
  A2_re -= g22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += g20_re * a0_im;
  A2_im += g20_im * a0_re;
  A2_im += g21_re * a1_im;
  A2_im += g21_im * a1_re;
  A2_im += g22_re * a2_im;
  A2_im += g22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += g20_re * b0_re;
  B2_re -= g20_im * b0_im;
  B2_re += g21_re * b1_re;
  B2_re -= g21_im * b1_im;
  B2_re += g22_re * b2_re;
  B2_re -= g22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += g20_re * b0_im;
  B2_im += g20_im * b0_re;
  B2_im += g21_re * b1_im;
  B2_im += g21_im * b1_re;
  B2_im += g22_re * b2_im;
  B2_im += g22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re -= a*B0_im;
  o20_im += a*B0_re;
  o30_re -= a*A0_im;
  o30_im += a*A0_re;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re -= a*B1_im;
  o21_im += a*B1_re;
  o31_re -= a*A1_im;
  o31_im += a*A1_re;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re -= a*B2_im;
  o22_im += a*B2_re;
  o32_re -= a*A2_im;
  o32_im += a*A2_re;
  
}

if ( isActive(dim,0,-1,coord,param.commDim,param.X) && coord[0]==0 )
{
  // Projector P0+
  // 1 0 0 i 
  // 0 1 i 0 
  // 0 -i 1 0 
  // -i 0 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,0,param);
  const int sp_idx = face_idx + param.ghostOffset[0][0];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[0][0];
#endif
  
  const int ga_idx = Vh+face_idx;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[0];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 1);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE1TEX, 1, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(1);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += gT00_re * a0_re;
  A0_re -= gT00_im * a0_im;
  A0_re += gT01_re * a1_re;
  A0_re -= gT01_im * a1_im;
  A0_re += gT02_re * a2_re;
  A0_re -= gT02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += gT00_re * a0_im;
  A0_im += gT00_im * a0_re;
  A0_im += gT01_re * a1_im;
  A0_im += gT01_im * a1_re;
  A0_im += gT02_re * a2_im;
  A0_im += gT02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += gT00_re * b0_re;
  B0_re -= gT00_im * b0_im;
  B0_re += gT01_re * b1_re;
  B0_re -= gT01_im * b1_im;
  B0_re += gT02_re * b2_re;
  B0_re -= gT02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += gT00_re * b0_im;
  B0_im += gT00_im * b0_re;
  B0_im += gT01_re * b1_im;
  B0_im += gT01_im * b1_re;
  B0_im += gT02_re * b2_im;
  B0_im += gT02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += gT10_re * a0_re;
  A1_re -= gT10_im * a0_im;
  A1_re += gT11_re * a1_re;
  A1_re -= gT11_im * a1_im;
  A1_re += gT12_re * a2_re;
  A1_re -= gT12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += gT10_re * a0_im;
  A1_im += gT10_im * a0_re;
  A1_im += gT11_re * a1_im;
  A1_im += gT11_im * a1_re;
  A1_im += gT12_re * a2_im;
  A1_im += gT12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += gT10_re * b0_re;
  B1_re -= gT10_im * b0_im;
  B1_re += gT11_re * b1_re;
  B1_re -= gT11_im * b1_im;
  B1_re += gT12_re * b2_re;
  B1_re -= gT12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += gT10_re * b0_im;
  B1_im += gT10_im * b0_re;
  B1_im += gT11_re * b1_im;
  B1_im += gT11_im * b1_re;
  B1_im += gT12_re * b2_im;
  B1_im += gT12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += gT20_re * a0_re;
  A2_re -= gT20_im * a0_im;
  A2_re += gT21_re * a1_re;
  A2_re -= gT21_im * a1_im;
  A2_re += gT22_re * a2_re;
  A2_re -= gT22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += gT20_re * a0_im;
  A2_im += gT20_im * a0_re;
  A2_im += gT21_re * a1_im;
  A2_im += gT21_im * a1_re;
  A2_im += gT22_re * a2_im;
  A2_im += gT22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += gT20_re * b0_re;
  B2_re -= gT20_im * b0_im;
  B2_re += gT21_re * b1_re;
  B2_re -= gT21_im * b1_im;
  B2_re += gT22_re * b2_re;
  B2_re -= gT22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += gT20_re * b0_im;
  B2_im += gT20_im * b0_re;
  B2_im += gT21_re * b1_im;
  B2_im += gT21_im * b1_re;
  B2_im += gT22_re * b2_im;
  B2_im += gT22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re += a*B0_im;
  o20_im -= a*B0_re;
  o30_re += a*A0_im;
  o30_im -= a*A0_re;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re += a*B1_im;
  o21_im -= a*B1_re;
  o31_re += a*A1_im;
  o31_im -= a*A1_re;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re += a*B2_im;
  o22_im -= a*B2_re;
  o32_re += a*A2_im;
  o32_im -= a*A2_re;
  
}

if ( isActive(dim,1,+1,coord,param.commDim,param.X) && coord[1]==X2m1 )
{
  // Projector P1-
  // 1 0 0 -1 
  // 0 1 1 0 
  // 0 1 1 0 
  // -1 0 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,1,param);
  const int sp_idx = face_idx + param.ghostOffset[1][1];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[1][1];
#endif
  
  const int ga_idx = sid;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[1];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 2);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE0TEX, 2, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(2);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += g00_re * a0_re;
  A0_re -= g00_im * a0_im;
  A0_re += g01_re * a1_re;
  A0_re -= g01_im * a1_im;
  A0_re += g02_re * a2_re;
  A0_re -= g02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += g00_re * a0_im;
  A0_im += g00_im * a0_re;
  A0_im += g01_re * a1_im;
  A0_im += g01_im * a1_re;
  A0_im += g02_re * a2_im;
  A0_im += g02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += g00_re * b0_re;
  B0_re -= g00_im * b0_im;
  B0_re += g01_re * b1_re;
  B0_re -= g01_im * b1_im;
  B0_re += g02_re * b2_re;
  B0_re -= g02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += g00_re * b0_im;
  B0_im += g00_im * b0_re;
  B0_im += g01_re * b1_im;
  B0_im += g01_im * b1_re;
  B0_im += g02_re * b2_im;
  B0_im += g02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += g10_re * a0_re;
  A1_re -= g10_im * a0_im;
  A1_re += g11_re * a1_re;
  A1_re -= g11_im * a1_im;
  A1_re += g12_re * a2_re;
  A1_re -= g12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += g10_re * a0_im;
  A1_im += g10_im * a0_re;
  A1_im += g11_re * a1_im;
  A1_im += g11_im * a1_re;
  A1_im += g12_re * a2_im;
  A1_im += g12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += g10_re * b0_re;
  B1_re -= g10_im * b0_im;
  B1_re += g11_re * b1_re;
  B1_re -= g11_im * b1_im;
  B1_re += g12_re * b2_re;
  B1_re -= g12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += g10_re * b0_im;
  B1_im += g10_im * b0_re;
  B1_im += g11_re * b1_im;
  B1_im += g11_im * b1_re;
  B1_im += g12_re * b2_im;
  B1_im += g12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += g20_re * a0_re;
  A2_re -= g20_im * a0_im;
  A2_re += g21_re * a1_re;
  A2_re -= g21_im * a1_im;
  A2_re += g22_re * a2_re;
  A2_re -= g22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += g20_re * a0_im;
  A2_im += g20_im * a0_re;
  A2_im += g21_re * a1_im;
  A2_im += g21_im * a1_re;
  A2_im += g22_re * a2_im;
  A2_im += g22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += g20_re * b0_re;
  B2_re -= g20_im * b0_im;
  B2_re += g21_re * b1_re;
  B2_re -= g21_im * b1_im;
  B2_re += g22_re * b2_re;
  B2_re -= g22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += g20_re * b0_im;
  B2_im += g20_im * b0_re;
  B2_im += g21_re * b1_im;
  B2_im += g21_im * b1_re;
  B2_im += g22_re * b2_im;
  B2_im += g22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re += a*B0_re;
  o20_im += a*B0_im;
  o30_re -= a*A0_re;
  o30_im -= a*A0_im;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re += a*B1_re;
  o21_im += a*B1_im;
  o31_re -= a*A1_re;
  o31_im -= a*A1_im;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re += a*B2_re;
  o22_im += a*B2_im;
  o32_re -= a*A2_re;
  o32_im -= a*A2_im;
  
}

if ( isActive(dim,1,-1,coord,param.commDim,param.X) && coord[1]==0 )
{
  // Projector P1+
  // 1 0 0 1 
  // 0 1 -1 0 
  // 0 -1 1 0 
  // 1 0 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,1,param);
  const int sp_idx = face_idx + param.ghostOffset[1][0];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[1][0];
#endif
  
  const int ga_idx = Vh+face_idx;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[1];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 3);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE1TEX, 3, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(3);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += gT00_re * a0_re;
  A0_re -= gT00_im * a0_im;
  A0_re += gT01_re * a1_re;
  A0_re -= gT01_im * a1_im;
  A0_re += gT02_re * a2_re;
  A0_re -= gT02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += gT00_re * a0_im;
  A0_im += gT00_im * a0_re;
  A0_im += gT01_re * a1_im;
  A0_im += gT01_im * a1_re;
  A0_im += gT02_re * a2_im;
  A0_im += gT02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += gT00_re * b0_re;
  B0_re -= gT00_im * b0_im;
  B0_re += gT01_re * b1_re;
  B0_re -= gT01_im * b1_im;
  B0_re += gT02_re * b2_re;
  B0_re -= gT02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += gT00_re * b0_im;
  B0_im += gT00_im * b0_re;
  B0_im += gT01_re * b1_im;
  B0_im += gT01_im * b1_re;
  B0_im += gT02_re * b2_im;
  B0_im += gT02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += gT10_re * a0_re;
  A1_re -= gT10_im * a0_im;
  A1_re += gT11_re * a1_re;
  A1_re -= gT11_im * a1_im;
  A1_re += gT12_re * a2_re;
  A1_re -= gT12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += gT10_re * a0_im;
  A1_im += gT10_im * a0_re;
  A1_im += gT11_re * a1_im;
  A1_im += gT11_im * a1_re;
  A1_im += gT12_re * a2_im;
  A1_im += gT12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += gT10_re * b0_re;
  B1_re -= gT10_im * b0_im;
  B1_re += gT11_re * b1_re;
  B1_re -= gT11_im * b1_im;
  B1_re += gT12_re * b2_re;
  B1_re -= gT12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += gT10_re * b0_im;
  B1_im += gT10_im * b0_re;
  B1_im += gT11_re * b1_im;
  B1_im += gT11_im * b1_re;
  B1_im += gT12_re * b2_im;
  B1_im += gT12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += gT20_re * a0_re;
  A2_re -= gT20_im * a0_im;
  A2_re += gT21_re * a1_re;
  A2_re -= gT21_im * a1_im;
  A2_re += gT22_re * a2_re;
  A2_re -= gT22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += gT20_re * a0_im;
  A2_im += gT20_im * a0_re;
  A2_im += gT21_re * a1_im;
  A2_im += gT21_im * a1_re;
  A2_im += gT22_re * a2_im;
  A2_im += gT22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += gT20_re * b0_re;
  B2_re -= gT20_im * b0_im;
  B2_re += gT21_re * b1_re;
  B2_re -= gT21_im * b1_im;
  B2_re += gT22_re * b2_re;
  B2_re -= gT22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += gT20_re * b0_im;
  B2_im += gT20_im * b0_re;
  B2_im += gT21_re * b1_im;
  B2_im += gT21_im * b1_re;
  B2_im += gT22_re * b2_im;
  B2_im += gT22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re -= a*B0_re;
  o20_im -= a*B0_im;
  o30_re += a*A0_re;
  o30_im += a*A0_im;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re -= a*B1_re;
  o21_im -= a*B1_im;
  o31_re += a*A1_re;
  o31_im += a*A1_im;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re -= a*B2_re;
  o22_im -= a*B2_im;
  o32_re += a*A2_re;
  o32_im += a*A2_im;
  
}

if ( isActive(dim,2,+1,coord,param.commDim,param.X) && coord[2]==X3m1 )
{
  // Projector P2-
  // 1 0 -i 0 
  // 0 1 0 i 
  // i 0 1 0 
  // 0 -i 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,2,param);
  const int sp_idx = face_idx + param.ghostOffset[2][1];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[2][1];
#endif
  
  const int ga_idx = sid;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[2];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 4);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE0TEX, 4, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(4);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += g00_re * a0_re;
  A0_re -= g00_im * a0_im;
  A0_re += g01_re * a1_re;
  A0_re -= g01_im * a1_im;
  A0_re += g02_re * a2_re;
  A0_re -= g02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += g00_re * a0_im;
  A0_im += g00_im * a0_re;
  A0_im += g01_re * a1_im;
  A0_im += g01_im * a1_re;
  A0_im += g02_re * a2_im;
  A0_im += g02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += g00_re * b0_re;
  B0_re -= g00_im * b0_im;
  B0_re += g01_re * b1_re;
  B0_re -= g01_im * b1_im;
  B0_re += g02_re * b2_re;
  B0_re -= g02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += g00_re * b0_im;
  B0_im += g00_im * b0_re;
  B0_im += g01_re * b1_im;
  B0_im += g01_im * b1_re;
  B0_im += g02_re * b2_im;
  B0_im += g02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += g10_re * a0_re;
  A1_re -= g10_im * a0_im;
  A1_re += g11_re * a1_re;
  A1_re -= g11_im * a1_im;
  A1_re += g12_re * a2_re;
  A1_re -= g12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += g10_re * a0_im;
  A1_im += g10_im * a0_re;
  A1_im += g11_re * a1_im;
  A1_im += g11_im * a1_re;
  A1_im += g12_re * a2_im;
  A1_im += g12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += g10_re * b0_re;
  B1_re -= g10_im * b0_im;
  B1_re += g11_re * b1_re;
  B1_re -= g11_im * b1_im;
  B1_re += g12_re * b2_re;
  B1_re -= g12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += g10_re * b0_im;
  B1_im += g10_im * b0_re;
  B1_im += g11_re * b1_im;
  B1_im += g11_im * b1_re;
  B1_im += g12_re * b2_im;
  B1_im += g12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += g20_re * a0_re;
  A2_re -= g20_im * a0_im;
  A2_re += g21_re * a1_re;
  A2_re -= g21_im * a1_im;
  A2_re += g22_re * a2_re;
  A2_re -= g22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += g20_re * a0_im;
  A2_im += g20_im * a0_re;
  A2_im += g21_re * a1_im;
  A2_im += g21_im * a1_re;
  A2_im += g22_re * a2_im;
  A2_im += g22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += g20_re * b0_re;
  B2_re -= g20_im * b0_im;
  B2_re += g21_re * b1_re;
  B2_re -= g21_im * b1_im;
  B2_re += g22_re * b2_re;
  B2_re -= g22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += g20_re * b0_im;
  B2_im += g20_im * b0_re;
  B2_im += g21_re * b1_im;
  B2_im += g21_im * b1_re;
  B2_im += g22_re * b2_im;
  B2_im += g22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re -= a*A0_im;
  o20_im += a*A0_re;
  o30_re += a*B0_im;
  o30_im -= a*B0_re;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re -= a*A1_im;
  o21_im += a*A1_re;
  o31_re += a*B1_im;
  o31_im -= a*B1_re;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re -= a*A2_im;
  o22_im += a*A2_re;
  o32_re += a*B2_im;
  o32_im -= a*B2_re;
  
}

if ( isActive(dim,2,-1,coord,param.commDim,param.X) && coord[2]==0 )
{
  // Projector P2+
  // 1 0 i 0 
  // 0 1 0 -i 
  // -i 0 1 0 
  // 0 i 0 1 
  
  faceIndexFromCoords<4,1>(face_idx,coord,2,param);
  const int sp_idx = face_idx + param.ghostOffset[2][0];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[2][0];
#endif
  
  const int ga_idx = Vh+face_idx;
  
  spinorFloat a0_re, a0_im;
  spinorFloat a1_re, a1_im;
  spinorFloat a2_re, a2_im;
  spinorFloat b0_re, b0_im;
  spinorFloat b1_re, b1_im;
  spinorFloat b2_re, b2_im;
  
  
  const int sp_stride_pad = ghostFace[2];
  
  // read half spinor from device memory
  READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 5);
  
  a0_re = i00_re;  a0_im = i00_im;
  a1_re = i01_re;  a1_im = i01_im;
  a2_re = i02_re;  a2_im = i02_im;
  b0_re = i10_re;  b0_im = i10_im;
  b1_re = i11_re;  b1_im = i11_im;
  b2_re = i12_re;  b2_im = i12_im;
  
  
  // read gauge matrix from device memory
  READ_GAUGE_MATRIX(G, GAUGE1TEX, 5, ga_idx, param.gauge_stride);
  
  // reconstruct gauge matrix
  RECONSTRUCT_GAUGE_MATRIX(5);
  
  // multiply row 0
  spinorFloat A0_re = 0;
  A0_re += gT00_re * a0_re;
  A0_re -= gT00_im * a0_im;
  A0_re += gT01_re * a1_re;
  A0_re -= gT01_im * a1_im;
  A0_re += gT02_re * a2_re;
  A0_re -= gT02_im * a2_im;
  spinorFloat A0_im = 0;
  A0_im += gT00_re * a0_im;
  A0_im += gT00_im * a0_re;
  A0_im += gT01_re * a1_im;
  A0_im += gT01_im * a1_re;
  A0_im += gT02_re * a2_im;
  A0_im += gT02_im * a2_re;
  spinorFloat B0_re = 0;
  B0_re += gT00_re * b0_re;
  B0_re -= gT00_im * b0_im;
  B0_re += gT01_re * b1_re;
  B0_re -= gT01_im * b1_im;
  B0_re += gT02_re * b2_re;
  B0_re -= gT02_im * b2_im;
  spinorFloat B0_im = 0;
  B0_im += gT00_re * b0_im;
  B0_im += gT00_im * b0_re;
  B0_im += gT01_re * b1_im;
  B0_im += gT01_im * b1_re;
  B0_im += gT02_re * b2_im;
  B0_im += gT02_im * b2_re;
  
  // multiply row 1
  spinorFloat A1_re = 0;
  A1_re += gT10_re * a0_re;
  A1_re -= gT10_im * a0_im;
  A1_re += gT11_re * a1_re;
  A1_re -= gT11_im * a1_im;
  A1_re += gT12_re * a2_re;
  A1_re -= gT12_im * a2_im;
  spinorFloat A1_im = 0;
  A1_im += gT10_re * a0_im;
  A1_im += gT10_im * a0_re;
  A1_im += gT11_re * a1_im;
  A1_im += gT11_im * a1_re;
  A1_im += gT12_re * a2_im;
  A1_im += gT12_im * a2_re;
  spinorFloat B1_re = 0;
  B1_re += gT10_re * b0_re;
  B1_re -= gT10_im * b0_im;
  B1_re += gT11_re * b1_re;
  B1_re -= gT11_im * b1_im;
  B1_re += gT12_re * b2_re;
  B1_re -= gT12_im * b2_im;
  spinorFloat B1_im = 0;
  B1_im += gT10_re * b0_im;
  B1_im += gT10_im * b0_re;
  B1_im += gT11_re * b1_im;
  B1_im += gT11_im * b1_re;
  B1_im += gT12_re * b2_im;
  B1_im += gT12_im * b2_re;
  
  // multiply row 2
  spinorFloat A2_re = 0;
  A2_re += gT20_re * a0_re;
  A2_re -= gT20_im * a0_im;
  A2_re += gT21_re * a1_re;
  A2_re -= gT21_im * a1_im;
  A2_re += gT22_re * a2_re;
  A2_re -= gT22_im * a2_im;
  spinorFloat A2_im = 0;
  A2_im += gT20_re * a0_im;
  A2_im += gT20_im * a0_re;
  A2_im += gT21_re * a1_im;
  A2_im += gT21_im * a1_re;
  A2_im += gT22_re * a2_im;
  A2_im += gT22_im * a2_re;
  spinorFloat B2_re = 0;
  B2_re += gT20_re * b0_re;
  B2_re -= gT20_im * b0_im;
  B2_re += gT21_re * b1_re;
  B2_re -= gT21_im * b1_im;
  B2_re += gT22_re * b2_re;
  B2_re -= gT22_im * b2_im;
  spinorFloat B2_im = 0;
  B2_im += gT20_re * b0_im;
  B2_im += gT20_im * b0_re;
  B2_im += gT21_re * b1_im;
  B2_im += gT21_im * b1_re;
  B2_im += gT22_re * b2_im;
  B2_im += gT22_im * b2_re;
  
#ifdef SPINOR_DOUBLE
  spinorFloat a = param.a;
#else
  spinorFloat a = param.a_f;
#endif
  o00_re += a*A0_re;
  o00_im += a*A0_im;
  o10_re += a*B0_re;
  o10_im += a*B0_im;
  o20_re += a*A0_im;
  o20_im -= a*A0_re;
  o30_re -= a*B0_im;
  o30_im += a*B0_re;
  
  o01_re += a*A1_re;
  o01_im += a*A1_im;
  o11_re += a*B1_re;
  o11_im += a*B1_im;
  o21_re += a*A1_im;
  o21_im -= a*A1_re;
  o31_re -= a*B1_im;
  o31_im += a*B1_re;
  
  o02_re += a*A2_re;
  o02_im += a*A2_im;
  o12_re += a*B2_re;
  o12_im += a*B2_im;
  o22_re += a*A2_im;
  o22_im -= a*A2_re;
  o32_re -= a*B2_im;
  o32_im += a*B2_re;
  
}

if ( isActive(dim,3,+1,coord,param.commDim,param.X) && coord[3]==X4m1 )
{
  // Projector P3-
  // 0 0 0 0 
  // 0 0 0 0 
  // 0 0 2 0 
  // 0 0 0 2 
  
  faceIndexFromCoords<4,1>(face_idx,coord,3,param);
  const int sp_idx = face_idx + param.ghostOffset[3][1];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[3][1];
#endif
  
  const int ga_idx = sid;
  
  if (param.gauge_fixed && ga_idx < X4X3X2X1hmX3X2X1h)
  {
    spinorFloat a0_re, a0_im;
    spinorFloat a1_re, a1_im;
    spinorFloat a2_re, a2_im;
    spinorFloat b0_re, b0_im;
    spinorFloat b1_re, b1_im;
    spinorFloat b2_re, b2_im;
    
    
    const int sp_stride_pad = ghostFace[3];
    const int t_proj_scale = TPROJSCALE;
    
    // read half spinor from device memory
    READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 6);
    
    a0_re = t_proj_scale*i00_re;  a0_im = t_proj_scale*i00_im;
    a1_re = t_proj_scale*i01_re;  a1_im = t_proj_scale*i01_im;
    a2_re = t_proj_scale*i02_re;  a2_im = t_proj_scale*i02_im;
    b0_re = t_proj_scale*i10_re;  b0_im = t_proj_scale*i10_im;
    b1_re = t_proj_scale*i11_re;  b1_im = t_proj_scale*i11_im;
    b2_re = t_proj_scale*i12_re;  b2_im = t_proj_scale*i12_im;
    
    
    // identity gauge matrix
    spinorFloat A0_re = a0_re; spinorFloat A0_im = a0_im;
    spinorFloat B0_re = b0_re; spinorFloat B0_im = b0_im;
    spinorFloat A1_re = a1_re; spinorFloat A1_im = a1_im;
    spinorFloat B1_re = b1_re; spinorFloat B1_im = b1_im;
    spinorFloat A2_re = a2_re; spinorFloat A2_im = a2_im;
    spinorFloat B2_re = b2_re; spinorFloat B2_im = b2_im;
    
#ifdef SPINOR_DOUBLE
    spinorFloat a = param.a;
#else
    spinorFloat a = param.a_f;
#endif
    o20_re += a*A0_re;
    o20_im += a*A0_im;
    o30_re += a*B0_re;
    o30_im += a*B0_im;
    
    o21_re += a*A1_re;
    o21_im += a*A1_im;
    o31_re += a*B1_re;
    o31_im += a*B1_im;
    
    o22_re += a*A2_re;
    o22_im += a*A2_im;
    o32_re += a*B2_re;
    o32_im += a*B2_im;
    
  } else {
    spinorFloat a0_re, a0_im;
    spinorFloat a1_re, a1_im;
    spinorFloat a2_re, a2_im;
    spinorFloat b0_re, b0_im;
    spinorFloat b1_re, b1_im;
    spinorFloat b2_re, b2_im;
    
    
    const int sp_stride_pad = ghostFace[3];
    const int t_proj_scale = TPROJSCALE;
    
    // read half spinor from device memory
    READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 6);
    
    a0_re = t_proj_scale*i00_re;  a0_im = t_proj_scale*i00_im;
    a1_re = t_proj_scale*i01_re;  a1_im = t_proj_scale*i01_im;
    a2_re = t_proj_scale*i02_re;  a2_im = t_proj_scale*i02_im;
    b0_re = t_proj_scale*i10_re;  b0_im = t_proj_scale*i10_im;
    b1_re = t_proj_scale*i11_re;  b1_im = t_proj_scale*i11_im;
    b2_re = t_proj_scale*i12_re;  b2_im = t_proj_scale*i12_im;
    
    
    // read gauge matrix from device memory
    READ_GAUGE_MATRIX(G, GAUGE0TEX, 6, ga_idx, param.gauge_stride);
    
    // reconstruct gauge matrix
    RECONSTRUCT_GAUGE_MATRIX(6);
    
    // multiply row 0
    spinorFloat A0_re = 0;
    A0_re += g00_re * a0_re;
    A0_re -= g00_im * a0_im;
    A0_re += g01_re * a1_re;
    A0_re -= g01_im * a1_im;
    A0_re += g02_re * a2_re;
    A0_re -= g02_im * a2_im;
    spinorFloat A0_im = 0;
    A0_im += g00_re * a0_im;
    A0_im += g00_im * a0_re;
    A0_im += g01_re * a1_im;
    A0_im += g01_im * a1_re;
    A0_im += g02_re * a2_im;
    A0_im += g02_im * a2_re;
    spinorFloat B0_re = 0;
    B0_re += g00_re * b0_re;
    B0_re -= g00_im * b0_im;
    B0_re += g01_re * b1_re;
    B0_re -= g01_im * b1_im;
    B0_re += g02_re * b2_re;
    B0_re -= g02_im * b2_im;
    spinorFloat B0_im = 0;
    B0_im += g00_re * b0_im;
    B0_im += g00_im * b0_re;
    B0_im += g01_re * b1_im;
    B0_im += g01_im * b1_re;
    B0_im += g02_re * b2_im;
    B0_im += g02_im * b2_re;
    
    // multiply row 1
    spinorFloat A1_re = 0;
    A1_re += g10_re * a0_re;
    A1_re -= g10_im * a0_im;
    A1_re += g11_re * a1_re;
    A1_re -= g11_im * a1_im;
    A1_re += g12_re * a2_re;
    A1_re -= g12_im * a2_im;
    spinorFloat A1_im = 0;
    A1_im += g10_re * a0_im;
    A1_im += g10_im * a0_re;
    A1_im += g11_re * a1_im;
    A1_im += g11_im * a1_re;
    A1_im += g12_re * a2_im;
    A1_im += g12_im * a2_re;
    spinorFloat B1_re = 0;
    B1_re += g10_re * b0_re;
    B1_re -= g10_im * b0_im;
    B1_re += g11_re * b1_re;
    B1_re -= g11_im * b1_im;
    B1_re += g12_re * b2_re;
    B1_re -= g12_im * b2_im;
    spinorFloat B1_im = 0;
    B1_im += g10_re * b0_im;
    B1_im += g10_im * b0_re;
    B1_im += g11_re * b1_im;
    B1_im += g11_im * b1_re;
    B1_im += g12_re * b2_im;
    B1_im += g12_im * b2_re;
    
    // multiply row 2
    spinorFloat A2_re = 0;
    A2_re += g20_re * a0_re;
    A2_re -= g20_im * a0_im;
    A2_re += g21_re * a1_re;
    A2_re -= g21_im * a1_im;
    A2_re += g22_re * a2_re;
    A2_re -= g22_im * a2_im;
    spinorFloat A2_im = 0;
    A2_im += g20_re * a0_im;
    A2_im += g20_im * a0_re;
    A2_im += g21_re * a1_im;
    A2_im += g21_im * a1_re;
    A2_im += g22_re * a2_im;
    A2_im += g22_im * a2_re;
    spinorFloat B2_re = 0;
    B2_re += g20_re * b0_re;
    B2_re -= g20_im * b0_im;
    B2_re += g21_re * b1_re;
    B2_re -= g21_im * b1_im;
    B2_re += g22_re * b2_re;
    B2_re -= g22_im * b2_im;
    spinorFloat B2_im = 0;
    B2_im += g20_re * b0_im;
    B2_im += g20_im * b0_re;
    B2_im += g21_re * b1_im;
    B2_im += g21_im * b1_re;
    B2_im += g22_re * b2_im;
    B2_im += g22_im * b2_re;
    
#ifdef SPINOR_DOUBLE
    spinorFloat a = param.a;
#else
    spinorFloat a = param.a_f;
#endif
    o20_re += a*A0_re;
    o20_im += a*A0_im;
    o30_re += a*B0_re;
    o30_im += a*B0_im;
    
    o21_re += a*A1_re;
    o21_im += a*A1_im;
    o31_re += a*B1_re;
    o31_im += a*B1_im;
    
    o22_re += a*A2_re;
    o22_im += a*A2_im;
    o32_re += a*B2_re;
    o32_im += a*B2_im;
    
  }
}

if ( isActive(dim,3,-1,coord,param.commDim,param.X) && coord[3]==0 )
{
  // Projector P3+
  // 2 0 0 0 
  // 0 2 0 0 
  // 0 0 0 0 
  // 0 0 0 0 
  
  faceIndexFromCoords<4,1>(face_idx,coord,3,param);
  const int sp_idx = face_idx + param.ghostOffset[3][0];
#if (DD_PREC==2)
    sp_norm_idx = face_idx + param.ghostNormOffset[3][0];
#endif
  
  const int ga_idx = Vh+face_idx;
  
  if (param.gauge_fixed && ga_idx < X4X3X2X1hmX3X2X1h)
  {
    spinorFloat a0_re, a0_im;
    spinorFloat a1_re, a1_im;
    spinorFloat a2_re, a2_im;
    spinorFloat b0_re, b0_im;
    spinorFloat b1_re, b1_im;
    spinorFloat b2_re, b2_im;
    
    
    const int sp_stride_pad = ghostFace[3];
    const int t_proj_scale = TPROJSCALE;
    
    // read half spinor from device memory
    READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 7);
    
    a0_re = t_proj_scale*i00_re;  a0_im = t_proj_scale*i00_im;
    a1_re = t_proj_scale*i01_re;  a1_im = t_proj_scale*i01_im;
    a2_re = t_proj_scale*i02_re;  a2_im = t_proj_scale*i02_im;
    b0_re = t_proj_scale*i10_re;  b0_im = t_proj_scale*i10_im;
    b1_re = t_proj_scale*i11_re;  b1_im = t_proj_scale*i11_im;
    b2_re = t_proj_scale*i12_re;  b2_im = t_proj_scale*i12_im;
    
    
    // identity gauge matrix
    spinorFloat A0_re = a0_re; spinorFloat A0_im = a0_im;
    spinorFloat B0_re = b0_re; spinorFloat B0_im = b0_im;
    spinorFloat A1_re = a1_re; spinorFloat A1_im = a1_im;
    spinorFloat B1_re = b1_re; spinorFloat B1_im = b1_im;
    spinorFloat A2_re = a2_re; spinorFloat A2_im = a2_im;
    spinorFloat B2_re = b2_re; spinorFloat B2_im = b2_im;
    
#ifdef SPINOR_DOUBLE
    spinorFloat a = param.a;
#else
    spinorFloat a = param.a_f;
#endif
    o00_re += a*A0_re;
    o00_im += a*A0_im;
    o10_re += a*B0_re;
    o10_im += a*B0_im;
    
    o01_re += a*A1_re;
    o01_im += a*A1_im;
    o11_re += a*B1_re;
    o11_im += a*B1_im;
    
    o02_re += a*A2_re;
    o02_im += a*A2_im;
    o12_re += a*B2_re;
    o12_im += a*B2_im;
    
  } else {
    spinorFloat a0_re, a0_im;
    spinorFloat a1_re, a1_im;
    spinorFloat a2_re, a2_im;
    spinorFloat b0_re, b0_im;
    spinorFloat b1_re, b1_im;
    spinorFloat b2_re, b2_im;
    
    
    const int sp_stride_pad = ghostFace[3];
    const int t_proj_scale = TPROJSCALE;
    
    // read half spinor from device memory
    READ_SPINOR_GHOST(GHOSTSPINORTEX, sp_stride_pad, sp_idx, sp_norm_idx, 7);
    
    a0_re = t_proj_scale*i00_re;  a0_im = t_proj_scale*i00_im;
    a1_re = t_proj_scale*i01_re;  a1_im = t_proj_scale*i01_im;
    a2_re = t_proj_scale*i02_re;  a2_im = t_proj_scale*i02_im;
    b0_re = t_proj_scale*i10_re;  b0_im = t_proj_scale*i10_im;
    b1_re = t_proj_scale*i11_re;  b1_im = t_proj_scale*i11_im;
    b2_re = t_proj_scale*i12_re;  b2_im = t_proj_scale*i12_im;
    
    
    // read gauge matrix from device memory
    READ_GAUGE_MATRIX(G, GAUGE1TEX, 7, ga_idx, param.gauge_stride);
    
    // reconstruct gauge matrix
    RECONSTRUCT_GAUGE_MATRIX(7);
    
    // multiply row 0
    spinorFloat A0_re = 0;
    A0_re += gT00_re * a0_re;
    A0_re -= gT00_im * a0_im;
    A0_re += gT01_re * a1_re;
    A0_re -= gT01_im * a1_im;
    A0_re += gT02_re * a2_re;
    A0_re -= gT02_im * a2_im;
    spinorFloat A0_im = 0;
    A0_im += gT00_re * a0_im;
    A0_im += gT00_im * a0_re;
    A0_im += gT01_re * a1_im;
    A0_im += gT01_im * a1_re;
    A0_im += gT02_re * a2_im;
    A0_im += gT02_im * a2_re;
    spinorFloat B0_re = 0;
    B0_re += gT00_re * b0_re;
    B0_re -= gT00_im * b0_im;
    B0_re += gT01_re * b1_re;
    B0_re -= gT01_im * b1_im;
    B0_re += gT02_re * b2_re;
    B0_re -= gT02_im * b2_im;
    spinorFloat B0_im = 0;
    B0_im += gT00_re * b0_im;
    B0_im += gT00_im * b0_re;
    B0_im += gT01_re * b1_im;
    B0_im += gT01_im * b1_re;
    B0_im += gT02_re * b2_im;
    B0_im += gT02_im * b2_re;
    
    // multiply row 1
    spinorFloat A1_re = 0;
    A1_re += gT10_re * a0_re;
    A1_re -= gT10_im * a0_im;
    A1_re += gT11_re * a1_re;
    A1_re -= gT11_im * a1_im;
    A1_re += gT12_re * a2_re;
    A1_re -= gT12_im * a2_im;
    spinorFloat A1_im = 0;
    A1_im += gT10_re * a0_im;
    A1_im += gT10_im * a0_re;
    A1_im += gT11_re * a1_im;
    A1_im += gT11_im * a1_re;
    A1_im += gT12_re * a2_im;
    A1_im += gT12_im * a2_re;
    spinorFloat B1_re = 0;
    B1_re += gT10_re * b0_re;
    B1_re -= gT10_im * b0_im;
    B1_re += gT11_re * b1_re;
    B1_re -= gT11_im * b1_im;
    B1_re += gT12_re * b2_re;
    B1_re -= gT12_im * b2_im;
    spinorFloat B1_im = 0;
    B1_im += gT10_re * b0_im;
    B1_im += gT10_im * b0_re;
    B1_im += gT11_re * b1_im;
    B1_im += gT11_im * b1_re;
    B1_im += gT12_re * b2_im;
    B1_im += gT12_im * b2_re;
    
    // multiply row 2
    spinorFloat A2_re = 0;
    A2_re += gT20_re * a0_re;
    A2_re -= gT20_im * a0_im;
    A2_re += gT21_re * a1_re;
    A2_re -= gT21_im * a1_im;
    A2_re += gT22_re * a2_re;
    A2_re -= gT22_im * a2_im;
    spinorFloat A2_im = 0;
    A2_im += gT20_re * a0_im;
    A2_im += gT20_im * a0_re;
    A2_im += gT21_re * a1_im;
    A2_im += gT21_im * a1_re;
    A2_im += gT22_re * a2_im;
    A2_im += gT22_im * a2_re;
    spinorFloat B2_re = 0;
    B2_re += gT20_re * b0_re;
    B2_re -= gT20_im * b0_im;
    B2_re += gT21_re * b1_re;
    B2_re -= gT21_im * b1_im;
    B2_re += gT22_re * b2_re;
    B2_re -= gT22_im * b2_im;
    spinorFloat B2_im = 0;
    B2_im += gT20_re * b0_im;
    B2_im += gT20_im * b0_re;
    B2_im += gT21_re * b1_im;
    B2_im += gT21_im * b1_re;
    B2_im += gT22_re * b2_im;
    B2_im += gT22_im * b2_re;
    
#ifdef SPINOR_DOUBLE
    spinorFloat a = param.a;
#else
    spinorFloat a = param.a_f;
#endif
    o00_re += a*A0_re;
    o00_im += a*A0_im;
    o10_re += a*B0_re;
    o10_im += a*B0_im;
    
    o01_re += a*A1_re;
    o01_im += a*A1_im;
    o11_re += a*B1_re;
    o11_im += a*B1_im;
    
    o02_re += a*A2_re;
    o02_im += a*A2_im;
    o12_re += a*B2_re;
    o12_im += a*B2_im;
    
  }
}



// write spinor field back to device memory
WRITE_SPINOR(param.sp_stride);

// undefine to prevent warning when precision is changed
#undef spinorFloat
#undef WRITE_SPINOR_SHARED
#undef READ_SPINOR_SHARED
#undef SHARED_STRIDE

#undef g00_re
#undef g00_im
#undef g01_re
#undef g01_im
#undef g02_re
#undef g02_im
#undef g10_re
#undef g10_im
#undef g11_re
#undef g11_im
#undef g12_re
#undef g12_im
#undef g20_re
#undef g20_im
#undef g21_re
#undef g21_im
#undef g22_re
#undef g22_im

#undef i00_re
#undef i00_im
#undef i01_re
#undef i01_im
#undef i02_re
#undef i02_im
#undef i10_re
#undef i10_im
#undef i11_re
#undef i11_im
#undef i12_re
#undef i12_im
#undef i20_re
#undef i20_im
#undef i21_re
#undef i21_im
#undef i22_re
#undef i22_im
#undef i30_re
#undef i30_im
#undef i31_re
#undef i31_im
#undef i32_re
#undef i32_im

#undef acc00_re
#undef acc00_im
#undef acc01_re
#undef acc01_im
#undef acc02_re
#undef acc02_im
#undef acc10_re
#undef acc10_im
#undef acc11_re
#undef acc11_im
#undef acc12_re
#undef acc12_im
#undef acc20_re
#undef acc20_im
#undef acc21_re
#undef acc21_im
#undef acc22_re
#undef acc22_im
#undef acc30_re
#undef acc30_im
#undef acc31_re
#undef acc31_im
#undef acc32_re
#undef acc32_im

#undef c00_00_re
#undef c01_01_re
#undef c02_02_re
#undef c10_10_re
#undef c11_11_re
#undef c12_12_re
#undef c01_00_re
#undef c01_00_im
#undef c02_00_re
#undef c02_00_im
#undef c10_00_re
#undef c10_00_im
#undef c11_00_re
#undef c11_00_im
#undef c12_00_re
#undef c12_00_im
#undef c02_01_re
#undef c02_01_im
#undef c10_01_re
#undef c10_01_im
#undef c11_01_re
#undef c11_01_im
#undef c12_01_re
#undef c12_01_im
#undef c10_02_re
#undef c10_02_im
#undef c11_02_re
#undef c11_02_im
#undef c12_02_re
#undef c12_02_im
#undef c11_10_re
#undef c11_10_im
#undef c12_10_re
#undef c12_10_im
#undef c12_11_re
#undef c12_11_im

#undef o00_re
#undef o00_im
#undef o01_re
#undef o01_im
#undef o02_re
#undef o02_im
#undef o10_re
#undef o10_im
#undef o11_re
#undef o11_im
#undef o12_re
#undef o12_im
#undef o20_re
#undef o20_im
#undef o21_re
#undef o21_im
#undef o22_re
#undef o22_im
#undef o30_re
#undef o30_im
#undef o31_re
#undef o31_im
#undef o32_re
#undef o32_im

#undef VOLATILE

#endif // MULTI_GPU
#ifndef included_a_f_h
#define included_a_f_h

/*****************************************************************************/
/*                                                                           */
/* Copyright (c) 1990-2008 Morgan Stanley All rights reserved.*/
/* See .../src/LICENSE for terms of distribution.                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#ifdef _AIX
#define BUGGY_AIX_COMPILER
#endif

#include <math.h>
#include <errno.h>

#include <a/k.h>
#include <a/arthur.h>

#define ERR(s,x) {if((I)(x)==-1)R perror(s),q=9,0;}
#define QF(a) (!QA(a)||((A)a)->t>Et)
#define CT  (1E-13)
#define CT1 (1-1E-13)
#define CT2 (1+1E-13)
#define XA I at=a->t,ar=a->r,an=a->n,*ad=a->d
#define XW I wt=w->t,wr=w->r,wn=w->n,*wd=w->d

#define H1(f) I f(A a)
#define H2(f) I f(A a,A w)

#define G1(T,f) Z I f(T *r,T *w,I n)
#define G2(T,f) Z I f(T *r,T *a,T *w,I n)
#define G0(T,f) Z I f(T *r,I *a,T *w,I n)
#define GV1(T,f) Z void f(T *r,T *w,I n)
#define GV2(T,f) Z void f(T *r,T *a,T *w,I n)
#define GV0(T,f) Z void f(T *r,I *a,T *w,I n)

#ifdef BUGGY_AIX_COMPILER
#define C1(f) {g=(PFI)(f);R(*((I(*)(I *,I *,I))(g)))(z->p,a->p,a->n),(I)z;}
#define C2(f) {g=(PFI)(f);R(*(((I(*)(I *,I *,I *,I))(g))))(z->p,a->p,w->p,z->n),(I)z;}
#else
#define C1(f) R(*(I(*)(I *,I *,I))(g=(PFI)(f)))(z->p,a->p,a->n),(I)z;
#define C2(f) R(*(I(*)(I *,I *,I *,I))(g=(PFI)(f)))(z->p,a->p,w->p,z->n),(I)z;
#endif

#define F1 {if(a->t!=Ft&&!(a=ep_cf(0)))R 0;}
#define F2 {if(a->t!=Ft&&!(a=ep_cf(0))||w->t!=Ft&&!(w=ep_cf(1)))R 0;}
#define I1 {if(a->t!=It&&!(a=ci(0)))R 0;}
#define I2 {if(a->t!=It&&!(a=ci(0))||w->t!=It&&!(w=ci(1)))R 0;}
#define I0 {I1 Q(a->n!=1,12)}
#define X1 {if(a->t>Ft&&!(a=ci(0)))R 0;}
#define X2 {if(a->t!=w->t)if(a->t+w->t==1)F2 else if(!a->n)a=(A)cn(0,w->t); \
            else if(!w->n)w=(A)cn(1,a->t);else R q=6,0;}
#define V0 {if(!wr)*wd=wr=1;}
#define OF(r,x,y) {F f=(F)(x)*(F)(y);Q(f>2e9,9)r=f;}

#define EQ(i,x) for(;Xf?xf():0,z=(x),q;)if(dc((A)z),q>-2)aplus_err(q,(A)fnc); \
                                        else if(q=0,ep_cf(i),n-i==2)ep_cf(i+1);

#define ND1 Q(!QA(a)||Et<(a)->t,18)
#define ND2 Q(!QA(a)||!QA(w)||Et<(a)->t||Et<(w)->t,18)

typedef union brokenDouble {
     int i[2];
     double d;
   } BD;

#define NANMASK   0x7ff00000
#if defined(__osf__)
#include <math.h>
#include <machine/fpu.h>
extern unsigned long fpe_bits;
extern int nan_bits;
#define FPERRMASK  ( IEEE_STATUS_INV )

#define RESETXCP  { nan_bits=errno=0; \
                    ieee_set_fp_control(ieee_get_fp_control()&~FPERRMASK); \
                  }
#define CHECKXCP  { if(nan_bits||((fpe_bits=ieee_get_fp_control())&FPERRMASK))\
                       q=9;\
                  } 
#define NAN_CHECK(v,xx) {F vv; vv=xx; if( isnan(vv) ) nan_bits=1; v=vv;}

#elif defined(_AIX)
#include <float.h>
#include <fpxcp.h>
#define FPERRMASK  ( FP_INVALID )
extern fpflag_t fpe_bits;
extern int nan_bits;
#define RESETXCP  { nan_bits=errno=0;fp_clr_flag(FP_ALL_XCP);}
#define CHECKXCP  { if(nan_bits||((fpe_bits=fp_read_flag())&FPERRMASK)) q=9; } 
#define NAN_CHECK(v,xx) {F vv; vv=xx; if( isnan(vv) ) nan_bits=1; v=vv;}


#elif defined(linux) || defined(_HP) || defined(__APPLE__)
#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#else
#include <fenv.h> 
#endif

#if defined(_HP)
#define finite isfinite
#endif

/* Set mask for exeption invalid operation */ 
#define FPERRMASK  ( FE_INVALID ) 

extern fexcept_t fpe_bits;
extern int nan_bits;
#define RESETXCP  { nan_bits=errno=0;feclearexcept(FE_ALL_EXCEPT);}
#define CHECKXCP  { if(nan_bits||(fpe_bits=fetestexcept(FPERRMASK))) q=9; } 
#define NAN_CHECK(v,xx) {F vv; vv=xx; if( isnan(vv) ) nan_bits=1; v=vv;}

#elif defined(HAVE_SVR4)
#if !defined(fp_except)
#define fp_except int
#endif
#include <ieeefp.h>

/* Set mask for exeption invalid operation */
#define FPERRMASK  ( FP_X_INV )
extern fp_except fpe_bits;
extern int nan_bits;
#define RESETXCP  { nan_bits=errno=0;fpsetsticky(0);}
#define CHECKXCP  { if(nan_bits||((fpe_bits=fpgetsticky())&FPERRMASK)) q=9; } 
#define NAN_CHECK(v,xx) {F vv; vv=xx; if( isnan(vv) ) nan_bits=1; v=vv;}

#elif defined(__FreeBSD__) || defined(__NetBSD__)
#if defined(__FreeBSD__)
typedef int fp_except;
#endif
# include <ieeefp.h>

/* Set mask for exeption invalid operation */
/* #define FPERRMASK  ( FP_X_INV | FP_X_DZ ) */
#define FPERRMASK  ( FP_X_INV )
extern fp_except fpe_bits;
extern int nan_bits;
#if defined(__NetBSD__)
#define RESETXCP  { nan_bits=errno=0;fpsetsticky(0);}
#else
#define RESETXCP  { nan_bits=errno=0;fpresetsticky(FPERRMASK);}
#endif
#define CHECKXCP  { if(nan_bits||((fpe_bits=fpgetsticky())&FPERRMASK)) q=9; } 
#define NAN_CHECK(v,xx) {F vv; vv=xx; if( isnan(vv) ) nan_bits=1; v=vv;}

#else
#define RESETXCP    
#define CHECKXCP   
#define NAN_CHECK(v,xx) v= xx;
#endif

#endif

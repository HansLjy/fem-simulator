//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_TRITRITEST_H
#define FEM_TRITRITEST_H

// Blame This guy: https://github.com/benardp/contours

//typedef __float128 real;
typedef double real;

# ifndef SWIG
static const real M_EPSILON  = 0.00000001;
# endif // SWIG

int tri_tri_intersection_test_3d(real p1[3], real q1[3], real r1[3],
								 real p2[3], real q2[3], real r2[3],
								 int * coplanar,
								 real source[3],real target[3]);

#endif //FEM_TRITRITEST_H

//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_PGS_H
#define FEM_PGS_H

#include "LCPSolver.h"

class PGSParameter : public LCPSolverParameter {
public:
	PGSParameter(int max_step, double lambda) : LCPSolverParameter(max_step), _lambda(lambda){}

	DECLARE_ACCESSIBLE_MEMBER(double, Lambda, _lambda)
};

class PGS : public LCPSolver {
public:
	VectorXd Solve(const SparseMatrixXd &A, const VectorXd &b, const VectorXd &x0 = VectorXd()) const override;

protected:
	double _lambda;
};

#endif //FEM_PGS_H

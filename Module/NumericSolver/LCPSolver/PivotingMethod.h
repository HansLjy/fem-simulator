//
// Created by hansljy on 2022/4/11.
//

#ifndef FEM_PIVOT_H
#define FEM_PIVOT_H

#include "LCPSolver.h"

class PivotingMethodParameter : public LCPSolverParameter {
public:
	PivotingMethodParameter(int max_step, double max_error) : LCPSolverParameter(max_step, max_error) {}

	DERIVED_DECLARE_CLONE(LCPSolverParameter)
};

class PivotingMethod : public LCPSolver {
public:
	VectorXd Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0 = VectorXd(), int block_size = 1) const override;
};

#endif //FEM_PIVOT_H

//
// Created by hansljy on 2022/4/14.
//

#ifndef FEM_BGS_H
#define FEM_BGS_H

#include "LCPSolver.h"

class BGSParameter : public LCPSolverParameter {
public:
	BGSParameter(int max_step, double max_error) : LCPSolverParameter(max_step, max_error) {}
	DERIVED_DECLARE_CLONE(LCPSolverParameter)
};

class BGS : public LCPSolver {
public:
	VectorXd Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0, int block_size = 1) const override;
};

#endif //FEM_BGS_H

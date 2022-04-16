//
// Created by hansljy on 2022/4/15.
//

#ifndef TEST_OSQPWRAPPER_H
#define TEST_OSQPWRAPPER_H

#include "LCPSolver.h"

class OSQPWrapperParameter : public LCPSolverParameter {
public:
	OSQPWrapperParameter(int max_step, double max_error) : LCPSolverParameter(max_step, max_error) {};
	DERIVED_DECLARE_CLONE(LCPSolverParameter)
};


class OSQPWrapper : public LCPSolver {
public:
	VectorXd Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0 = VectorXd(), int block_size = 1) const override;
};

#endif //TEST_OSQPWRAPPER_H

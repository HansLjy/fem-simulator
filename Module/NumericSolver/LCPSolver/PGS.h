//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_PGS_H
#define FEM_PGS_H

#include "LCPSolver.h"

class PGSParameter : public LCPSolverParameter {
public:
	PGSParameter(int max_step, double max_error,  double lambda) : LCPSolverParameter(max_step, max_error), _lambda(lambda){}

	DERIVED_DECLARE_CLONE(LCPSolverParameter)
	DECLARE_ACCESSIBLE_MEMBER(double, Lambda, _lambda)
};

class PGS : public LCPSolver {
public:
	void Initialize(const LCPSolverParameter &para) override {
		LCPSolver::Initialize(para);
		_lambda = para.GetLambda();
	}
	VectorXd Solve(const MatrixXd &A, const VectorXd &b, const VectorXd &x0 = VectorXd()) const override;

protected:
	double _lambda;
};

#endif //FEM_PGS_H

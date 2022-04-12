//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_LCPSOLVER_H
#define FEM_LCPSOLVER_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"

class LCPSolverParameter {
public:
	LCPSolverParameter(int max_step) : _max_step(max_step) {}

	DECLARE_ACCESSIBLE_MEMBER(int, MaxStep, _max_step)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, Lambda)
};

enum class LCPSolverType {
	kPGS
};

class LCPSolver {
public:
	virtual VectorXd Solve(const SparseMatrixXd &A, const VectorXd& b, const VectorXd& x0 = VectorXd()) const = 0;
	virtual ~LCPSolver() = default;

protected:
	double _max_error;
	int _max_step;
};

#endif //FEM_LCPSOLVER_H

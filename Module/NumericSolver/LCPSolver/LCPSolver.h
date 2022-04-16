//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_LCPSOLVER_H
#define FEM_LCPSOLVER_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"

class LCPSolverParameter {
public:
	LCPSolverParameter(int max_step, double max_error) : _max_step(max_step), _max_error(max_error) {}
	BASE_DECLARE_CLONE(LCPSolverParameter)

	virtual ~LCPSolverParameter() = default;

	DECLARE_ACCESSIBLE_MEMBER(int, MaxStep, _max_step)

	// Maximum error is the tolerance of the average of x_iy_i
	DECLARE_ACCESSIBLE_MEMBER(double, MaxError, _max_error)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, Lambda)
};

enum class LCPSolverType {
	kPGS,
	kBGS,
	kPivot,
	kOSQP,
};

class LCPSolver {
public:
	virtual void Initialize(const LCPSolverParameter& para) {
		_max_step = para.GetMaxStep();
		_max_error = para.GetMaxError();
	}
	virtual VectorXd Solve(const MatrixXd &A, const VectorXd& b, const VectorXd& x0 = VectorXd(), int block_size = 1) const = 0;
	virtual ~LCPSolver() = default;

protected:
	int _max_step;
	double _max_error;
};

#endif //FEM_LCPSOLVER_H

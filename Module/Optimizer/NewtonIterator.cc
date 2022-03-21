//
// Created by hansljy on 2022/3/21.
//

#include "NewtonIterator.h"
#include "Util/EigenAll.h"
#include <spdlog/spdlog.h>

using Eigen::ConjugateGradient;

NewtonIteratorParameter::NewtonIteratorParameter(double max_error,
												 int max_step) : OptimizerParameter(max_error, max_step) {}

DEFINE_CLONE(OptimizerParameter, NewtonIteratorParameter)

DEFINE_CLONE(Optimizer, NewtonIterator)

VectorXd NewtonIterator::Optimize(const VectorXd &x0) const {
	VectorXd x = x0;

	int step = 0;
	ConjugateGradient<SparseMatrixXd> solver;
	while (step++ < _max_step) {
		spdlog::info("Newton Iterator, round {}", step);
		SparseMatrixXd hessian = _target->Hessian(x);
		VectorXd gradient = _target->Gradient(x);
		solver.compute(hessian);
		VectorXd delta = solver.solve(gradient);
		x -= delta;
		if (delta.norm() < _max_error) {
			break;
		}
		spdlog::info("Error: {}, continue iterating", delta.norm());
	}
	return x;
}
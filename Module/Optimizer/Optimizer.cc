//
// Created by hansljy on 2022/2/23.
//

#include "Optimizer.h"

void Optimizer::Initialize(const OptimizerParameter &para) {
	_max_error = para._max_error;
	_max_step = para._max_step;
}

void Optimizer::SetInitial(const VectorXd &x) {
	_x0 = x;
}


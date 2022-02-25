//
// Created by hansljy on 2022/2/23.
//

#include "Optimizer.h"

OptimizerParameter::OptimizerParameter(double max_error, int max_step) :_max_error(max_error), _max_step(max_step) {}

DEFINE_ACCESSIBLE_MEMBER(OptimizerParameter, double, MaxError, _max_error)
DEFINE_ACCESSIBLE_MEMBER(OptimizerParameter, int, MaxStep, _max_step)

DEFINE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(OptimizerParameter, double, Mu)


void Optimizer::Initialize(const OptimizerParameter &para) {
	_max_step = para.GetMaxStep();
	_max_error = para.GetMaxError();
}

void Optimizer::SetTarget(const Function& func) {
	_target = func.Clone();
}

void Optimizer::AddConstraint(const Function& cons) {
	_constraints.push_back(cons.Clone());
}

Optimizer::~Optimizer() {
	delete _target;
	for (auto constraint : _constraints) {
		delete constraint;
	}
}

Optimizer::Optimizer(const Optimizer& optimizer) {
	_max_error = optimizer._max_error;
	_max_step = optimizer._max_step;
	delete _target;
	_target = optimizer._target->Clone();
	_constraints.clear();
	for (auto& constraint : optimizer._constraints) {
		_constraints.push_back(constraint->Clone());
	}
}
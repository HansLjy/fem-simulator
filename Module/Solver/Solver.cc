//
// Created by hansljy on 2022/2/22.
//

#include "Solver.h"
#include "Util/Factory.h"

SolverParameter::SolverParameter(double step, OptimizerType opt_type,
								 const OptimizerParameter &opt_para,
								 const TargetParameter &tar_para)
								 : _step(step), _opt_type(opt_type),
								   _opt_para(opt_para.Clone()),
								   _tar_para(tar_para.Clone())
								 {}

DEFINE_CLONE(SolverParameter, SolverParameter)
DEFINE_ACCESSIBLE_MEMBER(SolverParameter, double, Step, _step)
DEFINE_ACCESSIBLE_MEMBER(SolverParameter, OptimizerType, OptimizerType, _opt_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(SolverParameter, OptimizerParameter, OptimizerParameter, _opt_para)
DEFINE_ACCESSIBLE_POINTER_MEMBER(SolverParameter, TargetParameter, TargetParameter, _tar_para)

void Solver::Initialize(const SolverParameter &para) {
	_step = para.GetStep();
	_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(para.GetOptimizerType());
	_optimizer->Initialize(*para.GetOptimizerParameter());
	_target = CreateCorrespondingTarget();
	_target->Initialize(*para.GetTargetParameter());
	_target->SetMesh(_reference);
}

void Solver::Step(double dt) {
	VectorXd& x = _current.GetPoints();
	_target->SetX(x);
	_target->SetV(_v);
	_target->SetDt(dt);
	_optimizer->SetTarget(*_target);
	x = _optimizer->Optimize(x);
}

Solver::Solver(const Solver& solver) {
	_reference = solver._reference;
	_step = solver._step;
	_current = solver._current;
	_v = solver._v;
	_optimizer = solver._optimizer->Clone();
	_target = dynamic_cast<Target*>(solver._target->Clone());
}

Solver::~Solver() {
	delete _optimizer;
	delete _target;
}

Mesh& Solver::GetCurrentMesh() {
	return _current;
}

void Solver::SetMesh(const Mesh& mesh) {
	_reference = mesh;
	_current = mesh;
}
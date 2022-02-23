//
// Created by hansljy on 2022/2/22.
//

#include "Solver.h"
#include "Util/Factory.h"

void Solver::Initialize(const SolverParameter &para) {
	_step = para._step;
	_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(para._opt_type);
	_target = SetCorrespondingTarget();
	_target->Initialize(para._tar_para);
	_target->SetMesh(_reference);
}

void Solver::Step(double dt) {
	VectorXd& x = _current.GetPoints();
	_target->SetX(x);
	_target->SetV(_v);
	_target->SetDt(dt);
	_optimizer->SetTarget(*target);
	_optimizer->SetInitial(x);
	x = _optimizer->Optimize();
}
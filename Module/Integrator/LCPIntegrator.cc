//
// Created by hansljy on 2022/4/9.
//

#include "LCPIntegrator.h"
#include "Util/Factory.h"
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#include <Eigen/Cholesky>
#include <Eigen/CholmodSupport>
#include <spdlog/spdlog.h>
#include <iostream>

DEFINE_CLONE(IntegratorParameter, LCPIntegratorParameter)
DEFINE_ACCESSIBLE_MEMBER(LCPIntegratorParameter, LCPSolverType, LCPSolverType, _lcp_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(LCPIntegratorParameter, LCPSolverParameter, LCPSolverParameter, _lcp_para)

void LCPIntegrator::Initialize(const IntegratorParameter &para) {
	_solver = LCPSolverFactory::GetInstance()->GetLCPSolver(para.GetLCPSolverType());
	_solver->Initialize(*para.GetLCPSolverParameter());
}
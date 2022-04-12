//
// Created by hansljy on 2022/4/11.
//

#include "LCPSolver.h"

DEFINE_ACCESSIBLE_MEMBER(LCPSolverParameter, int, MaxStep, _max_step)
DEFINE_ACCESSIBLE_MEMBER(LCPSolverParameter, double, MaxError, _max_error)
DEFINE_VIRTUAL_ACCESSIBLE_MEMBER(LCPSolverParameter, double, Lambda)
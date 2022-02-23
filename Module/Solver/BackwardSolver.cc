//
// Created by hansljy on 2022/2/22.
//

#include "BackwardSolver.h"
#include "BackwardTarget.h"

Target *BackwardSolver::CreateCorrespondingTarget() {
	return new BackwardTarget;
}
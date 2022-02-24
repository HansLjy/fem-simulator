//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_BACKWARDSOLVER_H
#define FEM_BACKWARDSOLVER_H

#include "Solver.h"

class BackwardSolver : public Solver {
public:
	Target * CreateCorrespondingTarget() override;
	DERIVED_DECLARE_CLONE(Solver)
};

#endif //FEM_BACKWARDSOLVER_H

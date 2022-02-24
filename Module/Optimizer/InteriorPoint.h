//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_INTERIORPOINT_H
#define FEM_INTERIORPOINT_H

#include "Optimizer.h"

struct InteriorPointParameter : public OptimizerParameter {
	double _mu;
};

class InteriorPoint : public Optimizer {
public:
	VectorXd Optimize(const VectorXd &x0) const override;

	DERIVED_DECLARE_CLONE(Optimizer)

private:
	double _mu;
};

#endif //FEM_INTERIORPOINT_H

//
// Created by hansljy on 2022/3/21.
//

#ifndef FEM_NEWTONITERATOR_H
#define FEM_NEWTONITERATOR_H

#include "Optimizer.h"

class NewtonIteratorParameter : public OptimizerParameter {
public:
	NewtonIteratorParameter(double max_error, int max_step);

	DERIVED_DECLARE_CLONE(OptimizerParameter)
};

class NewtonIterator : public Optimizer {
public:
	VectorXd Optimize(const VectorXd &x0) const override;

	DERIVED_DECLARE_CLONE(Optimizer)
};

#endif //FEM_NEWTONITERATOR_H

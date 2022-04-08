//
// Created by hansljy on 2022/3/21.
//

#ifndef FEM_NEWTONITERATOR_H
#define FEM_NEWTONITERATOR_H

#include "Optimizer.h"

class NewtonIteratorParameter : public OptimizerParameter {
public:
	NewtonIteratorParameter(double max_error, int max_step, double armijo, double curvature);
	DERIVED_DECLARE_CLONE(OptimizerParameter)

	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER_ACCESSOR(double, Armijo)
	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER_ACCESSOR(double, Curvature)

private:
	double _armijo, _curvature;
};

class NewtonIterator : public Optimizer {
public:
	void Initialize(const OptimizerParameter &para) override;
	VectorXd Optimize(const VectorXd &x0) const override;

	DERIVED_DECLARE_CLONE(Optimizer)

private:
	double _armijo;
};

#endif //FEM_NEWTONITERATOR_H

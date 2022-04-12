//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_INTERIORPOINT_H
#define FEM_INTERIORPOINT_H

#include "Optimizer.h"

class InteriorPointParameter : public OptimizerParameter {
public:
	InteriorPointParameter(double max_error, int max_step, double mu);

	DECLARE_OVERWRITE_ACCESSIBLE_MEMBER(double, Mu)
	DERIVED_DECLARE_CLONE(OptimizerParameter)

private:
	double _mu;
};

class InteriorPoint : public Optimizer {
public:
	void Initialize(const OptimizerParameter &para) override;
	VectorXd Optimize(const VectorXd &x0) const override;
	~InteriorPoint() = default;
	DERIVED_DECLARE_CLONE(Optimizer)

private:
	double _mu;
};

#endif //FEM_INTERIORPOINT_H

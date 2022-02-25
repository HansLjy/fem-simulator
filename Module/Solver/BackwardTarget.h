//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_BACKWARDTARGET_H
#define FEM_BACKWARDTARGET_H

#include "Target.h"

class BackwardTarget : public Target {
public:
	double Value(const VectorXd &x) const override;
	VectorXd Gradient(const VectorXd &x) const override;
	MatrixXd Hessian(const VectorXd &x) const override;

	DERIVED_DECLARE_CLONE(Function)

	~BackwardTarget() = default;
};

#endif //FEM_BACKWARDTARGET_H

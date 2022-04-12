//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_GRAVITY_H
#define FEM_GRAVITY_H

#include "ExternalForce.h"
#include "SoftBody/SoftBody.h"

class Gravity : public ExternalForce {
public:
	Gravity(double g);
	~Gravity() = default;

	double Energy(const SoftBody &body) const override;
	VectorXd Gradient(const SoftBody &body) const override;
	SparseMatrixXd Hessian(const SoftBody &body) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

private:
	double _g;
};

#endif //FEM_GRAVITY_H

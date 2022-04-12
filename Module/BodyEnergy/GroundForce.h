//
// Created by hansljy on 2022/3/14.
//

#ifndef FEM_GROUNDFORCE_H
#define FEM_GROUNDFORCE_H

#include "ExternalForce.h"
#include "SoftBody/SoftBody.h"

class GroundForce : public ExternalForce {
public:
	explicit GroundForce(double stiffness);

	double Energy(const SoftBody &body) const override;
	VectorXd Gradient(const SoftBody &body) const override;
	SparseMatrixXd
	Hessian(const SoftBody &body) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

private:
	double _stiffness;
};

#endif //FEM_GROUNDFORCE_H

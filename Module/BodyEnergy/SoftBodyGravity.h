//
// Created by hansljy on 22-5-24.
//

#ifndef FEM_SOFTBODYGRAVITY_H
#define FEM_SOFTBODYGRAVITY_H

#include "Object/SoftBody/SoftBody.h"
#include "ExternalForce.h"

class SoftBodyGravity : public ExternalForce {
public:
	SoftBodyGravity(const SoftBody& soft_body, double g);
	double Energy() const override;
	VectorXd Gradient() const override;
	SparseMatrixXd Hessian() const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

protected:
	const SoftBody* _soft_body;
	double _g;
};

#endif //FEM_SOFTBODYGRAVITY_H

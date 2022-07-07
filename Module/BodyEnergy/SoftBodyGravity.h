//
// Created by hansljy on 22-5-24.
//

#ifndef FEM_SOFTBODYGRAVITY_H
#define FEM_SOFTBODYGRAVITY_H

#include "Object/SoftBody/SoftBody.h"
#include "ExternalForce.h"

class SoftBodyGravity : public ExternalForce {
public:
	SoftBodyGravity(double g);
	double Energy(const Object &obj) const override;
	VectorXd Gradient(const Object &obj) const override;
	void HessianCOO(const Object &obj, COO &coo, int x_offset, int y_offset) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

protected:
	double _g;
};

#endif //FEM_SOFTBODYGRAVITY_H

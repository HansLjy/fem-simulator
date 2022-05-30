//
// Created by hansljy on 22-5-30.
//

#ifndef FEM_ROBOTARMFORCE_H
#define FEM_ROBOTARMFORCE_H

#include "ExternalForce.h"

class RobotArmForce : public ExternalForce {
public:
	RobotArmForce(const Vector3d& direction, double force);
	RobotArmForce(const RobotArmForce& rhs) = default;

	double Energy(const Object &obj) const override;
	VectorXd Gradient(const Object &obj) const override;
	SparseMatrixXd Hessian(const Object &obj) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

protected:
	Vector3d _direction;
	double _force;
};

#endif //FEM_ROBOTARMFORCE_H

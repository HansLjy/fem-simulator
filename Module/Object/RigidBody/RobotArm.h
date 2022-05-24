//
// Created by hansljy on 22-5-14.
//

#ifndef FEM_ROBOTARM_H
#define FEM_ROBOTARM_H

#include "RigidBody.h"
#include "Shape/Rectangle.h"

class RobotArm : public RigidBody {
public:
	RobotArm(double mu, double rho, const Vector3d& center, const Vector3d& euler_angles, const Vector3d& size, const Vector3d& direction)
	 : RigidBody(mu, rho, center, euler_angles, new Rectangle(*this, size)), _direction(direction) {
		_mass.push_back(Triplet(0, 0, rho * _shape->GetVolume()));
		_x.resize(1);
		_v.resize(1);
		_x.setZero();
		_v.setZero();
	}

	int GetDOF() const override {
		return 1;
	}

	COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
			 const VectorXd &point, const VectorXd &normal) const override;

	Vector3d GetCenter() const override {
		return _x(0) * _direction + _center;
	}

	Matrix3d GetRotation() const override {
		return _rotation;
	}

	DERIVED_DECLARE_CLONE(Object)

protected:
	Vector3d _direction;
};

#endif //FEM_ROBOTARM_H

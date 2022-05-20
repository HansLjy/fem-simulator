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
	 : RigidBody(mu, rho, new Rectangle(center, euler_angles, size)), _direction(direction) {
		_mass.push_back(Triplet(0, 0, rho * _shape->GetVolume()));
		_x.resize(1);
		_v.resize(1);
		_x(0) = _v(0) = 0;
	}

	int GetDOF() const override {
		return 1;
	}

	VectorXd & GetX() override {
		return _x;
	}

	const VectorXd & GetX() const override {
		return _x;
	}

	VectorXd & GetV() override {
		return _v;
	}

	const VectorXd & GetV() const override {
		return _v;
	}

	COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
			 const VectorXd &point, const VectorXd &normal) const override;

	DERIVED_DECLARE_CLONE(Object)

protected:
	VectorXd _x;
	VectorXd _v;
	Vector3d _direction;
};

#endif //FEM_ROBOTARM_H

//
// Created by hansljy on 22-5-14.
//

#ifndef FEM_ROBOTARM_H
#define FEM_ROBOTARM_H

#include "RigidBody.h"
#include "Shape/Rectangle.h"

class RobotArmForce;
class RobotArmDOFShapeConverter;

class RobotArm : public RigidBody {
public:

	RobotArm(double mu, double rho, const Vector3d &center,
			 const Vector3d &euler_angles, const Vector3d &size,
			 const Vector3d &direction);

	int GetDOF() const override {
		return 1;
	}

	Vector3d GetCenter() const override {
		return _x(0) * _direction + _center;
	}

	Matrix3d GetRotation() const override {
		return _rotation;
	}

	DERIVED_DECLARE_CLONE(Object)

	friend class RobotArmForce;
	friend class RobotArmDOFShapeConverter;

protected:
	Vector3d _direction;
};

#include "RigidBodyDOFShapeConverter.h"

class RobotArmDOFShapeConverter : public RigidBodyDOFShapeConverter {
public:
	SparseMatrixXd GetJ(const Object &obj, int idx, const Vector3d &point) const override;
	DERIVED_DECLARE_CLONE(DOFShapeConverter)
};

#endif //FEM_ROBOTARM_H

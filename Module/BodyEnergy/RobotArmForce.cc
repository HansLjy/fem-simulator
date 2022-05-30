//
// Created by hansljy on 22-5-30.
//

#include "RobotArmForce.h"
#include "Object/RigidBody/RobotArm.h"

DEFINE_CLONE(ExternalForce, RobotArmForce)

RobotArmForce::RobotArmForce(const Vector3d &direction, double force) : ExternalForce(), _direction(direction), _force(force) {}

double RobotArmForce::Energy(const Object &obj) const {
	const auto& robot_arm = dynamic_cast<const RobotArm&>(obj);
	return _force * _direction.dot(robot_arm._direction) * robot_arm._x(0);
}

VectorXd RobotArmForce::Gradient(const Object &obj) const {
	const auto& robot_arm = dynamic_cast<const RobotArm&>(obj);
	Vector1d gradient;
	gradient << -_force * _direction.dot(robot_arm._direction);
	return gradient;
}

SparseMatrixXd RobotArmForce::Hessian(const Object &obj) const {
	return SparseMatrixXd(1, 1);
}


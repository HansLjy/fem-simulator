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

void RobotArmForce::HessianCOO(const Object &obj, COO &coo, int x_offset,
							   int y_offset) const {
	// do nothing since the hessian is zero
}


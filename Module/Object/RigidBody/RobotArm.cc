//
// Created by hansljy on 22-5-14.
//

#include "RobotArm.h"
#include "Util/EigenAll.h"

DEFINE_CLONE(Object, RobotArm)

RobotArm::RobotArm(double mu, double rho, const Vector3d &center,
				   const Vector3d &euler_angles, const Vector3d &size,
				   const Vector3d &direction)
				   : RigidBody(mu, rho, center, euler_angles, Rectangle(size)), _direction(direction) {
	_mass.push_back(Tripletd(0, 0, rho * _shape->_volume));
	_x.resize(1);
	_v.resize(1);
	_x.setZero();
	_v.setZero();

}

SparseMatrixXd RobotArm::GetJ(int idx, const Vector3d &point) const {
	std::vector<Tripletd> coo;
	for (int i = 0; i < 3; i++) {
		coo.push_back(Tripletd(i, 0, _direction(i)));
	}
	SparseMatrixXd JT(3, 1);
	JT.setFromTriplets(coo.begin(), coo.end());
	return JT;
}
//
// Created by hansljy on 22-5-14.
//

#include "RobotArm.h"
#include "Util/EigenAll.h"

DEFINE_CLONE(Object, RobotArm)
DEFINE_CLONE(DOFShapeConverter, RobotArmDOFShapeConverter)

RobotArm::RobotArm(double mu, double rho, const Vector3d &center,
				   const Vector3d &euler_angles, const Vector3d &size,
				   const Vector3d &direction)
				   : RigidBody(mu, rho, center, euler_angles, Rectangle(size)), _direction(direction) {
	_mass.push_back(Triplet(0, 0, rho * _shape->_volume));
	_x.resize(1);
	_v.resize(1);
	_x.setZero();
	_v.setZero();

	_DOF_converter = new RobotArmDOFShapeConverter();
}

SparseMatrixXd RobotArmDOFShapeConverter::GetJ(const Object &obj, int idx,
											   const Vector3d &point) const {
	const auto& robot_arm = dynamic_cast<const RobotArm&>(obj);
	std::vector<Triplet> coo;
	for (int i = 0; i < 3; i++) {
		coo.push_back(Triplet(i, 0, robot_arm._direction(i)));
	}
	SparseMatrixXd JT(3, 1);
	JT.setFromTriplets(coo.begin(), coo.end());
	return JT;
}


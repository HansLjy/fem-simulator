//
// Created by hansljy on 22-5-14.
//

#include "RobotArm.h"
#include "Util/EigenAll.h"

DEFINE_CLONE(Object, RobotArm)

COO RobotArm::GetJ(const SurfaceElements::SurfaceType &type, int idx,
				   const VectorXd &point,
				   const VectorXd &normal) const {
	COO coo;
	coo.push_back(Triplet(0, 0, _direction.dot(normal)));
	return coo;
}


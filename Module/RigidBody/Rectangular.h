//
// Created by hansljy on 2022/4/11.
//

#ifndef FEM_RECTANGULAR_H
#define FEM_RECTANGULAR_H

#include "RigidBody.h"
#include "Eigen/Geometry"

using Eigen::AngleAxisd;

class Rectangular : public RigidBody {
public:
	Rectangular(double mu, Vector3d center, Vector3d euler_angles, Vector3d shape) :
			RigidBody(mu), _center(center), _euler_angles(euler_angles), _shape(shape) {
		_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
				  * AngleAxisd(euler_angles(1), Vector3d::UnitX())
				  * AngleAxisd(euler_angles(2), Vector3d::UnitZ());
	}

	double Distance(Vector3d point) const override;
	Vector3d Normal(Vector3d point) const override;

protected:
	Matrix3d _rotation;					// rotation matrix for euler angles
	Vector3d _center;					// position of the center
	Vector3d _euler_angles;				// phi, theta, psi, in that order
	Vector3d _shape;					// span in x, y, z direction respectively
};

#endif //FEM_RECTANGULAR_H

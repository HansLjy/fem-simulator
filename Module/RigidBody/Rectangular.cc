//
// Created by hansljy on 2022/4/11.
//

#include "Rectangular.h"
#include "Eigen/Geometry"

using Eigen::AngleAxisd;

// Don't take this seriously, it only judges whether this point is inside the box or not
double Rectangular::Distance(Vector3d point) const {
	Vector3d coord = _rotation.transpose() * (point - _center);
	double dis[3];
	for (int i = 0; i < 3; i++) {
		dis[i] = std::max(- _shape(i) / 2 - coord(i), coord(i) - _shape(i) / 2);
	}
	return *std::max_element(dis, dis + 3);
}

Vector3d Rectangular::Normal(Vector3d point) const {
	Vector3d coord = _rotation.transpose() * (point - _center);

	double min_dis = +_shape.sum();
	Vector3d normal;
	for (int i = 0; i < 3; i++) {
		if (-_shape(i) / 2 < coord(i) && coord(i) < _shape(i) / 2) {
			// inside the box
			if (coord(i) > 0) {
				double cur_dis = _shape(i) / 2 - coord(i);
				if (cur_dis < min_dis) {
					min_dis = cur_dis;
					normal = Vector3d::Unit(i);
				}
			} else {
				double cur_dis = coord(i) + _shape(i) / 2;
				if (cur_dis < min_dis) {
					min_dis = cur_dis;
					normal = -Vector3d::Unit(i);
				}
			}
		}
	}
	assert(normal.norm() != 0);
	return normal;
}
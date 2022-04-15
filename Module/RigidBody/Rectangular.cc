//
// Created by hansljy on 2022/4/11.
//

#include "Rectangular.h"
#include "Eigen/Geometry"
#include "Util/Pattern.h"
#include <fstream>

using std::fstream;
using Eigen::AngleAxisd;

DEFINE_CLONE(RigidBody, Rectangular)

// Don't take this seriously, it only judges whether this point is inside the box or not
double Rectangular::Distance(const Vector3d& point) const {
	Vector3d coord = _rotation.transpose() * (point - _center);
	double dis[3];
	for (int i = 0; i < 3; i++) {
		dis[i] = std::max(- _shape(i) / 2 - coord(i), coord(i) - _shape(i) / 2);
	}
	return *std::max_element(dis, dis + 3);
}

Vector3d Rectangular::Normal(const Vector3d& point) const {
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
	return _rotation * normal;
}

void Rectangular::Store(const std::string &path) {
	Vector3d position[8];
	const int sign[] = {-1, 1};
	for (int i = 0; i < 8; i++) {
		position[i].x() = _shape(0) / 2 * sign[i & 1];
		position[i].y() = _shape(1) / 2 * sign[(i >> 1) & 1];
		position[i].z() = _shape(2) / 2 * sign[(i >> 2) & 1];
	}
	for (int i = 0; i < 8; i++) {
		position[i] = _rotation * position[i] + _center;
	}
	fstream file_stream(path, std::ios::out | std::ios::trunc);
	file_stream.precision(10);
	file_stream << "# vtk DataFile Version 2.0\n"
				<< "Rigid body" << std::endl
				<< "ASCII\n"
				<< "DATASET UNSTRUCTURED_GRID\n";

	file_stream << "POINTS " << 8 << " double" << std::endl;
	for (int i = 0; i < 8; i++) {
		file_stream << position[i](0) << " " << position[i](1) << " " << position[i](2) << std::endl;
	}
	file_stream << "CELLS " << 1 << " " << 9<< std::endl;
	file_stream << "8 0 1 2 3 4 5 6 7" << std::endl;
	file_stream << "CELL_TYPES " << 1 << std::endl;
	file_stream << "11" << std::endl;
}
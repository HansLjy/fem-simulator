//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_RECTANGLE_H
#define FEM_RECTANGLE_H

#include "Shape.h"
#include "Util/EigenAll.h"
#include "Eigen/Geometry"
#include "Object/RigidBody/RigidBody.h"
#include <fstream>
#include <vector>

using Eigen::AngleAxisd;
using std::vector;

struct Rectangle : public Shape {
public:
	Rectangle(const Vector3d& size) : Shape() {
		_offsets.resize(8);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				_offsets[i](j) = ((i >> j) & 1) ? -size(j) / 2 : size(j) / 2;
			}
		}
		_surface_topo.resize(12, 3);
		_surface_topo << 0, 1, 3,
				 0, 3, 2,
				 0, 5, 1,
				 0, 4, 5,
				 0, 2, 4,
				 2, 6, 4,
				 2, 3, 7,
				 2, 7, 6,
				 4, 6, 7,
				 4, 7, 5,
				 1, 5, 3,
				 3, 5, 7;

		_volume_topo.resize(5, 4);
		_volume_topo << 1, 2, 4, 0,
						1, 2, 3, 7,
						1, 2, 4, 7,
						2, 4, 6, 7,
						1, 4, 5, 7;

		/*
		 * The rectangle will be numbered as below:
		 *   3 - 1
		 *  /   /|
		 * 2 - 0 5
		 * |   |/
		 * 6 - 4
		 */

		_volume = size[0] * size[1] * size[2];
	}

	DERIVED_DECLARE_CLONE(Shape)
};

#endif //FEM_RECTANGLE_H

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

class Rectangle : public Shape {
public:
	Rectangle(const RigidBody& rigid_body, const Vector3d& size) : Shape(rigid_body), _size(size) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				_offset[i](j) = ((i >> j) & 1) ? -_size(j) : _size(j);
			}
		}
	}

	int GetNumFaces() const override {
		return 12;
	}

	SurfaceElements::Face GetFace(int idx) const override {
		SurfaceElements::Face face;
		for (int i = 0; i < 3; i++) {
			face._vertex[i] = _rigid_body->GetRotation() * _offset[_face[idx][i]] + _rigid_body->GetCenter();
		}
		return face;
	}

	void Store(const std::string &file) const override {
		std::fstream output(file);
		output << "# vtk DataFile Version 2.0\n"
				  "Rectangle\n"
				  "ASCII\n"
				  "DATASET UNSTRUCTURED_GRID\n";
		output << "POINTS 8 double\n";
		for (int i = 0; i < 8; i++) {
			 output << _rigid_body->GetRotation() * _offset[i] + _rigid_body->GetCenter() << std::endl;
		}
		output << "CELLS 1 9\n"
				  "8 0 1 2 3 4 5 6 7\n"
				  "CELL_TYPES 1\n"
				  "11";
	}

	double GetVolume() const override {
		return _size[0] * _size[1] * _size[2];
	}

	DERIVED_DECLARE_CLONE(Shape)

public:
	Vector3d _size;					// span in x, y, z direction respectively
	vector<vector<int>> _face = {
		{0, 1, 3}, {0, 3, 2}, {0, 5, 1}, {0, 4, 5}, {0, 2, 4}, {2, 6, 4},
		{2, 3, 7}, {2, 7, 6}, {4, 6, 7}, {4, 7, 5}, {1, 5, 3}, {3, 5, 7}
	};
	std::array<Vector3d, 8> _offset;

	/*
	 * The rectangle will be numbered as below:
	 *   3 - 1
	 *  /   /|
	 * 2 - 0 5
	 * |   |/
	 * 6 - 4
	 */
};

#endif //FEM_RECTANGLE_H

//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_SHAPE_H
#define FEM_SHAPE_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"

enum class OutputFormatType;

struct Shape {
	Shape() = default;

	std::vector<Vector3d> _offsets;	// offset of the vertices from center
	Matrix<int, Dynamic, 3> _surface_topo;	// topologies of the vertices on the surface
	Matrix<int, Dynamic, 4> _volume_topo;	// topologies of all vertices

	double _volume;

	BASE_DECLARE_CLONE(Shape)
};

#endif //FEM_SHAPE_H

//
// Created by hansljy on 22-6-30.
//

#include "RigidBody.h"
#include "RigidBodyDOFShapeConverter.h"
#include <fstream>

MatrixXd RigidBodyDOFShapeConverter::GetSurfacePosition(const Object &obj) const {
	auto& rigid_obj = dynamic_cast<const RigidBody&>(obj);
	const auto& shape = rigid_obj._shape;
	const auto& offsets = shape->_offsets;
	const auto& topo = shape->_surface_topo;
	const int num_vertices = offsets.size();

	Vector3d center = rigid_obj.GetCenter();
	Matrix3d rotation = rigid_obj.GetRotation();
	MatrixXd position(num_vertices, 3);
	for (int i = 0; i < num_vertices; i++) {
		position.row(i) = rotation * offsets[i] + center;
	}
	return position;
}

Matrix<int, Dynamic, 3>
RigidBodyDOFShapeConverter::GetSurfaceTopo(const Object &obj) const {
	auto& rigid_obj = dynamic_cast<const RigidBody&>(obj);
	return rigid_obj._shape->_surface_topo;
}

void RigidBodyDOFShapeConverter::Store(const Object &obj,
									   const std::string &filename,
									   const OutputFormatType &format) const {
	auto& rigid_obj = dynamic_cast<const RigidBody&>(obj);
	const auto& volume_topo = rigid_obj._shape->_volume_topo;
	const auto& offsets = rigid_obj._shape->_offsets;
	const auto rotation = rigid_obj.GetRotation();
	const auto center = rigid_obj.GetCenter();
	int num_vertices = offsets.size();
	int num_cells = volume_topo.rows();

	std::fstream output(filename, std::ios::out | std::ios::trunc);

	output << "# vtk DataFile Version 2.0\n"
			  "RigidBody\n"
			  "ASCII\n"
			  "DATASET UNSTRUCTURED_GRID\n";
	output << "POINTS " << num_vertices << " double\n";
	for (int i = 0; i < num_vertices; i++) {
		const auto coord = rotation * offsets[i] + center;
		output << coord(0) << " " << coord(1) << " " << coord(2) << std::endl;
	}

	output << "CELLS " << num_cells << " " << num_cells * 5 << std::endl;
	for (int i = 0; i < num_cells; i++) {
		output << 4 << " " << volume_topo.row(i) << std::endl;
	}

	output << "CELL_TYPES " << num_cells << std::endl;
	for (int i = 0; i < num_cells; i++) {
		output << 10 << std::endl;
	}
	output.close();
}
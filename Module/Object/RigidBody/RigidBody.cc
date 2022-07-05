//
// Created by hansljy on 22-5-17.
//

#include <fstream>
#include "RigidBody.h"

RigidBody::RigidBody(double mu, double rho, const Vector3d &center,
					 const Vector3d &euler_angles, const Shape &shape)
					 : _mu(mu), _rho(rho), _center(center), _shape(shape.Clone()) {
	_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
				* AngleAxisd(euler_angles(1), Vector3d::UnitX())
				* AngleAxisd(euler_angles(2), Vector3d::UnitZ());
}

RigidBody::~RigidBody() noexcept {
	delete _shape;
}

RigidBody::RigidBody(const RigidBody &rhs) : Object(rhs) {
	_mu = rhs._mu;
	_rho = rhs._rho;
	_shape = rhs._shape->Clone();
	_center = rhs._center;
	_rotation = rhs._rotation;
	_x = rhs._x;
	_v = rhs._v;
	_mass = rhs._mass;
}

MatrixXd RigidBody::GetSurfacePosition() const {
	const auto& offsets = _shape->_offsets;
	const auto& topo = _shape->_surface_topo;
	const int num_vertices = offsets.size();

	Vector3d center = GetCenter();
	Matrix3d rotation = GetRotation();
	MatrixXd position(num_vertices, 3);
	for (int i = 0; i < num_vertices; i++) {
		position.row(i) = rotation * offsets[i] + center;
	}
	return position;
}

Matrix<int, Dynamic, 3> RigidBody::GetSurfaceTopo() const {
	return _shape->_surface_topo;
}

void RigidBody::Store(const std::string &filename,
					  const OutputFormatType &format) const {
	const auto& volume_topo = _shape->_volume_topo;
	const auto& offsets = _shape->_offsets;
	const auto rotation = GetRotation();
	const auto center = GetCenter();
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
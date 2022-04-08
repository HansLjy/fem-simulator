//
// Created by hansljy on 2022/2/24.
//

#include "Mesh.h"
#include <spdlog/spdlog.h>
#include "Util/Factory.h"
#include <fstream>
#include <iostream>
using std::getline;
using std::fstream;
using Eigen::Vector3d;

MeshParameter::MeshParameter(const string &input_file)
							 : _input_file(input_file) {}

DEFINE_ACCESSIBLE_MEMBER(MeshParameter, string, InputFile, _input_file)

void Mesh::Initialize(const MeshParameter &para) {
	Load(para.GetInputFile());
	spdlog::info("Mesh intialized");
}

void Mesh::Load(const string &file) {
	fstream file_stream(file, std::ios::in);
	string line;
	// Header
	getline(file_stream, line);
	if (line != "# vtk DataFile Version 2.0") {
		spdlog::error("Unsupported VTK format, use VTk 2.0 instead");
		throw std::exception();
	}

	// Title, I don't care
	getline(file_stream, line);
	_title = line;

	// ASCII / BINARY
	getline(file_stream, line);
	if (line != "ASCII") {
		spdlog::error("Unsupported file format, use ASCII instead");
		throw std::exception();
	}

	// Dataset
	getline(file_stream, line);
	if (line != "DATASET UNSTRUCTURED_GRID") {
		spdlog::error("Unsupported mesh format, use unstructured grid instead");
		throw std::exception();
	}

	// POINT data
	string word, datatype;
	int num_of_points;
	file_stream >> word >> num_of_points >> datatype;
	if (word != "POINTS" || (datatype != "double" && datatype != "float")) {
		spdlog::error("Unsupported point type");
		throw std::exception();
	}

	_points.resize(3 * num_of_points, 1);
	for (int i = 0; i < num_of_points; i++) {
		double x, y, z;
		file_stream >> x >> y >> z;
		_points(3 * i) = x;
		_points(3 * i + 1) = y;
		_points(3 * i + 2) = z;
	}

	// CELL
	int num_of_tets, num_of_figures;
	file_stream >> word >> num_of_tets >> num_of_figures;
	if (word != "CELLS") {
		spdlog::error("Unexpected descriptor");
		throw std::exception();
	}

	for (int i = 0; i < num_of_tets; i++) {
		int points_per_tet;
		file_stream >> points_per_tet;
		if (points_per_tet != 4) {
			spdlog::error("Unsupported grid, use tet instead");
			throw std::exception();
		}
		std::array<int, 4> tet;
		for (int j = 0; j < points_per_tet; j++) {
			file_stream >> tet[j];
		}
		_tets.push_back(tet);
	}

	// CELL_TYPE
	int num_of_tets2;
	file_stream >> word >> num_of_tets2;
	if (word != "CELL_TYPES" || num_of_tets != num_of_tets2) {
		spdlog::error("Unexpected descriptor");
		throw std::exception();
	}
	for (int i = 0; i < num_of_tets; i++) {
		int tet_type;
		file_stream >> tet_type;
		if (tet_type != 10) {
			// for now, only accept tet mesh
			spdlog::error("Unsupported mesh, use tet mesh instead");
			throw std::exception();
		}
	}
	file_stream.close();

	// Shift positive
	double min_shift = 0;
	for (int i = 0; i < num_of_points; i++) {
		if (_points(3 * i + 2) < 0) {
			min_shift = std::max(min_shift, -_points(3 * i + 2));
		}
	}
	for (int i = 0; i < num_of_points; i++) {
		_points(3 * i + 2) += min_shift;
	}
}

void Mesh::Store(const string &file) const {
	fstream file_stream(file, std::ios::out | std::ios::trunc);
	file_stream.precision(10);
	file_stream << "# vtk DataFile Version 2.0\n"
				<< _title << std::endl
				<< "ASCII\n"
				<< "DATASET UNSTRUCTURED_GRID\n";
	int num_of_x = _points.size();
	file_stream << "POINTS " << num_of_x / 3 << " double" << std::endl;
	for (int i = 0; i < num_of_x; i += 3) {
		file_stream << _points(i) << " " << _points(i + 1) << " " << _points(i + 2) << std::endl;
	}
	int num_of_tets = _tets.size();
	file_stream << "CELLS " << num_of_tets << " " << num_of_tets * 5 << std::endl;
	for (auto itr : _tets) {
		file_stream << "4 " << itr[0] << " " << itr[1] << " " << itr[2] << " " << itr[3] << std::endl;
	}
	file_stream << "CELL_TYPES " << num_of_tets << std::endl;
	for (int i = 0; i < num_of_tets; i++) {
		file_stream << "10" << std::endl;
	}
	file_stream.close();
}

#define TETS vector<array<int, 4>>
DEFINE_ACCESSIBLE_MEMBER(Mesh, VectorXd, Points, _points)
DEFINE_ACCESSIBLE_MEMBER(Mesh, string, Title, _title)
DEFINE_ACCESSIBLE_MEMBER(Mesh, TETS, Tets, _tets)
#undef TETS
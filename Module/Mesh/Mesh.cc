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
	CalculateSurface();
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
		std::vector<int> tet(points_per_tet);
		for (int j = 0; j < points_per_tet; j++) {
			file_stream >> tet[j];
		}
		std::sort(tet.begin(), tet.end());
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

struct SurfaceTriangle {
	SurfaceTriangle(int point_id1, int point_id2, int point_id3, bool inverted) :
		_inverted(inverted) {
		_point_id[0] = point_id1;
		_point_id[1] = point_id2;
		_point_id[2] = point_id3;
	}

	bool operator<(const SurfaceTriangle& rhs) const {
		for (int i = 0; i < 3; i++) {
			if (_point_id[i] < rhs._point_id[i]) {
				return true;
			} else if (_point_id[i] > rhs._point_id[i]) {
				return false;
			}
		}
		return false;
	}

	bool operator==(const SurfaceTriangle& rhs) const {
		return !(*this < rhs) && !(rhs < *this);
	}

	bool _inverted;		// whether the order of the last two points have been inverted.
	int _point_id[3];	// end points
};

void Mesh::CalculateSurface() {
	_surface.clear();
	const int num_tets = _tets.size();
	vector<SurfaceTriangle> surface_candidate;
	for (int i = 0; i < num_tets; i++) {
		auto tet = _tets[i];
		std::sort(tet.begin(), tet.end());
		Vector3d X0 = _points.block<3, 1>(3 * tet[0], 0);
		Vector3d X1 = _points.block<3, 1>(3 * tet[1], 0);
		Vector3d X2 = _points.block<3, 1>(3 * tet[2], 0);
		Vector3d X3 = _points.block<3, 1>(3 * tet[3], 0);
		if ((X3 - X0).dot((X2 - X0).cross(X1 - X0)) > 0) {
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[1], tet[2], false));
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[2], tet[3], false));
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[1], tet[3], true));
			surface_candidate.push_back(
					SurfaceTriangle(tet[1], tet[2], tet[3], true));
		} else {
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[1], tet[2], true));
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[2], tet[3], true));
			surface_candidate.push_back(
					SurfaceTriangle(tet[0], tet[1], tet[3], false));
			surface_candidate.push_back(
					SurfaceTriangle(tet[1], tet[2], tet[3], false));
		}
	}
	vector<SurfaceTriangle> surface;
	std::sort(surface_candidate.begin(), surface_candidate.end());
	const int num_surface = 4 * num_tets;
	for (int i = 0; i < num_surface; i++) {
		if (i < num_surface - 1 && surface_candidate[i] == surface_candidate[i + 1]) {
			while (i < num_surface - 1 && surface_candidate[i] == surface_candidate[i + 1]) {
				i++;
			}
		} else {
			surface.push_back(surface_candidate[i]);
		}
	}
	for (auto& surface_element : surface) {
		vector<int> surface_ids(3);
		surface_ids[0] = surface_element._point_id[0];
		surface_ids[1] = surface_element._inverted ? surface_element._point_id[2] : surface_element._point_id[1];
		surface_ids[2] = surface_element._inverted ? surface_element._point_id[1] : surface_element._point_id[2];
		_surface.push_back(surface_ids);
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

#define TETS vector<vector<int>>
DEFINE_ACCESSIBLE_MEMBER(Mesh, VectorXd, Points, _points)
DEFINE_ACCESSIBLE_MEMBER(Mesh, string, Title, _title)
DEFINE_ACCESSIBLE_MEMBER(Mesh, TETS, Tets, _tets)
#undef TETS
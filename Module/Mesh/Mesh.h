//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MESH_H
#define FEM_MESH_H

#include <string>
#include <array>
#include <vector>
#include "Eigen/Dense"
#include "Util/Pattern.h"

using std::string, std::vector, std::array;
using Eigen::VectorXd;
using Eigen::Matrix3d;

class MeshParameter {
public:
	MeshParameter(const string& input_file);

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(string, InputFile, _input_file)
};

#define TETS vector<array<int, 4>>

class Mesh {
public:
	void Initialize(const MeshParameter& para);
	void Store(const string& file) const;

private:
	void ComputeInverse();
	void Load(const string& file);

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(VectorXd, Points, _points)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(string, Title, _title)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(VectorXd, Mass, _mass)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(TETS, Tets, _tets)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(vector<Matrix3d>, B, _B)
};

#undef TETS

#endif //FEM_MESH_H

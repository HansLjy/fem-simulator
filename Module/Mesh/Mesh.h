//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MESH_H
#define FEM_MESH_H

#include <string>
#include <array>
#include <vector>
#include "Eigen/Dense"
#include "Mass/MassModel.h"

using std::string, std::vector, std::array;
using Eigen::VectorXd;
using Eigen::Matrix3d;

struct MeshParameter {
	string _input_file;
	MassModelType _mass_model;
};

class Mesh {
public:
	void Initialize(const MeshParameter& para);
	void Load(const string& file);
	void Store(const string& file) const;
	std::string GetTitle() const;
	VectorXd& GetPoints();
	const VectorXd& GetPoints() const;
	vector<array<int, 4>>& GetTets();
	const vector<array<int, 4>>& GetTets() const;
	VectorXd& GetMass() const;
	const std::vector<Matrix3d>& GetB() const;

	void ComputeInverse();
	void ComputeMass();

private:
	std::string _title;
	VectorXd _points;
	VectorXd _mass;
	std::vector<std::array<int, 4>> _tets;
	std::vector<Matrix3d> _B;

};

#endif //FEM_MESH_H

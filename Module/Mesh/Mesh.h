//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_MESH_H
#define FEM_MESH_H

#include <string>
#include <array>
#include <vector>
#include "Util/Pattern.h"
#include "Util/EigenAll.h"

using std::string, std::vector, std::array;

class MeshParameter {
public:
	MeshParameter(const string& input_file);

	DECLARE_ACCESSIBLE_MEMBER(string, InputFile, _input_file)
};

#define TETS vector<vector<int>>

class Mesh {
public:
	void Initialize(const MeshParameter& para);
	void Store(const string& file) const;

private:
	void Load(const string& file);

	DECLARE_ACCESSIBLE_MEMBER(VectorXd, Points, _points)
	DECLARE_ACCESSIBLE_MEMBER(string, Title, _title)
	DECLARE_ACCESSIBLE_MEMBER(TETS, Tets, _tets)
};

#undef TETS

#endif //FEM_MESH_H

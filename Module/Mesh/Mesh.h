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
#include "Contact/Surface.h"

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
	/**
	 * Store the mesh in vtk format
	 * @param file filename to be stored (path included)
	 */
	void Store(const string& file) const;

	const vector<vector<int>>& GetSurface() const {
		return _surface;
	}

private:
	/**
	 * Load the mesh from vtk file
	 * @param file filename to load from (path included)
	 */
	void Load(const string& file);
	void CalculateSurface();

	DECLARE_ACCESSIBLE_MEMBER(VectorXd, Points, _points)
	DECLARE_ACCESSIBLE_MEMBER(string, Title, _title)
	DECLARE_ACCESSIBLE_MEMBER(TETS, Tets, _tets)

private:
	vector<vector<int>> _surface;	// _surface[i][j] stores the jth vertex of
									// the ith surface primitive
};

#undef TETS

#endif //FEM_MESH_H

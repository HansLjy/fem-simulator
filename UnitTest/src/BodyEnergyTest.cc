//
// Created by hansljy on 2022/3/9.
//

#include "../Test.h"
#include <algorithm>

void Test::TestBodyEnergy() {
	// Separating every tet to test whether
	Mesh mesh;
	auto& points = mesh.GetPoints();
	auto& tet = mesh.GetTets();
	auto& title = mesh.GetTitle();
	title = "Tet for Test";

	const int num_of_tets = 10;
	tet.resize(num_of_tets);
	points.resize(4 * num_of_tets);

	double energy;
	VectorXd gradient;
	MatrixXd hessian;

	for (int i = 0; i < num_of_tets; i++) {
		Vector3d X[4];
		for (int j = 0; j < 4; j++) {
			X[j] = Vector3d::Random();
			points.block<3, 1>(3 * (4 * i + j), 0) = X[j];
			tet[i][j] = 4 * i + j;
		}

		Matrix3d D;
		for (int j = 0; j < 3; j++) {
			D.col(j) = X[j] - X[3];
		}
		double W = D.determinant();
		Matrix3d B = D.inverse();
	}

	std::random_shuffle(tet.begin(), tet.end());

	mesh.Store("../Resource/output/test.vtk");

	Mesh reference;
	reference.Initialize(MeshParameter("../Resource/output/test.vtk"));


}
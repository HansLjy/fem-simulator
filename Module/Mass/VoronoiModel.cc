//
// Created by hansljy on 2022/2/23.
//

#include "VoronoiModel.h"

VectorXd VoronoiModel::GetMassDistribution(const Mesh &mesh) {
	const VectorXd& points = mesh.GetPoints();
	const auto& tets = mesh.GetTets();
	int num_of_points = points.size() / 3;
	int num_of_tets = tets.size();
	VectorXd mass(num_of_points);
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Matrix3d D;
		for (int j = 0; j < 3; j++) {
			D.col(j) = points.block<3, 1>(3 * tet[j], 0) - points.block<3, 1>(3 * tet[3], 0);
		}
		double W = std::abs(D.determinant());
		for (int j = 0; j < 4; j++) {
			mass(tet[j]) += W / 4;
		}
	}
	return mass;
}
//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_BODYENERGY_H
#define FEM_BODYENERGY_H

#include "ElementEnergy/ElasticEnergy.h"
#include "ElementEnergy/DissipationEnergy.h"
#include "Mesh/Mesh.h"
#include <vector>

using Eigen::VectorXd;
using Eigen::MatrixXd;

namespace BodyEnergy {

	double EEnergy(
		const ElasticEnergy& elas_model,
		const Mesh& reference,
		const VectorXd& X
	);

	double DEnergy(
		const DissipationEnergy& diss_model,
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);

	VectorXd EGradient(
		const ElasticEnergy& elas_model,
		const Mesh& reference,
		const VectorXd& X
	);

	VectorXd DGradient(
		const DissipationEnergy& diss_model,
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);

	MatrixXd EHessian(
		const ElasticEnergy& elas_model,
		const Mesh& reference,
		const VectorXd& X
	);

	MatrixXd DHessian(
		const DissipationEnergy& diss_model,
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);
}

#endif //FEM_BODYENERGY_H

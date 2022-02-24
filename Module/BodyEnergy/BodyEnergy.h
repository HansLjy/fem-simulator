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

struct BodyEnergyParameter {
	ElasticEnergyModelType _elas_type;
	ElasticEnergyModelParameter _elas_para;
	DissipationEnergyModelType _diss_type;
	DissipationEnergyModelParameter _diss_para;
	ConsistencyModelType _cons_type;
	ConsistencyModelParameter _cons_para;
};

class BodyEnergy {
public:
	void Initialize(const BodyEnergyParameter& para);

	double EEnergy(
		const Mesh& reference,
		const VectorXd& X
	);

	double DEnergy(
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);

	VectorXd EGradient(
		const Mesh& reference,
		const VectorXd& X
	);

	VectorXd DGradient(
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);

	MatrixXd EHessian(
		const Mesh& reference,
		const VectorXd& X
	);

	MatrixXd DHessian(
		const Mesh& reference,
		const VectorXd& X,
		const VectorXd& V
	);

private:
	ElasticEnergyModel* _elas_model;
	DissipationEnergyModel* _diss_model;
	ConsistencyModel* _cons_model;
};

#endif //FEM_BODYENERGY_H

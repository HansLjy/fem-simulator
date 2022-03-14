//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_BODYENERGY_H
#define FEM_BODYENERGY_H

#include "ElementEnergy/ElasticEnergy.h"
#include "ElementEnergy/DissipationEnergy.h"
#include "Mesh/Mesh.h"
#include <vector>
#include "Util/Pattern.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::VectorX;

class BodyEnergyParameter {
public:
	BodyEnergyParameter(
		const ElasticEnergyModelType& elas_type,
		const ElasticEnergyModelParameter &elas_para,
		const DissipationEnergyModelType& diss_type,
		const DissipationEnergyModelParameter &diss_para,
		const ConsistencyModelType& cons_type,
		const ConsistencyModelParameter &cons_para
	);


	DERIVED_DECLARE_CLONE(BodyEnergyParameter)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(ElasticEnergyModelType, ElasticEnergyModelType, _elas_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(ElasticEnergyModelParameter, ElasticEnergyModelParameter, _elas_para)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(DissipationEnergyModelType, DissipationEnergyModelType, _diss_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(DissipationEnergyModelParameter, DissipationEnergyModelParameter, _diss_para)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(ConsistencyModelType, ConsistencyModelType, _cons_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(ConsistencyModelParameter, ConsistencyModelParameter, _cons_para)
};

class BodyEnergy {
public:
	void Initialize(const BodyEnergyParameter& para);

	double
	EEnergy(const Mesh &reference, const VectorXd &W,
			const VectorX<Matrix3d> &inv,
			const VectorXd &X);

	double
	DEnergy(const Mesh &reference, const VectorXd &W, const VectorXd &mass,
			const VectorX<Matrix3d> &inv, const VectorXd &X,
			const VectorXd &V);

	VectorXd
	EGradient(const Mesh &reference, const VectorXd &W,
			  const VectorX<Matrix3d> &inv, const VectorXd &X);

	VectorXd
	DGradient(const Mesh &reference, const VectorXd &W, const VectorXd &mass,
			  const VectorX<Matrix3d> &inv, const VectorXd &X,
			  const VectorXd &V);

	MatrixXd
	EHessian(const Mesh &reference, const VectorXd &W,
			 const VectorX<Matrix3d> &inv,
			 const VectorXd &X);

	MatrixXd
	DHessian(const Mesh &reference, const VectorXd &W, const VectorXd &mass,
			 const VectorX<Matrix3d> &inv, const VectorXd &X,
			 const VectorXd &V);

	BodyEnergy() = default;
	BodyEnergy(const BodyEnergy& body_energy);
	~BodyEnergy();

	DERIVED_DECLARE_CLONE(BodyEnergy)

private:
	ElasticEnergyModel* _elas_model = nullptr;
	DissipationEnergyModel* _diss_model = nullptr;
	ConsistencyModel* _cons_model = nullptr;
};

#endif //FEM_BODYENERGY_H

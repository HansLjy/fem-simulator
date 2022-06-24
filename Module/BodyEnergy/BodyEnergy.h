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
#include "Util/EigenAll.h"
#include "ConstituteModel/ConstituteModel.h"

class SoftBody;

class BodyEnergyParameter {
public:
	BodyEnergyParameter(
		const ElasticEnergyModelType& elas_type,
		const ElasticEnergyModelParameter &elas_para,
		const DissipationEnergyModelType& diss_type,
		const DissipationEnergyModelParameter &diss_para,
		const ConstituteModelType& cons_type,
		const ConstituteModelParameter &cons_para
	);


	DERIVED_DECLARE_CLONE(BodyEnergyParameter)
	DECLARE_ACCESSIBLE_MEMBER(ElasticEnergyModelType, ElasticEnergyModelType, _elas_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(ElasticEnergyModelParameter, ElasticEnergyModelParameter, _elas_para)
	DECLARE_ACCESSIBLE_MEMBER(DissipationEnergyModelType, DissipationEnergyModelType, _diss_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(DissipationEnergyModelParameter, DissipationEnergyModelParameter, _diss_para)
	DECLARE_ACCESSIBLE_MEMBER(ConstituteModelType, ConstituteModelType, _cons_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(ConstituteModelParameter, ConstituteModelParameter, _cons_para)
};

/**
 * BodyEnergy: a class responsible for the calculation of the elasticity force
 * and damping force within the soft bodies.
 * @note: All functions take a soft_body object as input and output the
 * corresponding physical entity as the name suggests.
 */
class BodyEnergy {
public:
	BodyEnergy() = default;
	void Initialize(const BodyEnergyParameter& para);

	//->elasticity energy
	double EEnergy(const SoftBody &soft_body) const;

	//->damping energy
	double DEnergy(const SoftBody &soft_body) const;

	//->gradient of elasticity energy, negate of elasticity force
	VectorXd EGradient(const SoftBody &soft_body) const;

	//->gradient of damping energy, negate of damping force
	VectorXd DGradient(const SoftBody &soft_body) const;

	//->hessian of elasticity energy, negate of the derivative of elasticity force
	SparseMatrixXd EHessian(const SoftBody &soft_body) const;

	//->COO form of EHessian
	COO EHessianCOO(const SoftBody &soft_body) const;

	//->hessian of damping energy, negate of the derivative of damping force
	SparseMatrixXd DHessian(const SoftBody &soft_body) const;

	//->COO form of DHessian
	COO DHessianCOO(const SoftBody &soft_body) const;
	BodyEnergy(const BodyEnergy& body_energy);
	~BodyEnergy();

	friend class SoftBody;

	DERIVED_DECLARE_CLONE(BodyEnergy)

private:
	ElasticEnergyModel* _elas_model = nullptr;
	DissipationEnergyModel* _diss_model = nullptr;
	ConstituteModel* _cons_model = nullptr;
};

#endif //FEM_BODYENERGY_H

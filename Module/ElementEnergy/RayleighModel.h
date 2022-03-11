//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_RAYLEIGHMODEL_H
#define FEM_RAYLEIGHMODEL_H

#include "DissipationEnergy.h"
#include "Util/Pattern.h"
#include "ElasticEnergy.h"

class RayleighModelParameter : public DissipationEnergyModelParameter {
public:
	RayleighModelParameter(double alpha1, double alpha2);

	DERIVED_DECLARE_CLONE(DissipationEnergyModelParameter)

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, Alpha1, _alpha1)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, Alpha2, _alpha2)
};

class RayleighModel : public DissipationEnergyModel {
public:
	void Initialize(const DissipationEnergyModelParameter &para) override;
	double Energy(const ConsistencyModel &cons_model,
				  const ElasticEnergyModel &elas_model,
				  double W, const Matrix3d &B, const Vector4d &mass,
				  const Vector12d &V,
				  const Matrix3d &Ds) const override;
	Vector12d Gradient(const ConsistencyModel &cons_model,
					   const ElasticEnergyModel &elas_model, double W,
					   const Matrix3d &B, const Vector4d &mass,
					   const Vector12d &V,
					   const Matrix3d &Ds) const override;
	Matrix12d Hessian(const ConsistencyModel &cons_model,
					  const ElasticEnergyModel &elas_model, double W,
					  const Matrix3d &B, const Vector4d &mass,
					  const Vector12d &V,
					  const Matrix3d &Ds) const override;

	~RayleighModel();
	DERIVED_DECLARE_CLONE(DissipationEnergyModel)
private:
	double _alpha1, _alpha2;
};

#endif //FEM_RAYLEIGHMODEL_H

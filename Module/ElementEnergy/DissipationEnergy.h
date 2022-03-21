//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_DISSIPATIONENERGY_H
#define FEM_DISSIPATIONENERGY_H

#include "ConsistencyModel/ConsitencyModel.h"
#include "Util/Pattern.h"
#include "Util/EigenAll.h"
#include "ElasticEnergy.h"

enum class DissipationEnergyModelType {
	kRayleigh
};

class DissipationEnergyModelParameter {
public:
	DissipationEnergyModelParameter();
	BASE_DECLARE_CLONE(DissipationEnergyModelParameter)

	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(double, Alpha1)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(double, Alpha2)

};

class DissipationEnergyModel {
public:
	virtual void Initialize(const DissipationEnergyModelParameter& para);

	// Here B is the inverse of [X1 - X4, X2 - X4, X3 - X4]
	virtual double Energy(const ConsistencyModel &cons_model,
						  const ElasticEnergyModel &elas_model,
						  double W, const Matrix3d &B, const Vector4d &mass,
						  const Vector12d &V,
						  const Matrix3d &Ds, const Matrix12x9d &pFpX) const = 0;

	virtual Vector12d Gradient(const ConsistencyModel &cons_model,
							   const ElasticEnergyModel &elas_model, double W,
							   const Matrix3d &B, const Vector4d &mass,
							   const Vector12d &V,
							   const Matrix3d &Ds,
							   const Matrix12x9d &pFpX) const = 0;

	virtual Matrix12d Hessian(const ConsistencyModel &cons_model,
							  const ElasticEnergyModel &elas_model, double W,
							  const Matrix3d &B, const Vector4d &mass,
							  const Vector12d &V,
							  const Matrix3d &Ds, const Matrix12x9d &pFpX) const = 0;

	BASE_DECLARE_CLONE(DissipationEnergyModel)

	virtual ~DissipationEnergyModel() = default;
};

#endif //FEM_DISSIPATIONENERGY_H

//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_RAYLEIGHMODEL_H
#define FEM_RAYLEIGHMODEL_H

#include "DissipationEnergy.h"
#include "Util/Pattern.h"
#include "ElasticEnergy.h"

struct RayleighModelParameter : public DissipationEnergyModelParameter {
	double _alpha1, _alpha2;
};

class RayleighModel : public DissipationEnergyModel {
public:
	double Energy(const ConsistencyModel &cons_model, const ElasticEnergyModel &elas_model, const Matrix3d &B, const Matrix3d &Ds, const Vector4d &mass, const Vector12d &v) const override;
	Vector12d Gradient(const ConsistencyModel &cons_model, const ElasticEnergyModel &elas_model, const Matrix3d &B, const Matrix3d &Ds, const Vector4d &mass, const Vector12d &v) const override;
	Matrix12d Hessian(const ConsistencyModel &cons_model, const ElasticEnergyModel &elas_model, const Matrix3d &B, const Matrix3d &Ds, const Vector4d &mass, const Vector12d &v) const override;

	DERIVED_DECLARE_CLONE(DissipationEnergyModel)
private:
	double _alpha1, _alpha2;
};

#endif //FEM_RAYLEIGHMODEL_H

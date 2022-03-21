//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SIMPLEMODEL_H
#define FEM_SIMPLEMODEL_H

#include "ElasticEnergy.h"

class SimpleModelParameter : public ElasticEnergyModelParameter {
public:
	DERIVED_DECLARE_CLONE(ElasticEnergyModelParameter)
};

class SimpleModel : public ElasticEnergyModel {
public:
	void Initialize(const ElasticEnergyModelParameter &para) override;

	double
	Energy(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
		   const Matrix3d &Ds) const override;
	Vector12d
	Gradient(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			 const Matrix3d &Ds) const override;
	Matrix12d
	Hessian(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			const Matrix3d &Ds, const Matrix12x9d &pFpX) const override;
	~SimpleModel();

	DERIVED_DECLARE_CLONE(ElasticEnergyModel)
};

#endif //FEM_SIMPLEMODEL_H

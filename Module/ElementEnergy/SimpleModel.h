//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_SIMPLEMODEL_H
#define FEM_SIMPLEMODEL_H

#include "ElasticEnergy.h"

class SimpleModel : public ElasticEnergyModel {
public:
	double
	Energy(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
		   const Matrix3d &Ds) const override;
	Vector12d
	Gradient(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			 const Matrix3d &Ds) const override;
	Matrix12d
	Hessian(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			const Matrix3d &Ds) const override;
	~SimpleModel();

	DERIVED_DECLARE_CLONE(ElasticEnergyModel)
};

#endif //FEM_SIMPLEMODEL_H

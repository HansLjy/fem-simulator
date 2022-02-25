//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_STVKMODEL_H
#define FEM_STVKMODEL_H

#include "ConsitencyModel.h"

class StVKModelParameter : public ConsistencyModelParameter {
public:
	StVKModelParameter(double youngs_module, double poisson_ratio);
	DERIVED_DECLARE_CLONE(ConsistencyModelParameter)

	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, YoungsModule, _youngs_module)
	DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(double, PoissonRatio, _poisson_ratio)
};

class StVKModel : public ConsistencyModel {
public:
	void Initialize(const ConsistencyModelParameter &para) override;
	double EnergyDensity(const Matrix3d &F) const override;
	Matrix3d Piola(const Matrix3d &F) const override;
	Matrix3d PiolaDifferential(const Matrix3d &F, const Matrix3d &dF) const override;
	~StVKModel();

	DERIVED_DECLARE_CLONE(ConsistencyModel)

protected:
	double _lame_mu;
	double _lame_lambda;
};

#endif //FEM_STVKMODEL_H

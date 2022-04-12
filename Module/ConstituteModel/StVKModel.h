//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_STVKMODEL_H
#define FEM_STVKMODEL_H

#include "ConstituteModel.h"

class StVKModelParameter : public ConstituteModelParameter {
public:
	StVKModelParameter(double youngs_module, double poisson_ratio);
	DERIVED_DECLARE_CLONE(ConstituteModelParameter)

	DECLARE_ACCESSIBLE_MEMBER(double, YoungsModule, _youngs_module)
	DECLARE_ACCESSIBLE_MEMBER(double, PoissonRatio, _poisson_ratio)
};

class StVKModel : public ConstituteModel {
public:
	void Initialize(const ConstituteModelParameter &para) override;
	double EnergyDensity(const Matrix3d &F) const override;
	Matrix3d Piola(const Matrix3d &F) const override;
	Matrix9d PiolaDifferential(const Matrix3d &F) const override;
	~StVKModel();

	DERIVED_DECLARE_CLONE(ConstituteModel)

protected:
	double _lame_mu;
	double _lame_lambda;
};

#endif //FEM_STVKMODEL_H

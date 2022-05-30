//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_CONSTITUTEMODEL_H
#define FEM_CONSTITUTEMODEL_H

#include "Util/Pattern.h"
#include "Util/EigenAll.h"


enum class ConstituteModelType {
	kStVK,
	kCoro,
	kLinear
};

class ConstituteModelParameter {
public:
	ConstituteModelParameter();
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, YoungsModule)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, PoissonRatio)

	BASE_DECLARE_CLONE(ConstituteModelParameter)
};

class ConstituteModel {
public:
	virtual void Initialize(const ConstituteModelParameter& para);
	virtual double EnergyDensity(const Matrix3d& F) const = 0;
	virtual Matrix3d Piola(const Matrix3d& F) const = 0;
	virtual Matrix9d PiolaDifferential(const Matrix3d &F) const = 0;

	BASE_DECLARE_CLONE(ConstituteModel)

	virtual ~ConstituteModel();

};

#endif //FEM_CONSTITUTEMODEL_H

//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_CONSITENCYMODEL_H
#define FEM_CONSITENCYMODEL_H

#include "Eigen/Dense"
#include "Util/Pattern.h"

using Eigen::Matrix3d;
using Eigen::Vector3d;

enum class ConsistencyModelType {
	kStVK,
};

class ConsistencyModelParameter {
public:
	ConsistencyModelParameter();
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(double, YoungsModule)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(double, PoissonRatio)

	BASE_DECLARE_CLONE(ConsistencyModelParameter)
};

class ConsistencyModel {
public:
	virtual void Initialize(const ConsistencyModelParameter& para);
	virtual double EnergyDensity(const Matrix3d& F) const = 0;
	virtual Matrix3d Piola(const Matrix3d& F) const = 0;
	virtual Matrix3d PiolaDifferential(const Matrix3d& F, const Matrix3d& dF) const = 0;

	BASE_DECLARE_CLONE(ConsistencyModel)

	virtual ~ConsistencyModel();

};

#endif //FEM_CONSITENCYMODEL_H

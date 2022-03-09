//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_ELEMENTENERGY_H
#define FEM_ELEMENTENERGY_H

#include "ElementDefinition.h"
#include "ConsistencyModel/ConsitencyModel.h"
#include "Util/Pattern.h"

using Eigen::Vector3d;
using Eigen::Matrix3d;



enum class ElasticEnergyModelType {
	kSimple,
};

class ElasticEnergyModelParameter {
public:
	ElasticEnergyModelParameter() = default;
	virtual ~ElasticEnergyModelParameter() = default;
};

class ElasticEnergyModel {
public:
	virtual void Initialize(const ElasticEnergyModelParameter& para);

	// Here B is the inverse of [X1 - X4, X2 - X4, X3 - X4]
	virtual double
	Energy(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
		   const Matrix3d &Ds) const = 0;
	virtual Vector12d
	Gradient(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			 const Matrix3d &Ds) const = 0;
	virtual Matrix12d
	Hessian(const ConsistencyModel &cons_model, double W, const Matrix3d &B,
			const Matrix3d &Ds) const = 0;

	BASE_DECLARE_CLONE(ElasticEnergyModel)

	virtual ~ElasticEnergyModel() = default;

protected:
};

#endif //FEM_ELEMENTENERGY_H

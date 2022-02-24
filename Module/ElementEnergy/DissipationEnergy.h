//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_DISSIPATIONENERGY_H
#define FEM_DISSIPATIONENERGY_H

#include "ElementDefinition.h"
#include "ConsistencyModel/ConsitencyModel.h"
#include "Util/Pattern.h"
#include "ElasticEnergy.h"

using Eigen::Vector4d;

enum class DissipationEnergyModelType {
	kRayleigh
};

struct DissipationEnergyModelParameter {
};

class DissipationEnergyModel {
public:
	void Initialize(const DissipationEnergyModelParameter& para);

	// Here B is the inverse of [X1 - X4, X2 - X4, X3 - X4]
	virtual double Energy(
		const ConsistencyModel& cons_model,
		const ElasticEnergyModel& elas_model,
		const Matrix3d& B,
		const Matrix3d& Ds,
		const Vector4d& mass,
		const Vector12d& v
	) const = 0;

	virtual Vector12d Gradient(
			const ConsistencyModel& cons_model,
			const ElasticEnergyModel& elas_model,
			const Matrix3d& B,
			const Matrix3d& Ds,
			const Vector4d& mass,
			const Vector12d& v
	) const = 0;

	virtual Matrix12d Hessian(
			const ConsistencyModel& cons_model,
			const ElasticEnergyModel& elas_model,
			const Matrix3d& B,
			const Matrix3d& Ds,
			const Vector4d& mass,
			const Vector12d& v
	) const = 0;

	BASE_DECLARE_CLONE(DissipationEnergyModel)

	virtual ~DissipationEnergyModel();
};

#endif //FEM_DISSIPATIONENERGY_H

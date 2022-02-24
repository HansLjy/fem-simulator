//
// Created by hansljy on 2022/2/24.
//

#include "RayleighModel.h"

#define GET_DAMPING_MATRIX(C) \
	Matrix12d K = elas_model.Hessian(cons_model, B, Ds);	\
	Vector12d M;											\
	for (int i = 0; i < 12; i += 3) {						\
		M(i) = M(i + 1) = M(i + 2) = mass(i);				\
	}                         								\
	Matrix12d C = _alpha1 * M + _alpha2 * K;

double RayleighModel::Energy(const ConsistencyModel &cons_model,
							 const ElasticEnergyModel &elas_model,
							 const Matrix3d &B, const Matrix3d &Ds,
							 const Vector4d &mass, const Vector12d &v) const {
	GET_DAMPING_MATRIX(C)
	return 0.5 * v.transpose() * C * v;
}

Vector12d RayleighModel::Gradient(const ConsistencyModel &cons_model,
								  const ElasticEnergyModel &elas_model,
								  const Matrix3d &B, const Matrix3d &Ds,
								  const Vector4d &mass,
								  const Vector12d &v) const {
	GET_DAMPING_MATRIX(C)
	return C * v;
}

Matrix12d RayleighModel::Hessian(const ConsistencyModel &cons_model,
								 const ElasticEnergyModel &elas_model,
								 const Matrix3d &B, const Matrix3d &Ds,
								 const Vector4d &mass,
								 const Vector12d &v) const {
	GET_DAMPING_MATRIX(C)
	return C;
}

DEFINE_CLONE(DissipationEnergyModel, RayleighModel)
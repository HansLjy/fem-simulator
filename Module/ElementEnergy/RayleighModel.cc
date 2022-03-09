//
// Created by hansljy on 2022/2/24.
//

#include "RayleighModel.h"

RayleighModelParameter::RayleighModelParameter(double alpha1, double alpha2) : _alpha1(alpha1), _alpha2(alpha2) {}

DEFINE_CLONE(DissipationEnergyModelParameter, RayleighModelParameter)

DEFINE_ACCESSIBLE_MEMBER(RayleighModelParameter, double, Alpha1, _alpha1)
DEFINE_ACCESSIBLE_MEMBER(RayleighModelParameter, double, Alpha2, _alpha2)

void RayleighModel::Initialize(const DissipationEnergyModelParameter &para) {
	_alpha1 = para.GetAlpha1();
	_alpha2 = para.GetAlpha2();
}

#define GET_DAMPING_MATRIX(C) \
	Matrix12d K = elas_model.Hessian(cons_model, W, B, Ds);	\
	Vector12d M;											\
	for (int i = 0; i < 12; i += 3) {						\
		M(i) = M(i + 1) = M(i + 2) = mass(i);				\
	}                         								\
	Matrix12d C = _alpha1 * M.asDiagonal().toDenseMatrix() + _alpha2 * K;

double RayleighModel::Energy(const ConsistencyModel &cons_model,
							 const ElasticEnergyModel &elas_model,
							 double W, const Matrix3d &B, const Vector4d &mass,
							 const Vector12d &v,
							 const Matrix3d &Ds) const {
	GET_DAMPING_MATRIX(C)
	return 0.5 * v.transpose() * C * v;
}

Vector12d RayleighModel::Gradient(const ConsistencyModel &cons_model,
								  const ElasticEnergyModel &elas_model,
								  double W,
								  const Matrix3d &B, const Vector4d &mass,
								  const Vector12d &v,
								  const Matrix3d &Ds) const {
	GET_DAMPING_MATRIX(C)
	return C * v;
}

Matrix12d RayleighModel::Hessian(const ConsistencyModel &cons_model,
								 const ElasticEnergyModel &elas_model, double W,
								 const Matrix3d &B, const Vector4d &mass,
								 const Vector12d &v,
								 const Matrix3d &Ds) const {
	GET_DAMPING_MATRIX(C)
	return C;
}

RayleighModel::~RayleighModel() = default;


DEFINE_CLONE(DissipationEnergyModel, RayleighModel)
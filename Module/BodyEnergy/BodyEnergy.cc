//
// Created by hansljy on 2022/2/23.
//

#include "BodyEnergy.h"
#include "Util/Factory.h"

DEFINE_CLONE(BodyEnergyParameter, BodyEnergyParameter)

DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, ElasticEnergyModelType, ElasticEnergyModelType, _elas_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, ElasticEnergyModelParameter, ElasticEnergyModelParameter, _elas_para)
DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, DissipationEnergyModelType, DissipationEnergyModelType, _diss_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, DissipationEnergyModelParameter, DissipationEnergyModelParameter, _diss_para)
DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, ConsistencyModelType, ConsistencyModelType, _cons_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, ConsistencyModelParameter, ConsistencyModelParameter, _cons_para)

void BodyEnergy::Initialize(const BodyEnergyParameter &para) {
	_cons_model = ConsistencyModelFactory::GetInstance()->GetConsistencyModel(para.GetConsistencyModelType());
	_cons_model->Initialize(*para.GetConsistencyModelParameter());
	_elas_model = ElasticEnergyModelFactory::GetInstance()->GetElasticEnergyModel(para.GetElasticEnergyModelType());
	_elas_model->Initialize(*para.GetElasticEnergyModelParameter());
	_diss_model = DissipationEnergyModelFactory::GetInstance()->GetDissipationEnergyModel(para.GetDissipationEnergyModelType());
	_diss_model->Initialize(*para.GetDissipationEnergyModelParameter());
}

Matrix3d GetDs(const VectorXd& X, const std::array<int, 4>& tet) {
	Vector3d X1 = X.block<3, 1>(3 * tet[0], 0);	// TODO is this working?
	Vector3d X2 = X.block<3, 1>(3 * tet[1], 0);
	Vector3d X3 = X.block<3, 1>(3 * tet[2], 0);
	Vector3d X4 = X.block<3, 1>(3 * tet[3], 0);
	Matrix3d D;
	D.col(0) = X1 - X4;
	D.col(1) = X2 - X4;
	D.col(2) = X3 - X4;
	return D;
}

#define GET_MEMBERS(ref, tets, points, B)	\
	const auto& tets = ref.GetTets();		\
	const auto& points = ref.GetPoints(); 	\
	const auto& B = ref.GetB();

double
BodyEnergy::EEnergy(const Mesh &reference, const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)
	double energy = 0;
	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto D = GetDs(points, tets[i]);
		energy += _elas_model->Energy(*_cons_model, 0, B[i], D);
	}
	return energy;
}

VectorXd
BodyEnergy::EGradient(const Mesh &reference, const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)

	VectorXd gradient(X.size());
	gradient.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		auto D = GetDs(points, tet);
		Vector12d local_gradient = _elas_model->Gradient(*_cons_model, 0, B[i],
														 D);
		for (int j = 0; j < 4; j++) {
			gradient.block<3, 1>(3 * tet[j], 0) += local_gradient.block<3, 1>(3 * j, 0);
		}
	}

	return gradient;
}

MatrixXd
BodyEnergy::EHessian(const Mesh &reference, const VectorXd &X) {
	GET_MEMBERS(reference, tets, points, B)

	MatrixXd hessian(X.size(), X.size());
	hessian.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		auto D = GetDs(points, tet);
		Matrix12d local_hessian = _elas_model->Hessian(*_cons_model, 0, B[i],
													   D);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				hessian.block<3, 3>(3 * tet[j], 3 * tet[k]) += local_hessian.block<3, 3>(3 * j, 3 * k);
			}
		}
	}

	return hessian;
}

double BodyEnergy::DEnergy(const Mesh &reference, const VectorXd &X,
							 const VectorXd &V) {
	GET_MEMBERS(reference, tets, points, B)
	const auto& mass = reference.GetMass();

	double energy = 0;
	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = V.block<3, 1>(3 * tet[j], 0);
			m(j) = mass(tet[j]);
		}
		auto D = GetDs(points, tet);
		energy += _diss_model->Energy(*_cons_model, *_elas_model, 0, B[i], m, v,
									  D);
	}
	return energy;
}

VectorXd BodyEnergy::DGradient(const Mesh &reference, const VectorXd &X,
							   const VectorXd &V) {
	GET_MEMBERS(reference, tets, points, B)
	const auto& mass = reference.GetMass();

	VectorXd gradient(X.size());
	gradient.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = V.block<3, 1>(3 * tet[j], 0);
			m(j) = mass(tet[j]);
		}
		auto D = GetDs(points, tet);
		Vector12d local_gradient = _diss_model->Gradient(*_cons_model,
														 *_elas_model, 0, B[i],
														 m, v, D);
		for (int j = 0; j < 4; j++) {
			gradient.block<3, 1>(3 * tet[j], 0) += local_gradient.block<3, 1>(3 * j, 0);
		}
	}

	return gradient;
}

MatrixXd BodyEnergy::DHessian(const Mesh &reference, const VectorXd &X,
							  const VectorXd &V) {
	GET_MEMBERS(reference, tets, points, B)
	const auto& mass = reference.GetMass();

	MatrixXd hessian(X.size(), X.size());
	hessian.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = V.block<3, 1>(3 * tet[j], 0);
			m(j) = mass(tet[j]);
		}
		auto D = GetDs(points, tet);
		Matrix12d local_hessian = _diss_model->Hessian(*_cons_model,
													   *_elas_model, 0, B[i], m,
													   v, D);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				hessian.block<3, 3>(3 * tet[j], 3 * tet[k]) += local_hessian.block<3, 3>(3 * j, 3 * k);
			}
		}
	}

	return hessian;
}

BodyEnergy::~BodyEnergy() {
	delete _elas_model;
	delete _diss_model;
	delete _cons_model;
}

DEFINE_CLONE(BodyEnergy, BodyEnergy)
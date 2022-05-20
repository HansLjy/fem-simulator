//
// Created by hansljy on 2022/2/23.
//

#include "BodyEnergy.h"
#include "Object/SoftBody/SoftBody.h"
#include "Util/Factory.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <vector>
#include <omp.h>

typedef Eigen::Triplet<double> Triplet;

BodyEnergyParameter::BodyEnergyParameter(
		const ElasticEnergyModelType &elas_type,
		const ElasticEnergyModelParameter &elas_para,
		const DissipationEnergyModelType &diss_type,
		const DissipationEnergyModelParameter &diss_para,
		const ConstituteModelType &cons_type,
		const ConstituteModelParameter &cons_para) :
		_elas_type(elas_type), _elas_para(elas_para.Clone()),
		_diss_type(diss_type), _diss_para(diss_para.Clone()),
		_cons_type(cons_type), _cons_para(cons_para.Clone()) {}

DEFINE_CLONE(BodyEnergyParameter, BodyEnergyParameter)

DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, ElasticEnergyModelType, ElasticEnergyModelType, _elas_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, ElasticEnergyModelParameter, ElasticEnergyModelParameter, _elas_para)
DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, DissipationEnergyModelType, DissipationEnergyModelType, _diss_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, DissipationEnergyModelParameter, DissipationEnergyModelParameter, _diss_para)
DEFINE_ACCESSIBLE_MEMBER(BodyEnergyParameter, ConstituteModelType, ConstituteModelType, _cons_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, ConstituteModelParameter, ConstituteModelParameter, _cons_para)

#include <Eigen/Eigenvalues>

/**
 * Project a matrix into a positive definite one
 */

BodyEnergy::BodyEnergy(const SoftBody *soft_body) : _body(soft_body){}

template<int dim>
void PositiveProject(Eigen::Matrix<double, dim, dim>& matrix) {
	typedef Eigen::Matrix<double, dim, dim> MatrixOfSize;
	typedef Eigen::Vector<double, dim> VectorOfSize;
	Eigen::SelfAdjointEigenSolver<MatrixOfSize> eigens(matrix);
	MatrixOfSize eigen_vectors = eigens.eigenvectors();
	VectorOfSize eigen_values = eigens.eigenvalues();
	for (int i = 0; i < dim; i++) {
		if (eigen_values[i] < 0) {
			eigen_values[i] = 0;
		}
	}
	matrix = eigen_vectors * eigen_values.asDiagonal() * eigen_vectors.transpose();
}

void BodyEnergy::Initialize(const BodyEnergyParameter &para) {
	_cons_model = ConstituteModelFactory::GetInstance()->GetConstituteModel(para.GetConstituteModelType());
	_cons_model->Initialize(*para.GetConstituteModelParameter());
	_elas_model = ElasticEnergyModelFactory::GetInstance()->GetElasticEnergyModel(para.GetElasticEnergyModelType());
	_elas_model->Initialize(*para.GetElasticEnergyModelParameter());
	_diss_model = DissipationEnergyModelFactory::GetInstance()->GetDissipationEnergyModel(para.GetDissipationEnergyModelType());
	_diss_model->Initialize(*para.GetDissipationEnergyModelParameter());

	spdlog::info("BodyEnergy initialized");
}

inline Matrix3d GetDs(const VectorXd& X, const std::vector<int>& tet) {
	Vector3d Xi[4];
	for (int i = 0; i < 4; i++) {
		Xi[i] = X.block<3, 1>(3 * tet[i], 0);
	}
	Matrix3d D;
	for (int i = 0; i < 3; i++) {
		D.col(i) = Xi[i] - Xi[3];
	}
	return D;
}

double BodyEnergy::EEnergy() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
	double energy = 0;
	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto D = GetDs(X, tets[i]);
		energy += _elas_model->Energy(*_cons_model, _body->_volume[i], _body->_inv[i], D);
	}
	return energy;
}

VectorXd
BodyEnergy::EGradient() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
	VectorXd gradient(X.size());
	gradient.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		auto D = GetDs(X, tet);
		Vector12d local_gradient = _elas_model->Gradient(*_cons_model, _body->_volume[i], _body->_inv[i], D);
		for (int j = 0; j < 4; j++) {
			gradient.block<3, 1>(3 * tet[j], 0) += local_gradient.block<3, 1>(3 * j, 0);
		}
	}

	return gradient;
}

SparseMatrixXd
BodyEnergy::EHessian() const {
	SparseMatrixXd hessian(_body->GetDOF(), _body->GetDOF());
	COO triplets = EHessianCOO();
	hessian.setFromTriplets(triplets.begin(), triplets.end());
	return hessian;
}

COO BodyEnergy::EHessianCOO() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
//	auto t = clock();

	int num_of_tets = tets.size();

//	auto start_allocation = clock();
	std::vector<Matrix12d> local_hessian(num_of_tets);
	std::vector<Triplet> triplets;
//	spdlog::info("Time spent in allocation: {} s", (double)(clock() - start_allocation) / CLOCKS_PER_SEC);

//	auto start_computing = clock();
//	#pragma omp parallel for default(none) shared(num_of_tets, tets, points, local_hessian, W, inv) num_threads(4)
	for (int i = 0; i < num_of_tets; i++) {
		auto& tet = tets[i];
		auto D = GetDs(X, tet);
		local_hessian[i] = _elas_model->Hessian(*_cons_model, _body->_volume[i], _body->_inv[i], D, _body->_pFpX[i]);
	}
	for (int i = 0; i < num_of_tets; i++) {
		PositiveProject(local_hessian[i]);
	}
//	spdlog::info("Time spent in computing hessian: {} s", (double) (clock() - start_computing) / CLOCKS_PER_SEC);

//	auto start_assembly = clock();
	for (int i = 0; i < num_of_tets; i++) {
		auto& tet = tets[i];
		auto& local = local_hessian[i];
//		#pragma omp parallel for default(none) shared(triplets, tet, local_hessian, i)
		for (int j = 0; j < 4; j++) {
			const int base_row = 3 * tet[j];
			const int base_row_local = 3 * j;
			for (int k = 0; k < 4; k++) {
				const int base_col = 3 * tet[k];
				const int base_col_local = 3 * k;
				for (int  row = 0; row < 3; row++) {
					for (int col = 0; col < 3; col++) {
						triplets.push_back(Triplet(base_row + row, base_col + col, local(base_row_local + row, base_col_local + col)));
					}
				}
			}
		}
	}
	return triplets;
}

double
BodyEnergy::DEnergy() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
	double energy = 0;
	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = _body->_v.block<3, 1>(3 * tet[j], 0);
			m(j) = _body->_mass(tet[j]);
		}
		auto D = GetDs(X, tet);
		energy += _diss_model->Energy(*_cons_model, *_elas_model, _body->_volume[i], _body->_inv[i], m, v, D, _body->_pFpX[i]);
	}
	return energy;
}

VectorXd BodyEnergy::DGradient() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
	VectorXd gradient(X.size());
	gradient.setZero();

	int num_of_tets = tets.size();
	for (int i = 0; i < num_of_tets; i++) {
		auto tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = _body->_v.block<3, 1>(3 * tet[j], 0);
			m(j) = _body->_mass(tet[j]);
		}
		auto D = GetDs(X, tet);
		Vector12d local_gradient = _diss_model->Gradient(*_cons_model,
														 *_elas_model, _body->_volume[i],
														 _body->_inv[i],
														 m, v, D,
														 _body->_pFpX[i]);
		for (int j = 0; j < 4; j++) {
			gradient.block<3, 1>(3 * tet[j], 0) += local_gradient.block<3, 1>(3 * j, 0);
		}
	}

	return gradient;
}

SparseMatrixXd BodyEnergy::DHessian() const {
	SparseMatrixXd hessian(_body->GetDOF(), _body->GetDOF());
	COO triplets = DHessianCOO();
	hessian.setFromTriplets(triplets.begin(), triplets.end());
	return hessian;
}

COO BodyEnergy::DHessianCOO() const {
	const auto& tets = _body->_mesh.GetTets();
	const auto& X = _body->_mesh.GetPoints();
	int num_of_tets = tets.size();
	std::vector<Matrix12d> local_hessian(num_of_tets);
	std::vector<Triplet> triplets;

//	#pragma omp parallel for default(none) shared(num_of_tets, tets, mass, points, local_hessian, W, inv, V) num_threads(4)
	for (int i = 0; i < num_of_tets; i++) {
		auto& tet = tets[i];
		Vector12d v;
		Vector4d m;
		for (int j = 0; j < 4; j++) {
			v.block<3, 1>(3 * j, 0) = _body->_v.block<3, 1>(3 * tet[j], 0);
			m(j) = _body->_mass(tet[j]);
		}
		auto D = GetDs(X, tet);
		local_hessian[i] = _diss_model->Hessian(*_cons_model, *_elas_model,
												_body->_volume[i], _body->_inv[i], m, v, D, _body->_pFpX[i]);
	}
	for (int i = 0; i < num_of_tets; i++) {
		PositiveProject(local_hessian[i]);
	}

	for (int i = 0; i < num_of_tets; i++) {
		auto& tet = tets[i];
//		#pragma omp parallel for default(none) shared(triplets, tet, local_hessian, i)
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				const int base_row = 3 * tet[j];
				const int base_col = 3 * tet[k];
				const int base_row_local = 3 * j;
				const int base_col_local = 3 * k;
				for (int  row = 0; row < 3; row++) {
					for (int col = 0; col < 3; col++) {
						triplets.push_back(Triplet(base_row + row, base_col + col, local_hessian[i](base_row_local + row, base_col_local + col)));
					}
				}
			}
		}
	}
	return triplets;
}



BodyEnergy::~BodyEnergy() {
	delete _elas_model;
	delete _diss_model;
	delete _cons_model;
}

BodyEnergy::BodyEnergy(const BodyEnergy &body_energy) {
	delete _elas_model;
	_elas_model = body_energy._elas_model->Clone();
	delete _diss_model;
	_diss_model = body_energy._diss_model->Clone();
	delete _cons_model;
	_cons_model = body_energy._cons_model->Clone();
}

DEFINE_CLONE(BodyEnergy, BodyEnergy)
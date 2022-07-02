//
// Created by hansljy on 22-5-20.
//

#include "Object.h"

SparseMatrixXd Object::InternalEnergyHessian() const {
	SparseMatrixXd hessian(GetDOF(), GetDOF());
	COO coo = InternalEnergyHessianCOO();
	hessian.setFromTriplets(coo.begin(), coo.end());
	return hessian;
}

double Object::Energy() const {
	return InternalEnergy() + ExternalEnergy();
}

VectorXd Object::EnergyGradient() const {
	return InternalEnergyGradient() + ExternalEnergyGradient();
}

SparseMatrixXd Object::EnergyHessian() const {
	return InternalEnergyHessian() + ExternalEnergyHessian();
}

void Object::AddExternalForce(const ExternalForce &external_force) {
	_external_force.push_back(external_force.Clone());
}

double Object::ExternalEnergy() const {
	double external_energy = 0;
	for (auto& external_force : _external_force) {
		external_energy += external_force->Energy(*this);
	}
	return external_energy;
}

VectorXd Object::ExternalEnergyGradient() const {
	VectorXd gradient(GetDOF());
	gradient.setZero();
	for (auto& force : _external_force) {
		gradient += force->Gradient(*this);
	}
	return gradient;
}

SparseMatrixXd Object::ExternalEnergyHessian() const {
	SparseMatrixXd hessian(GetDOF(), GetDOF());
	for (auto& force : _external_force) {
		hessian += force->Hessian(*this);
	}
	return hessian;
}

COO Object::ExternalEnergyHessianCOO() const {
	SparseMatrixXd hessian = ExternalEnergyHessian();
	COO hessian_coo;
	for (int i = 0; i < hessian.outerSize(); i++) {
		for (SparseMatrixXd::InnerIterator it(hessian, i); it; ++it) {
			hessian_coo.push_back(Tripletd(it.row(), it.col(), it.value()));
		}
	}
	return hessian_coo;
}

Object::~Object() noexcept {
	for (auto& ext_force : _external_force) {
		delete ext_force;
	}
}

Object::Object(const Object &obj) {
	for (auto& ext_force : obj._external_force) {
		_external_force.push_back(ext_force->Clone());
	}
}

Object &Object::operator=(const Object &rhs) {
	if (this == &rhs) {
		return *this;
	}
	for (auto& ext_force : _external_force) {
		delete ext_force;
	}
	_external_force.clear();
	for (auto& ext_force : rhs._external_force) {
		_external_force.push_back(ext_force);
	}
	return *this;
}


//
// Created by hansljy on 22-5-20.
//

#include "Object.h"

SparseMatrixXd Object::EnergyHessian() const {
	SparseMatrixXd hessian(GetDOF(), GetDOF());
	COO coo = EnergyHessianCOO();
	hessian.setFromTriplets(coo.begin(), coo.end());
	return hessian;
}

void Object::AddExternalForce(const ExternalForce &external_force) {
	_external_force.push_back(external_force.Clone());
}

double Object::ExternalEnergy() const {
	double external_energy = 0;
	for (auto& external_force : _external_force) {
		external_energy += external_force->Energy();
	}
	return external_energy;
}

VectorXd Object::ExternalEnergyGradient() const {
	VectorXd gradient(GetDOF());
	gradient.setZero();
	for (auto& force : _external_force) {
		gradient += force->Gradient();
	}
	return gradient;
}

SparseMatrixXd Object::ExternalEnergyHessian() const {
	SparseMatrixXd hessian(GetDOF(), GetDOF());
	for (auto& force : _external_force) {
		hessian += force->Hessian();
	}
	return hessian;
}

COO Object::ExternalEnergyHessianCOO() const {
	SparseMatrixXd hessian = ExternalEnergyHessian();
	COO hessian_coo;
	for (int i = 0; i < hessian.outerSize(); i++) {
		for (SparseMatrixXd::InnerIterator it(hessian, i); it; ++it) {
			hessian_coo.push_back(Triplet(it.row(), it.col(), it.value()));
		}
	}
	return hessian_coo;
}
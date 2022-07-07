//
// Created by hansljy on 22-5-20.
//

#include "Object.h"

double Object::Energy() const {
	return InternalEnergy() + ExternalEnergy();
}

VectorXd Object::EnergyGradient() const {
	return InternalEnergyGradient() + ExternalEnergyGradient();
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

void
Object::ExternalEnergyHessianCOO(COO &coo, int x_offset, int y_offset) const {
	for (auto& ext_force : _external_force) {
		ext_force->HessianCOO(*this, coo, x_offset, y_offset);
	}
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


//
// Created by hansljy on 2022/2/23.
//

#include "PlaneConstraint.h"

PlaneConstraint::PlaneConstraint(const Vector3d &norm, const Vector3d &point) : _norm(norm), _point(point) {}

double PlaneConstraint::Value(const VectorXd &x) const {
	int dimension = x.size();
	double result = 0;
	for (int i = 0; i < dimension; i += 3) {
		result += (x.block<3, 1>(i, 0) - _point).dot(_norm);
	}
	return result;
}

VectorXd PlaneConstraint::Gradient(const VectorXd &x) const {
	int dimension = x.size();
	VectorXd gradient(dimension);
	for (int i = 0; i < dimension; i += 3) {
		gradient.block<3, 1>(i, 0) = _norm;
	}
	return gradient;
}

MatrixXd PlaneConstraint::Hessian(const VectorXd &x) const {
	MatrixXd hessian(x.size(), x.size());
	hessian.setZero();
	return hessian;
}

DEFINE_CLONE(Function, PlaneConstraint)
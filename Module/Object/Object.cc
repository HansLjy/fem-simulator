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
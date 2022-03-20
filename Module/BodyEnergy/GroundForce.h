//
// Created by hansljy on 2022/3/14.
//

#ifndef FEM_GROUNDFORCE_H
#define FEM_GROUNDFORCE_H

#include "ExternalForce.h"

class GroundForce : public ExternalForce {
public:
	explicit GroundForce(double stiffness);

	double Energy(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;
	VectorXd Gradient(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;
	SparseMatrixXd
	Hessian(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

private:
	double _stiffness;
};

#endif //FEM_GROUNDFORCE_H

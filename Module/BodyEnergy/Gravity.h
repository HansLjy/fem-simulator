//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_GRAVITY_H
#define FEM_GRAVITY_H

#include "ExternalForce.h"

class Gravity : public ExternalForce {
public:
	Gravity(double g);
	~Gravity() = default;

	double Energy(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;
	VectorXd Gradient(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;
	SparseMatrixXd
	Hessian(const Mesh &mesh, const VectorXd &mass, const VectorXd &X, const VectorXd &V) const override;

	DERIVED_DECLARE_CLONE(ExternalForce)

private:
	double _g;
};

#endif //FEM_GRAVITY_H

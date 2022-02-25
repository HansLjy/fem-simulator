//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_PLANECONSTRAINTS_H
#define FEM_PLANECONSTRAINTS_H

#include "Constraint/Constraint.h"
#include "Util/Pattern.h"

using Eigen::Vector3d;

class PlaneConstraint : public Constraint {
public:
	void SetPlane(const Vector3d& norm, const Vector3d& point);
	double Value(const VectorXd &x) const override;
	VectorXd Gradient(const VectorXd &x) const override;
	MatrixXd Hessian(const VectorXd &x) const override;

	DERIVED_DECLARE_CLONE(Function)

private:
	Vector3d _norm, _point;
};

#endif //FEM_PLANECONSTRAINTS_H

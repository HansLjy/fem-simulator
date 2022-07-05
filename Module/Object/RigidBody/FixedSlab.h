//
// Created by hansljy on 22-5-24.
//

#ifndef FEM_FIXEDSLAB_H
#define FEM_FIXEDSLAB_H

#include "RigidBody.h"
#include "Shape/Rectangle.h"

class FixedSlab : public RigidBody {
public:

	FixedSlab(double mu, double rho, const Vector3d &center,
			  const Vector3d &euler_angles, const Vector3d &size);

	int GetDOF() const override {
		return 0;
	}
	Vector3d GetCenter() const override {
		return _center;
	}

	Matrix3d GetRotation() const override {
		return _rotation;
	}

	SparseMatrixXd GetJ(int idx, const Vector3d &point) const override;

	DERIVED_DECLARE_CLONE(Object)

protected:
};

#endif //FEM_FIXEDSLAB_H

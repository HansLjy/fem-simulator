//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_RIGIDBODY_H
#define FEM_RIGIDBODY_H

#include "Util/EigenAll.h"

// Currently, rigid bodies are not included into the dynamic
// system, so we only left an interface for contact generation
class RigidBody {
public:
	RigidBody(double mu) : _mu(mu) {}

	double GetMu() const {
		return _mu;
	}
	virtual double Distance(Vector3d point) const = 0;
	virtual Vector3d Normal(Vector3d point) const = 0;

protected:
	double _mu;
};

#endif //FEM_RIGIDBODY_H

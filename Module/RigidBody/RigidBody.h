//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_RIGIDBODY_H
#define FEM_RIGIDBODY_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"
#include <string>

// Currently, rigid bodies are not included into the dynamic
// system, so we only left an interface for contact generation
class RigidBody {
public:
	explicit RigidBody(double mu) : _mu(mu) {}

	double GetMu() const {
		return _mu;
	}
	virtual double Distance(const Vector3d &point) const { return 1;}
	virtual Vector3d Normal(const Vector3d &point) const { return Vector3d::Zero(); }
	virtual void Store(const std::string& path) = 0;

	virtual ~RigidBody() = default;

	BASE_DECLARE_CLONE(RigidBody)

protected:
	double _mu;
};

#endif //FEM_RIGIDBODY_H

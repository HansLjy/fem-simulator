//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_FRICTION_H
#define FEM_FRICTION_H

#include "ContactGenerator.h"
#include <stdexcept>

enum class FrictionModelType {
	kInscribedPolygon
};

class FrictionModelParameter {
public:
	FrictionModelParameter() = default;
	FrictionModelParameter(const FrictionModelParameter& rhs) = default;
	virtual ~FrictionModelParameter() = default;
	BASE_DECLARE_CLONE(FrictionModelParameter)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(int, NumTangent)
};

class FrictionModel {
public:
	virtual void Initialize(const FrictionModelParameter &para) {};

	/**
	 * @param system INPUT, the physical systems
	 * @param contacts INPUT, the contact points
	 * @param JnT OUTPUT, JnT is the transpose of Jn whose columns are the
	 * 			  normal vector of contact normal forces
	 * @param JtT OUTPUT, JtT is the transpose of Jt whose columns are the
	 *            normal vector of contact friction forces
	 * @param Mu  OUTPUT, coefficients of friction for each contact ponits
	 */
	virtual void
	GetJ(const System &system, const vector<ContactPoint> &contacts,
		 SparseMatrixXd &JnT, SparseMatrixXd &JtT, VectorXd &Mu) const = 0;

	virtual int GetNumTangent() const {
		throw std::logic_error("Unimplemented method");
	}
};

#endif //FEM_FRICTION_H

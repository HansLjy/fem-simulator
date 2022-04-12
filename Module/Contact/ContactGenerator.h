//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_CONTACTGENERATOR_H
#define FEM_CONTACTGENERATOR_H

#include "Util/EigenAll.h"
#include "System/System.h"

enum class ContactGeneratorType {
	kPolyhedralCone
};

class ContactGeneratorParameter {
public:
	explicit ContactGeneratorParameter(int num_tangent) : _num_tagent(num_tangent) {}
	BASE_DECLARE_CLONE(ContactGeneratorParameter)
	virtual ~ContactGeneratorParameter() = default;

	DECLARE_ACCESSIBLE_MEMBER(int, NumTangent, _num_tagent)
};

class ContactGenerator {
public:
	virtual void Initialize(const ContactGeneratorParameter& para) {
		_num_tangent = para.GetNumTangent();
	}
	virtual void
	GetContact(const System &system, SparseMatrixXd &Jn, SparseMatrixXd &Jt,
			   VectorXd &mu) const = 0;
	int GetNumTangent() const {
		return _num_tangent;
	}

	virtual ~ContactGenerator() = default;

protected:
	int _num_tangent;
};

#endif //FEM_CONTACTGENERATOR_H

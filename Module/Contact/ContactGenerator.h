//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_CONTACTGENERATOR_H
#define FEM_CONTACTGENERATOR_H

#include "Util/EigenAll.h"
#include "System/System.h"
#include "DCD/DCD.h"

enum class ContactGeneratorType {
	kDCD,
	kCCD
};

class ContactGeneratorParameter {
public:
	ContactGeneratorParameter() = default;
	virtual ~ContactGeneratorParameter() = default;
	BASE_DECLARE_CLONE(ContactGeneratorParameter)

	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(DCDType, DCDType)
	DECLARE_VIRTUAL_ACCESSIBLE_POINTER_MEMBER(DCDParameter, DCDPara)
};

struct ContactPoint {
	ContactPoint(int obj1, int obj2,
				 const SurfaceElements::SurfaceType &type1,
				 const SurfaceElements::SurfaceType &type2,
				 int idx1, int idx2, const Vector3d &point,
				 const Vector3d &normal, double mu)
			: _obj1(obj1), _obj2(obj2),
			  _type1(type1), _type2(type2),
			  _idx1(idx1), _idx2(idx2),
			  _point(point), _normal(normal),
			  _mu(mu) {}

	int _obj1, _obj2;
	SurfaceElements::SurfaceType _type1, _type2;
	int _idx1, _idx2;
	Vector3d _point;
	Vector3d _normal;    // normal points from 1 to 2
	double _mu;
};

class ContactGenerator {
public:
	virtual void Initialize(const ContactGeneratorParameter &para) {}

	virtual void
	GetContact(const System &system,
			   vector<ContactPoint> &contact_points) const = 0;

	virtual ~ContactGenerator() = default;
};

#endif //FEM_CONTACTGENERATOR_H

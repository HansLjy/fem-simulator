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

// TODO: coefficients of friction can be derived from colliding objects
//       and have no close connection with the contact, it should be removed
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

	int _obj1, _obj2;	// The ids of the colliding objects
	SurfaceElements::SurfaceType _type1, _type2;	// The types of the colliding primitives
	int _idx1, _idx2;	// The ids of colliding primitives on the two surfaces
	Vector3d _point;	// The colliding points
	Vector3d _normal;	// Normal points from 1 to 2
	double _mu;			// Friction coefficients of the contacts
};

class ContactGenerator {
public:
	virtual void Initialize(const ContactGeneratorParameter &para) {}

	/**
	 * @param system INPUT, the physics systems
	 * @param contact_points OUTPUT, it will be cleared and loaded with contact points
	 */
	virtual void
	GetContact(const System &system,
			   vector<ContactPoint> &contact_points) const = 0;

	virtual ~ContactGenerator() = default;
};

#endif //FEM_CONTACTGENERATOR_H

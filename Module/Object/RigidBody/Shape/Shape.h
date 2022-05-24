//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_SHAPE_H
#define FEM_SHAPE_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"
#include "Contact/Surface.h"

class RigidBody;

/**
 * The class shape is used for the case when there is no
 * deformation, once the shape is set, it cannot be changed
 * This is the common case for rigid bodies.
 */
class Shape {
public:
	Shape(const RigidBody& rigid_body) : _rigid_body(&rigid_body) {}

	virtual void Store(const std::string& file) const = 0;
	virtual int GetNumFaces() const = 0;
	virtual SurfaceElements::Face GetFace(int idx) const = 0;
	virtual double GetVolume() const = 0;

	virtual ~Shape() = default;

	BASE_DECLARE_CLONE(Shape)

protected:
	const RigidBody* _rigid_body;
};

#endif //FEM_SHAPE_H

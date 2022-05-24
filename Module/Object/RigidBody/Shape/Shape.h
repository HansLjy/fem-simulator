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
	Shape() = default;

	virtual void Store(const std::string &file, const Matrix3d &rotation,
					   const Vector3d &center) const = 0;
	virtual int GetNumFaces() const = 0;
	virtual SurfaceElements::Face
	GetFace(int idx, const Matrix3d &rotation, const Vector3d &center) const = 0;
	virtual double GetVolume() const = 0;

	Shape(const Shape& rhs) = default;
	virtual ~Shape() = default;

	BASE_DECLARE_CLONE(Shape)
};

#endif //FEM_SHAPE_H

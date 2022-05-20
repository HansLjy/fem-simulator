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
	Shape(const Vector3d& center, const Vector3d& euler_angles) : _center(center) {
		_rotation = AngleAxisd(euler_angles(0), Vector3d::UnitZ())
					* AngleAxisd(euler_angles(1), Vector3d::UnitX())
					* AngleAxisd(euler_angles(2), Vector3d::UnitZ());
	}

	/**
	 * Get the distance and direction of the point to the object
	 * @param point INPUT, the point to which the distance should be calculated
	 * @param direction OUTPUT, the gradient of the distance function (w.r.t.
	 * 					the coordinates of the point.)
	 * @return The distance of the point to the shape
	 */
	double GetDistance(const Vector3d & point, Vector3d& direction) const {
		/* The coordinate of point with respect to the current object */
		Vector3d coord = _rotation.transpose() * (point - _center);
		Vector3d normal_after_transform;
		double distance = GetDistanceAfterTransform(point, normal_after_transform);
		direction = _rotation * normal_after_transform;
		return distance;
	}

	/**
	 * Get the distance and direction of the point to the object after
	 * the coordinate of the point has been transformed into the coor-
	 * dinate system of the shape(namely, the center of the object can
	 * now be viewed as in the origin.)
	 * @param point INPUT, the coordinate of the point after the transform
	 * @param direction OUTPUT, the gradient of the distance after the transform
	 * @return the distance of the point to the object
	 */
	virtual double GetDistanceAfterTransform(const Vector3d& point, Vector3d& direction) const = 0;

	virtual void Store(const std::string& file) const = 0;
	virtual int GetNumFaces() const = 0;
	virtual SurfaceElements::Face GetFace(int idx) const = 0;

	virtual double GetVolume() const = 0;

	virtual ~Shape() = default;

	BASE_DECLARE_CLONE(Shape)
public:
	Vector3d _center;
	Matrix3d _rotation;
};

#endif //FEM_SHAPE_H

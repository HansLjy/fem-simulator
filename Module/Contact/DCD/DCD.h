//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_DCD_H
#define FEM_DCD_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"

enum class DCDType {
	kFast
};

class DCDParameter {
public:
	DCDParameter(int max_iter, double tolerance) : _max_iter(max_iter), _tolerance(tolerance) {}
	DERIVED_DECLARE_CLONE(DCDParameter)
	DECLARE_ACCESSIBLE_MEMBER(int, MaxIter, _max_iter)
	DECLARE_ACCESSIBLE_MEMBER(double, Tolerance, _tolerance)

};

class DCD {
public:
	virtual void Initialize(const DCDParameter& para) = 0;

	/**
	 * Test the intersection of two triangles
	 * @param face1 INPUT, the face to be tested for intersection
	 * @param face2 INPUT, the face to be tested for intersection
	 * @param point OUTPUT, the intersection point
	 * @param normal OUTPUT, the normal of the intersection plane
	 * @note normally the two objects will not be tangent in the
	 *       intersection point(for example, the face-vertex case)
	 *       in that case, the normal is chosen as the normal of
	 *       either surface.
	 * @return whether there is an intersection
	 */
	virtual bool GetIntersected(
			Vector3d A1, Vector3d B1, Vector3d C1,
			Vector3d A2, Vector3d B2, Vector3d C2,
			Vector3d &point, Vector3d &normal) = 0;

	virtual ~DCD() = default;

protected:
	int _max_iter;
	double _tolerance;
};

#endif //FEM_DCD_H

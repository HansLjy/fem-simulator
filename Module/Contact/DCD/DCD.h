//
// Created by hansljy on 22-5-20.
//

#ifndef FEM_DCD_H
#define FEM_DCD_H

#include "Util/EigenAll.h"
#include "Contact/Surface.h"
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
	virtual bool GetIntersected(const SurfaceElements::Face &face1,
								const SurfaceElements::Face &face2,
								Vector3d &point,
								Vector3d &normal) = 0;

	virtual ~DCD() = default;

protected:
	int _max_iter;
	double _tolerance;
};

#endif //FEM_DCD_H

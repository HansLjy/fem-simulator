//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_OBJECT_H
#define FEM_OBJECT_H

#include "Util/EigenAll.h"
#include "Contact/Surface.h"
#include "Util/Pattern.h"

class Object {
public:
	Object() = default;

	// The status of the object
	virtual int GetDOF() const = 0;
	virtual const VectorXd& GetX() const = 0;
	virtual VectorXd& GetX() = 0;
	virtual const VectorXd& GetV() const = 0;
	virtual VectorXd& GetV() = 0;

	// The inner state of the object
	virtual const COO& GetM() const = 0;

	// The inner energy of the object
	virtual double Energy() const = 0;
	virtual VectorXd EnergyGradient() const = 0;
	virtual COO EnergyHessianCOO() const = 0;
	SparseMatrixXd EnergyHessian() const;

	// For contact simulation
	virtual const Surface * GetSurface() const = 0;
	virtual COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
					 const VectorXd &point,
					 const VectorXd &normal) const = 0;
	virtual double GetMu() const = 0;

	// For output
	virtual void Store(const std::string& file) = 0;

	virtual ~Object() = default;
	Object(const Object& obj) = default;

	BASE_DECLARE_CLONE(Object)
};

#endif //FEM_OBJECT_H

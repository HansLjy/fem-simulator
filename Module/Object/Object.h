//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_OBJECT_H
#define FEM_OBJECT_H

#include "Util/EigenAll.h"
#include "Contact/Surface.h"
#include "Util/Pattern.h"
#include "BodyEnergy/ExternalForce.h"
#include <vector>

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

	// All energy combined
	double Energy() const;
	VectorXd EnergyGradient() const;
	SparseMatrixXd EnergyHessian() const;

	// For contact simulation
	virtual const Surface * GetSurface() const = 0;
	virtual COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
					 const VectorXd &point,
					 const VectorXd &normal) const = 0;
	virtual double GetMu() const = 0;

	void AddExternalForce(const ExternalForce& external_force);

	// For output
	virtual void Store(const std::string& file) = 0;
	virtual ~Object() noexcept;

	Object(const Object& obj);

	BASE_DECLARE_CLONE(Object)

protected:
	// The internal energy of the object
	virtual double InternalEnergy() const = 0;
	virtual VectorXd InternalEnergyGradient() const = 0;
	virtual COO InternalEnergyHessianCOO() const = 0;

	// The external force of the object
	SparseMatrixXd InternalEnergyHessian() const;
	double ExternalEnergy() const;
	VectorXd ExternalEnergyGradient() const;
	COO ExternalEnergyHessianCOO() const;
	SparseMatrixXd ExternalEnergyHessian() const;

	std::vector<const ExternalForce*> _external_force;
};

#endif //FEM_OBJECT_H

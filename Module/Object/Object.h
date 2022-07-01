//
// Created by hansljy on 22-5-12.
//

#ifndef FEM_OBJECT_H
#define FEM_OBJECT_H

#include "Util/EigenAll.h"
#include "DOFShapeConverter.h"
#include "Util/Pattern.h"
#include "BodyEnergy/ExternalForce.h"
#include <vector>

class Object {
public:
	Object() = default;

	//-> degree of freedom of the objects
	virtual int GetDOF() const = 0;

	//-> status vector of the object
	virtual const VectorXd& GetX() const = 0;

	//-> status vector of the object
	virtual VectorXd& GetX() = 0;

	//-> changing rate of the status vector (in a normal sense, velocity)
	virtual const VectorXd& GetV() const = 0;

	//-> changing rate of the status vector (in a normal sense, velocity)
	virtual VectorXd& GetV() = 0;

	//-> COO form of the mass for the objects
	// Technically speaking, an object of DOF n should possess a mass matrix of
	// size n by n DEFINED by F = Ma
	virtual const COO& GetM() const = 0;

	//-> All energy considered for this object (Both internal and external)
	// Note: collision force is not included
	double Energy() const;

	//-> Gradient of the energy against status vector, negate of the total force
	// (collision force not included)
	VectorXd EnergyGradient() const;

	//-> Hessian of the energy, negate of the derivative of the total force
	// (collision force not included) against the status vector
	SparseMatrixXd EnergyHessian() const;

	// For contact simulation
	virtual const DOFShapeConverter * GetDOFShapeConverter() const = 0;

	virtual double GetMu() const = 0;

	void AddExternalForce(const ExternalForce& external_force);

	/**
	 * Store the file in vtk form
	 * @param file filename to be stored (path included)
	 */
	virtual void Store(const std::string& file) = 0;
	virtual ~Object() noexcept;

	Object(const Object& obj);
	Object& operator=(const Object& rhs);

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

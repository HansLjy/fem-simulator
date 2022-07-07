//
// Created by hansljy on 2022/3/11.
//

#ifndef FEM_EXTERNALFORCE_H
#define FEM_EXTERNALFORCE_H

#include "Util/EigenAll.h"
#include "Util/Pattern.h"

class Object;

/**
 * ExternalForce: the class corresponds to the calculation of external forces
 * added on the objects.
 */
class ExternalForce {
public:
	/**
	 * External energy added on the objects.
	 * @note: Not all forces are conservative, but we can view them as
	 * 		  conservative at least locally. As a result, this function
	 * 		  actually returns the negate of the work done by the force
	 */
	virtual double Energy(const Object &obj) const = 0;

	//->Negate of the external force
	virtual VectorXd Gradient(const Object &obj) const = 0;

	//->Negate of the derivative of external force against DOF
	virtual void
	HessianCOO(const Object &obj, COO &coo, int x_offset, int y_offset) const = 0;

	virtual ~ExternalForce() = default;

	BASE_DECLARE_CLONE(ExternalForce);

protected:
};

#endif //FEM_EXTERNALFORCE_H

//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_ELEMENTENERGY_H
#define FEM_ELEMENTENERGY_H

#include "ConstituteModel/ConstituteModel.h"
#include "Util/Pattern.h"
#include "Util/EigenAll.h"


enum class ElasticEnergyModelType {
	kSimple,
};

class ElasticEnergyModelParameter {
public:
	ElasticEnergyModelParameter() = default;

	BASE_DECLARE_CLONE(ElasticEnergyModelParameter)
	virtual ~ElasticEnergyModelParameter() = default;
};

class ElasticEnergyModel {
public:
	virtual void Initialize(const ElasticEnergyModelParameter& para);

	/**
	 * Calculate the elasticity of a single tet
	 * @param cons_model constitute model
	 * @param W Volume of the tet
	 * @param B Suppose the vertices of the reference tet is {X1, X2, X3, X4},
	 *          then B is the inverse of the 3 by 3 matrix [X1 - X4, X2 - X4, X3 - X4]
	 * @param Ds Suppose the vertices of the current tet is {x1, x2, x3, x4},
	 *          then Ds is the 3 by 3 matrix [x1 - x4, x2 - x4, x3 - x4]
	 * @return The elasticity energy of a single tet
	 */
	virtual double
	Energy(const ConstituteModel &cons_model, double W, const Matrix3d &B,
		   const Matrix3d &Ds) const = 0;

	/**
	 * Calculate the elasticity of a single tet
	 * @param cons_model constitute model
	 * @param W Volume of the tet
	 * @param B Suppose the vertices of the reference tet is {X1, X2, X3, X4},
	 *          then B is the inverse of the 3 by 3 matrix [X1 - X4, X2 - X4, X3 - X4]
	 * @param Ds Suppose the vertices of the current tet is {x1, x2, x3, x4},
	 *          then Ds is the 3 by 3 matrix [x1 - x4, x2 - x4, x3 - x4]
	 * @return The gradient of the elasticity energy against the position of a
	 *         tet, which is a vector of 3x4=12 elements consist of coordinates
	 *         of the four vertices
	 */
	virtual Vector12d
	Gradient(const ConstituteModel &cons_model, double W, const Matrix3d &B,
			 const Matrix3d &Ds) const = 0;

	/**
	 * Calculate the elasticity of a single tet
	 * @param cons_model constitute model
	 * @param W Volume of the tet
	 * @param B Suppose the vertices of the reference tet is {X1, X2, X3, X4},
	 *          then B is the inverse of the 3 by 3 matrix [X1 - X4, X2 - X4, X3 - X4]
	 * @param Ds Suppose the vertices of the current tet is {x1, x2, x3, x4},
	 *          then Ds is the 3 by 3 matrix [x1 - x4, x2 - x4, x3 - x4]
	 * @return The hessian of the elasticity energy against the position of a
	 *         tet, which is a vector of 3x4=12 elements consist of coordinates
	 *         of the four vertices
	 */
	virtual Matrix12d
	Hessian(const ConstituteModel &cons_model, double W, const Matrix3d &B,
			const Matrix3d &Ds, const Matrix12x9d &pFpX) const = 0;

	BASE_DECLARE_CLONE(ElasticEnergyModel)

	virtual ~ElasticEnergyModel() = default;

protected:
};

#endif //FEM_ELEMENTENERGY_H

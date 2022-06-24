//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_CONSTITUTEMODEL_H
#define FEM_CONSTITUTEMODEL_H

#include "Util/Pattern.h"
#include "Util/EigenAll.h"


enum class ConstituteModelType {
	kStVK,
	kCoro,
	kLinear
};

class ConstituteModelParameter {
public:
	ConstituteModelParameter();
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, YoungsModule)
	DECLARE_VIRTUAL_ACCESSIBLE_MEMBER(double, PoissonRatio)

	BASE_DECLARE_CLONE(ConstituteModelParameter)
};

// TODO: Young's Module, Poisson radio or Lame Coefficients
//       should be passed as paras, not stored in the model
class ConstituteModel {
public:
	virtual void Initialize(const ConstituteModelParameter& para);

	/**
	 * @param F The gradient of deformation mapping
	 * @return The energy density at the point
	 * @note For tet meshes, the density within the tet is considerred
	 *       fixed due to the interpolation of F
	 */
	virtual double EnergyDensity(const Matrix3d& F) const = 0;

	//->The derivative of energy density against F, aka Piola tensor
	virtual Matrix3d Piola(const Matrix3d& F) const = 0;

	/**
	 * @param F The gradient of deformation mapping
	 * @return The derivative of Piola tensor against F
	 * @note This is actually a forth-order tensor, we have flatten it into
	 *       a matrix, where the [i, j]th block means the derivative of
	 *       Piola[i, j] against F (it is a 3 by 3 matrix)
	 */
	virtual Matrix9d PiolaDifferential(const Matrix3d &F) const = 0;

	BASE_DECLARE_CLONE(ConstituteModel)

	virtual ~ConstituteModel();

};

#endif //FEM_CONSTITUTEMODEL_H

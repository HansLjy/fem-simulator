//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_TARGET_H
#define FEM_TARGET_H

#include "Function/Function.h"
#include "ElementEnergy/ElasticEnergy.h"
#include "ElementEnergy/DissipationEnergy.h"
#include "Mesh/Mesh.h"

enum class TargetType {
	kBackward,
};

struct TargetParameter {
	ElasticEnergyType _elas_type;
	ElasticEnergyParameter _elas_para;
	DissipationEnergyType _diss_type;
	DissipationEnergyParameter _diss_para;
};

class Target : public Function {
public:
	double Value(const VectorXd &x) override = 0;
	VectorXd Gradient(const VectorXd &x) override = 0;
	MatrixXd Hessian(const VectorXd &x) override = 0;

	void Initialize(const TargetParameter& para);
	void SetMesh(const Mesh& mesh);

	void SetX(const VectorXd& x);	// set xn
	void SetV(const VectorXd& v);	// set vn
	void SetDt(double dt);

protected:
	// Data
	Mesh _reference;

	// Parameter
	ElasticEnergy* _elas_model = nullptr;
	DissipationEnergy* _diss_model = nullptr;

	// Outer Status
	VectorXd _x, _v;
	double _dt;
};

#endif //FEM_TARGET_H

//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_SOFTBODY_H
#define FEM_SOFTBODY_H

#include "Mesh/Mesh.h"
#include "Mass/MassModel.h"
#include "Contact/Surface.h"
#include "Object/Object.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Util/EigenAll.h"

class SoftBodySurface;

class SoftBodyParameter {
public:
	SoftBodyParameter(
			double mu,
			const MassModelType& mass_type,
			const MassModelParameter& mass_para,
			const BodyEnergyParameter& body_energy_para)
		: _mu(mu), _mass_type(mass_type), _mass_para(mass_para.Clone()), _body_energy_para(body_energy_para.Clone()) {}

	SoftBodyParameter(const SoftBodyParameter& rhs) {
		_body_energy_para = rhs.GetBodyEnergyParameter()->Clone();
		_mass_type = rhs.GetMassModelType();
		_mass_para = rhs.GetMassModelParameter()->Clone();
	}

	virtual ~SoftBodyParameter() {
		delete _body_energy_para;
		delete _mass_para;
	}

	DERIVED_DECLARE_CLONE(SoftBodyParameter)

	DECLARE_ACCESSIBLE_MEMBER(double, Mu, _mu)
	DECLARE_ACCESSIBLE_MEMBER(MassModelType, MassModelType, _mass_type)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(MassModelParameter, MassModelParameter, _mass_para)
	DECLARE_ACCESSIBLE_POINTER_MEMBER(BodyEnergyParameter, BodyEnergyParameter, _body_energy_para)
};

class SoftBodyGravity;

struct SoftBody : public Object {
	SoftBody(const Mesh& mesh) : SoftBody(mesh, mesh) {}

	SoftBody(const Mesh& rest, const Mesh& mesh);

	void Initialize(const SoftBodyParameter& soft_body_para);

	int GetDOF() const override {
		return _mesh.GetPoints().size();
	}

	const VectorXd & GetX() const override {
		return _mesh.GetPoints();
	}

	VectorXd & GetX() override {
		return _mesh.GetPoints();
	}

	const VectorXd & GetV() const override {
		return _v;
	}

	VectorXd & GetV() override {
		return _v;
	}

	const COO& GetM() const override {
		return _mass_coo;
	}

	double InternalEnergy() const override;
	VectorXd InternalEnergyGradient() const override;
	COO InternalEnergyHessianCOO() const override;

	const Surface * GetSurface() const override;
	COO GetJ(const SurfaceElements::SurfaceType &type, int idx,
			 const VectorXd &point,
			 const VectorXd &normal) const override;

	double GetMu() const override {
		return _mu;
	}

	void Store(const std::string &file) override {
		_mesh.Store(file);
	}

	SoftBody(const SoftBody& rhs);
	SoftBody& operator=(const SoftBody& rhs);

	DERIVED_DECLARE_CLONE(Object)

	Mesh _mesh;					// Note: the x item in it stores the current status
	Mesh _rest;					// rest shape
	VectorXd _v;				// velocity of each node
	VectorXd _mass;				// mass of each node
	COO _mass_coo;				// mass in a COO-form
	VectorXd _volume;			// volume
	vector<Matrix3d> _inv;		// inverse of Ds
	vector<Matrix12x9d> _pFpX;	// partial F partial X
	BodyEnergy* _body_energy;
	SoftBodySurface* _surface;
	double _mu;

	friend SoftBodySurface;
	friend ExternalForce;
};

class SoftBodySurface : public Surface {
public:
	SoftBodySurface(const SoftBody& soft_body) {
		_soft_body = &soft_body;
	}

	int GetNumFaces() const override;
	SurfaceElements::Face GetFace(int idx) const override;

protected:
	const SoftBody* _soft_body;
};

#endif //FEM_SOFTBODY_H

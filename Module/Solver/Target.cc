//
// Created by hansljy on 2022/2/23.
//

#include "Target.h"
#include "Util/Factory.h"
#include <spdlog/spdlog.h>

TargetParameter::TargetParameter(const BodyEnergyParameter &body_para,
								 const MassModelType &mass_type,
								 const MassModelParameter &mass_para)
								 : _body_para(body_para.Clone()),
								   _mass_type(mass_type),
								   _mass_para(mass_para.Clone())
								   {}

DEFINE_CLONE(TargetParameter, TargetParameter)
DEFINE_ACCESSIBLE_POINTER_MEMBER(TargetParameter, BodyEnergyParameter, BodyEnergyParameter, _body_para)
DEFINE_ACCESSIBLE_MEMBER(TargetParameter, MassModelType, MassModelType, _mass_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(TargetParameter, MassModelParameter, MassModelParameter, _mass_para)

void Target::Initialize(const TargetParameter &para) {
	_body_energy = new BodyEnergy;
	_body_energy->Initialize(*para.GetBodyEnergyParameter());
	_mass_model = MassModelFactory::GetInstance()->GetMassModel(para.GetMassModelType());
	_mass_model->Initialize(*para.GetMassModelParameter());
	spdlog::info("Target initialized");
}

Target::~Target() {
	delete _body_energy;
	delete _mass_model;
	int num_of_ext = _ext_force.size();
	for (int i = 0; i < num_of_ext; i++) {
		delete _ext_force[i];
	}
};

Target::Target(const Target &target) {
	_reference = target._reference;
	delete _body_energy;
	delete _mass_model;
	_body_energy = target._body_energy->Clone();
	_mass_model = target._mass_model->Clone();
	_x = target._x;
	_v = target._v;
	_dt = target._dt;
	for (const auto& force : _ext_force) {
		delete force;
	}
	_ext_force.clear();
	for (const auto& force : target._ext_force) {
		_ext_force.push_back(force->Clone());
	}
	_mass = target._mass;
	_mass_sparse = target._mass_sparse;
	_volumn = target._volumn;
	_inv = target._inv;
	_pFpX = target._pFpX;
}

void Target::SetX(const VectorXd &x) {
	_x = x;
}

void Target::SetV(const VectorXd &v) {
	_v = v;
}

void Target::SetDt(double dt) {
	_dt = dt;
}

void Target::SetMesh(const Mesh &mesh) {
	_reference = mesh;
	PreCompute();
}

void Target::PreCompute() {
	ComputeInverse();
	ComputeMass();
	ComputeVolumn();
	ComputePd();
}

void Target::ComputeInverse() {
	const auto& points = _reference.GetPoints();
	const auto& tets = _reference.GetTets();
	int num_of_tets = tets.size();
	_inv.resize(num_of_tets, 1);
	int index = 0;
	for (const auto& tet : tets) {
		Matrix3d D;
		for (int i = 0; i < 3; i++) {
			D.col(i) = points.block<3, 1>(3 * tet[i], 0) - points.block<3, 1>(3 * tet[3], 0);
		}
		_inv[index++] = D.inverse();
	}
}

void Target::ComputeMass() {
	_mass = _mass_model->GetMassDistribution(_reference);
	int num_of_tets = _mass.size();
	_mass_sparse.resize(3 * num_of_tets);
	for (int i = 0; i < 3 * num_of_tets; i += 3) {
		_mass_sparse(i) = _mass_sparse(i + 1) = _mass_sparse(i + 2) = _mass(i / 3);
	}
}

void Target::ComputeVolumn() {
	const auto& points = _reference.GetPoints();
	const auto& tets = _reference.GetTets();
	int num_of_tets = tets.size();
	_volumn.resize(num_of_tets);
	int index = 0;
	for (const auto& tet : tets) {
		Matrix3d D;
		for (int i = 0; i < 3; i++) {
			D.col(i) = points.block<3, 1>(3 * tet[i], 0) - points.block<3, 1>(3 * tet[3], 0);
		}
		_volumn[index++] = std::abs(D.determinant() / 6.0);
	}
}

void Target::ComputePd() {
	_pFpX.resizeLike(_inv);
	const int num_of_tets = _inv.size();

	Matrix3d I3 = Matrix3d::Identity();
	for (int n = 0; n < num_of_tets; n++) {
		auto& pdFX = _pFpX(n);
		auto& B = _inv(n);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				pdFX.block<3, 3>(3 * i, 3 * j) = B(i, j) * I3;
			}
		}
		for (int i = 0; i < 3; i++) {
			pdFX.row(i + 9) = - pdFX.row(i) - pdFX.row(i + 3) - pdFX.row(i + 6);
		}
	}
}

void Target::AddExternalForce(const ExternalForce &ext) {
	_ext_force.push_back(ext.Clone());
}

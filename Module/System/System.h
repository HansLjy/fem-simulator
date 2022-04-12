//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_SYSTEM_H
#define FEM_SYSTEM_H

#include <vector>
#include "Mesh/Mesh.h"
#include "BodyEnergy/ExternalForce.h"
#include "Mass/MassModel.h"
#include "SoftBody/SoftBody.h"
#include "RigidBody/RigidBody.h"
#include "BodyEnergy/BodyEnergy.h"

class SystemParameter {
public:
	DERIVED_DECLARE_CLONE(SystemParameter)
};

class System {
public:
	void Initialize(const SystemParameter& para) {};

	void AddSoftBody(const Mesh& mesh, const MassModel& mass_model) {
		_soft_bodies.push_back(SoftBody(mesh, mass_model));
		OnSoftBodyChange();
	}

	void AddRigidBody(const RigidBody& rigid_body) {
		_rigid_bodies.push_back(rigid_body);
	}

	void AddExternalForce(const ExternalForce& external_force) {
		_external_forces.push_back(external_force.Clone());
	}

	const std::vector<SoftBody>& GetSoftBodies() const {
		return _soft_bodies;
	}

	std::vector<SoftBody>& GetSoftBodies() {
		return _soft_bodies;
	}

	const std::vector<RigidBody>& GetRigidBodies() const {
		return _rigid_bodies;
	}

	const std::vector<const ExternalForce*>& GetExternalForces() const {
		return _external_forces;
	}

	void Store(const std::string& path, int frame_id) {
		int index = 0;
		for (auto& soft_body : _soft_bodies) {
			soft_body._mesh.Store(path + "/soft_obj" + std::to_string(index++) + "f" + std::to_string(frame_id) + ".vtk");
		}
	}

	/**
	 * On change, modify coord size
	 */
	void OnSoftBodyChange() {
		_coord_size = 0;
		for (auto& soft_body : _soft_bodies) {
			_coord_size += soft_body._sparse_mass.size();
		}
		_sys_sparse_mass.resize(_coord_size);
		const int num_soft = _soft_bodies.size();
		int cur_size = 0;
		for (int i = 0; i < num_soft; i++) {
			const int single_size = _soft_bodies[i]._sparse_mass.size();
			_sys_sparse_mass.block(cur_size, 0, single_size, 1) = _soft_bodies[i]._sparse_mass;
			cur_size += single_size;
		}
	}

	void GetSysMass(VectorXd& m) {
		m = _sys_sparse_mass;
	}

	void GetSysV(VectorXd& v) {
		v.resize(_coord_size);
		const int num_soft_bodies = _soft_bodies.size();
		int cur_size = 0;
		for (int i = 0; i < num_soft_bodies; i++) {
			int single_size = _soft_bodies[i]._v.size();
			v.block(cur_size, 0, single_size, 1) = _soft_bodies[i]._v;
			cur_size += single_size;
		}
	}

	void GetSysX(VectorXd& x) {
		x.resize(_coord_size);
		const int num_soft_bodies = _soft_bodies.size();
		int cur_size = 0;
		for (int i = 0; i < num_soft_bodies; i++) {
			int single_size = _soft_bodies[i]._v.size();
			x.block(cur_size, 0, single_size, 1) = _soft_bodies[i]._mesh.GetPoints();
			cur_size += single_size;
		}
	}

	// TODO: ADD DISSIPATION FORCE INTO CALCULATION!
	// dissipation forces are implemented but not used for various reasons

	void GetSysF(VectorXd& f, const BodyEnergy& body_energy) {
		f.resize(_coord_size);
		const int num_soft = _soft_bodies.size();
		const int num_ext = _external_forces.size();
		int current_size = 0;
		for (int i = 0; i < num_soft; i++) {
			int single_size = GetBodySize(i);
			f.block(current_size, 0, single_size, 1).setZero();
			for (int j = 0; j < num_ext; j++) {
				f.block(current_size, 0, single_size, 1) -= _external_forces[j]->Gradient(_soft_bodies[i]);
			}
			f.block(current_size, 0, single_size, 1) -= body_energy.EGradient(_soft_bodies[i]);
			current_size += single_size;
		}
	}

	void GetSysPFPX(SparseMatrixXd& pFpX, const BodyEnergy& body_energy) {
		int current_size = 0;
		const int num_soft = _soft_bodies.size();
		std::vector<Triplet> COO;
		for (int i = 0; i < num_soft; i++) {
			int single_size = GetBodySize(i);
			SparseMatrixXd hes = body_energy.EHessian(_soft_bodies[i]);
			for (int j = 0; j < hes.outerSize(); j++) {
				for (SparseMatrixXd::InnerIterator it(hes, j); it; ++it) {
					COO.push_back(Triplet(it.row() + current_size, it.col() + current_size, it.value()));
				}
			}
			current_size += single_size;
		}
		pFpX.setFromTriplets(COO.begin(), COO.end());
	}

	int GetBodySize(int idx) {
		return _soft_bodies[idx]._mesh.GetPoints().size();
	}

	void Update(const VectorXd& u, double h) {
		int current_size = 0;
		const int num_soft = _soft_bodies.size();
		for (int i = 0; i < num_soft; i++) {
			int size_single = GetBodySize(i);
			_soft_bodies[i]._mesh.GetPoints() += h * u.block(current_size, 0, size_single, 1);
			_soft_bodies[i]._v = u.block(current_size, 0, size_single, 1);
			current_size += size_single;
		}
	}

private:
	std::vector<const ExternalForce*> _external_forces;
	std::vector<SoftBody> _soft_bodies;
	std::vector<RigidBody> _rigid_bodies;
	VectorXd _sys_sparse_mass;

	// the size of status vector (for example, x) for the whole system
	DECLARE_ACCESSIBLE_MEMBER(int, CoordSize, _coord_size)
};

#endif //FEM_SYSTEM_H

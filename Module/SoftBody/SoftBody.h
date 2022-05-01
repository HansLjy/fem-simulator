//
// Created by hansljy on 2022/4/9.
//

#ifndef FEM_SOFTBODY_H
#define FEM_SOFTBODY_H

#include "Mesh/Mesh.h"
#include "Mass/MassModel.h"

struct SoftBody {
	SoftBody(const Mesh& mesh, const MassModel& mass_model) : _mesh(mesh) {
		// Set v to zero
		_v.resizeLike(mesh.GetPoints());
		_v.setZero();

		// Compute Mass
		_mass = mass_model.GetMassDistribution(mesh);
		int size = _mass.size();
		_sparse_mass.resize(3 * size);
		for (int i = 0, j = 0; i < size; i++, j += 3) {
			_sparse_mass(j) = _sparse_mass(j + 1) = _sparse_mass(j + 2) = _mass(i);
		}

		// Compute volume, inverse and pFpX
		const auto& tets = mesh.GetTets();
		const auto& points = mesh.GetPoints();
		const int num_tets = tets.size();
		_inv.resize(num_tets);
		_volume.resize(num_tets);
		_pFpX.resize(num_tets);

		Matrix3d I3 = Matrix3d::Identity();
		for (int i = 0; i < num_tets; i++) {
			auto tet = tets[i];
			Matrix3d D;
			for (int j = 0; j < 3; j++) {
				D.col(j) = points.block<3, 1>(3 * tet[j], 0) - points.block<3, 1>(3 * tet[3], 0);
			}
			_volume(i) = std::abs(D.determinant()) / 6;
			Matrix3d B = D.inverse();
			_inv(i) = B;

			auto& pFpX = _pFpX(i);
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					pFpX.block<3, 3>(3 * j, 3 * k) = B(j, k) * I3;
				}
			}
			for (int j = 0; j < 3; j++) {
				pFpX.row(j + 9) = - pFpX.row(j) - pFpX.row(j + 3) - pFpX.row(j + 6);
			}
		}
	}

	SoftBody(const Mesh& rest, const Mesh& initial, const MassModel& mass_model) : SoftBody(rest, mass_model) {
		_mesh = initial;
	}

	Mesh _mesh;					// Note: the x item in it stores the current status
	VectorXd _v;				// velocity of each node
	VectorXd _mass;				// mass of each node
	VectorXd _sparse_mass;		// mass of each node, stretch to align to mass
	VectorXd _volume;			// volume
	VectorX<Matrix3d> _inv;		// inverse of Ds
	VectorX<Matrix12x9d> _pFpX;	// partial F partial X

};

#endif //FEM_SOFTBODY_H

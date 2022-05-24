//
// Created by hansljy on 22-5-6.
//

#include "SoftBody.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Util/Factory.h"

DEFINE_CLONE(SoftBodyParameter, SoftBodyParameter)

DEFINE_ACCESSIBLE_MEMBER(SoftBodyParameter, double, Mu, _mu)
DEFINE_ACCESSIBLE_MEMBER(SoftBodyParameter, MassModelType, MassModelType, _mass_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(SoftBodyParameter, MassModelParameter, MassModelParameter, _mass_para)
DEFINE_ACCESSIBLE_POINTER_MEMBER(SoftBodyParameter, BodyEnergyParameter, BodyEnergyParameter, _body_energy_para)

DEFINE_CLONE(Object, SoftBody)

SoftBody::SoftBody(const Mesh &rest, const Mesh &mesh)
	: _rest(rest), _mesh(mesh),
	  _body_energy(new BodyEnergy(this)),
	  _surface(new SoftBodySurface(*this)){
	// Set v to zero
	_v.resizeLike(mesh.GetPoints());
	_v.setZero();

	// Compute volume, inverse and pFpX
	const auto& tets = rest.GetTets();
	const auto& points = rest.GetPoints();
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
		_inv[i] = B;

		auto& pFpX = _pFpX[i];
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

double SoftBody::InternalEnergy() const {
	return _body_energy->EEnergy();
}

VectorXd SoftBody::InternalEnergyGradient() const {
	return _body_energy->EGradient();
}

COO SoftBody::InternalEnergyHessianCOO() const {
	return _body_energy->EHessianCOO();
}

COO SoftBody::GetJ(const SurfaceElements::SurfaceType &type, int idx,
				   const VectorXd &point,
				   const VectorXd &normal) const {
	const auto& points = _mesh.GetPoints();
	vector<int> face = _mesh.GetSurface()[idx];
	Matrix3d X;
	for (int i = 0; i < 3; i++) {
		X.col(i) = points.block<3, 1>(3 * face[i], 0);
	}
	Vector3d barycentric = X.inverse() * point;	// barycentric coordinates
	COO coo;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			coo.push_back(Triplet(0, 3 * face[i] + j, barycentric[i] * normal[j]));
		}
	}
	return coo;
}

SoftBody::SoftBody(const SoftBody &rhs)
	: _mesh(rhs._mesh), _rest(rhs._rest),
	_v(rhs._v), _mass(rhs._mass), _mass_coo(rhs._mass_coo),
	_volume(rhs._volume), _inv(rhs._inv), _pFpX(rhs._pFpX), _mu(rhs._mu) {
	_body_energy = rhs._body_energy->Clone();
	_surface = new SoftBodySurface(*this);
	_body_energy = rhs._body_energy->Clone();
	_body_energy->_body = this;
}

const Surface * SoftBody::GetSurface() const {
	return _surface;
}

int SoftBodySurface::GetNumFaces() const {
	return _soft_body->_mesh.GetSurface().size();
}

SurfaceElements::Face SoftBodySurface::GetFace(int idx) const {
	const auto& face_id = _soft_body->_mesh.GetSurface()[idx];
	const auto& points = _soft_body->_mesh.GetPoints();
	SurfaceElements::Face face;
	for (int j = 0; j < 3; j++) {
		face._vertex[j] = points.block<3, 1>(3 * face_id[j], 0);
	}
	return face;
}

void SoftBody::Initialize(const SoftBodyParameter &para) {
	_body_energy->Initialize(*para.GetBodyEnergyParameter());
	auto mass_model = MassModelFactory::GetInstance()->GetMassModel(para.GetMassModelType());
	_mass = mass_model->GetMassDistribution(_rest);
	int size = _mass.size();
	_mass_coo.clear();
	for (int i = 0, j = 0; i < size; i++, j += 3) {
		_mass_coo.push_back(Triplet(j, j, _mass(i)));
		_mass_coo.push_back(Triplet(j + 1, j + 1, _mass(i)));
		_mass_coo.push_back(Triplet(j + 2, j + 2, _mass(i)));
	}
}
//
// Created by hansljy on 2022/4/9.
//

#include "LCPIntegrator.h"
#include "Util/Factory.h"
#include <Eigen/IterativeLinearSolvers>

DEFINE_ACCESSIBLE_MEMBER(LCPIntegratorParameter, LCPSolverType, LCPSolverType, _lcp_type)
DEFINE_ACCESSIBLE_POINTER_MEMBER(LCPIntegratorParameter, LCPSolverParameter, LCPSolverParameter, _lcp_para)

void LCPIntegrator::Initialize(const IntegratorParameter &para) {
	_solver = LCPSolverFactory::GetInstance()->GetLCPSolver(para.GetLCPSolverType());
	_solver->Initialize(*para.GetLCPSolverParameter());
}

void LCPIntegrator::Step(System &system, const ContactGenerator &contact,
						 const BodyEnergy &body_energy, double h,
						 const Solver &solver) {
	const int num_tangent = contact.GetNumTangent();

	SparseMatrixXd JnT, JtT;
	VectorXd Mu;

	contact.GetContact(system, JnT, JtT, Mu);

	const int num_contact = JnT.rows();

	int size = system.GetCoordSize();	// size for u, x, M and all such matrices
	SparseMatrixXd W(size, size);

	VectorXd M, u, f;
	system.GetSysMass(M);
	system.GetSysV(u);
	system.GetSysF(f, body_energy);
	system.GetSysPFPX(W, body_energy);

	W *= h * h;
	W.diagonal() += M;
	VectorXd c = M.asDiagonal() * u + h * f;

	Eigen::BiCGSTAB<SparseMatrixXd> linear_solver;
	linear_solver.compute(W);

	MatrixXd WiJn = linear_solver.solve(JnT.transpose().toDense());
	MatrixXd WiJt = linear_solver.solve(JtT.transpose().toDense());
	VectorXd Wic = linear_solver.solve(c);

	MatrixXd E(num_contact * num_tangent, num_contact);
	for (int i = 0; i < num_contact; i++) {
		E.block(i * num_tangent, i, num_tangent, 1).setOnes();
	}

	const int sizeA = num_contact * (2 + num_tangent);
	MatrixXd A(sizeA, sizeA);
	VectorXd b(sizeA);

	A.block(0, 0, num_contact, num_contact) = JnT * WiJn;
	A.block(0, num_contact, num_contact, num_tangent * num_contact) = JnT * WiJt;
	A.block(num_contact, 0, num_tangent * num_contact, num_contact) = JtT * WiJn;
	A.block(num_contact, num_contact, num_tangent * num_contact, num_tangent * num_contact) = JtT * WiJt;

	A.block(num_contact * (num_tangent + 1), 0, num_contact, num_contact) = Mu;
	A.block(num_contact * (num_tangent + 1), num_contact, num_contact, num_contact * num_tangent) = - E.transpose();
	A.block(num_contact, num_contact * (num_tangent + 1), num_tangent * num_contact, num_contact) = E;

	b.block(0, 0, num_contact, 1) = JnT * Wic;
	b.block(num_contact, 0, num_contact * num_tangent, 1) = JtT * Wic;

	VectorXd sol = solver.LCP->Solve(A, b);

	VectorXd u_plus = Wic + WiJn * sol.block(0, 0, num_contact, 1) + WiJt * sol.block(num_contact, 0, num_contact * num_tangent, 1);

	system.Update(u_plus, h);
}
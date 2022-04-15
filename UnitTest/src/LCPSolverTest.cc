//
// Created by hansljy on 2022/4/12.
//

#include "../Test.h"

void Test::TestLCPCommon() {
	const int size = 10;
	typedef Eigen::Matrix<double, size, size> MatrixOfSize;
	typedef Eigen::Vector<double, size> VectorOfSize;

	MatrixOfSize A;
	A.setRandom();
	A = A.transpose() * A;	// make it spd
	VectorOfSize b;
	b.setRandom();

	VectorOfSize x = _lcp_solver->Solve(A.sparseView(), b);
	VectorOfSize y = A * x + b;
	for (int i = 0; i < size; i++) {
		CPPUNIT_ASSERT(x(i) >= 0 && y(i) >= 0 && x(i) * y(i) < _eps);
	}
}

/**
 * Test whether the LCP solver is suited for the
 * LCP we will encounter in contact friction simu
 */
void Test::TestLCPFrictionMatrix() {
	const int num_contact = 1;
	const int num_tangent = 2;
	const int num_coord = 6;

	using Eigen::Matrix;
	using Eigen::Vector;

	Matrix<double, num_coord, num_coord> W;
	Matrix<double, num_coord, num_contact> Jn;
	Matrix<double, num_coord, num_contact * num_tangent> Jt;
	Matrix<double, num_contact, num_contact> mu;
	Matrix<double, num_contact * num_tangent, num_contact> E;
	Vector<double, num_contact * (2 + num_tangent)> b;

	W.setRandom();
	for (int i = 0; i < num_coord; i++) {
		for (int j = 0; j < i; j++) {
			W(i, j) = W(j, i);
		}
	}
	Jn.setRandom();
	Jt.setRandom();
	mu.setZero();
	mu.diagonal().setRandom();
	b.setRandom();
	E.setZero();
	for (int i = 0; i < num_contact; i++) {
		E.block<num_tangent, 1>(num_tangent * i, i).setOnes();
	}

	Matrix<double, num_contact * (2 + num_tangent), num_contact * (2 + num_tangent)> A;
	A.setZero();
	A.block<num_contact, num_contact>(0, 0) = Jn.transpose() * W * Jn;
	A.block<num_contact * num_tangent, num_contact>(num_contact, 0) = Jt.transpose() * W * Jn;
	A.block<num_contact, num_contact * num_tangent>(0, num_contact) = Jn.transpose() * W * Jt;
	A.block<num_contact * num_tangent, num_contact * num_tangent>(num_contact, num_contact) = Jt.transpose() * W * Jt;
	A.block<num_contact, num_contact>(num_contact * (num_tangent + 1), 0) = mu;
	A.block<num_contact, num_contact * num_tangent>(num_contact * (num_tangent + 1), num_contact) = -E.transpose();
	A.block<num_contact * num_tangent, num_contact>(num_contact, num_contact * (num_tangent + 1)) = E;

	VectorXd x = _lcp_solver->Solve(A, b, VectorXd(b.size()), 2 + num_tangent);
	VectorXd y = A * x + b;

	for (int i = 0; i < num_contact * (2 + num_tangent); i++) {
		CPPUNIT_ASSERT(x(i) >= 0 && y(i) >= 0 && x(i) * y(i) < _eps);
	}
}

void Test::TestLCPSmallScale() {

}
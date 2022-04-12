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

void Test::TestLCPFrictionMatrix() {

}
//
// Created by hansljy on 2022/2/26.
//

#include "Optimizer/Optimizer.h"
#include "Optimizer/InteriorPoint.h"
#include "Function/Function.h"
#include "Constraint/Constraint.h"
#include "Util/Factory.h"
#include <string>
#include <cppunit/Test.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <functional>

using std::string;

class SettableFunction : public Function {
public:
	double Value(const VectorXd &x) const override {
		return _val(x);
	}
	VectorXd Gradient(const VectorXd &x) const override {
		return _grad(x);
	}
	MatrixXd Hessian(const VectorXd &x) const override {
		return _hess(x);
	}

	void SetValueFunction(const std::function<double(VectorXd)>& func) {
		_val = func;
	}
	void SetGradientFunction(const std::function<VectorXd(VectorXd)>& func) {
		_grad = func;
	}
	void SetHessianFunction(const std::function<MatrixXd(VectorXd)>& func) {
		_hess = func;
	}

	DERIVED_DECLARE_CLONE(Function)

private:
	std::function<double(VectorXd)> _val;
	std::function<VectorXd(VectorXd)> _grad;
	std::function<MatrixXd(VectorXd)> _hess;
};

DEFINE_CLONE(Function, SettableFunction)

class Tester : public CppUnit::TestFixture {
private:
	Optimizer* _interior_pointer_optimizer;

public:
	void setUp() override {
		_interior_pointer_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(OptimizerType::kInteriorPoint);
	}

	void TestCG() {
		const double eps = 1e-12;
		// Conjugate Gradient method, minimize x'Ax - 2x'b to solve for Ax - b
		Matrix12d M = Matrix12d::Random();
		auto A = M.transpose() * M;	// Then A is SPD
		Vector12d b = Vector12d::Random();

		auto val = [A, b](const VectorXd& x) -> double {
			return x.transpose() * A * x - 2 * x.dot(b);
		};

		auto gra = [A, b](const VectorXd& x) -> VectorXd {
			return 2 * A * x - 2 * b;
		};

		auto hes = [A, b](const VectorXd& x) -> MatrixXd {
			return 2 * A;
		};

		SettableFunction func;
		func.SetValueFunction(val);
		func.SetGradientFunction(gra);
		func.SetHessianFunction(hes);

		_interior_pointer_optimizer->Initialize(InteriorPointParameter(1e-12, 100, 0.1));

		_interior_pointer_optimizer->SetTarget(func);
		auto x = _interior_pointer_optimizer->Optimize(Vector12d::Zero());

		CPPUNIT_ASSERT((A * x - b).norm() < 1e-12);
	}

	void tearDown() override {
		delete _interior_pointer_optimizer;
	}
};

int main() {
	CppUnit::TestCaller<Tester> test("Test CG", &Tester::TestCG);
	test.runTest();
	return 0;
}
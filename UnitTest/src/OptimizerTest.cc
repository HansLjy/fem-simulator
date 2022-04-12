//
// Created by hansljy on 2022/3/7.
//

#include "../Test.h"
#include "Function/Function.h"
#include "Eigen/Dense"

using Eigen::Matrix3d, Eigen::Vector3d;


class SettableFunction : public Function {
public:
	double Value(const VectorXd &x) const override {
		return _val(x);
	}
	VectorXd Gradient(const VectorXd &x) const override {
		return _grad(x);
	}
	SparseMatrixXd Hessian(const VectorXd &x) const override {
		return _hess(x);
	}

	void SetValueFunction(const std::function<double(VectorXd)>& func) {
		_val = func;
	}
	void SetGradientFunction(const std::function<VectorXd(VectorXd)>& func) {
		_grad = func;
	}
	void SetHessianFunction(const std::function<SparseMatrixXd(VectorXd)>& func) {
		_hess = func;
	}

	DERIVED_DECLARE_CLONE(Function)

private:
	std::function<double(VectorXd)> _val;
	std::function<VectorXd(VectorXd)> _grad;
	std::function<SparseMatrixXd(VectorXd)> _hess;
};

DEFINE_CLONE(Function, SettableFunction)

void Test::TestOptimizerCG() {
	const double eps = 1e-12;
	// Conjugate Gradient method, minimize x'Ax - 2x'b to solve for Ax - b

	for (int T = 0; T < 10; T++) {
		Matrix3d M = Matrix3d::Random();
		auto A = M.transpose() * M;    // Then A is SPD
		Vector3d b = Vector3d::Random();

		auto val = [A, b](const VectorXd &x) -> double {
			return x.transpose() * A * x - 2 * x.dot(b);
		};

		auto gra = [A, b](const VectorXd &x) -> VectorXd {
			return 2 * A * x - 2 * b;
		};

		auto hes = [A, b](const VectorXd &x) -> SparseMatrixXd {
			return 2 * A.sparseView();
		};

		SettableFunction func;
		func.SetValueFunction(val);
		func.SetGradientFunction(gra);
		func.SetHessianFunction(hes);

		_optimizer->SetTarget(func);

		Vector3d x0 = Vector3d::Zero();

		auto x = _optimizer->Optimize(x0);

		spdlog::info("Round {}, error {}", T, eps);

		CPPUNIT_ASSERT((A * x - b).norm() < eps);
	}
}

void Test::TestOptimizerCons() {
	// Test optimizer with constraints

	auto val = [](VectorXd x) -> double {
		return x.dot(x);
	};

	auto gra = [](VectorXd x) -> VectorXd {
		return 2 * x;
	};

	auto hes = [](VectorXd x) -> SparseMatrixXd {
		VectorXd lambda(x.size());
		lambda.setConstant(2);
		return lambda.asDiagonal().toDenseMatrix().sparseView();
	};

	SettableFunction func;
	func.SetValueFunction(val);
	func.SetGradientFunction(gra);
	func.SetHessianFunction(hes);

	_optimizer->SetTarget(func);

	Vector3d point, norm;
	point << 1, 1, 1;
	norm << 1, 1, 1;

//	_optimizer->AddConstraint(PlaneConstraint(norm, point));

	Vector3d x0 = Vector3d::Random() + 3 * norm;

	Vector3d sol = _optimizer->Optimize(x0);

	std::cerr << sol << std::endl;

	CPPUNIT_ASSERT((sol - point).norm() < 1e-5);
}
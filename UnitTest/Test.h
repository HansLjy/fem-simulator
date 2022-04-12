//
// Created by hansljy on 2022/3/7.
//

#ifndef FEM_TEST_H
#define FEM_TEST_H

#include <cppunit/Test.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <functional>
#include <cppunit/CompilerOutputter.h>
#include "spdlog/spdlog.h"
#include "Optimizer/Optimizer.h"
#include "ConstituteModel/ConstituteModel.h"
#include "ElementEnergy/ElasticEnergy.h"
#include "BodyEnergy/BodyEnergy.h"
#include "Solver/LCPSolver/LCPSolver.h"

class Test : public CppUnit::TestFixture {
public:
	void setUp() override;
	void tearDown() override;

	void TestOptimizerCG();
	void TestOptimizerCons();
	void TestConstituteModel();
	void TestElasticForce();
	void TestBodyEnergy();
	void TestLCPCommon();
	void TestLCPFrictionMatrix();

private:
	const double _eps = 1e-12;
	Optimizer *_optimizer;
	ConstituteModel	*_constitute_model;
	ElasticEnergyModel *_elas_model;
	BodyEnergy *_body_energy_model;
	LCPSolver *_lcp_solver;
	Mesh _mesh;
};

#endif //FEM_TEST_H

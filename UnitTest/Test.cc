//
// Created by hansljy on 2022/2/26.
//

#include "Test.h"
#include "Util/Factory.h"
#include "Optimizer/InteriorPoint.h"
#include "Optimizer/NewtonIterator.h"
#include "ConstituteModel/StVKModel.h"
#include "ElementEnergy/SimpleModel.h"
#include "ElementEnergy/RayleighModel.h"
#include "Solver/LCPSolver/PGS.h"
#include <ctime>
#include <algorithm>

void Test::setUp() {
	srand(0);
//	_interior_pointer_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(OptimizerType::kInteriorPoint);
//	_interior_pointer_optimizer->Initialize(InteriorPointParameter(10000, 50, 0.1));

	_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(OptimizerType::kNewtonIterator);
	_optimizer->Initialize(NewtonIteratorParameter(1e-5, 50, 0.15, 0));

	_constitute_model = ConstituteModelFactory::GetInstance()->GetConstituteModel(ConstituteModelType::kStVK);
	_constitute_model->Initialize(StVKModelParameter(1, 1));

	_elas_model = ElasticEnergyModelFactory::GetInstance()->GetElasticEnergyModel(ElasticEnergyModelType::kSimple);
	_elas_model->Initialize(SimpleModelParameter());

	_body_energy_model = new BodyEnergy;
	_body_energy_model->Initialize(
		BodyEnergyParameter (
			ElasticEnergyModelType::kSimple,
			SimpleModelParameter(),
			DissipationEnergyModelType::kRayleigh,
			RayleighModelParameter(1, 0),	// purely mass
			ConstituteModelType::kStVK,
			StVKModelParameter(1, 1)
		)
	);

	_mesh.Initialize(MeshParameter("../Resource/vtk/two-tet.vtk"));

	_lcp_solver = LCPSolverFactory::GetInstance()->GetLCPSolver(LCPSolverType::kPGS);
	_lcp_solver->Initialize(PGSParameter(100, _eps, 1));
}

void Test::tearDown() {
	delete _optimizer;
	delete _constitute_model;
	delete _elas_model;
	delete _body_energy_model;
	delete _lcp_solver;
}

int main() {
	CppUnit::TestSuite suite;

//	suite.addTest(new CppUnit::TestCaller<Test>("Test Optimizer", &Test::TestOptimizerCG));
//	suite.addTest(new CppUnit::TestCaller<Test>("Test LCP Solver", &Test::TestLCPCommon));
	suite.addTest(new CppUnit::TestCaller<Test>("Test LCP Solver for friction", &Test::TestLCPFrictionMatrix));
//	suite.addTest(new CppUnit::TestCaller<Test>("Test Optimizer with constraints", &Test::TestOptimizerCons));
//	suite.addTest(new CppUnit::TestCaller<Test>("Test Constitute Model", &Test::TestConstituteModel));
//	suite.addTest(new CppUnit::TestCaller<Test>("Test Elastic Energy Model", &Test::TestElasticForce));
//	suite.addTest(new CppUnit::TestCaller<Test>("Test Body Energy Model", &Test::TestBodyEnergy));

	CppUnit::TestResult result;
	CppUnit::TestResultCollector collected;
	result.addListener(&collected);
	suite.run(&result);
	CppUnit::CompilerOutputter outputter(&collected, std::cerr);
	spdlog::info("Cppunit test finished, the results:");
	outputter.write();
	return 0;
}
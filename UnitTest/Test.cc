//
// Created by hansljy on 2022/2/26.
//

#include "Test.h"
#include "Util/Factory.h"
#include <ctime>
#include <algorithm>

void Test::setUp() {
	srand(0);
	_interior_pointer_optimizer = OptimizerFactory::GetInstance()->GetOptimizer(OptimizerType::kInteriorPoint);
	_consistency_model = ConsistencyModelFactory::GetInstance()->GetConsistencyModel(ConsistencyModelType::kStVK);
	_elas_model = ElasticEnergyModelFactory::GetInstance()->GetElasticEnergyModel(ElasticEnergyModelType::kSimple);
}

void Test::tearDown() {
	delete _interior_pointer_optimizer;
	delete _consistency_model;
	delete _elas_model;
}

int main() {
	CppUnit::TestSuite suite;

	suite.addTest(new CppUnit::TestCaller<Test>("Test Optimizer", &Test::TestOptimizerCG));
	suite.addTest(new CppUnit::TestCaller<Test>("Test Constitute Model", &Test::TestConstituteModel));
	suite.addTest(new CppUnit::TestCaller<Test>("Test Elastic Energy Model", &Test::TestElasticForce));

	CppUnit::TestResult result;
	CppUnit::TestResultCollector collected;
	result.addListener(&collected);
	suite.run(&result);
	CppUnit::CompilerOutputter outputter(&collected, std::cerr);
	spdlog::info("Cppunit test finished, the results:");
	outputter.write();
	return 0;
}
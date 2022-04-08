#include "Simulator/Simulator.h"
#include "Optimizer/InteriorPoint.h"
#include "Optimizer/NewtonIterator.h"
#include "Solver/BackwardTarget.h"
#include "ElementEnergy/SimpleModel.h"
#include "ElementEnergy/RayleighModel.h"
#include "ConsistencyModel/StVKModel.h"
#include "BodyEnergy/Gravity.h"
#include "Mass/VoronoiModel.h"
#include "BodyEnergy/GroundForce.h"

#include <fstream>

int main() {
	string output_dir, input_file;
	double duration, step;

	int max_step;
	double max_error, mu, armijo, curvature;

	double alpha1, alpha2;
	double youngs_module, poisson_ratio, density;

	std::fstream cfg("./config");
	cfg >> input_file >> output_dir;
	cfg >> duration >> step;
	cfg >> max_error >> max_step >> armijo >> curvature;
	cfg >> alpha1 >> alpha2;
	cfg >> youngs_module >> poisson_ratio;
	cfg >> density;

	auto simulator = new Simulator;
	SimulatorParameter para(
		output_dir,		// output
		duration,						// duration
		step,						// step
		SolverType::kBackward,		// solver type
		SolverParameter(
				OptimizerType::kNewtonIterator,	// optimizer type
				NewtonIteratorParameter (
						max_error,
						max_step,
						armijo,
						curvature
				),
				TargetParameter(
						BodyEnergyParameter(
								ElasticEnergyModelType::kSimple,        // elastic energy model
								SimpleModelParameter(),
								DissipationEnergyModelType::kRayleigh,    // dissipation energy model
								RayleighModelParameter(
										alpha1,		// alpha1, for mass
										alpha2    	// alpha2, for K
								),
								ConsistencyModelType::kStVK,
								StVKModelParameter(
										youngs_module,    // Young's module
										poisson_ratio    // Poisson's ratio
								)
						),
						MassModelType::kVoronoi,
						VoronoiModelParameter(density)
				)
		),
		MeshParameter(
			input_file	// input file
		)
	);

	simulator->Initialize(para);
	simulator->AddExternalForce(GroundForce(10000));
	simulator->AddExternalForce(Gravity(9.8));

//	Vector3d norm, base;
//	norm << 0, 0, 1;
//	base << 0, 0, -0.1;
//
//	simulator->AddConstraint(PlaneConstraint(norm, base));
	simulator->Simulate();
	return 0;
}
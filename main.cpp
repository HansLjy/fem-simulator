#include "Simulator/Simulator.h"
#include "Optimizer/InteriorPoint.h"
#include "Solver/BackwardTarget.h"
#include "ElementEnergy/SimpleModel.h"
#include "ElementEnergy/RayleighModel.h"
#include "ConsistencyModel/StVKModel.h"
#include "BodyEnergy/Gravity.h"
#include "Constraint/PlaneConstraint.h"
#include "Mass/VoronoiModel.h"

int main() {
	auto simulator = new Simulator;
	SimulatorParameter para(
		"./Resource/output",		// output
		10.0,						// duration
		0.1,						// step
		SolverType::kBackward,		// solver type
		SolverParameter(
				OptimizerType::kInteriorPoint,	// optimizer type
				InteriorPointParameter (
						1e-5,	// max error
						100,	// max  step
						0.01	// mu
				),
				TargetParameter(
						BodyEnergyParameter(
								ElasticEnergyModelType::kSimple,        // elastic energy model
								SimpleModelParameter(),
								DissipationEnergyModelType::kRayleigh,    // dissipation energy model
								RayleighModelParameter(
										1,		// alpha1, for mass
										1    	// alpha2, for K
								),
								ConsistencyModelType::kStVK,
								StVKModelParameter(
										0.0078,    // Young's module
										0.47    // Poisson's ratio
								)
						),
						MassModelType::kVoronoi,
						VoronoiModelParameter()
				)
		),
		MeshParameter(
			"./Resource/vtk/bunny.vtk"	// input file
		)
	);

	simulator->Initialize(para);
	simulator->AddExternalForce(Gravity(9.8));

	Vector3d norm, base;
	norm << 0, 0, 1;
	base << 0, 0, 0;

	simulator->AddConstraint(PlaneConstraint(norm, base));
	simulator->Simulate();
	return 0;
}
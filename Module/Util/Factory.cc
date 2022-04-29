//
// Created by hansljy on 2022/2/22.
//

#include "Factory.h"
#include <iostream>


#include "ConstituteModel/StVKModel.h"
BEGIN_DEFINE_XXX_FACTORY(ConstituteModel)
		ADD_PRODUCT(ConstituteModelType::kStVK, StVKModel)
END_DEFINE_XXX_FACTORY

#include "ElementEnergy/SimpleModel.h"
BEGIN_DEFINE_XXX_FACTORY(ElasticEnergyModel)
		ADD_PRODUCT(ElasticEnergyModelType::kSimple, SimpleModel)
END_DEFINE_XXX_FACTORY

#include "ElementEnergy/RayleighModel.h"
BEGIN_DEFINE_XXX_FACTORY(DissipationEnergyModel)
		ADD_PRODUCT(DissipationEnergyModelType::kRayleigh, RayleighModel)
END_DEFINE_XXX_FACTORY

#include "Mass/VoronoiModel.h"
BEGIN_DEFINE_XXX_FACTORY(MassModel)
		ADD_PRODUCT(MassModelType::kVoronoi, VoronoiModel)
END_DEFINE_XXX_FACTORY

#include "NumericSolver/Optimizer/InteriorPoint.h"
#include "NumericSolver/Optimizer/NewtonIterator.h"
BEGIN_DEFINE_XXX_FACTORY(Optimizer)
		ADD_PRODUCT(OptimizerType::kInteriorPoint, InteriorPoint)
		ADD_PRODUCT(OptimizerType::kNewtonIterator, NewtonIterator)
END_DEFINE_XXX_FACTORY

#include "Integrator/NonFricLCPIntegrator.h"
#include "Integrator/StaggerLCPIntegrator.h"
BEGIN_DEFINE_XXX_FACTORY(Integrator)
		ADD_PRODUCT(IntegratorType::kNonFrictionLCP, NonFricLCPIntegrator)
		ADD_PRODUCT(IntegratorType::kStaggeringLCP, StaggerLCPIntegrator)
END_DEFINE_XXX_FACTORY

#include "NumericSolver/LCPSolver/PGS.h"
#include "NumericSolver/LCPSolver/BGS.h"
#include "NumericSolver/LCPSolver/PivotingMethod.h"
#include "NumericSolver/LCPSolver/OSQPWrapper.h"
BEGIN_DEFINE_XXX_FACTORY(LCPSolver)
		ADD_PRODUCT(LCPSolverType::kPGS, PGS)
		ADD_PRODUCT(LCPSolverType::kBGS, BGS)
		ADD_PRODUCT(LCPSolverType::kPivot, PivotingMethod)
		ADD_PRODUCT(LCPSolverType::kOSQP, OSQPWrapper)
END_DEFINE_XXX_FACTORY

#include "Contact/PolyhedralCone.h"
BEGIN_DEFINE_XXX_FACTORY(ContactGenerator)
		ADD_PRODUCT(ContactGeneratorType::kPolyhedralCone, PolyhedralCone)
END_DEFINE_XXX_FACTORY
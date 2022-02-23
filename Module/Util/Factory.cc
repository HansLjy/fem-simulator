//
// Created by hansljy on 2022/2/22.
//

#include "Factory.h"

#include "ConsistencyModel/StVKModel.h"
BEGIN_DEFINE_XXX_FACTORY(ConsistencyModel)
		ADD_PRODUCT(ConsistencyModelType::kStVK, StVKModel)
END_DEFINE_XXX_FACTORY

#include "ElementEnergy/SimpleModel.h"
BEGIN_DEFINE_XXX_FACTORY(ElasticEnergy)
		ADD_PRODUCT(ElasticEnergyType::kSimple, SimpleModel)
END_DEFINE_XXX_FACTORY

#include "ElementEnergy/RayleighModel.h"
BEGIN_DEFINE_XXX_FACTORY(DissipationEnergy)
		ADD_PRODUCT(DissipationEnergyType::kRayleigh, RayleighModel)
END_DEFINE_XXX_FACTORY

#include "Mass/VoronoiModel.h"
BEGIN_DEFINE_XXX_FACTORY(MassModel)
		ADD_PRODUCT(MassModelType::kVoronoi, VoronoiModel)
END_DEFINE_XXX_FACTORY

#include "Optimizer/InteriorPoint.h"
BEGIN_DEFINE_XXX_FACTORY(Optimizer)
		ADD_PRODUCT(OptimizerType::kInteriorPoint, InteriorPoint)
END_DEFINE_XXX_FACTORY

#include "Solver/BackwardSolver.h"

BEGIN_DEFINE_XXX_FACTORY(Solver)
		ADD_PRODUCT(SolverType::kBackward, BackwardSolver)
END_DEFINE_XXX_FACTORY

#include "Solver/BackwardTarget.h"
BEGIN_DEFINE_XXX_FACTORY(Target)
		ADD_PRODUCT(TargetType::kBackward, BackwardTarget)
END_DEFINE_XXX_FACTORY
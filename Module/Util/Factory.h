//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_FACTORY_H
#define FEM_FACTORY_H

#include "Pattern.h"

#include "ConsistencyModel/ConsitencyModel.h"
DECLARE_XXX_FACTORY(ConsistencyModel)

#include "ElementEnergy/ElasticEnergy.h"
DECLARE_XXX_FACTORY(ElasticEnergy)

#include "ElementEnergy/DissipationEnergy.h"
DECLARE_XXX_FACTORY(DissipationEnergy)

#include "Mass/MassModel.h"
DECLARE_XXX_FACTORY(MassModel)

#include "Optimizer/Optimizer.h"
DECLARE_XXX_FACTORY(Optimizer)

#include "Solver/Solver.h"
DECLARE_XXX_FACTORY(Solver)

#include "Solver/Target.h"
DECLARE_XXX_FACTORY(Target)

#endif //FEM_FACTORY_H

//
// Created by hansljy on 2022/4/9.
//

#include "System.h"

DEFINE_ACCESSIBLE_MEMBER(SystemParameter, std::vector<std::string>, InputFiles, _input_files)
DEFINE_ACCESSIBLE_MEMBER(SystemParameter, std::vector<MassModelType>, MassModelTypes, _mass_model_types)
DEFINE_CLONE(SystemParameter, SystemParameter)
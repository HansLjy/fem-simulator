//
// Created by hansljy on 22-6-30.
//

#include "FileOutput.h"

DEFINE_CLONE(SimulatorOutputParameter, FileOutputParameter)
DEFINE_ACCESSIBLE_MEMBER(FileOutputParameter, std::string, OutputDir, _output_dir)

FileOutputParameter::FileOutputParameter(const std::string &output_dir) :
	_output_dir(output_dir) {}

void FileOutput::Initialize(const SimulatorOutputParameter & para) {
	_output_dir = para.GetOutputDir();
}

void FileOutput::StepCB(const System &system, int frame_id) {
	system.Store(_output_dir, frame_id);
}

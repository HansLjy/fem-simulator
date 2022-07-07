//
// Created by hansljy on 22-7-6.
//

#ifndef FEM_APP_H
#define FEM_APP_H

#include "GUI/GUI.hpp"
#include "Simulator/Simulator.h"

class App : public GUI, public Simulator {
public:
	void Processing(Scene &scene) override;
	void InitializeScene(Scene &scene) override;

protected:
	double _current;
};

#endif //FEM_APP_H

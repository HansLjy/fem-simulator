//
// Created by hansljy on 2022/4/29.
//

#ifndef FEM_UTIL_H
#define FEM_UTIL_H

#include <ctime>

#define START_TIMING(t) auto t = clock();
#define STOP_TIMING_TICK(t, act) spdlog::info("Time spent on " act ":{}", clock() - t);
#define STOP_TIMING_SEC(t, act) spdlog::info("Time spent on " act ":{}", (double)(clock() - t) / CLOCKS_PER_SEC);

#endif //FEM_UTIL_H

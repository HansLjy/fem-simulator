# Fem-simulator

A finite element method simulator.

# Currently supported feature

Somehow, for the project where it is used, the following features are enough.

1. Soft body simulation
2. Contact & friction between:
    1. soft body and rigid body

However, the simulator in itself is of course incomplete, there are some other features to be added to make it whole.

# To be implemented

1. Rigid body simulation: currently the motion of rigid bodies is hard-wired, determined by the program, they don't contribute to the DOF of the whole system, they are just moving constraints.
2. Contact & friction between:
    1. soft body and soft body(including itself)
    2. rigid body and rigid body
3. Better IO: the configuration is not user-friendly now, I consider changing it into json format.
4. Parallel: I did try to make some parts of the code parallel (for example, compute the hessian matrix for each tet parallelly and then assemble them in a sequential way), but I didn't achieve the expected speedup.


# Usage

There are few dependencies:

1. Eigen
2. SparseSuite
3. OsqpEigen
4. spdlog

First you should edit the config in the root directory. The default one is for the demo.

Then make sure the output dir exists. For the demo, the output dir exists at ./Resource/output/moving_slabs.

After that, simply:

```
cmake .
make
```

A demo can be found in Resource/Demo
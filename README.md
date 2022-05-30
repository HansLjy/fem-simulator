# Fem-simulator

A finite element method simulator.

# Currently supported feature

Somehow, for the project where it is used, the following features are enough.

1. Soft body simulation
2. Rigid body simulation
3. Contact & friction between two distinct bodies

However, the simulator in itself is of course incomplete, there are some other features to be added to make it whole.

# To be implemented

1. Better IO: the configuration is not user-friendly now, I consider changing it into json format.
2. Parallel: I did try to make some parts of the code parallel (for example, compute the hessian matrix for each tet parallelly and then assemble them in a sequential way), but I didn't achieve the expected speedup.


# Usage

There are few dependencies:

1. Eigen
3. Osqp
4. OsqpEigen
2. SparseSuite
5. spdlog

First you should edit the config in the root directory. The default one is for the demo.

Then make sure the output dir exists. For the demo, the output dir exists at ./Resource/output/moving_slabs.

After that, simply:

```
cmake .
make -j
```

A demo can be found in Resource/Demo
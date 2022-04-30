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

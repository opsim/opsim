Engine
======
The main focus for OpSim is being able to solve steady state models. Future versions may leverage the models to be transient. By design the simulation engine is closely linked to the user interface, though completely impendent of it. With this partition, in a later stage of development, the engine may be packaged in a library (dynamic link library or shared object on Linux) and its interfaces exposed to the system environment. This design permits other applications to direct link and use the simulation engine e.g. MS Excel, Matlab, R, Scilab, etc.

The solver engine comprises basically:

   1. Solver
   2. Physical property models

Accessing the database is not a functionality of the engine. It does nothing about the database structure. The data needed to solve the simulation are supplied to the engine by other parts of the simulator. After solving the simulation case, the engine supplies the rest of the simulator (interface and data layer) with the partial or final results.
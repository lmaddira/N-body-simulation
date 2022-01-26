## N-body-simulation
N-body problem is the problem of predicting motions of a group of objects.Accurate simulation of this dynamic problem is considered difficult to solve due to computational complexity and numerical simulation of physics.N-body simulation is chaotic system that is highly dependent on the initial conditions and the temporal discretization.N-body simulation can also be used for imposing collective behavior of bodies like in swarm robotics.Our goal will be to create a simulation structure and use various runtime optimization methods.

# Gravity_sim
Simulation of large set of celestial bodies interaction with gravity. Accurate simulation of this dynamic problem is considered difficult to solve due to
computational complexity and numerical simulation of physics.

# Covid_sim
Interactions between different humans and population groups can be simulated using n-body simulation methods. Here the region of influence of each body is localized.This can be used to model the spread of infectious diseases given different constraints.

# Key Objectives
* Computational cost for bodies interactions is O(N^2).
* Efficient evaluation is required to simulate a large number of interactions.
* Direct integration is computationally very expensive for large N.
* Numerical Methods like Euler’s method, Runge-kutta methods etc will be explored to reduce complexity to O( logN ) or O( NlogN ).
* Variable timesteps for bodies will be explored to further reduce computational cost.Bodies with widely different dynamical times don't all have to be evolved forward at the rate of that with the shortest time.
* Visualization of the problem.

# Objectives Attained
* Calculation of mutual interactions of large number of bodies using gravity as the interaction force
* Calculation of mutual interactions of large number of bodies for modelling the spread of diseases
* Evaluated different data structures and compared their performance
* Parallelized the code for faster evaluation using openMP
* Solved the physics of the problem by time integration using Runge-Kutta 4th order method
* Adaptive time stepping performed using RK45
* Visualization of bodies, traces and spread of infection using openGL
* Generated data from simulations to analyze the results
* Unit Tests for checking the interaction values

# Gravity Simulation

# Covid Simulation


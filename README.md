# A Probabilistic Algorithm for Optimising the Steady-State Diffusional Flux into a Partially Absorbing Body #

This repository contains the implementation of the Fly-Now Pay-Later method, an efficient algorithm for calculating the diffusive flux across a 3D body with inhomogeneous partially absorbing boundary conditions.

## Overview ##
The algorithm is divided into two main components:

- **Fly-Now**: This phase simulates particles diffusing in the space surrounding the surface of interest. During the simulation, we track the collisions of particles with the body by maintaining a transition matrix. This matrix records the collision transitions across different discretized elements of the geometrical surface.

- **Pay-Later**: Using the transition matrix from the Fly-Now phase, this step calculates the diffusive flux across the surface for a given reactivity profile. Leveraging the Markovian nature of these transitions, the Pay-Later phase adjusts the transition matrix by incorporating the probability of absorption with each collision, proportional to the boundary's reactivity. This phase is computationally efficient, involving only a few steps of linear algebra.

Using this method, we demonstrated that for convex bodies with a fixed reactivity budget (integral of reactivity), allocating higher reactivity to regions with higher curvature modestly increases the total flux.

## Publications ##
For a detailed explanation of the method and results, please refer to our publication:

[A probabilistic algorithm for optimising the steady-state diffusional flux into a partially absorbing body](https://www.nature.com/articles/s41598-023-49566-4)

## Improved Implementation ##
We have enhanced the Fly-Now Pay-Later method to accommodate a wide variety of geometric settings by approximating them as triangular meshes. This improvement allows for greater flexibility and applicability in complex geometrical scenarios.

## Usage ##

This implementation is limited to the following convex bodies: sphere, spheroid and a spherocylinder. This limitation is due to their analytical (or easy to approximate) distance from surface function which is needed in the Fly-Now part.

To construct a Simulation object you need the following objects, a Body (in this implementation limited to: sphere, spheroid and spherocylinder), a source (uniform form infinity, or point source) and a the parameters of the simualtion, being the outer sphere radius, the brownian motion length, the thickness of the brownian blanket and last you need to specify the number of particles to be simulated. 

After running the simulation, the result files will be created which involves the transition matrix and some meta data. From these files you can do the Pay Later part to calculate the diffusive flux for different reactivities. There is an implementation of the Pay Later part in the src_analysis folder




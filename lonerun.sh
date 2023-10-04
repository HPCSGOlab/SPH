#!/bin/bash -xe


mpirun -np 6  ./bin/sph.out

#mpirun -np 3 valgrind ./bin/sph.out


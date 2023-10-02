#!/bin/bash -xe


mpirun -np 3  ./bin/sph.out

#mpirun -np 3 valgrind ./bin/sph.out


#!/bin/bash -xe


#mpirun -np 4  ./bin/sph.out

mpirun -np 4 valgrind ./bin/sph.out


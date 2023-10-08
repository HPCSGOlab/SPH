#!/bin/bash -xe


#mpirun -np 3  ./bin/sph.out

mpirun -np 3 valgrind --tool=memcheck ./bin/sph.out


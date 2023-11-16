#!/bin/bash -xe


#mpirun -np 5  ./bin/sph.out >> out.txt

#mpirun -np 5 valgrind --tool=memcheck --track-origins=yes --leak-check=full ./bin/sph.out >> out.txt

mpirun -np 5 valgrind  ./bin/sph.out >> out.txt


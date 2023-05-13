#!/bin/bash

gcc -Wall -c Constrict.c
gfortran -Wall -c FuncionObjetivo.f90
gfortran -Wall -orun FuncionObjetivo.o Constrict.o ../pso.o

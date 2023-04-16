#include "../pso.h"
#include <stdio.h>
 // Calcula l'arcotangente d'un ángulo basado en el triangulo rectángulo
 // con las medidas 3,4,5.

int main (int argc, char **argv){
  const float limites_catetos [4]={1,1,0,0};
  if(argc<2) exit(0);
  else {
    ProcesoPSO(20,2,limites_catetos,limites_catetos+2,20, 0.12, 2, 2, );
  }
}

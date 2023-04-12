#include "pso.h"
#include<math.h>
#include<time.h>

//PARAMETROS DE CONFIGURACION DEL PSO
#define DIM_ 2
const unsigned int NumeroParticulas=100; //Tamaño del enjambre, numero de particulas del enjambre
const unsigned int Dimension=DIM_; //Numero de variables del problema o dimension del problema.
const float LimiteSuperior[DIM_]={ 5.12,  5.12};
const float LimiteInferior[DIM_]={-5.12, -5.12};
const unsigned int NumeroMaximoDeIteraciones=300;

int main(){
  ENJAMBRE *Enj=NULL;
  //srand((unsigned) time(&tx));
  srand(time(NULL));
  unsigned int t=0;
  //Crear un enjambre de NumeroParticulas de Numero de parametros igual a Dimension
  printf("\nAntes del ENJAMBRE");
  Enj=CrearEnjambre(NumeroParticulas,Dimension);
  printf("\nSe creo el ENJAMBRE (IW)");
  InicializarEnjambre(Enj, 0.72984,2.05,2.05,NumeroMaximoDeIteraciones,LimiteInferior,LimiteSuperior);
  EvaluacionInicialEnjambre(Enj);
  ImprimeEnjambre(Enj);
  while(t<Enj->MaximoDeIteraciones){
    printf("\nIteracion: %i",t);
    ActualizarVelocidad(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj);
    ActualizarMejoresPosiciones(Enj);
    //ImprimeEnjambre(Enj);
    if((t%10)==0){
        printf("\nIteracion %i", t);
    }
    t++;
  }
  printf("\nBest Particle:");
  ImprimeParticula(Enj,Enj->MejorParticulaDelGrupo);
  EliminarEnjambre(Enj);
  printf("\nSe elimino el ENJAMBRE");
  return 0;
}



float FuncionObjetivo(float *__ValoresDeParametros__, unsigned int __CantidadDeParametros__){
  unsigned int k;
  float fit, aux = 0;
  // Funcion Rastriging (Buscamos el valor 0)
  for (k=0; k<__CantidadDeParametros__; k++)
    aux += pow(__ValoresDeParametros__[k],2)-10*cos(6.283185*__ValoresDeParametros__[k])+10;
  fit = 100 - aux; // El valor d'a precisión se ponderará en una escala del 1 al 100
  return fit;
}

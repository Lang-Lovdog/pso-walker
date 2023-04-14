#include "pso.h"
#include "Walker.h"
// Cabeceras necesarias para el proyecto
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

//InicializarEnjambre(Enj, 0.72984,2.05,2.05,NumeroMaximoDeIteraciones,LimiteInferior,LimiteSuperior);
//PARAMETROS DE CONFIGURACION DEL PSO
#define DIM_ 2
const unsigned int NumeroParticulas=100; //Tamaño del enjambre, numero de particulas del enjambre
const unsigned int Dimension=DIM_; //Numero de variables del problema o dimension del problema.
const unsigned int NumeroMaximoDeIteraciones=300;

int main (void){
  MAPA CaminoOrigen, Camino; 
  Crea
}




PARTICULA ProcesoPSO(
    const float         NumeroDeParticulas,
    const float         Dimension,
    const float        *LimiteSuperior,
    const float        *LimiteInferior,
    const unsigned int  NumeroMaximoDeIteraciones,
    const float         FactorDeConstriccion,
    const float         ValorPesoC1,
    const float         ValorPesoC2,
    const float        *ParametrosDeOperacion
  ){
  ENJAMBRE *Enj=NULL;
  PARTICULA Particle;
  srand(time(NULL));
  unsigned int t=0;
  //Crear un enjambre de NumeroParticulas de Numero de parametros igual a Dimension
  Enj=CrearEnjambre(NumeroDeParticulas,Dimension);
  InicializarEnjambre(
      Enj,
      FactorDeConstriccion,
      ValorPesoC1,
      ValorPesoC2,
      NumeroMaximoDeIteraciones,
      LimiteInferior,
      LimiteSuperior
    );
  EvaluacionInicialEnjambre(Enj,ParametrosDeOperacion);
  while((t++)<Enj->MaximoDeIteraciones){
    ActualizarVelocidad(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj,ParametrosDeOperacion);
    ActualizarMejoresPosiciones(Enj);
  }
  ImprimeParticula(Enj,Enj->MejorParticulaDelGrupo);

  Particle.Xi=(Enj->Part+Enj->MejorParticulaDelGrupo)->Xi;
  (Enj->Part+Enj->MejorParticulaDelGrupo)->Xi=NULL;
  Particle.Vi=(Enj->Part+Enj->MejorParticulaDelGrupo)->Vi;
  (Enj->Part+Enj->MejorParticulaDelGrupo)->Vi=NULL;
  Particle.Pi=(Enj->Part+Enj->MejorParticulaDelGrupo)->Pi;
  (Enj->Part+Enj->MejorParticulaDelGrupo)->Pi=NULL;
  Particle.Xfit=(Enj->Part+Enj->MejorParticulaDelGrupo)->Xfit;
  Particle.Pfit=(Enj->Part+Enj->MejorParticulaDelGrupo)->Pfit;

  EliminarEnjambre(Enj);
  return Particle;
}


float FuncionObjetivo(
    float        *__ValoresDeParametros__,  // D'a partícula
    unsigned int  __CantidadDeParametros__, // [3]{X,Y,W}
    const float  *__ParametrosDeOperacion__ // [5]{Xref,Yref,Vel,Xcaminante,Ycaminante}
  ){
  float fit;
  // z = G/(dist(Pi,Prefi)+Vel*dist(Pi,Pcaminante)); V=80km/h;
  fit=*(__ValoresDeParametros__+2)/(
    v_distancia(__ValoresDeParametros__,__ParametrosDeOperacion__) +
    *(__ParametrosDeOperacion__+2) *(
      v_distancia(
        __ParametrosDeOperacion__+3,
        __ValoresDeParametros__
      )
    )
  );
  return fit;
}

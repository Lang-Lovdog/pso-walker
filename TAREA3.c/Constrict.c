#include <stdio.h>
#include <math.h>
#include "../pso.h"
#include <time.h>
#include <stdlib.h>

// The update function uses the maximum value, so, if we search for the minimum value,
// then we need to invert the result of FuncionObjetivo.

void FuncionObjetivoF(
    float        *__ValoresDeParametros__,
    unsigned int  __CantidadDeParametros__,
    const float *__ResultadoDeOperacion__
);

int main (void){
  //Programa que obtenga los valores
  // Limites={InfX,InfY, SupX,SupY}
  PARTICULA LaSelecta;
  unsigned int Dimension=100,k=0;
  float *LimitesInf=(float*)malloc(Dimension*sizeof(float));
  float *LimitesSup=(float*)malloc(Dimension*sizeof(float));
  while (k<Dimension) {
    *(LimitesInf+k)=-10;
    *(LimitesSup+k)=10;
    ++k;
  }

  LaSelecta= ProcesoPSO(
    100, Dimension,
    LimitesSup, LimitesInf,
    20000, 0.6, 2.05, 2.05,
    NULL
  );
  ImprimeParticula(&LaSelecta,Dimension);
  free(LaSelecta.Pi);
  free(LaSelecta.Vi);
  free(LaSelecta.Xi);
  free(LimitesInf);
  free(LimitesSup);
}

  /*________________________________________________________________________________*/

float FuncionObjetivo(
    float        *__ValoresDeParametros__,
    unsigned int  __CantidadDeParametros__,
    const float  *__ParametrosDeOperacion__
){
  float ResultadoDeOperacion=0;
  FuncionObjetivoF(
    __ValoresDeParametros__,__CantidadDeParametros__,
    &ResultadoDeOperacion
  );
  return fabs(ResultadoDeOperacion);
}

PARTICULA ProcesoPSO(
    const float         __NumeroDeParticulas__,
    const float         __Dimension__,
    const float        *__LimiteSuperior__,
    const float        *__LimiteInferior__,
    const unsigned int  __NumeroMaximoDeIteraciones__,
    const float         __Factor_Constriccion_Inercia__,
    const float         __ValorPesoPersonalC1__,
    const float         __ValorPesoGlobalC2__,
    const float        *__ParametrosDeOperacion__
){
  ENJAMBRE *Enj=NULL;
  PARTICULA Particle;
  srand(time(NULL));
  unsigned int t=0;

  //Crear un enjambre de NumeroParticulas de Numero de parametros igual a Dimension
  Enj=CrearEnjambre(
    __NumeroDeParticulas__,
    __Dimension__
  );

  InicializarEnjambre(
    Enj,
    __Factor_Constriccion_Inercia__,
    __ValorPesoPersonalC1__,
    __ValorPesoGlobalC2__,
    __NumeroMaximoDeIteraciones__,
    __LimiteInferior__,
    __LimiteSuperior__
  );

  EvaluacionInicialEnjambreMin(Enj,__ParametrosDeOperacion__);

  while((t++)<Enj->MaximoDeIteraciones){
    ActualizarVelocidadConstriction(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambreMin(Enj,__ParametrosDeOperacion__);
    ActualizarMejoresPosicionesMin(Enj);
  }

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

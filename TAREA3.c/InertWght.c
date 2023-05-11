#include <stdio.h>
#include <math.h>
#include "../pso.h"
#include <time.h>
#include <stdlib.h>

int main (void){
  //Programa que obtenga los valores
  // Limites={InfX,InfY, SupX,SupY}
  PARTICULA LaSelecta;
  const float Limites[4]={-6.28,-6.28,6.28,6.28};
  LaSelecta= ProcesoPSO(
    12, 2,
    Limites+2, Limites,
    20, 0, 2, 2,
    NULL
  );
  ImprimeParticula(&LaSelecta,2);
  free(LaSelecta.Pi);
  free(LaSelecta.Vi);
  free(LaSelecta.Xi);
}

  /*________________________________________________________________________________*/

float FuncionObjetivo(
    float        *__ValoresDeParametros__,
    unsigned int  __CantidadDeParametros__,
    const float  *__ParametrosDeOperacion__
){
  float f_xy=
    10*exp(
      -(pow(*(__ValoresDeParametros__)+1,2)+pow(*(__ValoresDeParametros__+1)-3.14,2))/25
    )+
    cos(*(__ValoresDeParametros__  )*2)+
    sin(*(__ValoresDeParametros__+1)*2);
  return f_xy;
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

  EvaluacionInicialEnjambre(Enj,__ParametrosDeOperacion__);

  while((t++)<Enj->MaximoDeIteraciones){
    ActualizarVelocidadInerciaW(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj,__ParametrosDeOperacion__);
    ActualizarMejoresPosiciones(Enj);
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

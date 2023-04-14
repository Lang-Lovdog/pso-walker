#include "pso.h"
#include<math.h>
#include<time.h>

//InicializarEnjambre(Enj, 0.72984,2.05,2.05,NumeroMaximoDeIteraciones,LimiteInferior,LimiteSuperior);
//PARAMETROS DE CONFIGURACION DEL PSO
/*#define DIM_ 2
const unsigned int NumeroParticulas=100; //Tamaño del enjambre, numero de particulas del enjambre
const unsigned int Dimension=DIM_; //Numero de variables del problema o dimension del problema.
const float LimiteSuperior[DIM_]={ 5.12,  5.12};
const float LimiteInferior[DIM_]={-5.12, -5.12};
const unsigned int NumeroMaximoDeIteraciones=300;*/
float v_distancia(float* __A__,float* __B__);

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
  Enj=CrearEnjambre(NumeroParticulas,Dimension);
  InicializarEnjambre(
      Enj,
      FactorDeConstriccion,
      ValorPesoC1,
      ValorPesoC2,
      NumeroMaximoDeIteraciones,
      LimiteInferior,
      LimiteSuperior
    );
  EvaluacionInicialEnjambre(Enj);
  while((t++)<Enj->MaximoDeIteraciones){
    ActualizarVelocidad(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj);
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
    float        *__ValoresDeParametros__,
    unsigned int  __CantidadDeParametros__,
    const float  *__ParametrosDeOperacion__
  ){
  unsigned int k=0;
  float fit, aux = 0;
  // Funcion Rastriging (Buscamos el valor 0)
  while((k++)<__CantidadDeParametros__)
    aux += pow(__ValoresDeParametros__[k]-__ParametrosDeOperacion__[k],2);
  // z = G/(sqrt(aux)(1+1/V)); V=80km/h;
  // Los parámetros 1 y 2 son (X,Y)
  // El parámetro 3 es Velociadad
  // El parámetro 4 es el peso del nodo
  // Los parámetros 5 y 6 corresponderán al punto actual
  fit=sqrt(aux); // Se calcula según el radio de separación en x e y,
                 // menor el radio, mejor la partícula
  return fit;
}

float v_distancia(float* __A__, float* __B__){
  unsigned int k=0;
  floar suma;
  while(k<2)
    suma += pow(__A__[k]-__B__[k],2);
  return sqrt(suma)
}

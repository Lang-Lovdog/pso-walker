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
  CAMINANTE Metro;
  TIROLESA Referencia;

  unsigned int* ElementosEncontrados;

  CrearMapa(&CaminoOrigen,6);
  CrearMapa(&Camino,6);
  AgregaNodoMapaXY(&CaminoOrigen, 0  , 0  , -1 );
  AgregaNodoMapaXY(&CaminoOrigen, 1  , 1  , 0.1);
  AgregaNodoMapaXY(&CaminoOrigen, 1.5, 2  , 15 );
  AgregaNodoMapaXY(&CaminoOrigen, 2  , 1  , 0.1);
  AgregaNodoMapaXY(&CaminoOrigen, 2.5, 2.5, 15 );
  AgregaNodoMapaXY(&CaminoOrigen, 3  , 3  , -2 );
  BuscaPuntosIF(&CaminoOrigen,&Metro,&Referencia,0.5,0.75);
  ImprimirCaminante(&Metro,NULL);
  ImprimirTirolesa(&Referencia,NULL);
  ImprimirMapa(&CaminoOrigen,&Metro.Velocidad);
  AvanzaPref(&Referencia); 
   
  ImprimirTirolesa(&Referencia,NULL);
  ImprimirMapa(&Camino,&Metro.Velocidad);
  EliminarMapa(&CaminoOrigen);
  EliminarMapa(&Camino);
}

void ImprimirTirolesa(
    const TIROLESA* __Tirolesa__,
    const float*    __ParametrosDeOperacion__
  ){
  printf("\n\nm=%.2f\nPrefi=(%.2f, %.2f:  %.2f)\n\n",
    __Tirolesa__->PendienteDaRecta,
    *(__Tirolesa__->Coordenadas_Pref),
    *(__Tirolesa__->Coordenadas_Pref+1),
    __Tirolesa__->Paso
  );
}

void ImprimirCaminante(
    const CAMINANTE* __Caminante__,
    const float*     __ParametrosDeOperacion__
  ){
  printf("(  X  ,  Y  :    W  ,  V^(-1)  )\n\n");
  printf("(%.2f,%.2f:  %.2f,%.2f)",
    *(__Caminante__->Coordenadas_Pcaminante),
    *(__Caminante__->Coordenadas_Pcaminante+1),
    __Caminante__->PesoAcumulado,
    __Caminante__->Velocidad
  );
  printf("\n");
}

void ImprimirMapa(
    const MAPA  *__Mapa__,
    const float *__ParametrosDeOperacion__
  ){
  printf("(  X  ,  Y  :    W  ,  d  ,  t  )\t   SW  \t   Sd  \t   St  \n\n");
  unsigned int k=0;
  float sumaPesos=0,sumaDistantets=0,sumaVelocidad=0;
  float coordenadas[4];
  coordenadas[0]=*(__Mapa__->CoordenadaX);
  coordenadas[1]=*(__Mapa__->CoordenadaY);
  while(*(__Mapa__->PesoDelNodo+k)!=-3){
    coordenadas[2]=*(__Mapa__->CoordenadaX+k);
    coordenadas[3]=*(__Mapa__->CoordenadaY+k);
    printf("(%.2f, %.2f:  %.2f, %.2f, %.2f)\t %.2f\t %.2f\t %.2f\n",
      coordenadas[2],
      coordenadas[3],
      *(__Mapa__->PesoDelNodo+k),
      v_distancia(coordenadas,coordenadas+2),
      v_distancia(coordenadas,coordenadas+2)*(*__ParametrosDeOperacion__),
      sumaPesos+=*(__Mapa__->PesoDelNodo+k),
      sumaDistantets+=v_distancia(coordenadas,coordenadas+2),
      sumaVelocidad+=v_distancia(coordenadas,coordenadas+2)*(*__ParametrosDeOperacion__)
    );
    if(*(__Mapa__->PesoDelNodo+k)==-2) break;
    coordenadas[0]=coordenadas[2];
    coordenadas[1]=coordenadas[3];
    ++k;
  }
  printf("\n");
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

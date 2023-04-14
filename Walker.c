#include "Walker.h"
#include <stdlib.h>
#include <math.h>


float *f221(
    const float *__A__,
    const float *__B__,
    const float __ID__
){
  float* NeuVar=(float*)malloc(2*sizeof(float));
  NeuVar[0]=__A__[__ID__];
  NeuVar[1]=__B__[__ID__];
  return NeuVar;
}


float v_distancia(
    const float* __A__,
    const float* __B__
){
  unsigned int k=0;
  float suma=0;
  while(k<2)
    suma=pow(*(__A__+k)-*(__B__+k),2);
  return sqrt(suma);
}


void BuscaPuntosIF(
    const MAPA *__Mapa__,
    CAMINANTE  *__Caminante__,
    TIROLESA   *__tirolesa__
  ){
  unsigned int k=0;
  while(*(__Mapa__->PesoDelNodo+k)!=-1)
    ++k;
  __Caminante__->Coordenadas_Pcaminante[0]=
    __tirolesa__->Coordenadas_Pref[0]=
      __Mapa__->CoordenadaX[k];
  __Caminante__->Coordenadas_Pcaminante[1]=
    __tirolesa__->Coordenadas_Pref[1]=
      __Mapa__->CoordenadaY[k];
  while(*(__Mapa__->PesoDelNodo+k)!=-2)
    ++k;
  __tirolesa__->PendienteDaRecta=
    __tirolesa__->Coordenadas_Pref[0]!=__Mapa__->CoordenadaX[k]?
    (__Mapa__->CoordenadaY[k]-__tirolesa__->Coordenadas_Pref[1])/
    (__Mapa__->CoordenadaX[k]-__tirolesa__->Coordenadas_Pref[0]):0;
  __tirolesa__->AvanceX=
    __tirolesa__->Coordenadas_Pref[0]!=__Mapa__->CoordenadaX[k]?1:0;
}

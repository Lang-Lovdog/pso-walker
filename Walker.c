#include "Walker.h"
#include <stdlib.h>
#include <math.h>


float *f221(
    const float        *__A__,
    const float        *__B__,
    const unsigned int  __ID__
){
  float* NeuVar=(float*)malloc(2*sizeof(float));
  NeuVar[0]=__A__[__ID__];
  NeuVar[1]=__B__[__ID__];
  return NeuVar;
}

float pendiente(
    const float *__A__,
    const float *__B__
){
  return (__B__[1]-__A__[1])/(__B__[0]-__A__[0]);
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
    TIROLESA   *__tirolesa__,
    const float __Paso__
  ){
  unsigned int k=0;
  float *F=NULL;
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
    pendiente(
      __tirolesa__->Coordenadas_Pref,
      F=f221(__Mapa__->CoordenadaX,__Mapa__->CoordenadaY,k)
    ):0;
  free(F);
  __tirolesa__->Paso=__Paso__;
}

void AvanzaPref(
    TIROLESA    *__tirolesa__
){
  __tirolesa__->Coordenadas_Pref[0]+=
    __tirolesa__->Paso;
  __tirolesa__->Coordenadas_Pref[1]+=
    __tirolesa__->PendienteDaRecta*__tirolesa__->Paso;
}

void AvanzaCaminante(
    const float *__NuevoPunto__,
    CAMINANTE   *__Caminante__
){
  __Caminante__->Coordenadas_Pcaminante[0]=__NuevoPunto__[0];
  __Caminante__->Coordenadas_Pcaminante[1]=__NuevoPunto__[1];
}

unsigned int* busqueda(
    const float  __Actual__,
    const float  __Paso__,
    const MAPA  *__Mapa__
){
  unsigned int k, contador, *elementos;
  k=contador=0; elementos=NULL;
  while(__Mapa__->PesoDelNodo[k]!=-2){
    if(
      __Mapa__->CoordenadaX[k]<__Actual__+(__Paso__*(1.1)) &&
      __Mapa__->CoordenadaX[k]>__Actual__
    ) ++contador;
    ++k;
  }
  k=0; contador=0;
  elementos=(unsigned int*)malloc(sizeof(unsigned int)*contador);
  while(__Mapa__->PesoDelNodo[k]!=-2){
    if(
      __Mapa__->CoordenadaX[k]<__Actual__+(__Paso__*(1.1)) &&
      __Mapa__->CoordenadaX[k]>__Actual__
    ){
      elementos[contador]=k;
      ++contador;
    }
    ++k;
  }
  return elementos;
}
// Comparación de puntos con seleccionado
float precision(
    const float* __A__,
    const float  __WA__,
    const float* __B__,
    const float  __WB__
){
  return (0.8*v_distancia(__A__,__B__)+1.4*(__WB__-__WA__))/2;
}

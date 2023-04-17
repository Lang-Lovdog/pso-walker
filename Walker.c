#include "Walker.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


void CrearMapa( // <Confirmado!>
    MAPA *__Mapa__,
    const float __tamanno__
){
  if(__tamanno__>0){
    __Mapa__->CoordenadaX=(float*)malloc(__tamanno__*sizeof(float));
    if(!__Mapa__->CoordenadaX){
      printf("Error En creación de CoordenadaX");
      exit(-11);
    }
    __Mapa__->CoordenadaY=(float*)malloc(__tamanno__*sizeof(float));
    if(!__Mapa__->CoordenadaY){
      printf("Error En creación de CoordenadaY");
      exit(-12);
    }
  }
  __Mapa__->PesoDelNodo=(float*)malloc((__tamanno__+1)*sizeof(float));
  if(!__Mapa__->PesoDelNodo){
    printf("Error En creación de peso del nodo");
    exit(-13);
  }
  unsigned int k=0;
  while(k<__tamanno__){
    *(__Mapa__->CoordenadaX+k)=
      *(__Mapa__->CoordenadaY+k)=0;
    *(__Mapa__->PesoDelNodo+k)=-3; // Nodo no utilizado en -3
    ++k;
  };
  *(__Mapa__->PesoDelNodo+k)=-4;
}

void EliminarMapa(
    MAPA *__Mapa__
){
  if(__Mapa__->CoordenadaX)
    free(__Mapa__->CoordenadaX);
  __Mapa__->CoordenadaX=NULL;
  if(__Mapa__->CoordenadaY)
    free(__Mapa__->CoordenadaY);
  __Mapa__->CoordenadaY=NULL;
  if(__Mapa__->PesoDelNodo)
    free(__Mapa__->PesoDelNodo);
  __Mapa__->PesoDelNodo=NULL;
}

void AgregaNodoMapa( // <Confirmado!>
    MAPA *__Mapa__,
    const float* __Nodo__,
    const float  __Peso__
){
  unsigned int k=0;
  while(*(__Mapa__->PesoDelNodo+k)!=-3)
    if(*(__Mapa__->PesoDelNodo+k)==-4) return;
    else ++k;
  *(__Mapa__->PesoDelNodo+k)=__Peso__;
  *(__Mapa__->CoordenadaX+k)=*(__Nodo__);
  *(__Mapa__->CoordenadaY+k)=*(__Nodo__+1);
}

void AgregaNodoMapaXY( // <Confirmado!>
    MAPA *__Mapa__,
    const float __X__,
    const float __Y__,
    const float __Peso__
){
  unsigned int k=0;
  while(*(__Mapa__->PesoDelNodo+k)!=-3)
    if(*(__Mapa__->PesoDelNodo+k)==-4) return;
    else ++k;
  *(__Mapa__->PesoDelNodo+k)=__Peso__;
  *(__Mapa__->CoordenadaX+k)=__X__;
  *(__Mapa__->CoordenadaY+k)=__Y__;
}

unsigned int ArchivoMapa( // <Confirmado!> 
    const char* __Archivo__,
    MAPA*       __Mapa__
){
  FILE* ElCamino;
  char* Linea;
  float Valores[3];
  unsigned int CantidadDePuntos;
  if((ElCamino=fopen(__Archivo__,"rt")))
    fscanf(ElCamino,"%d\n",&CantidadDePuntos);
  if(CantidadDePuntos){
    //printf("CantidadDePuntos: %d",CantidadDePuntos);
    CrearMapa(__Mapa__,CantidadDePuntos);
    while(fscanf(ElCamino,"%f,%f,%f\n",Valores,Valores+1,Valores+2)!=EOF){
      AgregaNodoMapa(__Mapa__, Valores, *(Valores+2));
     //printf("%f,%f,%f\n",*Valores,*(Valores+1),*(Valores+2));
    }
    //float vel=1;
    //ImprimirMapa(__Mapa__,&vel);
    fclose(ElCamino);
  }else CrearMapa(__Mapa__,CantidadDePuntos=0);
  return CantidadDePuntos;
}

float* LimitesMapa( // <Confirmado!>
    const MAPA* __Mapa__
){
  unsigned int k=0;
  float max[3]={0,0,0};
  float* Limites=(float*)malloc(3*sizeof(float));
  while (*(__Mapa__->PesoDelNodo+k)!=-3&&*(__Mapa__->PesoDelNodo+k)!=-4) {
    if(*( max ) < *(__Mapa__->CoordenadaX+k)) *( max )=*(__Mapa__->CoordenadaX+k);
    if(*(max+1) < *(__Mapa__->CoordenadaY+k)) *(max+1)=*(__Mapa__->CoordenadaY+k);
    if(*(max+2) < *(__Mapa__->PesoDelNodo+k)) *(max+2)=*(__Mapa__->PesoDelNodo+k);
    ++k;
  }
  *(Limites)=*(max); *(Limites+1)=*(max+1); *(Limites+2)=*(max+2);
  return Limites;
}

float *f221( // <Confirmado!>
    const float        *__A__,
    const float        *__B__,
    const unsigned int  __ID__
){
  float* NeuVar=(float*)malloc(2*sizeof(float));
  NeuVar[0]=__A__[__ID__];
  NeuVar[1]=__B__[__ID__];
  return NeuVar;
}

float pendiente( // <Confirmado!>
    const float *__A__,
    const float *__B__
){
  return (__B__[1]-__A__[1])/(__B__[0]-__A__[0]);
}

float v_distancia( // <Confirmado!>
    const float* __A__,
    const float* __B__
){
  unsigned int k=0;
  float suma=0;
  while(k<2){
    //printf("(%.2f-%.2f)**2+",*(__A__+k),*(__B__+k));
    suma+=pow(*(__A__+k)-*(__B__+k),2);
    ++k;
  }
  //printf("\b= %.2f\n",suma);
  return sqrt(suma);
}


void BuscaPuntosIF( // <Confirmado!>
    const MAPA *__Mapa__,
    CAMINANTE  *__Caminante__,
    TIROLESA   *__tirolesa__,
    const float __Paso__,
    const float __Vel1__
){
  unsigned int k=0;
  float *F=NULL;
  /*-----------------------------------------------------*/
  __Caminante__->Velocidad=__Vel1__;
  __Caminante__->PesoAcumulado=__Caminante__->PesoActual=0;
  __tirolesa__->Paso=__Paso__;
  /*-----------------------------------------------------*/
  while(*(__Mapa__->PesoDelNodo+k)!=-1) ++k;
  __Caminante__->Coordenadas_Pcaminante[0]=
  __tirolesa__->Coordenadas_Pref[0]=
    __Mapa__->CoordenadaX[k];
  __Caminante__->Coordenadas_Pcaminante[1]=
  __tirolesa__->Coordenadas_Pref[1]=
    __Mapa__->CoordenadaY[k];
  /*-----------------------------------------------------*/
  while(*(__Mapa__->PesoDelNodo+k)!=-2) ++k;
  __tirolesa__->PendienteDaRecta=
    __tirolesa__->Coordenadas_Pref[0]!=__Mapa__->CoordenadaX[k]?
    pendiente(
      __tirolesa__->Coordenadas_Pref,
      F=f221(__Mapa__->CoordenadaX,__Mapa__->CoordenadaY,k)
    ):0;
  free(F);
  /*-----------------------------------------------------*/
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
  __Caminante__->PesoAcumulado+=__Caminante__->PesoActual=__NuevoPunto__[2];
}

unsigned int busqueda(
    const float    __Actual__,
    const float    __Paso__,
    const MAPA*    __Mapa__,
    unsigned int** __ElementosEncontrados__
){
  unsigned int k,l, contador, *elementos;
  k=contador=0; elementos=NULL;
  while(__Mapa__->PesoDelNodo[k]!=-2){
    if(
      __Mapa__->CoordenadaX[k]<__Actual__+(__Paso__*(1.1)) &&
      __Mapa__->CoordenadaX[k]>__Actual__
    ) ++contador;
    ++k;
  }
  elementos= !contador?NULL:(unsigned int*)malloc(sizeof(unsigned int)*contador);
  k=0; l=0;
  if(elementos){
    while(__Mapa__->PesoDelNodo[k]!=-2){
      if(
        __Mapa__->CoordenadaX[k]<__Actual__+(__Paso__*(1.1)) &&
        __Mapa__->CoordenadaX[k]>__Actual__
      ){
        elementos[l]=k;
        ++l;
      }
      ++k;
    }
  }
  *__ElementosEncontrados__=elementos;
  return contador;
}
// Comparación de punos, entre más similares, menor el valor de salida
float precision(
    const float* __A__,
    const float  __WA__,
    const float* __B__,
    const float  __WB__
){
  return (fabs(v_distancia(__A__,__B__))+fabs(__WA__-__WB__))/2;
}

#include "pso.h"

// Definición d'as funciones

ENJAMBRE* CrearEnjambre(
    //ENJAMBRE* __Enjambre__,
    unsigned int __CantidadDeParticulas__,
    unsigned int __CantidadDeParametros__
  ){
  ENJAMBRE *ptr=NULL;
  //Reservar la memoria para la estructura del enjambre
  ptr=(ENJAMBRE *)malloc(sizeof(ENJAMBRE));
  if(ptr==NULL){
    printf("Error al reservar la memoria para la estructura ENJAMBRE.");
    exit(0);
  }
  ptr->CantidadDeParticulas=__CantidadDeParticulas__;
  ptr->CantidadDeParametros=__CantidadDeParametros__;

  //Reservar la memoria para N particulas de M parametros
  ptr->Part=NULL;
  ptr->Part=(PARTICULA *)malloc(__CantidadDeParticulas__*sizeof(PARTICULA));
  if(ptr->Part==NULL){
    printf("Error al reservar la memoria para las Particulas.");
    exit(0);
  }
  //Reservar memoria para los 3 vectores de cada Particula
  for(unsigned int i=0; i<__CantidadDeParticulas__; ++i){
    ptr->Part[i].Xi=(float *)malloc(__CantidadDeParametros__*sizeof(float));
    ptr->Part[i].Vi=(float *)malloc(__CantidadDeParametros__*sizeof(float));
    ptr->Part[i].Pi=(float *)malloc(__CantidadDeParametros__*sizeof(float));
  }
  return ptr;
}


void InicializarEnjambre(
    ENJAMBRE    *__Enjambre__,
    float        __FactorConstriccion__,
    float        __ValorDePeso_C1__,
    float        __ValorDePeso_C2__,
    unsigned int __MaximoDeIteraciones__,
    const float *__LimitesInferiores__,
    const float *__LimitesSuperiores__
){
  if(__Enjambre__){
  float aux,rango;
  __Enjambre__->X                      = __FactorConstriccion__;
  __Enjambre__->C1                     = __ValorDePeso_C1__;
  __Enjambre__->C2                     = __ValorDePeso_C2__;
  __Enjambre__->MaximoDeIteraciones    = __MaximoDeIteraciones__;
  __Enjambre__->MejorParticulaDelGrupo = 0;
  __Enjambre__->LimitesInferiores      = __LimitesInferiores__;
  __Enjambre__->LimitesSuperiores      = __LimitesSuperiores__;
  //Inicializar cada vector de cada particula
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; ++i) //Para cada particula i
    for(unsigned int j=0; j<__Enjambre__->CantidadDeParametros; ++j) //Para cada parametro j de cada vector de la particula i
    { rango=__Enjambre__->LimitesSuperiores[j]-__Enjambre__->LimitesInferiores[j];
      aux= ((float)rand()/RAND_MAX) * rango + __Enjambre__->LimitesInferiores[j];
      __Enjambre__->Part[i].Xi[j]=aux;
      __Enjambre__->Part[i].Vi[j]=0;
      __Enjambre__->Part[i].Pi[j]=aux;
    }
  }
}


void EliminarEnjambre(ENJAMBRE* __Enjambre__)
{ //Liberar la memoria para de los 3 vectores de cada Particula
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; ++i)
     { free(__Enjambre__->Part[i].Xi);
       free(__Enjambre__->Part[i].Vi);
       free(__Enjambre__->Part[i].Pi);
     }
  //Liberar la memoria de las estructuras particula
  free(__Enjambre__->Part);
  //Liberar la memoria de la estrcutura del enajmbre
  free(__Enjambre__);
}


void ImprimeParticula(ENJAMBRE *__Enjambre__, unsigned int __ID_Particula__){
  printf("\nP%i,Xi: ",__ID_Particula__);
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParametros; i++)
    printf("%f, ",__Enjambre__->Part[__ID_Particula__].Xi[i]);
  printf("\nP%i,Vi: ",__ID_Particula__);
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParametros; i++)
    printf("%f, ",__Enjambre__->Part[__ID_Particula__].Vi[i]);
  printf("\nP%i,Pi: ",__ID_Particula__);
    for(unsigned int i=0; i<__Enjambre__->CantidadDeParametros; i++)
  printf("%f, ",__Enjambre__->Part[__ID_Particula__].Pi[i]);
  printf("\nP%i,Xfit=%f",__ID_Particula__,__Enjambre__->Part[__ID_Particula__].Xfit);
  printf("\nP%i,Pfit=%f",__ID_Particula__,__Enjambre__->Part[__ID_Particula__].Pfit);
}

void ImprimeEnjambre(ENJAMBRE *__Enjambre__)
{ for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; ++i) //Para cada particula i
    ImprimeParticula(__Enjambre__,i);
}


void EvaluarEnjambre(ENJAMBRE *__Enjambre__,const float* __ParametrosDeOperacion__){
  float BestFit;
  // Calcular el valor de Fitness de cada particula
  BestFit = FuncionObjetivo(
      __Enjambre__->Part[0].Xi,
      __Enjambre__->CantidadDeParametros,
      __ParametrosDeOperacion__
    );
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; i++){
    __Enjambre__->Part[i].Xfit = FuncionObjetivo(
        __Enjambre__->Part[i].Xi,
        __Enjambre__->CantidadDeParametros,
        __ParametrosDeOperacion__
      );
    // Almacena el indice de la mejor particula de todo en enjambre
    if(__Enjambre__->Part[i].Xfit>BestFit){
      BestFit = __Enjambre__->Part[i].Xfit;
      __Enjambre__->MejorParticulaDelGrupo =i;
    }
  }
}

void EvaluacionInicialEnjambre(ENJAMBRE *__Enjambre__,const float* __ParametrosDeOperacion__){
  if(__Enjambre__){
  float aux,BestFit;
  //Calcular el valor de fitness de cada Particula
  BestFit=FuncionObjetivo(
      __Enjambre__->Part[0].Xi,
      __Enjambre__->CantidadDeParametros,
      __ParametrosDeOperacion__
    );
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; i++){
    aux=FuncionObjetivo(
        __Enjambre__->Part[i].Xi,
        __Enjambre__->CantidadDeParametros,
        __ParametrosDeOperacion__
      );
    __Enjambre__->Part[i].Xfit=aux;
    __Enjambre__->Part[i].Pfit=aux;
    //Almacena el indice de la mejor particula de todo el enjambre
    if(aux>BestFit){
      BestFit=aux;
      __Enjambre__->MejorParticulaDelGrupo=i;
    }
  }
  }
}


void ActualizarVelocidad(ENJAMBRE *__Enjambre__){
  float Y1,Y2;
  //Actualizar cada vector velocidad Vi de cada particula
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; i++) //Para cada particula i
    for(unsigned int j=0; j<__Enjambre__->CantidadDeParametros; j++) //Para cada parametro j de cada vector Vi de la particula i
    {
      Y1=(float)rand()/RAND_MAX;
      Y2=(float)rand()/RAND_MAX;
      __Enjambre__->Part[i].Vi[j] = __Enjambre__->X*((__Enjambre__->Part[i].Vi[j])+
                                   (__Enjambre__->C1*Y1*(__Enjambre__->Part[i].Pi[j]-__Enjambre__->Part[i].Xi[j]))+
                                   (__Enjambre__->C2*Y2*(__Enjambre__->Part[__Enjambre__->MejorParticulaDelGrupo].Pi[j]-__Enjambre__->Part[i].Xi[j])) );
    }
}

void ActualizarPosicion(ENJAMBRE *__Enjambre__){
  // Acutailzsr cada vector Posicion XI de cada particula
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; i++) //Para cada particula i
    for(unsigned int j=0; j<__Enjambre__->CantidadDeParametros; j++) //Para cada parametro j de cada vector de la particula i
      __Enjambre__->Part[i].Xi[j] += __Enjambre__->Part[i].Vi[j];
}

void ActualizarMejoresPosiciones(ENJAMBRE *__Enjambre__){
  for(unsigned int i=0; i<__Enjambre__->CantidadDeParticulas; i++)
    if(__Enjambre__->Part[i].Xfit > __Enjambre__->Part[i].Pfit){
      __Enjambre__->Part[i].Pfit = __Enjambre__->Part[i].Xfit;
      for(unsigned int j=0; j<__Enjambre__->CantidadDeParametros; j++) //Para cada parametro j de cada vector de la particula i
        __Enjambre__->Part[i].Pi[j] = __Enjambre__->Part[i].Xi[j];
    }
}


/*float FuncionObjetivo(float *__ValoresDeParametros__, unsigned int __CantidadDeParametros__){
  unsigned int k;
  float fit, aux = 0;
  // Maximar la siguiente funcion:
  //   f(x,y)=50-(x-5)^2-(y-5)^2; 
  //fit=250-pow(Xi[0]+7,2)-pow(Xi[1]-3,2)-pow(Xi[2]-3,2)-pow(Xi[3]-5,2)-pow(Xi[4]-8,2);

  // Funcion Rastriging (Buscamos el valor 0)
  for (k=0; k<__CantidadDeParametros__; k++)
    aux += pow(__ValoresDeParametros__[k],2)-10*cos(6.283185*__ValoresDeParametros__[k])+10;
  fit = 100 - aux; // El valor d'a precisión se ponderará en una escala del 1 al 100
  return fit;
}*/

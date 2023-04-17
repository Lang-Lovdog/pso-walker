#include "../pso.h"
#include "../Walker.h"
// Cabeceras necesarias para el proyecto
#include <math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

void Procesamiento(const MAPA *,CAMINANTE*,TIROLESA*);

int main (int __main_CantidadDeArgumentos__, char** __main_ArgumentosDaFuncion__){
  MAPA CaminoOrigen, RutaSeleccionada;
  CAMINANTE Metro;
  TIROLESA Referencia;
  float* Final=NULL;

  /*________________________________________________________________________________*/

  if(__main_CantidadDeArgumentos__<2){
    CrearMapa(&CaminoOrigen,6);
    CrearMapa(&RutaSeleccionada,6);
    AgregaNodoMapaXY(&CaminoOrigen, 0  , 0  , -1 );
    AgregaNodoMapaXY(&CaminoOrigen, 1  , 1  , 0.1);
    AgregaNodoMapaXY(&CaminoOrigen, 1.5, 2  , 15 );
    AgregaNodoMapaXY(&CaminoOrigen, 2  , 1  , 0.1);
    AgregaNodoMapaXY(&CaminoOrigen, 2.5, 2.5, 15 );
    AgregaNodoMapaXY(&CaminoOrigen, 3  , 3  , -2 );
  }else {
    printf("%s\n\n",*(__main_ArgumentosDaFuncion__+1));
    CrearMapa(
      &RutaSeleccionada,
      ArchivoMapa(*(__main_ArgumentosDaFuncion__+1),&CaminoOrigen)
    );
    if(*CaminoOrigen.PesoDelNodo==-4){ printf("Sin archivo o sin datos\n"); exit(1);}
  }
  Final=f221(CaminoOrigen.CoordenadaX,CaminoOrigen.CoordenadaY,5);
  AgregaNodoMapaXY(&RutaSeleccionada, *CaminoOrigen.CoordenadaX , *CaminoOrigen.CoordenadaY , 0 );

  /*________________________________________________________________________________*/
  
  BuscaPuntosIF(&CaminoOrigen,&Metro,&Referencia,1,0.75);
  Metro.Velocidad=0.75;

  /*________________________________________________________________________________*/

  ImprimirCaminante(&Metro,NULL);
  ImprimirTirolesa(&Referencia,NULL);
  ImprimirMapa(&CaminoOrigen,&Metro.Velocidad);
  ImprimirMapa(&RutaSeleccionada,&Metro.Velocidad);
 
  while(precision(Final, 0, Metro.Coordenadas_Pcaminante,0)){
    if(*(Referencia.Coordenadas_Pref)+Referencia.Paso>*(Final)){
      AgregaNodoMapaXY(
        &RutaSeleccionada,
        *( Final ),
        *(Final+1),
        0
      );
      break;
    }
    Procesamiento(&CaminoOrigen, &Metro, &Referencia);
    AgregaNodoMapa(
      &RutaSeleccionada,
      Metro.Coordenadas_Pcaminante,
      Metro.PesoActual
    );

    *( Referencia.Coordenadas_Pref )=*( Metro.Coordenadas_Pcaminante );
    *(Referencia.Coordenadas_Pref+1)=*(Metro.Coordenadas_Pcaminante+1);
  }
  Metro.PesoAcumulado+=2;

  printf("Ruta seleccionada:\n");
  ImprimirMapa(&RutaSeleccionada,&Metro.Velocidad);

  printf("Puntos propuestos:\n");
  PlotMapa(&CaminoOrigen,'p');
  printf("Mejor Camino:\n");
  PlotMapa(&RutaSeleccionada,'l');
  EliminarMapa(&CaminoOrigen);
  EliminarMapa(&RutaSeleccionada);
}

  /*________________________________________________________________________________*/

void Procesamiento(const MAPA* __Mapa__,CAMINANTE* __Caminante__,TIROLESA* __Tirolesa__){
  MAPA Camino;
  PARTICULA SELECTA;
  CrearMapa(&Camino,6);
  unsigned int CantidadDelementosEncontrados,*ElementosEncontrados=NULL;
  unsigned int k;
  float Parametros[5];
  float *LimitesSuperiores, LimitesInferiores[3];
  float* cooF=NULL;
  float MaximaPrecision, mpaux;
  unsigned int PuntoSeleccionado;

  /*________________________________________________________________________________*/

  AvanzaPref(__Tirolesa__);
  CantidadDelementosEncontrados=
    busqueda(*(__Caminante__->Coordenadas_Pcaminante),__Tirolesa__->Paso,__Mapa__,&ElementosEncontrados);
  if (ElementosEncontrados) {
    k=0;
    while (k<CantidadDelementosEncontrados) {
      AgregaNodoMapaXY(
        &Camino,
        *(__Mapa__->CoordenadaX+*(ElementosEncontrados+k)),
        *(__Mapa__->CoordenadaY+*(ElementosEncontrados+k)),
        *(__Mapa__->PesoDelNodo+*(ElementosEncontrados+k))
      );
      ++k;
    }
    printf("Posibles caminos:\n");
    ImprimirMapa(&Camino,&__Caminante__->Velocidad);
    
    // Llenado d'os parámetros d'operación
    // Es un vector de 5 elementos:
    // [5]{Xref,Yref,Vel,Xcaminante,Ycaminante} 
    *( Parametros )=*(__Tirolesa__->Coordenadas_Pref);
    *(Parametros+1)=*(__Tirolesa__->Coordenadas_Pref+1);
    *(Parametros+2)=  __Caminante__->Velocidad;
    *(Parametros+3)=*(__Caminante__->Coordenadas_Pcaminante);
    *(Parametros+4)=*(__Caminante__->Coordenadas_Pcaminante+1);
    // Limites
    // Será un vector de 4 elementos
    // [4]{LimiteSuperiorX,LimiteSuperiorY,LimiteInferiorX,LimiteInferiorY}
    // Los límites serán dados por valores en xy más altos d'os puntos encontrados
    *( LimitesInferiores )=*(__Caminante__->Coordenadas_Pcaminante)+(0.1*__Tirolesa__->Paso);
    *(LimitesInferiores+1)=0;
    *(LimitesInferiores+2)=0;
    LimitesSuperiores=LimitesMapa(__Mapa__);
    printf("\n\nLimitesSuperiores: (%.2f, %.2f, %.2f)\n",*(LimitesSuperiores),*(LimitesSuperiores+1),*(LimitesSuperiores+2));
    printf("LimitesSuperiores: (%.2f, %.2f, %.2f)\n",*(LimitesInferiores),*(LimitesInferiores+1),*(LimitesInferiores+2));

  /*________________________________________________________________________________*/

    SELECTA=
    ProcesoPSO(
      5, 3,
      LimitesSuperiores, LimitesInferiores,
      12, 0.72, 2.5, 2.5,
      Parametros
    );

    printf("\nPunto Ideal (x,y,w) = (%.2f, %.2f, %.2f) con una precisión de %f\n\n",
      *( SELECTA.Xi ),
      *(SELECTA.Xi+1),
      *(SELECTA.Xi+2),
      SELECTA.Xfit
    );

    /*________________________________________________________________________________*/

    k=0;
    MaximaPrecision=
    precision(
      SELECTA.Xi,
      *(SELECTA.Xi+2),
      cooF=f221(__Mapa__->CoordenadaX,__Mapa__->CoordenadaY,*(ElementosEncontrados+k)),
      *(__Mapa__->PesoDelNodo+*(ElementosEncontrados+k))
    );
    PuntoSeleccionado=*(ElementosEncontrados+k);
    free(cooF);
    ++k;
    while (k<CantidadDelementosEncontrados) {
      mpaux=precision(
        SELECTA.Xi,
        *(SELECTA.Xi+2),
        cooF=f221(__Mapa__->CoordenadaX,__Mapa__->CoordenadaY,*(ElementosEncontrados+k)),
        *(__Mapa__->PesoDelNodo+*(ElementosEncontrados+k))
      );
      if(MaximaPrecision>mpaux){
        PuntoSeleccionado=*(ElementosEncontrados+k);
        MaximaPrecision=mpaux;
      }
      free(cooF);
      ++k;
    }
    float CoordenadasPeso[3]={
       *(__Mapa__->CoordenadaX+PuntoSeleccionado),
       *(__Mapa__->CoordenadaY+PuntoSeleccionado),
       *(__Mapa__->PesoDelNodo+PuntoSeleccionado)
    };
    AvanzaCaminante(
      CoordenadasPeso,
      __Caminante__
    );
    __Caminante__->PesoAcumulado+=__Caminante__->PesoActual=*(__Mapa__->PesoDelNodo+PuntoSeleccionado);
    printf("Punto Seleccionado: ");
    ImprimirCaminante(__Caminante__,NULL);
    // Liberación de memoria
    free(LimitesSuperiores);
    free(ElementosEncontrados);
  }
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
  printf("(  X  ,  Y  :    W  ,  SW  ,  V^(-1)  )= ");
  printf("(%.2f,%.2f:  %.2f ,%.2f, %.2f)",
    *(__Caminante__->Coordenadas_Pcaminante),
    *(__Caminante__->Coordenadas_Pcaminante+1),
    __Caminante__->PesoActual,
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
  while(*(__Mapa__->PesoDelNodo+k)!=-3 && *(__Mapa__->PesoDelNodo+k)!=-4){
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
    coordenadas[0]=coordenadas[2];
    coordenadas[1]=coordenadas[3];
    ++k;
  }
  printf("\n");
}

void PlotMapa(
    const MAPA *__Mapa__,
    const char  __Type__
){
  FILE* Salida=fopen("TemporalPlot","w");
  float *LimitesDaGrafica;
  unsigned int k=0;
  while(*(__Mapa__->PesoDelNodo+k)!=-3 && *(__Mapa__->PesoDelNodo+k)!=-4){
    fprintf(Salida,"%f %f\n",*(__Mapa__->CoordenadaX+k),*(__Mapa__->CoordenadaY+k));
    ++k;
  }
  LimitesDaGrafica=LimitesMapa(__Mapa__);
  fclose(Salida);
  Salida=popen("gnuplot -persistent","w");
  fprintf(Salida,"set terminal dumb\n");
  fprintf(Salida,"set xrange [-1:%f]\nset yrange [-1:%f]\n",*(LimitesDaGrafica)+1,*(LimitesDaGrafica+1)+1);
  free(LimitesDaGrafica);
  if(__Type__=='l'){
    fprintf(Salida,"plot './TemporalPlot' with linespoints notitle\n");
  }else{
    fprintf(Salida,"plot './TemporalPlot' notitle\n");
  }
  pclose(Salida);
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
  //ImprimeEnjambre(Enj);
  EvaluacionInicialEnjambre(Enj,ParametrosDeOperacion);
  while((t++)<Enj->MaximoDeIteraciones){
    ActualizarVelocidad(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj,ParametrosDeOperacion);
    ActualizarMejoresPosiciones(Enj);
    //ImprimeEnjambre(Enj);
  }
  //ImprimeParticula(Enj,Enj->MejorParticulaDelGrupo);

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

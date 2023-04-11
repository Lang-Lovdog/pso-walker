#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

//---------------- PSO Constricted

// C1=C2=2.05
// X = 0.72984
// Soluciona el explotar del enjambre, ya no hay que jugar con el factor de peso para la velocidad


//#include<time>
//ESTRUCTURAS DEL PSO
typedef struct { float *Xi;   //Posicion
                 float *Vi;   //Velocidad
                 float *Pi;   //Mejor Posicion Historica
                 float  Xfit; //Valor de Fitnes para la posicion actual
                 float  Pfit; //Valor de Fitnes para la Mejor Posicion
                }PARTICULA;

typedef struct{ PARTICULA *Part;
                unsigned int NumPart;
                unsigned int NumParam;
                unsigned int IdPg;
                unsigned int NumMaxIter;
                float C1;
                float C2;
                const float *LimSup;
                const float *LimInf;
                float X;
               }ENJAMBRE;

//PARAMETROS DE CONFIGURACION DEL PSO
#define DIM_ 2
const unsigned int NumeroParticulas=100; //Tamaño del enjambre, numero de particulas del enjambre
const unsigned int Dimension=DIM_; //Numero de variables del problema o dimension del problema.
const float LimiteSuperior[DIM_]={ 5.12,  5.12};
const float LimiteInferior[DIM_]={-5.12, -5.12};
const unsigned int NumeroMaximoDeIteraciones=300;

ENJAMBRE* CrearEnjambre(unsigned int NumPart,unsigned int NumParam);
void InicializarEnjambre(ENJAMBRE *Enj, float X, float C1, float C2, unsigned int NumIterMax,const float *LInf,const float *LSup);
void EliminarEnjambre(ENJAMBRE *Enj);
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id);
void ImprimeEnjambre(ENJAMBRE *Enj);
void EvaluarEnjambre(ENJAMBRE *Enj);
void EvaluacionInicialEnjambre(ENJAMBRE *Enj);
void ActualizarVelocidad(ENJAMBRE *Enj);
void ActualizarPosicion(ENJAMBRE *Enj);
void ActualizarMejoresPosiciones(ENJAMBRE *Enj);
float FuncionObjetivo(float *Xi, unsigned int Np);

int main()
{ ENJAMBRE *Enj;
  time_t tx;
  //srand((unsigned) time(&tx));
  srand(time(NULL));
  unsigned int t=0;
  //Crear un enjambre de NumeroParticulas de Numero de parametros igual a Dimension
  Enj=CrearEnjambre(NumeroParticulas,Dimension);
  printf("\nSe creo el ENJAMBRE (IW)");
  InicializarEnjambre(Enj, 0.72984,2.05,2.05,NumeroMaximoDeIteraciones,LimiteInferior,LimiteSuperior);
  EvaluacionInicialEnjambre(Enj);
  ImprimeEnjambre(Enj);
  while(t<Enj->NumMaxIter)
  { //printf("\nIteracion: %i",t);
    ActualizarVelocidad(Enj);
    ActualizarPosicion(Enj);
    EvaluarEnjambre(Enj);
    ActualizarMejoresPosiciones(Enj);
    //ImprimeEnjambre(Enj);
    if((t%10)==0){
        printf("\nIteracion %i", t);
    }
    t++;
  }
  printf("\nBest Particle:");
  ImprimeParticula(Enj,Enj->IdPg);
  EliminarEnjambre(Enj);
  printf("\nSe elimino el ENJAMBRE");
  return 0;
}

float FuncionObjetivo(float *Xi, unsigned int Np)
{ unsigned int k;
  float fit, aux = 0;
  /* Maximar la siguiente funcion:
     f(x,y)=50-(x-5)^2-(y-5)^2;
  */
  //fit=250-pow(Xi[0]+7,2)-pow(Xi[1]-3,2)-pow(Xi[2]-3,2)-pow(Xi[3]-5,2)-pow(Xi[4]-8,2);

  // Funcion Rastriging
  for (k=0; k<Np; k++)
    aux += pow(Xi[k],2)-10*cos(6.283185*Xi[k])+10;

  fit = 100 - aux;

  return fit;
}

void ActualizarMejoresPosiciones(ENJAMBRE *Enj)
{
    for(unsigned int i=0; i<Enj->NumPart; i++)
    {
        if(Enj->Part[i].Xfit > Enj->Part[i].Pfit)
        {
            Enj->Part[i].Pfit = Enj->Part[i].Xfit;
            for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
             {
                Enj->Part[i].Pi[j] = Enj->Part[i].Xi[j];

              }
        }
    }
}
/*
void ActulizarMejoresPosiciones(ENJAMBRE *Enj)
{ for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula
     if( Enj->Part[i].Xfit > Enj->Part[i].Pfit )
       { Enj->Part[i].Pfit=Enj->Part[i].Xfit;
           for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector Vi de la particula i
               Enj->Part[i].Pi[j]=Enj->Part[i].Xi[j];
       }
}*/

void EvaluarEnjambre(ENJAMBRE *Enj)
{
    float BestFit;
    // Calcular el valor de Fitness de cada particula
    BestFit = FuncionObjetivo(Enj->Part[0].Xi, Enj->NumParam);
    for(unsigned int i=0; i<Enj->NumPart; i++)
    {
        Enj->Part[i].Xfit = FuncionObjetivo(Enj->Part[i].Xi, Enj->NumParam);
        // Almacena el indice de la mejor particula de todo en enjambre
        if(Enj->Part[i].Xfit>BestFit)
        {
            BestFit = Enj->Part[i].Xfit;
            Enj->IdPg =i;
        }
    }
}
/*
void EvaluarEnjambre(ENJAMBRE *Enj)
{ float BestFit;
  //Calcular el valor de fitness de cada Particula
  BestFit=Enj->Part[Enj->IdPg].Pfit;
  for(unsigned int i=0; i<Enj->NumPart; i++)
      { Enj->Part[i].Xfit=FuncionObjetivo(Enj->Part[i].Xi);
        //Almacena el indice de la mejor particula de todo el enjambre
        if(Enj->Part[i].Xfit>BestFit)
           { BestFit=Enj->Part[i].Xfit;
             Enj->IdPg=i;
           }
        }
}*/

/*
void ActualizarPosicion(ENJAMBRE *Enj)
{ //Actualizar cada vector posicion Xi de cada particula
  for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
  for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector Vi de la particula i
      Enj->Part[i].Xi[j]+=Enj->Part[i].Vi[j];
}
*/

void ActualizarPosicion(ENJAMBRE *Enj)
{
    // Acutailzsr cada vector Posicion XI de cada particula
  for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
  for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
     {
        Enj->Part[i].Xi[j] += Enj->Part[i].Vi[j];

      }
}

void ActualizarVelocidad(ENJAMBRE *Enj)
{ float Y1,Y2;
  //Actualizar cada vector velocidad Vi de cada particula
  for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
  for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector Vi de la particula i
     {
    Y1=(float)rand()/RAND_MAX;
      Y2=(float)rand()/RAND_MAX;
      Enj->Part[i].Vi[j] = Enj->X*((Enj->Part[i].Vi[j])+
                                   (Enj->C1*Y1*(Enj->Part[i].Pi[j]-Enj->Part[i].Xi[j]))+
                                   (Enj->C2*Y2*(Enj->Part[Enj->IdPg].Pi[j]-Enj->Part[i].Xi[j])) );

      }
}

void EvaluacionInicialEnjambre(ENJAMBRE *Enj)
{ float aux,BestFit;
  //Calcular el valor de fitness de cada Particula
  BestFit=FuncionObjetivo(Enj->Part[0].Xi, Enj->NumParam);
  for(unsigned int i=0; i<Enj->NumPart; i++)
      { aux=FuncionObjetivo(Enj->Part[i].Xi, Enj->NumParam);
        Enj->Part[i].Xfit=aux;
        Enj->Part[i].Pfit=aux;
        //Almacena el indice de la mejor particula de todo el enjambre
        if(aux>BestFit)
           { BestFit=aux;
             Enj->IdPg=i;
           }
        }
}

void ImprimeEnjambre(ENJAMBRE *Enj)
{ for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
      { //printf("\n");
        ImprimeParticula(Enj,i);
      }
}
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id)
    { printf("\nP%i,Xi: ",Id);
      for(unsigned int i=0; i<Enj->NumParam; i++)
          printf("%f, ",Enj->Part[Id].Xi[i]);
      printf("\nP%i,Vi: ",Id);
      for(unsigned int i=0; i<Enj->NumParam; i++)
        printf("%f, ",Enj->Part[Id].Vi[i]);
      printf("\nP%i,Pi: ",Id);
      for(unsigned int i=0; i<Enj->NumParam; i++)
        printf("%f, ",Enj->Part[Id].Pi[i]);
      printf("\nP%i,Xfit=%f",Id,Enj->Part[Id].Xfit);
      printf("\nP%i,Pfit=%f",Id,Enj->Part[Id].Pfit);
      }

void InicializarEnjambre(ENJAMBRE *Enj, float X, float C1, float C2, unsigned int NumIterMax,const float *LInf, const float *LSup)
{ float aux,rango;
  Enj->X=X;
  Enj->C1=C1;
  Enj->C2=C2;
  Enj->NumMaxIter=NumIterMax;
  Enj->IdPg=0;
  Enj->LimSup=LSup;
  Enj->LimInf=LInf;
  //Inicializar cada vector de cada particula
  for(unsigned int i=0; i<Enj->NumPart; i++) //Para cada particula i
  for(unsigned int j=0; j<Enj->NumParam; j++) //Para cada parametro j de cada vector de la particula i
     { rango=Enj->LimSup[j]-Enj->LimInf[j];
       aux= ((float)rand()/RAND_MAX) * rango + Enj->LimInf[j];
       Enj->Part[i].Xi[j]=aux;
       Enj->Part[i].Vi[j]=0;
       Enj->Part[i].Pi[j]=aux;
      }
}

void EliminarEnjambre(ENJAMBRE* Enj)
{ //Liberar la memoria para de los 3 vectores de cada Particula
  for(unsigned int i=0; i<Enj->NumPart; i++)
     { free(Enj->Part[i].Xi);
       free(Enj->Part[i].Vi);
       free(Enj->Part[i].Pi);
     }
  //Liberar la memoria de las estructuras particula
  free(Enj->Part);
  //Liberar la memoria de la estrcutura del enajmbre
  free(Enj);
}

ENJAMBRE* CrearEnjambre( unsigned int NumPart,unsigned int NumParam)
{ ENJAMBRE *ptr;
  //Reservar la memoria para la estructura del enjambre
  ptr=(ENJAMBRE *)malloc(sizeof(ENJAMBRE));
  if(ptr==NULL)
    { printf("Error al reservar la memoria para la estructura ENJAMBRE.");
      exit(0);
    }
  ptr->NumPart=NumPart;
  ptr->NumParam=NumParam;

  //Reservar la memoria para N particulas de M parametros
  ptr->Part=(PARTICULA *)malloc(NumPart*sizeof(PARTICULA));
  if(ptr->Part==NULL)
    { printf("Error al reservar la memoria para las Particulas.");
      exit(0);
    }
  //Reservar memoria para los 3 vectores de cada Particula
  for(unsigned int i=0; i<NumPart; i++)
      { ptr->Part[i].Xi=(float *)malloc(NumParam*sizeof(float));
        ptr->Part[i].Vi=(float *)malloc(NumParam*sizeof(float));
        ptr->Part[i].Pi=(float *)malloc(NumParam*sizeof(float));
      }
  return ptr;
}

#ifndef __pso__header__
#define __pso__header__

// Cabeceras necesarias para el proyecto
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// Definición de la estructura Patícula
// Esta partícula representa a un individuo
// El individuo buscará tener la mejor posición
// Habrán dos criterios: En el primero, ve la mejor
// posición que ha tenido durante su existencia; en
// el segundo, ve a aquel que tiene la mejor posición
// en ese momento.
// Recalcula su valor de paso (velocidad) y a partir de
// ahí, suma el paso a su posición actual, obteniendo el
// nuevo valor de posición.
typedef struct { 
  float *Xi;   //Posicion
  float *Vi;   //Velocidad
  float *Pi;   //Mejor Posicion Historica
  float  Xfit; //Valor de Fitnes para la posicion actual
  float  Pfit; //Valor de Fitnes para la Mejor Posicion
}PARTICULA;

// Definición de la estructura Enjambre
// El enjambre es un conjunto de partículas
// Este conjunto actuará para encontrar soluciones
// Cada solución es repensada según los valores históricos
// y valores presentes.
typedef struct{
  PARTICULA *Part;
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


// Operadores del enjambre (métodos)

/* Creador de enjambres: Recive el número de partículas y el
 * número de parámetros (variables del problema)*/
ENJAMBRE* CrearEnjambre(unsigned int __NumeroParticulas__ ,unsigned int __NumeroVariables__);
/* */
void InicializarEnjambre(ENJAMBRE *Enj, float X, float C1, float C2, unsigned int NumIterMax,const float *LInf,const float *LSup);
/* Creador de enjambres*/
void EliminarEnjambre(ENJAMBRE *Enj);
/* Creador de enjambres*/
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id);
/* Creador de enjambres*/
void ImprimeEnjambre(ENJAMBRE *Enj);
/* Creador de enjambres*/
void EvaluarEnjambre(ENJAMBRE *Enj);
/* Creador de enjambres*/
void EvaluacionInicialEnjambre(ENJAMBRE *Enj);
/* Creador de enjambres*/
void ActualizarVelocidad(ENJAMBRE *Enj);
/* Creador de enjambres*/
void ActualizarPosicion(ENJAMBRE *Enj);
/* Creador de enjambres*/
void ActualizarMejoresPosiciones(ENJAMBRE *Enj);
/* Creador de enjambres*/
float FuncionObjetivo(float *Xi, unsigned int Np);

#endif __pso__header__

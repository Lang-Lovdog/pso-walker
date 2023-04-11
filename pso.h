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
// el segundo, ve qué partícula tiene la mejor posición
// en ese momento.
// Recalcula su valor de paso (velocidad) y, a partir de
// ahí, suma el paso a su posición actual, obteniendo el
// nuevo valor de posición.
// La partícula requiere saber en cuántas dimensiones
// estará moviéndose. Dichas dimensiones definirán al vector
// posición y al vector velocidad.
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

/* Creador de enjambres:
 * Recibe el número de partículas y el número de parámetros
 * (variables del problema)*/
ENJAMBRE* CrearEnjambre(
    unsigned int __NumeroParticulas__,
    unsigned int __NumeroVariables__
  );
/* Inicializador de enajmbres:
 * Defie los valoes predeterminados (de inicio), de los individuos.
 * Recibe el enjambre, la posición inicial, las variables del problema,
 * y los límites*/
void InicializarEnjambre(
    ENJAMBRE *Enj,
    float X,
    float C1,
    float C2,
    unsigned int NumIterMax,
    const float *LInf,
    const float *LSup
  );
/* Una vez terminado el programa, ésta función liberará la memoria
 * que se reservó durante la creación del enjambre. Como argumento,
 * recibe al apuntador del enjambre.*/
void EliminarEnjambre(ENJAMBRE *Enj);
/* Nos permite visualizar los parámetros de la partícula.*/
void ImprimeParticula(ENJAMBRE *Enj, unsigned int Id);
/* Permite visualizar los parámetros del enjambre, y las partículas
 * que le componen.*/
void ImprimeEnjambre(ENJAMBRE *Enj);
/* Permite valorar al enjambre, según los criterios del PSO
 * y de la función objetivo*/
void EvaluarEnjambre(ENJAMBRE *Enj);
/* Similar a EvaluarEnjambre, con la particularidad de que Inicializa
 * los valores de Mejor Posicion Historica, de las particulas*/
void EvaluacionInicialEnjambre(ENJAMBRE *Enj);
/* Renueva la valocidad basado en los vectores de
 * Posición Actual,
 * Mejor Posicion Historica y
 * Mejor Posicion Global Actual*/
void ActualizarVelocidad(ENJAMBRE *Enj);
/* Suma los valores de velocidad a la posición actual de
 * cada partícula.*/
void ActualizarPosicion(ENJAMBRE *Enj);
/* */
void ActualizarMejoresPosiciones(ENJAMBRE *Enj);
/* Creador de enjambres*/
float FuncionObjetivo(float *Xi, unsigned int Np);

#endif __pso__header__

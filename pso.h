#ifndef __pso__header__
#define __pso__header__

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
  float  Pfit; //Valor de Fitnes para la Mejor Posicion Historica
}PARTICULA;

// Definición de la estructura Enjambre
// El enjambre es un conjunto de partículas
// Este conjunto actuará para encontrar soluciones
// Cada solución es repensada según los valores históricos
// y valores presentes.
typedef struct{
  PARTICULA *Part;                      // Partículas
  unsigned int CantidadDeParticulas;    // Número de partículas
  unsigned int CantidadDeParametros;    // Número de parámetros del problema
  unsigned int MejorParticulaDelGrupo;  // ID de la mejor partícula del grupo
  unsigned int MaximoDeIteraciones;     // Número máximo d'iteraciones a realizar
  float C1;                             // Valor de peso C1
  float C2;                             // Valor de peso C2
  const float *LimitesSuperiores;       // Limites Superiores
  const float *LimitesInferiores;       // Limites Inferiores
  float X;                              // Factor de constricción (convergencia)
  float Constriccion;                   // Factor de constricción (convergencia)
}ENJAMBRE;


// Operadores del enjambre (métodos)

/* Creador de enjambres:
 * Recibe el número de partículas y el número de parámetros
 * (variables del problema)*/
ENJAMBRE* CrearEnjambre(
    //ENJAMBRE     *__Enjambre__,
    unsigned int  __CantidadDeParticulas__,
    unsigned int  __CantidadDeParametros__
  );
/* Inicializador de enajmbres:
 * Defie los valoes predeterminados (de inicio), de los individuos.
 * Recibe el enjambre, la posición inicial, las variables del problema,
 * y los límites*/
void InicializarEnjambre(
    ENJAMBRE    *__Enjambre__,
    float        __FactorConstriccion__,
    float        __ValorDePeso_C1__,
    float        __ValorDePeso_C2__,
    unsigned int __MaximoDeIteraciones__,
    const float *__LimitesInferiores__,
    const float *__LimitesSuperiores__
  );
/* Una vez terminado el programa, ésta función liberará la memoria
 * que se reservó durante la creación del enjambre. Como argumento,
 * recibe al apuntador del enjambre.*/
void EliminarEnjambre(
    ENJAMBRE *__Enjambre__
  );
/* Nos permite visualizar los parámetros de la partícula.*/
void ImprimeParticulaID(
    ENJAMBRE     *__Enjambre__,
    unsigned int  __ID_Particula__
  );
/*Imprime la particula sin enjambre*/
void ImprimeParticula(
  const PARTICULA    *__Particula__,
  const unsigned int  __CantidadDeParametros__
  );
/* Permite visualizar los parámetros del enjambre, y las partículas
 * que le componen.*/
void ImprimeEnjambre(
    ENJAMBRE *__Enjambre__
  );
/* Permite valorar al enjambre, según los criterios del PSO
 * y de la función objetivo*/
void EvaluarEnjambre(
    ENJAMBRE    *__Enjambre__,
    const float *__ParametrosDeOperacion__
  );
/* Similar a EvaluarEnjambre, con la particularidad de que Inicializa
 * los valores de Mejor Posicion Historica, de las particulas*/
void EvaluacionInicialEnjambre(
    ENJAMBRE    *__Enjambre__,
    const float *__ParametrosDeOperacion__
  );
/* Renueva la valocidad basado en los vectores de
 * Posición Actual,
 * Mejor Posicion Historica y
 * Mejor Posicion Global Actual*/
void ActualizarVelocidad(
    ENJAMBRE *__Enjambre__
  );
void ActualizarVelocidadInerciaW(
    ENJAMBRE *__Enjambre__
  );
void ActualizarVelocidadConstriction(
    ENJAMBRE *__Enjambre__
  );
/* Suma los valores de velocidad a la posición actual de
 * cada partícula.*/
void ActualizarPosicion(
    ENJAMBRE *__Enjambre__
  );
/* Valora, en cada partícula, si el valor actual es mejor que'l mejor
 * valor histórico; si los valores actuales son mejores, actualiza
 * los parametros. */
void ActualizarMejoresPosiciones(
    ENJAMBRE *__Enjambre__
  );

/* La función a evaluar, regresa el valor de fitness (precisión)
 * Requiere ser definida para l'evaluación d'as partículas
 * Valores De Parametros .... (arreglo float)
 * Cantidad De Parametros ... (int)
 * Parametros De Operacion .. (arreglo float)
*/
float FuncionObjetivo(
    float          *__ValoresDeParametros__,
    unsigned int    __CantidadDeParametros__,
    const float    *__ParametrosDeOperacion__
  );

/* Funcion que se puede definir para realizar el procesamiento pso,
 * puede ser ignorado o definido y consta d'os sig. elementos, en
 * el orden en que se presentan a continuación:
 * Numero De Particulas ................. (float)
 * Dimension ............................ (float)
 * Límites Superiores ................... (arreglo float)
 * Límite Inferiores .................... (arreglo float)
 * Numero Máximo De Iteraciones ......... (int)
 * Factor De Constriccion O De Inercia .. (float)
 * Valor Peso C1: Mejor Personal ........ (float)
 * Valor Peso C2: Mejor Global .......... (float)
 * Parametros De Operacion .............. (arreglo float) */
PARTICULA ProcesoPSO(
    const float        __NumeroDeParticulas__,
    const float        __Dimension__,
    const float       *__LimiteSuperior__,
    const float       *__LimiteInferior__,
    const unsigned int __NumeroMaximoDeIteraciones__,
    const float        __Factor_Constriccion_Inercia__,
    const float        __ValorPesoPersonalC1__,
    const float        __ValorPesoGlobalC2__,
    const float       *__ParametrosDeOperacion__
  );

#endif

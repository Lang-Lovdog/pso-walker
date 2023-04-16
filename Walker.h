#ifndef __LangLovdog_Walker_Header__
#define __LangLovdog_Walker_Header__

// Definición d'as partes del programa

// El mapa, definido por 3 vectores: Coordenada X, Coordenada Y
// y Peso del nodo.
typedef struct{
  float* CoordenadaX;
  float* CoordenadaY;
  float* PesoDelNodo;
}MAPA;
// La tirolesa, definida por 1 valor y 2 vectoes: Pendiente d'a
// rectoa, Coordenada X y Coordenada Y de Prefi
typedef struct{
  float PendienteDaRecta;
  float Coordenadas_Pref[2];
  float Paso;
}TIROLESA;
// El caminante, sólo guarda un vector con las coordenadas X,Y
typedef struct{
  float Coordenadas_Pcaminante[2];
  float PesoAcumulado;
  float PesoActual;
  float Velocidad; // unidad sobre tiempo (Inversa multiplicativa d'a velocidad)
}CAMINANTE;

// Utilidad d'inicialización d mapa
void CrearMapa(
    MAPA *__Mapa__,
    const float __tamanno__
  );
void EliminarMapa(
    MAPA *__Mapa__
  );
void AgregaNodoMapa(
    MAPA *__Mapa__,
    const float* __Nodo__,
    const float  __Peso__
  );
void AgregaNodoMapaXY(
    MAPA *__Mapa__,
    const float __X__,
    const float __Y__,
    const float __Peso__
  );
// Utilidad de Impresión de mapa
void ImprimirMapa(
    const MAPA *__Mapa__,
    const float *__ParametrosDeOperacion__
  );
// Utilidad de Impresión de caminante
void ImprimirCaminante(
    const CAMINANTE* __Caminante__,
    const float *__ParametrosDeOperacion__
  );
// Utilidad de Impresión de tirolesa
void ImprimirTirolesa(
    const TIROLESA* __Tirolesa__,
    const float *__ParametrosDeOperacion__
  );

// Utilidad que permita encontrar los límites de un mapa
float* LimitesMapa(const MAPA* __Mapa__);

// Utilidad para pasar de 2 *x *y arreglos a un arreglo *xy
// Usar siempre a través d'un apuntador que permita la
// liberación d'a memoria después de su utilización.
float *f221(
    const float        *__A__,
    const float        *__B__,
    const unsigned int  __ID__
  );
// Utilidad para'l cálculo d'a distancia
float v_distancia(
    const float* __A__,
    const float* __B__
  );
// Utilidad para'l cálculo d'a pendiente
float pendiente(
    const float *__A__,
    const float *__B__
  );

// Funciones d'operación

// Búsqueda d'os puntos extremos
void BuscaPuntosIF(
    const MAPA *__Mapa__,
    CAMINANTE  *__Caminante__,
    TIROLESA   *__tirolesa__,
    const float __Paso__,
    const float __Vel1__
  );
// Posiciona'l punto referencia
void AvanzaPref(
    TIROLESA    *__tirolesa__
  );
// Posiciona'l caminante
void AvanzaCaminante(
    const float *__NuevoPunto__,
    CAMINANTE   *__Caminante__
  );
// Busqueda de puntos en rango
unsigned int busqueda(
    const float    __Actual__,
    const float    __Paso__,
    const MAPA*    __Mapa__,
    unsigned int** __ElementosEncontrados__
  );
// Comparación de puntos con seleccionado
float precision(
    const float* __A__,
    const float  __WA__,
    const float* __B__,
    const float  __WB__
  );

#endif

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
  char  AvanceX;
}TIROLESA;
// El caminante, sólo guarda un vector con las coordenadas X,Y
typedef struct{
  float Coordenadas_Pcaminante[2];
  float PesoAcumulado;
  float Velocidad; // unidad sobre tiempo (Inversa multiplicativa d'a velocidad)
}CAMINANTE;
// Utilidad para pasar de 2 *x *y arreglos a un arreglo *xy
// Usar siempre a través d'un apuntador que permita la
// liberación d'a memoria después de su utilización.
float *f221(
    const float *__A__,
    const float *__B__,
    const float __ID__
  );
// Utilidad para'l cálculo d'a distancia
float v_distancia(
    const float* __A__,
    const float* __B__
  );

// Funciones d'operación

// Búsqueda d'os puntos extremos
void BuscaPuntosIF(
    const MAPA *__Mapa__,
    CAMINANTE  *__Caminante__,
    TIROLESA   *__tirolesa__
  );
// Cálculo d'a pendiente
float pendiente(
    const float *__A__,
    const float *__B__
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
    const float __Paso__,
    const MAPA* __Mapa__
  );
// Comparación de puntos con seleccionado
float precision(const float* __A__,
    const float* __B__
  );
#endif

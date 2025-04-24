#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#define MAX 100

void Datos(int *v, int *l) {
  do {
    printf("Por favor ingresa el n%cmero de v%crtices:\n", 163, 130);
    if (scanf("%d", v) != 1 || *v < 1 || *v > MAX) {
      printf("Entrada invalida. Solo permitimos enteros positivos.\n");
      while (getchar() != '\n')
        ;
    } else
      break;
  } while (1);

  do {
    printf("Ingresa el n%cmero de l%cneas:\n", 163, 161);
    if (scanf("%d", l) != 1 || *l < 1 || *l > MAX) {
      printf("Entrada invalida. Solo permitimos enteros positivos.\n");
      while (getchar() != '\n')
        ;
    } else
      break;
  } while (1);
}

int tieneBucles(int lol, int aristas[][2]) {
  for (int i = 0; i < lol; i++) {
    if (aristas[i][0] == aristas[i][1]) {
      return 1; // Hay al menos un bucle
    }
  }
  return 0; // No hay bucles
}

int tieneLineasParalelas(int l, int aristas[][2], int esDirigida) {
  for (int i = 0; i < l; i++) {
    for (int j = i + 1; j < l; j++) {
      if (esDirigida) {
        if (aristas[i][0] == aristas[j][0] && aristas[i][1] == aristas[j][1]) {
          return 1;
        }
      } else {
        if ((aristas[i][0] == aristas[j][0] &&
             aristas[i][1] == aristas[j][1]) ||
            (aristas[i][0] == aristas[j][1] &&
             aristas[i][1] == aristas[j][0])) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void pedirRelaciones(int l, int v, int aristas[][2]) {
  for (int i = 0; i < l; i++) {
    printf("Relacion %d:\n", i + 1);
    do {
      printf("Vertice origen (1-%d): ", v);
      scanf("%d", &aristas[i][0]);
    } while (aristas[i][0] < 1 || aristas[i][0] > v);

    do {
      printf("Vertice destino (1-%d): ", v);
      scanf("%d", &aristas[i][1]);
    } while (aristas[i][1] < 1 || aristas[i][1] > v);
  }
}

void matrizincidencia(int v, int l, int aristas[][2], int esDirigida) {
  int AG[MAX][MAX];
  int bucle[MAX] = {0}; 
  // Se inicia con ceros matriz con ceros
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < l; j++) {
      AG[i][j] = 0;
    }
  }
  for (int i=0;i<l;i++) {
    int sale = aristas[i][0] - 1;
    int llega = aristas[i][1] - 1;
    AG[sale][i] = 1;
    if (sale == llega) {
      bucle[i] = 1; //Esto es para decirle al programa que es bucle
    } else if (esDirigida) {//cambia si es dirigida
      AG[llega][i] = -1;
    } else {
      AG[llega][i] = 1;
    }
  }
  //Impresión
  printf("\nLa matriz de Incidencia es:\n    ");
  for (int j=0;j<l;j++)
    printf("L%d ",j+1);
  printf("\n");
  for (int i=0;i<v;i++) {
    printf("V%c ",'A'+i);
    for (int j = 0; j < l; j++) {
      if (bucle[j] && i == aristas[j][0] -1 && esDirigida) {
        printf("±1");
      } else {
        printf("%2d ", AG[i][j]);
      }
    }
    printf("\n");
  }
}

void matrizadyacencia(int v,int l, int esDirigida, int aristas[][2],
                      int ady[MAX][MAX]) {
  for (int i=0;i<v;i++)
    for (int j = 0; j < v; j++)
      ady[i][j] = 0;

  for (int i = 0; i < l; i++) {
    int origen = aristas[i][0] - 1;
    int destino = aristas[i][1] - 1;
    ady[origen][destino] = 1;
    if (!esDirigida)
      ady[destino][origen] = 1;
  }

  printf("\nMatriz de Adyacencia:\n   ");
  for (int j = 0; j < v; j++)
    printf(" %c",'A'+j);
  printf("\n");

  for (int i = 0; i < v; i++) {
    printf("%c |", 'A' + i);
    for (int j = 0; j < v; j++) {
      printf(" %d",ady[i][j]);
    }
    printf("\n");
  }
}

void dfs(int v, int mat[MAX][MAX], int origen, int visitado[MAX]) {
  for (int i = 0; i < v; i++) {
    if (mat[origen][i] && !visitado[i]) {
      visitado[i] = 1;
      dfs(v, mat, i, visitado);
    }
  }
}

void matrizaccesibilidad(int v, int XG[MAX][MAX]) {
  int MG[MAX][MAX];
  int Potencia[MAX][MAX];
  int temp[MAX][MAX];

  // Aqui creo las matrices potencia :))
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < v; j++) {
      MG[i][j] = XG[i][j];
      Potencia[i][j] = XG[i][j];
    }
  }

  //Aqui se suman las potencias
  for (int step = 2; step <= v - 1; step++) {
    // temp=Potencia*XG
    for (int i = 0; i < v; i++) {
      for (int j = 0; j < v; j++) {
        temp[i][j] = 0;
        for (int k = 0; k < v; k++) {
          temp[i][j] += Potencia[i][k] * XG[k][j];
        }
      }
    }
    // Potencia = temp
    for (int i = 0; i < v; i++)
      for (int j = 0; j < v; j++)
        Potencia[i][j] = temp[i][j];
    // MG = MG + Potencia
    for (int i = 0; i < v; i++)
      for (int j = 0; j < v; j++)
        MG[i][j] += Potencia[i][j];
  }
  // Igual se inicia con ceros (osea, use el diagrama)
  printf("\nLa matriz de Accesibilidad es:\n   ");
  for (int j = 0; j < v; j++)
    printf(" %c", 'A' + j);
  printf("\n");
  for (int i = 0; i < v; i++) {
    printf("%c |", 'A' + i);
    for (int j = 0; j < v; j++) {
      if (MG[i][j] == 0)
        printf(" 0");
      else
        printf(" +");
    }
    printf("\n");
  }
}

void calcularGrados(int v, int esDirigida, int aristas[][2], int l) {
  int gradoIn[MAX] = {0}, gradoOut[MAX] = {0};
  for (int i = 0; i < l; i++) {
    int origen=aristas[i][0] - 1;
    int destino=aristas[i][1] - 1;
    if (esDirigida){
      gradoOut[origen]++;
      gradoIn[destino]++;
    } else {
      gradoIn[origen]++;
      gradoIn[destino]++;
    }
  }

  printf("\nGrado de los V%crtices:\n", 130);
  for (int i = 0; i < v; i++) {
    if (esDirigida)
      printf("V%c -> Interno: %d, Externo: %d\n", 'A' + i, gradoIn[i], gradoOut[i]);
    else
      printf("V%c -> Grado: %d\n", 'A' + i, gradoIn[i]);
  }
}

void verificarAisladosColgantes(int v, int aristas[][2], int l) {
  int grado[MAX] = {0};

  for (int i = 0; i < l; i++) {
    grado[aristas[i][0] - 1]++;
    grado[aristas[i][1] - 1]++;
  }

  printf("\nV%crtices Aislados y Colgantes:\n", 130);
  for (int i = 0; i < v; i++) {
    if (grado[i] == 0)
      printf("V%c -> Aislado\n", 'A' + i);
    else if (grado[i] == 1)
      printf("V%c -> Colgante\n", 'A' + i);
  }
}

void clasificarSimpleGeneral(int l, int aristas[][2], int esDirigida) {
  int tieneBucle = tieneBucles(l, aristas);
  int tieneParalela = tieneLineasParalelas(l, aristas, esDirigida);

  if (!tieneBucle && !tieneParalela)
    printf("\nLa gr%cfica es SIMPLE.\n", 160);
  else
    printf("\nLa gr%cfica es GENERAL.\n", 160);
}

void clasificarCompletaRegular(int v, int ady[MAX][MAX], int esDirigida) {
  int regular = 1, completa = 1;
  int refGrado = 0;

  for (int i = 0; i < v; i++) {
    int grado = 0;
    for (int j = 0; j < v; j++) {
      if (i != j)
        grado += ady[i][j];
      else if (ady[i][j] != 0)
        completa = 0; // no debe haber bucle
    }

    if (i == 0)
      refGrado = grado;
    else if (grado != refGrado)
      regular = 0;

    if (grado != (esDirigida ? v - 1 : v - 1))
      completa = 0;
  }

  printf("\nClasificaci%cn:\n", 162);
  if (regular)
    printf("Regular: S%c\n", 161);
else
    printf("Regular: No\n");
}

void clasificarNula(int l) {
  if (l == 0)
    printf("\nLa gr%cfica es NULA (sin aristas).\n", 160);
  else
    printf("\nLa gr%cfica NO es nula.\n", 160);
}

void clasificarConectada(int v, int ady[MAX][MAX]) {
  int visitado[MAX] = {0};

  dfs(v, ady, 0, visitado);

  for (int i = 0; i < v; i++) {
    if (!visitado[i]) {
      printf("La gr%cfica NO es CONECTADA.\n", 160);
      return;
    }
  }

  printf("La gr%cfica es CONECTADA.\n", 160);
}


int tieneCiclo = 0;
int visitadoGlobal[MAX];

void dfsArbol(int u, int parent, int v, int ady[MAX][MAX]) {
  visitadoGlobal[u] = 1;
  for (int i = 0; i < v; i++) {
    if (ady[u][i]) {
      if (!visitadoGlobal[i])
        dfsArbol(i, u, v, ady);
      else if (i != parent)
        tieneCiclo = 1;
    }
  }
}

int esArbol(int v, int l, int ady[MAX][MAX]) {
  tieneCiclo = 0;
  for (int i = 0; i < MAX; i++) visitadoGlobal[i] = 0;

  dfsArbol(0, -1, v, ady);

  for (int i = 0; i < v; i++) {
    if (!visitadoGlobal[i])
      return 0; // No está conectado
  }

  return !tieneCiclo;
}

void clasificarArbol(int v, int l, int ady[MAX][MAX]) {
  if (esArbol(v, l, ady))
    printf("La gr%cfica es un %crbol.\n", 160, 181);
  else
    printf("La gr%cfica NO es un %crbol.\n", 160, 181);
}

void clasificarSimetrica(int v, int ady[MAX][MAX]) {
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < v; j++) {
      if (ady[i][j] != ady[j][i]) {
        printf("La digr%cfica NO es Sim%ctrica.\n", 160,130);
        return;
      }
    }
  }
  printf("La digr%cfica es Sim%ctrica.\n", 160,130);
}

void clasificarBalanceada(int v, int ady[MAX][MAX]) {
  for (int i = 0; i < v; i++) {
    int in = 0, out = 0;
    for (int j = 0; j < v; j++) {
      out += ady[i][j];
      in += ady[j][i];
    }
    if (in != out) {
      printf("La digr%cfica NO es Balanceada.\n", 160);
      return;
    }
  }
  printf("La digr%cfica es Balanceada.\n", 160);
}

void verificarInicialesFinales(int v, int ady[MAX][MAX]) {
  printf("\nV%crtices Iniciales y Finales (solo digráfica):\n", 130);

  for (int i = 0; i < v; i++) {
    int in = 0, out = 0;

    for (int j = 0; j < v; j++) {
      in += ady[j][i];
      out += ady[i][j];
    }

    if (in == 0)
      printf("V%c -> Inicial\n", 'A' + i);
    if (out == 0)
      printf("V%c -> Final\n", 'A' + i);
  }
}

void detectarLineasEnSerie(int v, int l, int aristas[][2]) {
    int grado[MAX] = {0};

    // Contar el grado de cada vértice
    for (int i = 0; i < l; i++) {
        grado[aristas[i][0] - 1]++;
        grado[aristas[i][1] - 1]++;
    }

    printf("\nL%cneas en Serie:\n", 161);

    int encontradas = 0;
    for (int i = 0; i < l; i++) {
        for (int j = i + 1; j < l; j++) {
            // Buscar vértice en común
            int a1 = aristas[i][0], b1 = aristas[i][1];
            int a2 = aristas[j][0], b2 = aristas[j][1];

            int comun = -1;
            if (a1 == a2 || a1 == b2) comun = a1;
            else if (b1 == a2 || b1 == b2) comun = b1;

            if (comun != -1 && grado[comun - 1] == 2) {
                printf("Aristas L%d y L%d están en serie mediante V%c\n", i + 1, j + 1, 'A' + comun - 1);
                encontradas = 1;
            }
        }
    }

    if (!encontradas)
        printf("No hay l%cneas en serie encontradas.\n", 161);
}

int esConectada(int v, int ady[MAX][MAX]) {
  int visitado[MAX] = {0};
  dfs(v, ady, 0, visitado);

  for (int i = 0; i < v; i++) {
    if (!visitado[i])
      return 0;
  }
  return 1;
}

void clasificarEulerianaUnicursal(int v, int ady[MAX][MAX]) {
  if (!esConectada(v, ady)) {
    printf("\nLa gr%cfica NO es conexa, por lo tanto:\n", 160);
    printf("- NO es Euleriana\n");
    printf("- NO es Unicursal\n");
    return;
  }

  int gradosImpares = 0;
  for (int i = 0; i < v; i++) {
    int grado = 0;
    for (int j = 0; j < v; j++) {
      grado += ady[i][j];
    }
    if (grado % 2 != 0)
      gradosImpares++;
  }

  printf("\nClasificaci%cn Euleriana/Unicursal:\n", 162);
  if (gradosImpares == 0) {
    printf("La gr%cfica es EULERIANA.\n", 160);
  } else if (gradosImpares == 2) {
    printf("La gr%cfica es UNICURSAL.\n", 160);
  } else {
    printf("La gr%cfica NO es Euleriana ni Unicursal.\n", 160);
  }
}

	int main() {
  int v, l, g;
  char reintentar;

  printf("Bienvenido al generador de matrices de graf%cs.\n", 162);
  printf("Creado por:\n");
  printf("Ruiz Garc%ca Emiliano\n", 161);
  printf("Moreno Vigueras Arturo Tadeo\n");
  printf("Rodr%cguez Rodr%cguez Erick Tadeo\n", 161, 161);

  do {
    Datos(&v, &l); // Solicita numero de vertices y lineas
    int aristas[MAX][2];

    // Seleccionar tipo de grafica
    do {
      printf("%cTipo de gr%cfica?\n1. Dirigida\n2. No dirigida\n", 168, 160);
      scanf("%d", &g);
    } while (g != 1 && g != 2);

    // Solicitar relaciones entre los vertices
    pedirRelaciones(l, v, aristas);

    // Matriz de incidencia
    matrizincidencia(v, l, aristas, g == 1);

    // Matriz de adyacencia
    int ady[MAX][MAX];
    matrizadyacencia(v, l, g == 1, aristas, ady);

    // Matriz de accesibilidad
    matrizaccesibilidad(v, ady);

    // Analisis de bucles
    if (tieneBucles(l, aristas)) {
      printf("La gr%cfica contiene al menos un bucle.\n", 160);
    } else {
      printf("La gr%cfica NO contiene bucles.\n", 160);
    }

    // Analisis de lineas paralelas
    if (tieneLineasParalelas(l, aristas, g == 1)) {
      printf("La gr%cfica contiene l%cneas paralelas.\n", 160, 161);
    } else {
      printf("La gr%cfica NO contiene l%cneas paralelas.\n", 160, 161);
    }

    // Lineas en serie (solo graficas no dirigidas)
    if (g == 2) {
      detectarLineasEnSerie(v, l, aristas);
    }

    // Informacion de vertices
    calcularGrados(v, g == 1, aristas, l);
    if (g == 2) {
      verificarAisladosColgantes(v, aristas, l);
    }

    // Clasificaciones
    clasificarSimpleGeneral(l, aristas, g == 1);
    clasificarCompletaRegular(v, ady, g == 1);
    clasificarNula(l);

    if (g == 2) { // No dirigida
      clasificarConectada(v, ady);
      clasificarArbol(v, l, ady);
      clasificarEulerianaUnicursal(v, ady);
    } else { // Dirigida
      clasificarSimetrica(v, ady);
      clasificarBalanceada(v, ady);
    }

    // V\xA2rtices iniciales y finales
    if (g == 1) {
      verificarInicialesFinales(v, ady);
    }

    printf("\n%cDesea ingresar otra gr%cfica? (s/n): ", 168, 160);
    scanf(" %c", &reintentar);

  } while (reintentar == 's' || reintentar == 'S');

  return 0;
}

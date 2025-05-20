#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <graphics.h>
#include <cmath>
#include <math.h>

#define MAX 100
#define PI 3.14159265



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
  int encontrado = 0;
  for (int i = 0; i < lol; i++) {
    if (aristas[i][0] == aristas[i][1]) {
      printf("Bucle detectado en el nodo V%c (arista L%d)\n", 'A' + aristas[i][0] - 1, i + 1);
      encontrado = 1;
    }  
      
  }
  if (encontrado !=1){
	  printf("No se encontraron bucles ;((");
}
  return encontrado;
}

int tieneLineasParalelas(int l, int aristas[][2], int esDirigida) {
  int encontrado = 0;
  for (int i = 0; i < l; i++) {
    for (int j = i + 1; j < l; j++) {
      if (esDirigida) {
        if (aristas[i][0] == aristas[j][0] && aristas[i][1] == aristas[j][1]) {
          printf("L%c neas paralelas detectadas entre V%c ? V%c (L%d y L%d)\n", 161,
                 'A' + aristas[i][0] - 1, 'A' + aristas[i][1] - 1, i + 1, j + 1);
          encontrado = 1;
        }
      } else {
        if ((aristas[i][0] == aristas[j][0] && aristas[i][1] == aristas[j][1]) ||
            (aristas[i][0] == aristas[j][1] && aristas[i][1] == aristas[j][0])) {
          printf("L%c neas paralelas detectadas entre V%c - V%c (L%d y L%d)\n", 161,
                 'A' + aristas[i][0] - 1, 'A' + aristas[i][1] - 1, i + 1, j + 1);
          encontrado = 1;
        }
      }
    }
  }
  if (encontrado !=1){
	  printf("No se encontraron lineas paralelas ;((");
}
  return encontrado;
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
    } else if (esDirigida==2) {//cambia si es dirigida
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
      if (bucle[j] && i == aristas[j][0] -1 && esDirigida==2) {
        printf("%c1 ", 241);  // Mostrará ±1 en consolas compatibles
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

  // Copia inicial de la matriz de adyacencia
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < v; j++) {
      MG[i][j] = XG[i][j];
      Potencia[i][j] = XG[i][j];
    }
  }

  // Usamos la fórmula (v^2 + v) / 2 para la cantidad de pasos
  int pasos = (v * v + v) / 2;

  for (int step = 2; step <= pasos; step++) {
    // temp = Potencia * XG
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

  // Impresión de matriz de accesibilidad
  printf("\nMatriz de Accesibilidad:\n   ");
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

//creacion Nodo
struct Nodo {
    int x, y;
    char nombre;
};

int contarRepetidas(int aristas[][2], int cantidad, int origen, int destino, int actual) {
    int conteo = 0;
    for (int i = 0; i < actual; i++) {
        if ((aristas[i][0] == origen && aristas[i][1] == destino) ||
            (aristas[i][0] == destino && aristas[i][1] == origen)) {
            conteo++;
        }
    }
    return conteo;
}

void dibujarGrafo(int v, int aristas[][2], int l, bool esDirigida) {
    struct Nodo nodo[MAX];

    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(WHITE);
    cleardevice();
    setcolor(BLACK);

    int cx = getmaxx() / 2;
    int cy = getmaxy() / 2;
    int radio = 200;

    for (int i = 0; i < v; i++) {
        float angulo = 2 * PI * i / v;
        nodo[i].x = cx + radio * cos(angulo);
        nodo[i].y = cy + radio * sin(angulo);
        nodo[i].nombre = 'A' + i;
    }

    for (int i = 0; i < l; i++) {
        int origen = aristas[i][0] - 1;
        int destino = aristas[i][1] - 1;

        int x1 = nodo[origen].x;
        int y1 = nodo[origen].y;
        int x2 = nodo[destino].x;
        int y2 = nodo[destino].y;

        if (origen == destino) {
            setcolor(RED);
            arc(x1, y1 - 20, 0, 360, 15);
            char etiqueta[10];
            sprintf(etiqueta, "L%d", i + 1);
            outtextxy(x1 + 10, y1 - 35, etiqueta);

            if (esDirigida) {
                int fx = x1;
                int fy = y1 - 35;
                float angle = atan2(y1 - fy, x1 - fx);
                int arrowSize = 10;
                line(fx, fy, fx - arrowSize * cos(angle - PI / 6), fy - arrowSize * sin(angle - PI / 6));
                line(fx, fy, fx - arrowSize * cos(angle + PI / 6), fy - arrowSize * sin(angle + PI / 6));
            }

            continue;
        }

        int rep = contarRepetidas(aristas, l, aristas[i][0], aristas[i][1], i);
        float angle = atan2(y2 - y1, x2 - x1);
        int offset = 20;

        float dx = x2 - x1;
        float dy = y2 - y1;
        float length = sqrt(dx * dx + dy * dy);
        float ox = -dy / length * 15 * rep;
        float oy = dx / length * 15 * rep;

        int x1_shift = x1 + ox;
        int y1_shift = y1 + oy;
        int x2_shift = x2 + ox - offset * cos(angle);
        int y2_shift = y2 + oy - offset * sin(angle);

        setcolor(BLUE);
        line(x1_shift, y1_shift, x2_shift, y2_shift);

        int xm = (x1_shift + x2_shift) / 2;
        int ym = (y1_shift + y2_shift) / 2;
        char etiqueta[10];
        sprintf(etiqueta, "L%d", i + 1);
        outtextxy(xm, ym, etiqueta);

        if (esDirigida) {
            int arrowSize = 10;
            int px = x2_shift;
            int py = y2_shift;
            line(px, py, px - arrowSize * cos(angle - PI / 6), py - arrowSize * sin(angle - PI / 6));
            line(px, py, px - arrowSize * cos(angle + PI / 6), py - arrowSize * sin(angle + PI / 6));
        }
    }

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, CYAN);
    for (int i = 0; i < v; i++) {
        fillellipse(nodo[i].x, nodo[i].y, 20, 20);
        char nombre[2] = {nodo[i].nombre, '\0'};
        outtextxy(nodo[i].x - 5, nodo[i].y - 5, nombre);
    }

    getch();
    closegraph();
}


void limpiarPantalla() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

int main() {
  int v, l, g;
  int aristas[MAX][2];
  int ady[MAX][MAX];
  char continuar;
  int opcion, subopcion;

  do {
    // Menú principal con créditos visibles siempre
    do {
      limpiarPantalla();
      printf("Bienvenido al generador de matrices de graf%cs.\n", 162);
      printf("Creado por:\n");
      printf("Ruiz Garc%ca Emiliano\n", 161);
      printf("Moreno Vigueras Arturo Tadeo\n");
      printf("Rodr%cguez Rodr%cguez Erick Tadeo\n\n", 161, 161);
      printf("Seleccione el tipo de gr%cfica:\n", 160);
      printf("1. Gr%cfica NO dirigida\n", 160);
      printf("2. Digr%cfica (dirigida)\n", 160);
      printf("3. Salir\n");
      scanf("%d", &g);
      if (g == 3) return 0;
    } while (g != 1 && g != 2);

    limpiarPantalla();
    Datos(&v, &l);
    pedirRelaciones(l, v, aristas);

    do {
      limpiarPantalla();
      printf("Seleccione una opci%cn para la gr%cfica:\n", 162, 160);
      printf("1. Ver matriz de incidencia\n");
      printf("2. Ver matriz de adyacencia\n");
      printf("3. Ver matriz de accesibilidad\n");
      printf("4. Ver bucles\n");
      printf("5. Ver l%cneas paralelas\n", 161);
      if (g == 2) {
        printf("6. Ver grados de los v%crtices\n", 130);
        printf("7. Ver clasificaciones (sim%ctrica, balanceada, etc.)\n", 130);
        printf("8. Ver v%crtices iniciales/finales\n", 130);
        printf("9. Ver visualizaci%cn del grafo\n", 162);
        printf("10. Editar datos ingresados\n");
        printf("11. Volver al men%c principal\n", 163);
        printf("12. Salir\n");
      } else {
        printf("6. Ver l%cneas en serie\n", 161);
        printf("7. Ver grados de los v%crtices\n", 130);
        printf("8. Ver v%crtices aislados/colgantes\n", 130);
        printf("9. Ver clasificaciones (simple, arbol, etc.)\n");
        printf("10. Ver visualizaci%cn del grafo\n", 162);
        printf("11. Editar datos ingresados\n");
        printf("12. Volver al men%c principal\n", 163);
        printf("13. Salir\n");
      }

      scanf("%d", &opcion);

      if ((g == 2 && opcion == 12) || (g == 1 && opcion == 13)) return 0;
      if ((g == 2 && opcion == 11) || (g == 1 && opcion == 12)) break;

      switch (opcion) {
        case 1:
          limpiarPantalla();
          matrizincidencia(v, l, aristas, g);
          break;
        case 2:
          limpiarPantalla();
          matrizadyacencia(v, l, g == 1, aristas, ady);
          break;
        case 3:
          limpiarPantalla();
          matrizaccesibilidad(v, ady);
          break;
        case 4:
          limpiarPantalla();
          tieneBucles(l, aristas);
          break;
        case 5:
          limpiarPantalla();
          tieneLineasParalelas(l, aristas, g == 1);
          break;
        case 6:
          limpiarPantalla();
          if (g == 1)
            detectarLineasEnSerie(v, l, aristas);
          else
            calcularGrados(v, 1, aristas, l);
          break;
        case 7:
          limpiarPantalla();
          if (g == 1)
            calcularGrados(v, 0, aristas, l);
          else
            clasificarSimetrica(v, ady), clasificarBalanceada(v, ady);
          break;
        case 8:
          limpiarPantalla();
          if (g == 1)
            verificarAisladosColgantes(v, aristas, l);
          else
            verificarInicialesFinales(v, ady);
          break;
        case 9:
          limpiarPantalla();
          if (g == 1) {
            clasificarSimpleGeneral(l, aristas, 0);
            clasificarCompletaRegular(v, ady, 0);
            clasificarNula(l);
            clasificarConectada(v, ady);
            clasificarArbol(v, l, ady);
            clasificarEulerianaUnicursal(v, ady);
          } else {
            dibujarGrafo(v, aristas, l, true);
          }
          break;
        case 10:
          limpiarPantalla();
          if (g == 1)
            dibujarGrafo(v, aristas, l, false);
          else {
            limpiarPantalla();
            printf("Opciones para editar:\n");
            printf("1. Cambiar n%cmero de v%crtices\n", 163, 130);
            printf("2. Cambiar n%cmero de l%cneas\n", 163, 161);
            printf("3. Cambiar conexiones (aristas)\n");
            printf("4. Cancelar\n");
            scanf("%d", &subopcion);
            switch (subopcion) {
              case 1:
              case 2:
                limpiarPantalla();
                Datos(&v, &l);
                break;
              case 3:
                limpiarPantalla();
                pedirRelaciones(l, v, aristas);
                break;
              case 4:
                break;
            }
          }
          break;
        case 11:
          limpiarPantalla();
          if (g == 1) {
            printf("Opciones para editar:\n");
            printf("1. Cambiar n%cmero de v%crtices\n", 163, 130);
            printf("2. Cambiar n%cmero de l%cneas\n", 163, 161);
            printf("3. Cambiar conexiones (aristas)\n");
            printf("4. Cancelar\n");
            scanf("%d", &subopcion);
            switch (subopcion) {
              case 1:
              case 2:
                limpiarPantalla();
                Datos(&v, &l);
                break;
              case 3:
                limpiarPantalla();
                pedirRelaciones(l, v, aristas);
                break;
              case 4:
                break;
            }
          }
          break;
        default:
          printf("Opci%cn no v%clida.\n", 162, 160);
          break;
      }

      printf("\n%cDesea realizar otra acci%cn sobre esta gr%cfica? (s/n): ", 168, 162, 160);
      scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

  } while (1);

  return 0;
}

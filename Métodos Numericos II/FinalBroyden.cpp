#include <stdio.h>
#include <math.h>
#include <locale.h>
#define MAX_N 3  

int N, i, j, k, iter, MAX_ITER;
double B[3][3], fx[3], s[3], y[3], x_new[3], fx_new[3];
double norm, error_rel, TOL;
// Definición de los sistemas de ecuaciones
void system_1(double x[], double f[]) {
    f[0] = x[0] * x[0] + x[0] * x[1] + 2 * x[1] * x[1] - 5;
    f[1] = 5 * x[1] - 2 * x[0] * x[1] * x[1] + 3;
}

void system_2(double x[], double f[]) {
    f[0] = x[0] * x[0] - 3 * x[1] * x[1]- 10;
    f[1] = 2* x[1] * x[1] -3* x[0] * x[1] + 1;
}

void system_3(double x[], double f[]) {
    f[0] = 3 * x[0] * x[0] + x[1] * x[1] - 8 * x[1] + 2 * x[2] * x[2] - 5;
    f[1] = -2 * x[0]*x[0] - 12 * x[0] + x[1] * x[1]-3*x[2]*x[2] + 10;
    f[2] = -x[0] + 2 * x[1] + 5 * x[2];
}

void system_4(double x[], double f[]) {
    f[0] = x[0] * x[0] + x[1] * x[1] - 2 * x[2] + 3;
    f[1] = x[0] + x[1]+x[2] - 5;
    f[2] = x[0] * x[0] - x[1] * x[1] + x[2] * x[2] - 9;
}

// Aproximación de la matriz Jacobiana
void jacobian_approx(double x[], double B[MAX_N][MAX_N], void (*func)(double[], double[])) {
    double fx[MAX_N], fxh[MAX_N], h = 1e-5;
    func(x, fx);

    for (j = 0; j < N; j++) {
        double xh[MAX_N];
        for (i = 0; i < N; i++) xh[i] = x[i];
        xh[j] += h;
        func(xh, fxh);
        
        for (i = 0; i < N; i++) 
            B[i][j] = (fxh[i] - fx[i]) / h;
    }
}

// Resolver sistema lineal por eliminación de Gauss con pivoteo
void solve_linear_system(double B[MAX_N][MAX_N], double fx[MAX_N], double s[MAX_N]) {
    double temp[MAX_N];

    for (i = 0; i < N; i++) temp[i] = -fx[i];

    for (i = 0; i < N; i++) {
        int pivot = i;
        for (k = i + 1; k < N; k++)
            if (fabs(B[k][i]) > fabs(B[pivot][i])) pivot = k;

        if (pivot != i) {
            for (j = 0; j < N; j++) {
                double temp_val = B[i][j];
                B[i][j] = B[pivot][j];
                B[pivot][j] = temp_val;
            }
            double temp_b = temp[i];
            temp[i] = temp[pivot];
            temp[pivot] = temp_b;
        }

        for (k = i + 1; k < N; k++) {
            double factor = B[k][i] / B[i][i];
            for (j = i; j < N; j++)
                B[k][j] -= factor * B[i][j];
            temp[k] -= factor * temp[i];
        }
    }

    for (i = N - 1; i >= 0; i--) {
        s[i] = temp[i];
        for (j = i + 1; j < N; j++)
            s[i] -= B[i][j] * s[j];
        s[i] /= B[i][i];
    }
}

// Método de Broyden con solicitud de puntos iniciales
void broyden(double x[], void (*func)(double[], double[])) {
    double B[MAX_N][MAX_N], fx[MAX_N], s[MAX_N], y[MAX_N], x_new[MAX_N], fx_new[MAX_N];
    double norm, error_rel;
    
    jacobian_approx(x, B, func);
    
    for (iter = 0; iter < MAX_ITER; iter++) {
        func(x, fx);
        solve_linear_system(B, fx, s);

        for (i = 0; i < N; i++) 
            x_new[i] = x[i] + s[i];

        func(x_new, fx_new);
        
        for (i = 0; i < N; i++) 
            y[i] = fx_new[i] - fx[i];

        norm = 0;
        for (i = 0; i < N; i++) 
            norm += fx_new[i] * fx_new[i];
        error_rel = sqrt(norm);

        printf("\nIteraci%cn %d:\n", 162, iter + 1);
        printf("x = (");
        for (i = 0; i < N; i++) printf("%.6f ", x_new[i]);
        printf(")\n");
        printf("Error relativo: %.6e\n", error_rel);

        if (error_rel < TOL) {
            printf("\nSoluci%cn encontrada en %d iteraciones: (", 162, iter + 1);
            for (i = 0; i < N; i++) printf("%.6f ", x_new[i]);
            printf(")\n");
            return;
        }

        double norm_s = 0;
        for (i = 0; i < N; i++) 
            norm_s += s[i] * s[i];
        norm_s = sqrt(norm_s);

        if (norm_s > 1e-10) {
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++) {
                    B[i][j] += ((y[i] - B[i][j] * s[j]) * s[j]) / (norm_s * norm_s);
                }
            }
        }
        
        for (i = 0; i < N; i++) 
            x[i] = x_new[i];
    }
    
    printf("No se encontr%c soluci%cn despu%cs de %d iteraciones.\nPor lo que diverge en %lf %lf", 162, 162, 130, MAX_ITER, x[0], x[1]);
}

//Broyden para sistemas de 3 variables
void broyden_3x3(double x[], void (*func)(double[], double[])) {
    double B[3][3], fx[3], s[3], y[3], x_new[3], fx_new[3], Bs[3], denom;
    double norm, error_rel;

    jacobian_approx(x, B, func);  // Aproximación inicial de la Jacobiana

    for (iter = 0; iter < MAX_ITER; iter++) {
        func(x, fx);  // Evaluar F(x)
        solve_linear_system(B, fx, s);  // Resolver B * s = -F(x)

        // Actualizar x
        for (i = 0; i < 3; i++) 
            x_new[i] = x[i] + s[i];

        func(x_new, fx_new);  // Evaluar F(x_new)

        // Calcular y = F(x_new) - F(x)
        for (i = 0; i < 3; i++) 
            y[i] = fx_new[i] - fx[i];

        // Cálculo del error
        norm = 0;
        for (i = 0; i < 3; i++) 
            norm += fx_new[i] * fx_new[i];
        error_rel = sqrt(norm);

        printf("\nIteraci%cn %d:\n", 162, iter + 1);
        printf("x = (");
        for (i = 0; i < 3; i++) printf("%.6f ", x_new[i]);
        printf(")\nError relativo: %.6e\n", error_rel);

        if (error_rel < TOL) {
            printf("\nSoluci%cn encontrada en %d iteraciones: (", 162, iter + 1);
            for (i = 0; i < 3; i++) printf("%.6f ", x_new[i]);
            printf(")\n");
            return;
        }

        // Cálculo de Bs = B * s
        for (i = 0; i < 3; i++) {
            Bs[i] = 0;
            for (j = 0; j < 3; j++) {
                Bs[i] += B[i][j] * s[j];
            }
        }

        // Denominador s^T * s
        denom = 0;
        for (i = 0; i < 3; i++) 
            denom += s[i] * s[i];

        if (fabs(denom) > 1e-10) {  // Evitar división por cero
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    B[i][j] += ((y[i] - Bs[i]) * s[j]) / denom;
                }
            }
        }

        // Actualizar x para la siguiente iteración
        for (i = 0; i < 3; i++) 
            x[i] = x_new[i];
    }

    printf("No se encontr%c soluci%cn despu%cs de %d iteraciones.\nPor lo que diverge en %lf %lf %lf", 162, 162, 130, MAX_ITER, x[0], x[1], x[2]);

}


// Menú para seleccionar el sistema y pedir los valores iniciales
int main() {
	
    int choice;
    char repeat;
    double x[MAX_N];

    do {
    	printf("\tM%ctodo de Broyden\n", 130);
    	printf("\tAlumnos:\n");
    	printf("\tMoreno Vigueras Arturo Tadeo\n");
    	printf("\tRodr%cguez Rodr%cguez Erick Tadeo\n", 161, 161);
    	printf("\tRuiz Garcia Emiliano\n\n");
        printf("\nSeleccione un sistema de ecuaciones:\n");
        printf("1. Sistema 1: \n");
        printf("   f1(x, y) = x^2 + x*y + 2*y^2 - 5\n");
        printf("   f2(x, y) = 5*y - 2*x*y^2 + 3\n\n");

        printf("2. Sistema 2: \n");
        printf("   f1(x, y) = x^2 - 3*y^2 - 10\n");
        printf("   f2(x, y) = 2*y^2 - 3*x*y + 1\n\n");

        printf("3. Sistema 3: \n");
        printf("   f1(x, y, z) = 3*x^2 + y^2 - 8*y + 2*z^2 - 5\n");
        printf("   f2(x, y, z) = -2*x^2 - 12*x + y^2 - 3*z^2 + 10\n");
        printf("   f3(x, y, z) = -x + 2*y + 5*z\n\n");

        printf("4. Sistema 4: \n");
        printf("   f1(x, y, z) = x^2 + y^2 - 2*z + 3\n");
        printf("   f2(x, y, z) = x + y + z - 5\n");
        printf("   f3(x, y, z) = x^2 - y^2 + z^2 - 9\n\n");

        printf("Opci%cn: ", 162);
        scanf("%d", &choice);
		printf("Ingrese el n%cmero m%cximo de iteraciones: ", 163, 160);
		scanf("%d", &MAX_ITER);
		printf("Ingrese la tolerancia deseada: ");
		scanf("%lf", &TOL);
        switch (choice) {
            case 1: 
                N = 2;
                printf("Ingrese los valores iniciales (x0, x1): ");
                scanf("%lf %lf", &x[0], &x[1]);
                broyden(x, system_1);
                break;
            case 2: 
                N = 2;
                printf("Ingrese los valores iniciales (x0, x1): ");
                scanf("%lf %lf", &x[0], &x[1]);
                broyden(x, system_2);
                break;
            case 3: 
                N = 3;
                printf("Ingrese los valores iniciales (x0, x1, x2): ");
                scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
                broyden_3x3(x, system_3);
                break;
            case 4: 
                N = 3;
                printf("Ingrese los valores iniciales (x0, x1, x2): ");
                scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
                broyden_3x3(x, system_4);
                break;
            default: 
                printf("Opción inválida.\n");
        }

    // LIMPIAR BUFFER ANTES DE PEDIR EL SIGUIENTE CARÁCTER
        while (getchar() != '\n');  

        printf("\nDesea resolver otro sistema? (s/n): ");
        scanf(" %c", &repeat);

    } while (repeat == 's' || repeat == 'S');
    return 0;
}

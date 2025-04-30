import os

# ==============================
# 0. Limpiar consola
# ==============================
def limpiar_pantalla():
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

# ==============================
# 1. Captura de puntos
# ==============================
def captura_puntos():
    while True:
        try:
            n = int(input("Ingrese el número de puntos: "))
            if n <= 0:
                print("Debe ingresar al menos 1 punto.")
                continue
            break
        except ValueError:
            print("Por favor ingrese un número entero válido.")

    puntos = []
    for i in range(n):
        while True:
            try:
                x = float(input(f"Ingrese x[{i}]: "))
                y = float(input(f"Ingrese y[{i}]: "))
                puntos.append((x, y))
                break
            except ValueError:
                print("Por favor ingrese valores numéricos válidos.")
    return puntos

# ==============================
# 2. Modificar puntos
# ==============================
def modificar_puntos(puntos):
    if not puntos:
        print("No hay puntos para modificar.")
        return puntos

    while True:
        print("\nPuntos actuales:")
        for idx, (x, y) in enumerate(puntos):
            print(f"{idx}: ({x}, {y})")
        resp = input("¿Son correctos los datos? (s/n): ").lower()
        if resp == 's':
            break
        else:
            try:
                idx_mod = int(input("Índice del punto a modificar: "))
                if idx_mod < 0 or idx_mod >= len(puntos):
                    print(f"Índice inválido. Ingrese un número entre 0 y {len(puntos)-1}.")
                    continue
                new_x = float(input(f"Nuevo x[{idx_mod}]: "))
                new_y = float(input(f"Nuevo y[{idx_mod}]: "))
                puntos[idx_mod] = (new_x, new_y)
            except ValueError:
                print("Entrada inválida. Intente nuevamente.")
    return puntos

# ==============================
# 3. Resolver sistema lineal
# ==============================
def resolver_sistema(A, b):
    n = len(b)
    for i in range(n):
        max_row = max(range(i, n), key=lambda r: abs(A[r][i]))
        if i != max_row:
            A[i], A[max_row] = A[max_row], A[i]
            b[i], b[max_row] = b[max_row], b[i]

        pivote = A[i][i]
        if pivote == 0:
            raise ValueError("Sistema no tiene solución única.")
        
        A[i] = [aij / pivote for aij in A[i]]
        b[i] /= pivote

        for j in range(i+1, n):
            factor = A[j][i]
            A[j] = [aij - factor * aii for aij, aii in zip(A[j], A[i])]
            b[j] -= factor * b[i]

    x = [0] * n
    for i in range(n-1, -1, -1):
        x[i] = b[i] - sum(A[i][j]*x[j] for j in range(i+1, n))
    return x

# ==============================
# 4. Construir Spline Cúbico
# ==============================
def spline_cubico(puntos):
    n = len(puntos) - 1
    h = [(puntos[i+1][0] - puntos[i][0]) for i in range(n)]

    A = [[0]*(n-1) for _ in range(n-1)]
    b = [0]*(n-1)

    for i in range(1, n):
        A[i-1][i-1] = 2 * (h[i-1] + h[i])
        if i-2 >= 0:
            A[i-1][i-2] = h[i-1]
        if i < n-1:
            A[i-1][i] = h[i]
        b[i-1] = 6 * ((puntos[i+1][1] - puntos[i][1]) / h[i] - (puntos[i][1] - puntos[i-1][1]) / h[i-1])

    c = [0] + resolver_sistema(A, b) + [0]

    splines = []
    for i in range(n):
        a = puntos[i][1]
        b_coef = (puntos[i+1][1] - puntos[i][1]) / h[i] - h[i]*(2*c[i] + c[i+1])/6
        d = (c[i+1] - c[i]) / (6*h[i])
        spline = (a, b_coef, c[i]/2, d, puntos[i][0], puntos[i+1][0])
        splines.append(spline)
    return splines

# ==============================
# 5. Mostrar polinomios spline
# ==============================
def mostrar_splines(splines):
    for idx, (a, b, c, d, xi, xf) in enumerate(splines):
        print(f"\nSpline {idx}:")
        print(f"S(x) = {a:.4f} + {b:.4f}(x - {xi:.4f}) + {c:.4f}(x - {xi:.4f})² + {d:.4f}(x - {xi:.4f})³")
        print(f"Intervalo: [{xi}, {xf}]")

# ==============================
# 6A. Graficar profesional con matplotlib
# ==============================
def graficar_splines_moderno(splines):
    import matplotlib.pyplot as plt
    import numpy as np

    plt.figure(figsize=(8, 6))

    for spline in splines:
        a, b_coef, c_coef, d_coef, xi, xf = spline
        x_vals = np.linspace(xi, xf, 100)
        y_vals = a + b_coef*(x_vals - xi) + c_coef*(x_vals - xi)**2 + d_coef*(x_vals - xi)**3
        plt.plot(x_vals, y_vals, label=f"[{xi:.2f}, {xf:.2f}]")

    plt.title('Splines Cúbicos Naturales')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    plt.legend()
    plt.show()

# ==============================
# 6B. Graficar en modo texto (ASCII)
# ==============================
def graficar_splines_texto(splines):
    ancho = 60
    alto = 20
    canvas = [[' ' for _ in range(ancho)] for _ in range(alto)]

    min_x = min(s[4] for s in splines)
    max_x = max(s[5] for s in splines)
    min_y = min(s[0] for s in splines)
    max_y = max(s[0] for s in splines)

    for spline in splines:
        a, b, c, d, xi, xf = spline
        x = xi
        while x <= xf:
            dx = x - xi
            y = a + b*dx + c*dx**2 + d*dx**3
            ix = int((x - min_x) / (max_x - min_x) * (ancho - 1))
            iy = int((y - min_y) / (max_y - min_y) * (alto - 1))
            iy = (alto - 1) - iy
            if 0 <= ix < ancho and 0 <= iy < alto:
                canvas[iy][ix] = '*'
            x += (xf - xi) / 100

    print("\nGráfica aproximada:")
    for row in canvas:
        print(''.join(row))

# ==============================
# 7. Programa principal
# ==============================
def main():
    while True:
        limpiar_pantalla()

        puntos = captura_puntos()
        puntos = modificar_puntos(puntos)
        puntos.sort()
        limpiar_pantalla()

        splines = spline_cubico(puntos)
        mostrar_splines(splines)

        if input("\n¿Desea graficar los splines? (s/n): ").lower() == 's':
            modo = input("Seleccione modo de gráfica: (1) Profesional o (2) Texto: ")
            if modo == '1':
                graficar_splines_moderno(splines)
            elif modo == '2':
                graficar_splines_texto(splines)
            else:
                print("Opción inválida, no se graficó.")

        if input("\n¿Desea realizar otro ajuste? (s/n): ").lower() != 's':
            limpiar_pantalla()
            break
        limpiar_pantalla()

# ==============================
# Ejecutar programa
# ==============================
if __name__ == "__main__":
    main()

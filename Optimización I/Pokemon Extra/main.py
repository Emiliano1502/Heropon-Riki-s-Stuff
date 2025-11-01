import pygame
import random
import json

pygame.init()

# --- CONFIGURACIÓN ---
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pokémon Lineal")

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (50, 150, 255)
RED = (255, 80, 80)
FONT = pygame.font.Font(None, 32)

# --- CARGA DE IMÁGENES ---
player_img = pygame.image.load("assets/player.png")
wild_img = pygame.image.load("assets/pokemon.png")

# --- ESTADO DEL JUGADOR ---
player_rect = player_img.get_rect(center=(WIDTH//2, HEIGHT//2))
speed = 5
vidas = 3
pokemons_capturados = []

# --- CARGA DE PREGUNTAS ---
with open("data/preguntas.json", "r", encoding="utf-8") as f:
    preguntas = json.load(f)

# --- FUNCIONES AUXILIARES ---

def mostrar_texto(texto, y, color=BLACK):
    render = FONT.render(texto, True, color)
    screen.blit(render, (50, y))

def batalla():
    """Pantalla de batalla con pregunta"""
    pregunta = random.choice(preguntas)
    seleccion = 0
    en_batalla = True
    resultado = None

    while en_batalla:
        screen.fill(WHITE)

        mostrar_texto("¡Un Pokémon salvaje apareció!", 40, RED)
        screen.blit(wild_img, (WIDTH//2 - 32, HEIGHT//2 - 100))

        mostrar_texto(pregunta["pregunta"], 150)
        y = 200
        for i, op in enumerate(pregunta["opciones"]):
            color = BLUE if i == seleccion else BLACK
            mostrar_texto(f"{i+1}. {op}", y, color)
            y += 40

        pygame.display.flip()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    seleccion = (seleccion - 1) % len(pregunta["opciones"])
                elif event.key == pygame.K_DOWN:
                    seleccion = (seleccion + 1) % len(pregunta["opciones"])
                elif event.key == pygame.K_RETURN:
                    respuesta = pregunta["opciones"][seleccion]
                    if respuesta == pregunta["respuesta"]:
                        resultado = "¡Correcto! Capturaste al Pokémon."
                        pokemons_capturados.append("Pokémon salvaje")
                    else:
                        resultado = "Respuesta incorrecta. ¡Tu Pokémon recibió daño!"
                        nonlocal vidas
                        vidas -= 1
                    en_batalla = False

    # Pantalla de resultado
    screen.fill(WHITE)
    mostrar_texto(resultado, 200, RED if "incorrecta" in resultado else BLUE)
    mostrar_texto(f"Vidas restantes: {vidas}", 260)
    mostrar_texto("Presiona ENTER para continuar...", 320)
    pygame.display.flip()

    esperando = True
    while esperando:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
                esperando = False


# --- BUCLE PRINCIPAL ---
clock = pygame.time.Clock()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        player_rect.y -= speed
    if keys[pygame.K_s]:
        player_rect.y += speed
    if keys[pygame.K_a]:
        player_rect.x -= speed
    if keys[pygame.K_d]:
        player_rect.x += speed

    # Aparición aleatoria de Pokémon salvaje
    if random.randint(0, 200) == 1:
        batalla()

    # Verificar si el jugador perdió
    if vidas <= 0:
        screen.fill(WHITE)
        mostrar_texto("Te has quedado sin vidas...", 250, RED)
        mostrar_texto("Fin del juego", 300)
        pygame.display.flip()
        pygame.time.wait(3000)
        break

    # Dibujar en pantalla
    screen.fill(WHITE)
    screen.blit(player_img, player_rect)
    mostrar_texto(f"Vidas: {vidas}", 10)
    mostrar_texto(f"Pokémon capturados: {len(pokemons_capturados)}", 40)
    pygame.display.flip()
    clock.tick(60)

pygame.quit()

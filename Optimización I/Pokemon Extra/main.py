import pygame
import random
import json

pygame.init()

# Configuración de ventana
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pokémon Lineal")

# Colores
WHITE = (255, 255, 255)

# Carga de sprites
player = pygame.image.load("assets/player.png")
player_rect = player.get_rect(center=(WIDTH//2, HEIGHT//2))

clock = pygame.time.Clock()
vel = 5
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Movimiento
    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        player_rect.y -= vel
    if keys[pygame.K_s]:
        player_rect.y += vel
    if keys[pygame.K_a]:
        player_rect.x -= vel
    if keys[pygame.K_d]:
        player_rect.x += vel

    # Aparición aleatoria de Pokémon
    if random.randint(0, 300) == 1:  # Probabilidad pequeña
        print("¡Un Pokémon salvaje ha aparecido!")

    # Renderizado
    screen.fill(WHITE)
    screen.blit(player, player_rect)
    pygame.display.flip()
    clock.tick(60)

pygame.quit()

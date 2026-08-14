#include "raylib.h"

int main() {
    // Initialise Raylib avec une fenêtre de 800x450
    InitWindow(800, 450, "Mon Moteur C++ - Test Raylib");

    // Définit le FPS cible à 60
    SetTargetFPS(60);

    // La Game Loop de Raylib (tant que la fenêtre n'est pas fermée)
    while (!WindowShouldClose()) {
        
        // Début du dessin
        BeginDrawing();
        
        // Efface l'écran avec une couleur (ici, gris foncé)
        ClearBackground(DARKGRAY);
        
        // Affiche un texte
        DrawText("Bravo ! Raylib est installe !", 250, 200, 20, RAYWHITE);
        
        // Fin du dessin
        EndDrawing();
    }

    // Ferme proprement la fenêtre
    CloseWindow();

    return 0;
}
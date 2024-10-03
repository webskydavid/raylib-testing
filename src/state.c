#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

// Global app state instance
AppState appState;

// Function to initialize the application state
void initAppState(const char *userName)
{
    appState.currentLevel = 1;
    appState.score = 0;
    appState.health = 100.0f;
    strncpy(appState.userName, userName, sizeof(appState.userName) - 1);
    appState.userName[sizeof(appState.userName) - 1] = '\0'; // Ensure null termination
}

// Function to update the score
void updateScore(int points)
{
    appState.score += points;
    printf("Score updated! Current score: %d\n", appState.score);
}

// Function to change the level
void changeLevel(int level)
{
    appState.currentLevel = level;
    printf("Level changed! Current level: %d\n", appState.currentLevel);
}

// Function to reduce health
void reduceHealth(float damage)
{
    appState.health -= damage;
    if (appState.health < 0)
        appState.health = 0;
    printf("Health reduced! Current health: %.2f\n", appState.health);
}

// Function to display the current state
void displayState()
{
    printf("Current App State:\n");
    printf("User: %s\n", appState.userName);
    printf("Level: %d\n", appState.currentLevel);
    printf("Score: %d\n", appState.score);
    printf("Health: %.2f\n", appState.health);
}

// Function to save state to a file
void saveStateToFile(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    fwrite(&appState, sizeof(AppState), 1, file);
    fclose(file);
    printf("App state saved to %s\n", filename);
}

// Function to load state from a file
void loadStateFromFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    fread(&appState, sizeof(AppState), 1, file);
    fclose(file);
    printf("App state loaded from %s\n", filename);
}

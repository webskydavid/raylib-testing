#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

// Define the state of the application
typedef struct {
    int currentLevel;
    int score;
    char userName[50];
    float health;
} AppState;

// Declare the global app state
extern AppState appState;

// Function declarations
void initAppState(const char *userName);
void updateScore(int points);
void changeLevel(int level);
void reduceHealth(float damage);
void displayState();
void saveStateToFile(const char *filename);
void loadStateFromFile(const char *filename);

#endif // STATE_MANAGER_H

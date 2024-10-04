#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

typedef struct {
    int currentLevel;
    int score;
    char userName[50];
    float health;
} AppState;

extern AppState appState;

void InitAppState(const char *userName);
void UpdateScore(int points);

#endif

#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AppState appState;

void InitAppState(const char *userName) {
    appState.score = 0;
}

void UpdateScore(int points) {
    appState.score += points;
    printf("Score updated! Current score: %d\n", appState.score);
}

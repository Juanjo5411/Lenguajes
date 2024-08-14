#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura que representa una transición en el DFA
typedef struct {
    char *from_state;  // Estado de partida
    char *symbol;      // Símbolo del alfabeto
    char *to_state;    // Estado de llegada
} Transition;

// Estructura que representa el DFA completo
typedef struct {
    char **states;            // Conjunto de estados
    char **alphabet;          // Alfabeto
    Transition *transitions;  // Funciones de transición
    char *initial_state;      // Estado inicial
    char **accepting_states;  // Estados de aceptación
    int num_states;           // Número de estados
    int num_alphabet;         // Número de símbolos en el alfabeto
    int num_transitions;      // Número de transiciones
    int num_accepting_states; // Número de estados de aceptación
} DFA;

// Función para dividir una línea en un arreglo de cadenas separadas por comas
char** split_line(char *line, int *count) {
    char **result = NULL;
    char *token = strtok(line, ",");
    int spaces = 0;

    // Divide la línea por comas y almacena los tokens en un arreglo
    while (token) {
        char **temp = realloc(result, sizeof(char*) * (spaces + 1));
        if (!temp) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }
        result = temp;
        result[spaces] = strdup(token);
        if (!result[spaces]) {
            perror("Error al duplicar cadena");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, ",");
        spaces++;
    }
    *count = spaces;
    return result;
}

// Función para liberar la memoria utilizada por el DFA
void free_dfa(DFA *dfa) {
    if (!dfa) return;

    for (int i = 0; i < dfa->num_states; i++) {
        free(dfa->states[i]);
    }
    free(dfa->states);

    for (int i = 0; i < dfa->num_alphabet; i++) {
        free(dfa->alphabet[i]);
    }
    free(dfa->alphabet);

    for (int i = 0; i < dfa->num_accepting_states; i++) {
        free(dfa->accepting_states[i]);
    }
    free(dfa->accepting_states);

    for (int i = 0; i < dfa->num_transitions; i++) {
        free(dfa->transitions[i].from_state);
        free(dfa->transitions[i].symbol);
        free(dfa->transitions[i].to_state);
    }
    free(dfa->transitions);

    free(dfa->initial_state);
    free(dfa);
}

// Función para leer el DFA desde un archivo de configuración
DFA* read_dfa_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    DFA *dfa = malloc(sizeof(DFA));
    if (!dfa) {
        perror("Error al asignar memoria para el DFA");
        exit(EXIT_FAILURE);
    }

    char line[256];
    
    // Leer estados (Q)
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Eliminar carácter de nueva línea
        dfa->states = split_line(line, &dfa->num_states);
    }

    // Leer alfabeto (Σ)
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Eliminar carácter de nueva línea
        dfa->alphabet = split_line(line, &dfa->num_alphabet);
    }

    // Leer estado inicial (q0)
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Eliminar carácter de nueva línea
        dfa->initial_state = strdup(line);
        if (!dfa->initial_state) {
            perror("Error al duplicar cadena");
            exit(EXIT_FAILURE);
        }
    }

    // Leer estados de aceptación (F)
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Eliminar carácter de nueva línea
        dfa->accepting_states = split_line(line, &dfa->num_accepting_states);
    }

    // Leer transiciones
    dfa->transitions = NULL;
    dfa->num_transitions = 0;
    while (fgets(line, sizeof(line), file)) {
        char *from_state = strtok(line, ",");
        char *symbol = strtok(NULL, ",");
        char *to_state = strtok(NULL, ",");
        if (to_state) {
            to_state[strcspn(to_state, "\n")] = 0; // Eliminar carácter de nueva línea
        }

        dfa->transitions = realloc(dfa->transitions, sizeof(Transition) * (dfa->num_transitions + 1));
        if (!dfa->transitions) {
            perror("Error al asignar memoria para transiciones");
            exit(EXIT_FAILURE);
        }

        dfa->transitions[dfa->num_transitions].from_state = strdup(from_state);
        dfa->transitions[dfa->num_transitions].symbol = strdup(symbol);
        dfa->transitions[dfa->num_transitions].to_state = strdup(to_state);
        if (!dfa->transitions[dfa->num_transitions].from_state ||
            !dfa->transitions[dfa->num_transitions].symbol ||
            !dfa->transitions[dfa->num_transitions].to_state) {
            perror("Error al duplicar cadena");
            exit(EXIT_FAILURE);
        }
        dfa->num_transitions++;
    }

    fclose(file);
    return dfa;
}

// Función de transición que devuelve el siguiente estado dado un estado actual y un símbolo
char* transition(DFA *dfa, char *current_state, char *symbol) {
    for (int i = 0; i < dfa->num_transitions; i++) {
        if (strcmp(dfa->transitions[i].from_state, current_state) == 0 &&
            strcmp(dfa->transitions[i].symbol, symbol) == 0) {
            return dfa->transitions[i].to_state;
        }
    }
    return NULL; // Si no se encuentra una transición válida
}

// Verifica si un estado es de aceptación
int is_accepting_state(DFA *dfa, char *state) {
    for (int i = 0; i < dfa->num_accepting_states; i++) {
        if (strcmp(dfa->accepting_states[i], state) == 0) {
            return 1;
        }
    }
    return 0;
}

// Procesa una cadena de entrada a través del DFA
void process_string(DFA *dfa, const char *input_string) {
    char *current_state = dfa->initial_state;
    char symbol[2] = {0};

    // Itera sobre cada símbolo de la cadena de entrada
    for (int i = 0; input_string[i] != '\0'; i++) {
        symbol[0] = input_string[i];
        printf("Current state: %s, Symbol: %s\n", current_state, symbol); // Depuración
        char *next_state = transition(dfa, current_state, symbol);
        if (!next_state) {
            printf("Error: Transición no definida para el estado '%s' con el símbolo '%s'.\n", current_state, symbol);
            return;
        }
        current_state = next_state;
        printf("Next state: %s\n", current_state); // Depuración
    }

    // Verifica si el estado final es un estado de aceptación
    if (is_accepting_state(dfa, current_state)) {
        printf("Resultado: La cadena fue aceptada.\n");
    } else {
        printf("Resultado: La cadena fue rechazada.\n");
    }
}

// Función principal del programa
int main() {
    char filepath[256];
    char input_string[256];
    // Solicita la ruta del archivo de configuración al usuario
    printf("Ingrese la ruta del archivo de configuración: ");
    scanf("%255s", filepath);
    // Lee el DFA del archivo especificado
    DFA *dfa = read_dfa_from_file(filepath);
    // Solicita la cadena de entrada al usuario
    printf("Ingrese la cadena de entrada: ");
    scanf("%255s", input_string);
    // Procesa la cadena a través del DFA
    process_string(dfa, input_string);
    // Liberar memoria asignada
    free_dfa(dfa);
    return 0;
}

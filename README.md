# Implementación de un Autómata Finito Determinista (AFD) en C
**Nombre:** Juan José Márquez Villarreal

Este programa implementa un Autómata Finito Determinista (AFD) configurable mediante un archivo de texto. El programa lee la configuración del AFD desde un archivo, procesa una cadena de entrada y determinar si dicha cadena es aceptada o rechazada por el autómata según la configuración definida.

## Funcionamiento del Programa

### 1. Lectura del Archivo de Configuración

- El programa comienza solicitando al usuario la ruta del archivo de configuración del AFD.
- Utiliza funciones estándar de C para abrir y leer el contenido del archivo.
- El archivo debe contener, en orden, la lista de estados, el alfabeto, el estado inicial, los estados de aceptación, y las transiciones.

### 2. Configuración del AFD

- Los estados, el alfabeto y las transiciones leídas del archivo son almacenados en estructuras de datos adecuadas:
  - **Estados** (`Q`): Un arreglo de cadenas que representa todos los estados del AFD.
  - **Alfabeto** (`Σ`): Un arreglo de cadenas que representa los símbolos permitidos en las transiciones.
  - **Transiciones** (`δ`): Una estructura que mapea cada estado y símbolo a un estado siguiente.
  - **Estado Inicial** (`q0`): Un puntero a la cadena que indica el estado inicial del AFD.
  - **Estados de Aceptación** (`F`): Un arreglo de cadenas que indica los estados de aceptación.

### 3. Procesamiento de la Cadena de Entrada

- Después de configurar el AFD, el programa solicita una cadena de entrada al usuario.
- El programa procesa la cadena carácter por carácter:
  - Utiliza la función de transición para determinar el estado siguiente basado en el estado actual y el símbolo actual de la cadena.
  - Si no se encuentra una transición válida para un símbolo, el programa finaliza indicando un error de transición no definida.

### 4. Verificación de Aceptación

- Una vez procesada toda la cadena, el programa verifica si el estado final es uno de los estados de aceptación.
- Si el estado final está dentro del conjunto de estados de aceptación, el programa indica que la cadena ha sido aceptada. De lo contrario, indica que la cadena ha sido rechazada.

### 5. Liberación de Memoria

- El programa libera toda la memoria dinámica utilizada antes de finalizar, asegurando que no haya fugas de memoria.

## Consideraciones

- **Visualización del Proceso**: Durante el procesamiento, el programa muestra el estado actual, el símbolo procesado y el estado siguiente. En caso de fallo, se indica la razón específica del error.

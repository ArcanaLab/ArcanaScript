#ifndef JAVA_FILE_UTILS_H
#define JAVA_FILE_UTILS_H

// Forward declaration para evitar incluir headers de frontend en el backend
struct Program;

/**
 * Devuelve el nombre de la primera clase pública encontrada en el bloque principal del programa.
 * Retorna NULL si no hay ninguna clase.
 * Requiere que el puntero a Program sea válido y compatible con la estructura del AST.
 */
char* getMainClassName(struct Program* program);

#endif // JAVA_FILE_UTILS_H 
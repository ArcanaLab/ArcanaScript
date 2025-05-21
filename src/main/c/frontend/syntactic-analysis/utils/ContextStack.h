#ifndef CONTEXT_STACK_H
#define CONTEXT_STACK_H

#include <stdlib.h> 
#include "../../../shared/Logger.h"

typedef struct ContextStack ContextStack;
typedef struct ContextStackNode ContextStackNode;
typedef enum ContextStackType ContextStackType;

enum ContextStackType {
    GLOBAL_CONTEXT,
    LAMBDA_CONTEXT,
    CLASS_CONTEXT,
    INTERFACE_CONTEXT,
    LOOP_CONTEXT,
};

struct ContextStackNode {
    ContextStackType type;
    struct ContextStackNode * next;
};

struct ContextStack {
    ContextStackNode * top;
    long size;
};

// ================== [ Context Stack Management ] ======================
#pragma region Context Stack Management
/**
 * Initializes the context stack. Must be called before using any other context stack functions.
 */
void initializeContextStack(void);

/**
 * Releases any resources used by the context stack. Call when done using the context stack.
 */
void shutdownContextStack(void);
#pragma endregion
// ================== [ Context Stack Management ] ======================

// ================== [ Context Stack Push&Pop ] ========================
#pragma region Context Stack Push&Pop
/**
 * Pushes a new context of the specified type onto the stack.
 * @param type The type of context to push onto the stack.
 */
void pushContext(ContextStackType type);

/**
 * Removes and returns the top context from the stack.
 * @return The type of the context that was removed from the stack.
 */
ContextStackType popContext(void);
#pragma endregion
// ================== [ Context Stack Push&Pop ] ========================

// ================== [ Context Stack Empty&In Context Checks ] ============
#pragma region Context Stack Empty&In Context Checks
/**
 * Checks if the context stack is empty.
 * @return true if the context stack is empty, false otherwise.
 */
boolean isContextStackEmpty(void);

/**
 * Checks if the specified context type is currently active (present in the stack).
 * @param type The context type to check for.
 * @return true if the specified context type is active, false otherwise.
 */
boolean inContext(ContextStackType type);
#pragma endregion
// =====================================================================

#endif

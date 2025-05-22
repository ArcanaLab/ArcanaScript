#include "ContextStack.h"

// ================== [ PRIVATE STATE ] ==================
static ContextStack * _contextStack = NULL;
static Logger * _logger = NULL;

// ================== [ PRIVATE FUNCTIONS ] ==================
#pragma region Private Functions
ContextStack * createContextStack(void) {
    ContextStack * contextStack = calloc(1, sizeof(ContextStack));
    contextStack->top = NULL;
    contextStack->size = 0;
    return contextStack;
}

void destroyContextStack(ContextStack * contextStack) {
    ContextStackNode * currentNode = contextStack->top;
    while (currentNode != NULL) {
        ContextStackNode * nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    free(contextStack);
}
#pragma endregion

// ================== [ Context Stack Initialization&Shutdown ] ==================
#pragma region Context Stack Initialization&Shutdown
void initializeContextStack(void) {
    _contextStack = createContextStack();
    _logger = createLogger("ContextStack");
}

void shutdownContextStack(void) {
    destroyContextStack(_contextStack);
    destroyLogger(_logger);
}
#pragma endregion

// ================== [ Context Stack Push&Pop ] ==================
#pragma region Context Stack Push&Pop
void pushContext(ContextStackType type) {
    ContextStackNode * newNode = calloc(1, sizeof(ContextStackNode));
    newNode->type = type;
    newNode->next = _contextStack->top;
    _contextStack->top = newNode;
    _contextStack->size++;
}

ContextStackType popContext(void) {
    if (isContextStackEmpty()) return GLOBAL_CONTEXT;

    ContextStackNode * topNode = _contextStack->top;
    ContextStackType type = topNode->type;
    _contextStack->top = topNode->next;
    _contextStack->size--;
    free(topNode);
    return type;
}
#pragma endregion
// ================== [ Context Stack Empty&In Context Checks ] ==================
#pragma region Context Stack Empty&In Context Checks
boolean isContextStackEmpty(void) {
    return _contextStack->size == 0 ? true : false;
}

boolean inContext(ContextStackType type) {
    if (isContextStackEmpty()) return type == GLOBAL_CONTEXT ? true : false;
    
    ContextStackNode * currentNode = _contextStack->top;
    while (currentNode != NULL) {
        if (currentNode->type == type) return true;
        currentNode = currentNode->next;
    }
    
    return false;
}
#pragma endregion

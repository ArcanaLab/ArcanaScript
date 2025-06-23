#include <stddef.h>

// Definiciones mínimas para manipular el AST sin incluir headers de frontend

typedef struct Object {
    char* name;
    void* genericList;
} Object;

typedef struct Class {
    Object* object;
    void* inherits;
    void* implementationList;
    void* block;
} Class;

typedef struct Interface {
    Object* object;
    void* extends;
    void* block;
} Interface;

typedef enum InstructionType {
    INSTRUCTION_ASSIGNMENT,
    INSTRUCTION_VARIABLE_DECLARATION,
    INSTRUCTION_EXPRESSION,
    INSTRUCTION_BLOCK,
    INSTRUCTION_CONDITIONAL,
    INSTRUCTION_LOOP,
    INSTRUCTION_CLASS,
    INSTRUCTION_INTERFACE,
    INSTRUCTION_RETURN,
    INSTRUCTION_PASS,
} InstructionType;

typedef struct Instruction {
    union {
        void* assignment;
        void* variableDeclaration;
        void* expression;
        void* block;
        void* loop;
        void* conditional;
        Class* class;
        Interface* interface;
        void* returnInstruction;
    };
    InstructionType type;
} Instruction;

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct List {
    Node* first;
    Node* last;
    int size;
} List;

typedef struct Program {
    void* importList;
    List* block;
    void* loop;
} Program;

char* getMainClassName(struct Program* program) {
    if (program == NULL || program->block == NULL) return NULL;
    Node* current = program->block->first;
    // Buscar primero una clase
    while (current != NULL) {
        Instruction* instr = (Instruction*)current->data;
        if (instr != NULL && instr->type == INSTRUCTION_CLASS && instr->class != NULL && instr->class->object != NULL && instr->class->object->name != NULL) {
            return instr->class->object->name;
        }
        current = current->next;
    }
    // Si no hay clase, buscar una interface
    current = program->block->first;
    while (current != NULL) {
        Instruction* instr = (Instruction*)current->data;
        if (instr != NULL && instr->type == INSTRUCTION_INTERFACE && instr->interface != NULL) {
            Interface* iface = (Interface*)instr->interface;
            if (iface->object != NULL && iface->object->name != NULL) {
                return iface->object->name;
            }
        }
        current = current->next;
    }
    return NULL;
} 
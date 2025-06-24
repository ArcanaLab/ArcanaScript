#include "ClassGenerator.h"
#include "Generator.h"
#include "StructureGenerator.h"
#include "VariableGenerator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void generateGeneric(const unsigned int indentationLevel, Generic* generic);
static void generateGenericList(const unsigned int indentationLevel, GenericList* genericList);

static void generateObject(const unsigned int indentationLevel, Object* object) {
    if (object == NULL) {
        return;
    }
    
    generatorOutput(0, "%s", object->name);
    
    if (object->genericList != NULL) {
        generateGenericList(indentationLevel, object->genericList);
    }
}

static void generateGenericList(const unsigned int indentationLevel, GenericList* genericList) {
    if (genericList == NULL) {
        return;
    }

    generatorOutput(0, "<");
    Node* current = genericList->first;
    bool first = true;
    while (current != NULL) {
        if (!first) {
            generatorOutput(0, ", ");
        }
        generateGeneric(0, (Generic*)current->data);
        first = false;
        current = current->next;
    }
    generatorOutput(0, ">");
}

static void generateGeneric(const unsigned int indentationLevel, Generic* generic) {
    if (generic == NULL) {
        return;
    }
    generateObject(indentationLevel, generic->object);
    if (generic->isObject != NULL) {
        generatorOutput(0, " extends ");
        generateObject(0, generic->isObject);
    }
}

void generateImplementationList(const unsigned int indentationLevel, ImplementationList* implementationList) {
    if (implementationList == NULL) {
        return;
    }
    Node* current = implementationList->first;
    bool first = true;
    
    while (current != NULL) {
        Object* object = (Object*)current->data;
        
        if (!first) {
            generatorOutput(0, ", ");
        }
        
        if (object != NULL) {
            generateObject(indentationLevel, object);
        }
        
        first = false;
        current = current->next;
    }
}

void generateInheritance(const unsigned int indentationLevel, Object* inherits) {
    if (inherits == NULL) {
        return;
    }
    
    generatorOutput(indentationLevel, " extends ");
    generateObject(indentationLevel, inherits);
}

void generateInterfaceInheritance(const unsigned int indentationLevel, ImplementationList* extends) {
    if (extends == NULL) {
        return;
    }
    
    generatorOutput(indentationLevel, " extends ");
    generateImplementationList(indentationLevel, extends);
}

void generateImplementations(const unsigned int indentationLevel, ImplementationList* implementationList) {
    if (implementationList == NULL) {
        return;
    }
    
    generatorOutput(indentationLevel, " implements ");
    generateImplementationList(indentationLevel, implementationList);
}

void generateClass(const unsigned int indentationLevel, Class* class) {
    if (class == NULL) {
        return;
    }
    if (class->privacyModifierList != NULL) {
        generatePrivacyModifiers(indentationLevel, class->privacyModifierList);
    } else {
        generatorOutput(indentationLevel, "public ");
    }
    
    generatorOutput(indentationLevel, "class ");
    
    if (class->object != NULL) {
        generateObject(indentationLevel, class->object);
    }
    
    if (class->inherits != NULL) {
        generateInheritance(indentationLevel, class->inherits);
    }
    
    if (class->implementationList != NULL) {
        generateImplementations(indentationLevel, class->implementationList);
    }
    
    if (class->block != NULL) {
        generatorOutput(indentationLevel, " {\n");
        generateClassBody(indentationLevel, class->block, class->object);
        generatorOutput(indentationLevel, "}\n");
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(indentationLevel, "\n");
}

void generateInterface(const unsigned int indentationLevel, Interface* interface) {
    if (interface == NULL) {
        return;
    }
    
    generatorOutput(indentationLevel, "public interface ");
    
    if (interface->object != NULL) {
        generateObject(indentationLevel, interface->object);
    }
    
    if (interface->extends != NULL) {
        generateInterfaceInheritance(indentationLevel, interface->extends);
    }
    
    if (interface->block != NULL) {
        generateScope(indentationLevel, interface->block);
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(indentationLevel, "\n");
}

void generateClassBody(const unsigned int indentationLevel, Block* block, Object* classObject) {
    if (block == NULL) {
        return;
    }
    
    InstructionNode* currentInstruction = block->first;
    while (currentInstruction != NULL) {
        Instruction* instruction = (Instruction*)currentInstruction->data;
        
        if (instruction->type == INSTRUCTION_CONSTRUCTOR) {
            if (classObject != NULL) {
                generatorOutput(indentationLevel + 1, "%s", classObject->name);
            }
            generateConstructor(indentationLevel + 1, instruction->constructor);
        } else {
            generateInstruction(indentationLevel + 1, instruction);
        }
        
        currentInstruction = currentInstruction->next;
    }
} 
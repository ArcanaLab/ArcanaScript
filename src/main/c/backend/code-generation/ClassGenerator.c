#include "ClassGenerator.h"
#include "Generator.h"
#include "StructureGenerator.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void generateGeneric(const unsigned int indentationLevel, Generic* generic);
static void generateGenericList(const unsigned int indentationLevel, GenericList* genericList);

void generateObject(const unsigned int indentationLevel, Object* object) {
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
    
    // ImplementationList is a List of Objects
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
    
    // Generate class declaration
    generatorOutput(indentationLevel, "public class ");
    
    // Generate class name and generics
    if (class->object != NULL) {
        generateObject(indentationLevel, class->object);
    }
    
    // Generate inheritance (extends)
    if (class->inherits != NULL) {
        generateInheritance(indentationLevel, class->inherits);
    }
    
    // Generate implementations (implements)
    if (class->implementationList != NULL) {
        generateImplementations(indentationLevel, class->implementationList);
    }
    
    // Generate class body
    if (class->block != NULL) {
        generateScope(indentationLevel, class->block);
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
    
    // Generate interface declaration
    generatorOutput(indentationLevel, "public interface ");
    
    // Generate interface name and generics
    if (interface->object != NULL) {
        generateObject(indentationLevel, interface->object);
    }
    
    // Generate interface inheritance (extends)
    if (interface->extends != NULL) {
        generateInterfaceInheritance(indentationLevel, interface->extends);
    }
    
    // Generate interface body
    if (interface->block != NULL) {
        generateScope(indentationLevel, interface->block);
    } else {
        generatorOutput(indentationLevel, " {\n");
        generatorOutput(indentationLevel, "}\n");
    }
    
    generatorOutput(indentationLevel, "\n");
} 
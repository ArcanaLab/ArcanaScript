#ifndef CLASS_GENERATOR_HEADER
#define CLASS_GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

/**
 * Generates code for a class declaration using direct output.
 * @param indentationLevel The current indentation level
 * @param class The class structure to generate code for
 */
void generateClass(const unsigned int indentationLevel, Class* class);

/**
 * Generates code for an interface declaration using direct output.
 * @param indentationLevel The current indentation level
 * @param interface The interface structure to generate code for
 */
void generateInterface(const unsigned int indentationLevel, Interface* interface);

/**
 * Generates code for class inheritance using direct output.
 * @param indentationLevel The current indentation level
 * @param inherits The inheritance object to generate code for
 */
void generateInheritance(const unsigned int indentationLevel, Object* inherits);

/**
 * Generates code for interface inheritance using direct output.
 * @param indentationLevel The current indentation level
 * @param extends The implementation list to generate code for
 */
void generateInterfaceInheritance(const unsigned int indentationLevel, ImplementationList* extends);

/**
 * Generates code for class implementations using direct output.
 * @param indentationLevel The current indentation level
 * @param implementationList The implementation list to generate code for
 */
void generateImplementations(const unsigned int indentationLevel, ImplementationList* implementationList);

/**
 * Generates code for an implementation list using direct output.
 * @param indentationLevel The current indentation level
 * @param implementationList The implementation list to generate code for
 */
void generateImplementationList(const unsigned int indentationLevel, ImplementationList* implementationList);

/**
 * Generates code for an object, including its name and generic parameters.
 * @param indentationLevel The current indentation level
 * @param object The object to generate code for
 */
void generateObject(const unsigned int indentationLevel, Object* object);

#endif 
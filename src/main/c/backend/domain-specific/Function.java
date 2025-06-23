package main.c.backend.domain-specific;
import java.lang.FunctionalInterface;

/**
 * Represents a function that accepts a variable number of arguments and produces a result.
 * This is a functional interface whose functional method is {@link #apply(Object...)}.
 *
 * @param <R> the type of the result of the function
 */
@FunctionalInterface
public interface Function <R> 
{   
    R apply(Object... args);

}

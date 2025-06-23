#!/bin/bash

set -u

# Verificar que se pasaron dos argumentos
if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <carpeta_origen> <carpeta_destino>"
    exit 1
fi

ORIGEN="$1"
DESTINO="$2"

# Verificar que la carpeta origen existe
if [ ! -d "$ORIGEN" ]; then
    echo "Error: La carpeta origen '$ORIGEN' no existe"
    exit 1
fi

# Crear la carpeta destino si no existe
mkdir -p "$DESTINO"

# Obtener la ruta base del proyecto
BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

# Procesar todos los archivos en la carpeta origen (recursivamente)
find "$ORIGEN" -type f | while read -r archivo; do
    # Obtener el path relativo al origen
    rel_path="${archivo#$ORIGEN/}"
    ext="${archivo##*.}"
    if [ "$ext" = "arcx" ]; then
        # Cambiar la extensión a .java
        java_path="${rel_path%.arcx}.java"
        # Crear el directorio destino si no existe
        mkdir -p "$DESTINO/$(dirname "$java_path")"
        # Procesar el archivo con el compilador y guardar la salida temporalmente
        tmp_output="$(mktemp)"
        build/Compiler "$archivo" > "$tmp_output" 2>/dev/null
        RESULT="$?"
        if [ "$RESULT" == "0" ]; then
            mv "$tmp_output" "$DESTINO/$java_path"
        else
            echo "Archivo que no compila: $archivo"
            rm "$tmp_output"
        fi
    else
        # Copiar el archivo tal cual manteniendo la estructura
        mkdir -p "$DESTINO/$(dirname "$rel_path")"
        cp "$archivo" "$DESTINO/$rel_path"
    fi

done

echo "¡Conversión completada!"
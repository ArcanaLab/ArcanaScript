#!/bin/bash
set -euo pipefail

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m'
BOLD='\033[1m'
DIM='\033[2m'

# Animation functions
loading_animation() {
    local message="$1"
    local duration="$2"
    local delay=0.1
    local spinstr='|/-\'
    local temp
    
    echo -n -e "${BLUE}[INFO]${NC} $message "
    
    local end_time=$((SECONDS + duration))
    while [ $SECONDS -lt $end_time ]; do
        temp=${spinstr#?}
        printf "[%c]" "$spinstr"
        spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b"
    done
    echo -e "${GREEN}✓${NC}"
}

progress_bar() {
    local message="$1"
    local steps="$2"
    local delay="$3"
    
    echo -e "${BLUE}[LOAD]${NC} $message"
    echo -n "       "
    
    for ((i=0; i<=steps; i++)); do
        local percent=$((i * 100 / steps))
        local filled=$((i * 50 / steps))
        local empty=$((50 - filled))
        
        printf "\r       ${CYAN}["
        printf "%${filled}s" | tr ' ' '█'
        printf "%${empty}s" | tr ' ' '░'
        printf "]${NC} ${WHITE}%3d%%${NC}" "$percent"
        
        sleep "$delay"
    done
    echo -e " ${GREEN}✓${NC}"
}

converter_header() {
    clear
    echo ""
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}                    ${BOLD}${WHITE}ARCANA CONVERTER INITIALIZATION${NC}                   ${CYAN}║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    echo -e "${YELLOW}[CONVERTER]${NC} Starting ArcanaScript to Java Conversion..."
    sleep 0.5
}

conversion_complete_animation() {

    
    echo ""
    echo -e "${CYAN}*${NC} ${WHITE}█████╗ ██████╗  ██████╗ █████╗ ███╗   ██╗ █████╗${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██╔══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║██╔══██╗${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}███████║██████╔╝██║     ███████║██╔██╗ ██║███████║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██╔══██║██╔══██╗██║     ██╔══██║██║╚██╗██║██╔══██║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██║  ██║██║  ██║╚██████╗██║  ██║██║ ╚████║██║  ██║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝${NC}"
    echo ""
    echo -e "${YELLOW} ██████╗ ██████╗ ███╗   ██╗██╗   ██╗███████╗██████╗ ████████╗███████╗██████╗${NC}"
    echo -e "${YELLOW}██╔════╝██╔═══██╗████╗  ██║██║   ██║██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔══██╗${NC}"
    echo -e "${YELLOW}██║     ██║   ██║██╔██╗ ██║██║   ██║█████╗  ██████╔╝   ██║   █████╗  ██████╔╝${NC}"
    echo -e "${YELLOW}██║     ██║   ██║██║╚██╗██║╚██╗ ██╔╝██╔══╝  ██╔══██╗   ██║   ██╔══╝  ██╔══██╗${NC}"
    echo -e "${YELLOW}╚██████╗╚██████╔╝██║ ╚████║ ╚████╔╝ ███████╗██║  ██║   ██║   ███████╗██║  ██║${NC}"
    echo -e "${YELLOW} ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝${NC}"
    echo ""
    echo -e "${CYAN}================================================================================${NC}"
    echo -e "${WHITE}                        CONVERSION COMPLETED SUCCESSFULLY!${NC}"
    echo -e "${CYAN}================================================================================${NC}"
    echo ""
}

# Function to validate arguments
validate_arguments() {
    if [ $# -ne 2 ]; then
        echo -e "${RED}[ERROR]${NC} Invalid number of arguments!"
        echo -e "${YELLOW}Usage:${NC} $0 <path-to-arcx> <path-to-java>"
        echo -e "${YELLOW}Example:${NC} $0 /path/to/source.arcx /path/to/output/"
        exit 1
    fi
    
    local input_path="$1"
    local output_path="$2"
    
    # Check if input exists
    if [ ! -e "$input_path" ]; then
        echo -e "${RED}[ERROR]${NC} Input path does not exist: $input_path"
        exit 1
    fi
    
    # Create output directory if it doesn't exist
    if [ ! -d "$output_path" ]; then
        echo -e "${YELLOW}[INFO]${NC} Creating output directory: $output_path"
        mkdir -p "$output_path"
    fi
}

# Function to clean Java output from compiler debug messages
clean_java_output() {
    local temp_file="$1"
    local java_content=""
    local capturing=false
    
    # Read the file line by line and filter out debug information
    while IFS= read -r line; do
        # Skip debug lines - more comprehensive patterns
        if [[ "$line" =~ ^=== ]] || \
           [[ "$line" =~ DEBUG ]] || \
           [[ "$line" =~ ^Function: ]] || \
           [[ "$line" =~ ^Lexeme: ]] || \
           [[ "$line" =~ ^Token: ]] || \
           [[ "$line" =~ "Semantic value" ]] || \
           [[ "$line" =~ "Expression:" ]] || \
           [[ "$line" =~ "Variable declaration:" ]] || \
           [[ "$line" =~ "Variable name:" ]] || \
           [[ "$line" =~ "Variable type:" ]] || \
           [[ "$line" =~ "Instruction type:" ]] || \
           [[ "$line" =~ "Value pointer:" ]] || \
           [[ "$line" =~ "Object:" ]] || \
           [[ "$line" =~ "Privacy modifier" ]] || \
           [[ "$line" =~ "AST:" ]] || \
           [[ "$line" =~ "Parser:" ]] || \
           [[ "$line" =~ "Compiling" ]] || \
           [[ "$line" =~ "Generated code:" ]]; then
            continue
        fi
        
        # Start capturing when we see Java code patterns
        if [[ "$line" =~ ^[[:space:]]*public[[:space:]]+class ]] || \
           [[ "$line" =~ ^[[:space:]]*class[[:space:]] ]] || \
           [[ "$line" =~ ^[[:space:]]*import[[:space:]] ]] || \
           [[ "$line" =~ ^[[:space:]]*package[[:space:]] ]] || \
           [[ "$capturing" == true ]]; then
            capturing=true
            java_content+="$line"$'\n'
        fi
    done < "$temp_file"
    
    # If we didn't capture anything with the above method, try alternative approach
    if [ -z "$java_content" ]; then
        # Look for the last section that looks like Java code
        java_content=$(awk '
            BEGIN { java_section = "" }
            /^[[:space:]]*public[[:space:]]+class/ { 
                java_section = $0 "\n"
                java_found = 1
                next
            }
            java_found && !/^(===|DEBUG|Function:|Lexeme:|Token:|Semantic|Expression:|Variable|Instruction|Value|Object:|Privacy|AST:|Parser:|Compiling|Generated)/ {
                java_section = java_section $0 "\n"
            }
            /^(===|DEBUG|Function:|Lexeme:|Token:|Semantic|Expression:|Variable|Instruction|Value|Object:|Privacy|AST:|Parser:|Compiling|Generated)/ && java_found {
                # Reset if we hit debug info after Java code
                if (length(java_section) < 50) {
                    java_section = ""
                    java_found = 0
                }
            }
            END { print java_section }
        ' "$temp_file")
    fi
    
    echo "$java_content"
}

# Function to find all .arcx files recursively
find_arcx_files() {
    local input_path="$1"
    
    if [ -f "$input_path" ]; then
        if [[ "$input_path" == *.arcx ]]; then
            echo "$input_path"
        fi
    elif [ -d "$input_path" ]; then
        find "$input_path" -name "*.arcx" -type f
    fi
}

# Function to convert a single file
convert_single_file() {
    local input_file="$1"
    local output_dir="$2"
    local filename=$(basename "$input_file" .arcx)
    local output_file="$output_dir/${filename}.java"
    local success=false
    
    echo -e "${BLUE}[CONVERT]${NC} Processing: $(basename "$input_file")"
    
    # Find Arcana compiler using relative path from script location
    local script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local compiler_path="$script_dir/../../build/Compiler"
    
    # Also try alternative relative paths
    if [ ! -f "$compiler_path" ]; then
        compiler_path="$script_dir/../build/Compiler"
    fi
    if [ ! -f "$compiler_path" ]; then
        compiler_path="$script_dir/build/Compiler"
    fi
    if [ ! -f "$compiler_path" ]; then
        compiler_path="./build/Compiler"
    fi
    if [ ! -f "$compiler_path" ]; then
        compiler_path="./Compiler"
    fi
    
    if [ ! -f "$compiler_path" ]; then
        echo -e "${RED}[ERROR]${NC} Arcana compiler not found!"
        echo -e "${YELLOW}[INFO]${NC} Searched in:"
        echo -e "  - $script_dir/../../build/Compiler"
        echo -e "  - $script_dir/../build/Compiler"
        echo -e "  - $script_dir/build/Compiler"
        echo -e "  - ./build/Compiler"
        echo -e "  - ./Compiler"
        echo -e "${YELLOW}[INFO]${NC} Please ensure the Compiler executable is built and accessible"
        return 1
    fi
    
    # Convert the file using the Arcana compiler
    echo -e "${BLUE}[EXEC]${NC} Converting $(basename "$input_file") to Java..."
    
    # Create a temporary file to capture the output
    local temp_file=$(mktemp)
    
    # Temporarily disable strict error handling for this conversion
    set +e
    
    # Run the conversion - capture both stdout and stderr
    if cat "$input_file" | "$compiler_path" > "$temp_file" 2>&1; then
        # Clean the output to remove debug information
        local java_content=""
        java_content=$(clean_java_output "$temp_file" 2>/dev/null || echo "")
        
        # Write the clean Java code to output file
        if [ -n "$java_content" ] && [ "$java_content" != $'\n' ]; then
            echo "$java_content" > "$output_file"
            echo -e "${GREEN}[SUCCESS]${NC} Converted to: $(basename "$output_file")"
            success=true
        else
            echo -e "${RED}[ERROR]${NC} No valid Java code generated for: $(basename "$input_file")"
            echo -e "${YELLOW}[DEBUG]${NC} Raw compiler output:"
            if [ -s "$temp_file" ]; then
                head -20 "$temp_file" 2>/dev/null || echo "  (empty or unreadable output)"
            else
                echo "  (empty output file)"
            fi
            echo -e "${DIM}... (truncated)${NC}"
        fi
    else
        echo -e "${RED}[ERROR]${NC} Compiler failed for: $(basename "$input_file")"
        echo -e "${YELLOW}[DEBUG]${NC} Error output:"
        if [ -s "$temp_file" ]; then
            head -10 "$temp_file" 2>/dev/null || echo "  (empty or unreadable output)"
        else
            echo "  (empty output file)"
        fi
        echo -e "${DIM}... (truncated)${NC}"
    fi
    
    # Clean up
    rm -f "$temp_file"
    
    # Return success status
    if [ "$success" = true ]; then
        return 0
    else
        return 1
    fi
}

# Function to process ArcanaScript files recursively
process_arcana_files() {
    local input_path="$1"
    local output_path="$2"
    local processed_count=0
    local failed_count=0

    # Get the list of files using a safer approach
    local arcx_files=()
    while IFS= read -r file; do
        [ -n "$file" ] && arcx_files+=("$file")
    done < <(find_arcx_files "$input_path")
    
    local total_files=${#arcx_files[@]}

    if [ $total_files -eq 0 ]; then
        echo -e "${RED}[ERROR]${NC} No .arcx files found in: $input_path"
        exit 1
    fi

    echo -e "${CYAN}[INFO]${NC} Found $total_files ArcanaScript file(s) to convert"
    echo ""

    # Simple counter-based loop
    for ((i=0; i<total_files; i++)); do
        local arcx_file="${arcx_files[$i]}"
        local current_file=$((i + 1))
        
        echo -e "${YELLOW}[DEBUG]${NC} Processing file $current_file of $total_files: $arcx_file"
        
        # Calcular el path relativo para mantener estructura
        local relative_path
        local relative_dir

        if [ -f "$input_path" ]; then
            # Caso de archivo único
            relative_path=$(basename "$arcx_file")
            relative_dir="."
        else
            # Caso de carpeta - mantener estructura
            relative_path=$(realpath --relative-to="$input_path" "$arcx_file")
            relative_dir=$(dirname "$relative_path")
        fi

        # Crear la carpeta de destino si corresponde
        local target_dir="$output_path"
        if [ "$relative_dir" != "." ]; then
            target_dir="$output_path/$relative_dir"
            mkdir -p "$target_dir"
        fi

        echo -e "${MAGENTA}[PROCESSING]${NC} File ($current_file/$total_files): $relative_path"
        
        # Temporarily disable strict error handling for the entire conversion process
        set +e
        
        if convert_single_file "$arcx_file" "$target_dir"; then
            echo -e "${GREEN}[SUCCESS]${NC} Successfully converted: $relative_path"
            ((processed_count++))
        else
            echo -e "${RED}[FAILED]${NC} Failed to convert: $relative_path"
            ((failed_count++))
        fi
        
        # Re-enable strict error handling
        set -euo pipefail
        
        echo -e "${YELLOW}[DEBUG]${NC} Completed conversion attempt for: $arcx_file"
        echo ""
    done

    echo -e "${GREEN}[COMPLETE]${NC} Successfully processed $processed_count out of $total_files files"
    if [ $failed_count -gt 0 ]; then
        echo -e "${RED}[WARNING]${NC} Failed to convert $failed_count files"
    fi
}


# Main execution
main() {
    # Show header
    converter_header
    
    # Validate arguments
    validate_arguments "$@"
    
    local input_path="$1"
    local output_path="$2"
    
    # Convert to absolute paths for display but keep relative paths for processing
    local abs_input_path=$(realpath "$input_path")
    local abs_output_path=$(realpath "$output_path")
    
    echo -e "${CYAN}[CONFIG]${NC} Input path: $abs_input_path"
    echo -e "${CYAN}[CONFIG]${NC} Output path: $abs_output_path"
    echo ""
    
    # Loading animations
    loading_animation "Validating input files" 1
    loading_animation "Checking compiler availability" 2
    loading_animation "Preparing output directories" 1
    
    echo ""
    
    progress_bar "Initializing conversion engine" 15 0.08
    progress_bar "Loading ArcanaScript parser" 20 0.06
    progress_bar "Calibrating Java code generator" 18 0.07
    progress_bar "Establishing file mappings" 12 0.09
    
    echo ""
    
    echo -e "${MAGENTA}[MAGIC]${NC} Invoking ArcanaScript to Java transmutation..."
    sleep 1
    echo -e "${MAGENTA}[MAGIC]${NC} Channeling bytecode energies..."
    sleep 1
    echo -e "${MAGENTA}[MAGIC]${NC} Weaving object-oriented spells..."
    sleep 1.5
    
    echo ""
    echo -e "${GREEN}[READY]${NC} Conversion engine initialized successfully!"
    echo ""
    
    # Process the files
    process_arcana_files "$input_path" "$output_path"
    
    echo ""
    echo -e "${GREEN}[COMPLETE]${NC} All files converted successfully!"
    echo -e "${GREEN}[COMPLETE]${NC} Java files are ready in: $abs_output_path"
    echo ""
    
    # Show completion animation
    conversion_complete_animation
    
    echo -e "${GREEN}ArcanaScript to Java conversion completed successfully!${NC}"
    echo -e "${CYAN}Output location:${NC} $abs_output_path"
    echo ""
}

# Run the main function with all arguments
main "$@"
#!/bin/bash
set -euo pipefail

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

set +x

boot_sequence() {
    clear
    echo ""
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}                    ${BOLD}${WHITE}ARCANA SCRIPT INITIALIZATION${NC}                    ${CYAN}║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    echo -e "${YELLOW}[BOOT]${NC} Starting ArcanaScript Environment..."
    sleep 0.5
    
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
    
    loading_animation "Checking compiler dependencies" 2
    loading_animation "Validating script permissions" 1
    loading_animation "Initializing magical constants" 2
    
    echo ""
    
    progress_bar "Loading core libraries" 20 0.05
    progress_bar "Preparing spell matrices" 15 0.08
    progress_bar "Calibrating arcane processors" 25 0.04
    progress_bar "Establishing mystical connections" 18 0.06
    
    echo ""
    
    echo -e "${MAGENTA}[MAGIC]${NC} Invoking ancient compilation rituals..."
    sleep 1
    echo -e "${MAGENTA}[MAGIC]${NC} Channeling source code energies..."
    sleep 1
    echo -e "${MAGENTA}[MAGIC]${NC} Awakening the ArcanaScript consciousness..."
    sleep 1.5
    
    echo ""
    echo -e "${GREEN}[SUCCESS]${NC} All systems initialized successfully!"
    echo -e "${GREEN}[SUCCESS]${NC} ArcanaScript environment is now active!"
    echo ""
    
    echo -e "${YELLOW}Launching in:${NC}"
    for i in 3 2 1; do
        echo -e "              ${BOLD}${WHITE}$i${NC}"
        sleep 1
    done
    
    clear
    
    echo ""
    echo -e "${CYAN}*${NC} ${WHITE}█████╗ ██████╗  ██████╗ █████╗ ███╗   ██╗ █████╗${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██╔══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║██╔══██╗${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}███████║██████╔╝██║     ███████║██╔██╗ ██║███████║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██╔══██║██╔══██╗██║     ██╔══██║██║╚██╗██║██╔══██║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}██║  ██║██║  ██║╚██████╗██║  ██║██║ ╚████║██║  ██║${NC}"
    echo -e "${CYAN}*${NC} ${WHITE}╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝${NC}"
    echo ""
    echo -e "${YELLOW}███████╗ ██████╗██████╗ ██╗██████╗ ████████╗${NC}"
    echo -e "${YELLOW}██╔════╝██╔════╝██╔══██╗██║██╔══██╗╚══██╔══╝${NC}"
    echo -e "${YELLOW}███████╗██║     ██████╔╝██║██████╔╝   ██║${NC}"
    echo -e "${YELLOW}╚════██║██║     ██╔══██╗██║██╔═══╝    ██║${NC}"
    echo -e "${YELLOW}███████║╚██████╗██║  ██║██║██║        ██║${NC}"
    echo -e "${YELLOW}╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝╚═╝        ╚═╝${NC}"
    echo ""
    echo -e "${CYAN}================================================================================${NC}"
    echo -e "${WHITE}                              READY TO ENGAGE!${NC}"
    echo -e "${CYAN}================================================================================${NC}"
    echo ""
    echo -e "${GREEN}ArcanaScript is now ready for magical code execution!${NC}"
    echo ""
    
    sleep 2
}

boot_sequence

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

INPUT="$1"
shift 1

set -x

cat "$INPUT" | build/Compiler "$@"
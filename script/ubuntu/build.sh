#!/bin/bash

set -euxo pipefail

# Colors for output (simplified for better compatibility)
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Get the project root directory (two levels up from script/ubuntu/)
BASE_PATH="$(cd "$(dirname "$0")/../.." && pwd)"
echo -e "${BLUE}Project root: $BASE_PATH${NC}"

cd "$BASE_PATH"

set +x
echo ""
echo -e "${CYAN}================================================================================${NC}"
echo -e "${WHITE}                           CLEANING PREVIOUS BUILD ARTIFACTS${NC}"
echo -e "${CYAN}================================================================================${NC}"
echo ""
set -x

# Clean build directory
if [ -d "build" ]; then
    echo -e "${BLUE}  Removing build directory...${NC}"
    rm -rf build
    echo -e "${GREEN}  ✓ Build directory removed${NC}"
else
    echo -e "${CYAN}  ℹ️  Build directory doesn't exist, skipping...${NC}"
fi

# Clean generated files
FILES_TO_REMOVE=(
    "src/main/c/frontend/lexical-analysis/FlexScanner.c"
    "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    "src/main/c/frontend/syntactic-analysis/BisonParser.h"
)

for file in "${FILES_TO_REMOVE[@]}"; do
    if [ -f "$file" ]; then
        echo -e "${BLUE}  Removing $(basename "$file")...${NC}"
        rm -f "$file"
        echo -e "${GREEN}  ✓ $(basename "$file") removed${NC}"
    else
        echo -e "${CYAN}  ℹ️  $(basename "$file") doesn't exist, skipping...${NC}"
    fi
done

set +x
echo ""
echo -e "${CYAN}================================================================================${NC}"
echo -e "${WHITE}                              STARTING BUILD PROCESS${NC}"
echo -e "${CYAN}================================================================================${NC}"
echo ""
set -x
# Build process
echo -e "${BLUE}  Configuring CMake...${NC}"
cmake -S . -B build

echo -e "${BLUE}  Compiling project...${NC}"
cmake --build build
set +x
echo ""
echo -e "${CYAN}================================================================================${NC}"
echo -e "${WHITE}                              BUILD COMPLETED SUCCESSFULLY${NC}"
echo -e "${CYAN}================================================================================${NC}"
echo ""
set -x

# Temporarily disable debug output for the completion message
set +x

# Stylized completion message
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

# Re-enable debug output
set -x
#! /bin/bash

set -u

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

# Check for parameter
if [ $# -eq 0 ]; then
    echo "Usage: $0 <test_number> [a|r]"
    echo "Example: $0 64 a"
    echo ""
    echo "Available accept tests:"
    ls src/test/c/accept/ 2>/dev/null | head -10
    echo "..."
    echo ""
    echo "Available reject tests:"
    ls src/test/c/reject/ 2>/dev/null | head -10
    echo "..."
    exit 1
fi

TEST_NUM="$1"
TEST_MODE="${2:-}"
FOUND_FILE=""
TEST_TYPE=""

if [ "$TEST_MODE" = "a" ]; then
    # Search only in accept
    FOUND_FILE=$(ls src/test/c/accept/${TEST_NUM}-*.arcx 2>/dev/null | head -n 1)
    TEST_TYPE="accept"
    if [ -z "$FOUND_FILE" ]; then
        echo -e "${RED}Error: Accept test file starting with '$TEST_NUM-' not found.${OFF}"
        echo ""
        echo "Available accept tests:"
        ls src/test/c/accept/
        exit 1
    fi
elif [ "$TEST_MODE" = "r" ]; then
    # Search only in reject
    FOUND_FILE=$(ls src/test/c/reject/${TEST_NUM}-*.arcx 2>/dev/null | head -n 1)
    TEST_TYPE="reject"
    if [ -z "$FOUND_FILE" ]; then
        echo -e "${RED}Error: Reject test file starting with '$TEST_NUM-' not found.${OFF}"
        echo ""
        echo "Available reject tests:"
        ls src/test/c/reject/
        exit 1
    fi
else
    # Search in accept
    FOUND_FILE=$(ls src/test/c/accept/${TEST_NUM}-*.arcx 2>/dev/null | head -n 1)
    TEST_TYPE="accept"
    if [ -z "$FOUND_FILE" ]; then
        # Search in reject
        FOUND_FILE=$(ls src/test/c/reject/${TEST_NUM}-*.arcx 2>/dev/null | head -n 1)
        TEST_TYPE="reject"
    fi
    if [ -z "$FOUND_FILE" ]; then
        echo -e "${RED}Error: Test file starting with '$TEST_NUM-' not found in accept or reject directories${OFF}"
        echo ""
        echo "Available accept tests:"
        ls src/test/c/accept/
        echo ""
        echo "Available reject tests:"
        ls src/test/c/reject/
        exit 1
    fi
fi

BASENAME=$(basename "$FOUND_FILE")
echo "=== Testing $TEST_TYPE file: $BASENAME ==="
cat "$FOUND_FILE" | build/Compiler "$BASENAME"
RESULT="$?"

if [ "$TEST_TYPE" = "accept" ]; then
    if [ "$RESULT" = "0" ]; then
        echo -e "    $BASENAME, ${GREEN}accepted correctly${OFF} (status $RESULT)"
    else
        STATUS=1
        echo -e "    $BASENAME, ${RED}rejected incorrectly${OFF} (status $RESULT)"
    fi
else
    if [ "$RESULT" != "0" ]; then
        echo -e "    $BASENAME, ${GREEN}rejected correctly${OFF} (status $RESULT)"
    else
        STATUS=1
        echo -e "    $BASENAME, ${RED}accepted incorrectly${OFF} (status $RESULT)"
    fi
fi

echo ""
echo "Test completed."
exit $STATUS
# Converts a binary file to a C header with an unsigned char array.
# Usage: cmake -DINPUT_FILE=... -DOUTPUT_FILE=... -DVAR_NAME=... -P bin2h.cmake

file(READ "${INPUT_FILE}" hex_content HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," hex_content "${hex_content}")
file(WRITE "${OUTPUT_FILE}"
    "#pragma once\n"
    "static const unsigned char ${VAR_NAME}[] = {${hex_content}};\n"
    "static const size_t ${VAR_NAME}_len = sizeof(${VAR_NAME});\n"
)

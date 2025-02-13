#pragma once


#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"
#define NEW_LINE "\n"

#define NEW_WORLD_COUNT 1

#define PRINTF_COLOR(color, format_string, ...) \
    printf("%s" format_string "%s", color, ##__VA_ARGS__, ANSI_RESET);

#define PRINTF_GROUP_TEXT(source, destinationText, ...) \
    PRINTF_COLOR(ANSI_CYAN, source); \
    PRINTF_COLOR(ANSI_RESET, destinationText, ##__VA_ARGS__); \

#define PRINTF_GROUP_SUCCESFUL(format, ...) \
    PRINTF_COLOR(ANSI_BLUE, "Group: Succesful "); \
    PRINTF_COLOR(ANSI_RESET, format, ##__VA_ARGS__); \

#define PRINTF_GROUP_FAILED(format, ...) \
    PRINTF_COLOR(ANSI_RED, "Group: Failed "); \
    PRINTF_COLOR(ANSI_RESET, format, ##__VA_ARGS__); \


#define PRINTF_COLOR_WITH_LINE(color, format_string, ...)       \
    PRINTF_COLOR(ANSI_MAGENTA, "[%s:%d] ", __FILE__, __LINE__);  \
    PRINTF_COLOR(color, format_string, ##__VA_ARGS__);

namespace printer {
    
}


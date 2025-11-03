#include <stdio.h>
#include <string.h>
#include "log_buffer.h"
#include <stdarg.h>

char log_buffer[LOG_BUFFER_SIZE][LOG_LINE_MAX_LEN];
int log_index = 0;

void add_log_line(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer[log_index], LOG_LINE_MAX_LEN, format, args);
    va_end(args);

    log_index = (log_index + 1) % LOG_BUFFER_SIZE;
}

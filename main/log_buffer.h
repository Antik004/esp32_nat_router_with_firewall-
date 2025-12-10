

#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#define LOG_BUFFER_SIZE 100
#define LOG_LINE_MAX_LEN 128

extern char log_buffer[LOG_BUFFER_SIZE][LOG_LINE_MAX_LEN];
extern int log_index;

void add_log_line(const char *format, ...);

#endif

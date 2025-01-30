#ifndef ERROR_LOGGING_H
#define ERROR_LOGGING_H

void Init(const char *logLevel);
void logger_error(const char *message, ...);
void logger_warning(const char *message, ...);
void logger_info(const char *message, ...);

#endif

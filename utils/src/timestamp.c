#include "../inc/timestamp.h"

void timestamp_to_string(time_t timestamp, char *buffer, size_t buffer_size) {
    struct tm *time_info = localtime(&timestamp);
    strftime(buffer, buffer_size, "%H:%M:%S", time_info);
}

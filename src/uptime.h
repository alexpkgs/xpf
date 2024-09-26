#ifndef UPTIME_H
#define UPTIME_H

#include <stdbool.h>

bool get_system_uptime(long *uptime_seconds);
void format_uptime(long uptime_seconds);

#endif // UPTIME_H

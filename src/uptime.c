#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <unistd.h>
#include "uptime.h"

bool get_system_uptime(long *uptime_seconds) {
#if defined(__linux__) || defined(__GNU__)
    FILE *file = fopen("/proc/uptime", "r");
    if (file == NULL)
        return false;
    
    double uptime;
    if (fscanf(file, "%lf", &uptime) != 1) {
        fclose(file);
        return false;
    }
    fclose(file);
    *uptime_seconds = (long)uptime;
    return true;
#elif defined(__APPLE__)
    int mib[2];
    mib[0] = CTL_KERN;
    mib[1] = KERN_BOOTTIME;
    struct timeval boottime;
    size_t length = sizeof(boottime);
    
    if (sysctl(mib, 2, &boottime, &length, NULL, 0) < 0)
        return false;

    struct timeval now;
    gettimeofday(&now, NULL);
    *uptime_seconds = now.tv_sec - boottime.tv_sec;
    return true;
#else
    return false;
#endif
}

void format_uptime(long uptime_seconds) {
    int days = uptime_seconds / (60 * 60 * 24);
    int hours = (uptime_seconds % (60 * 60 * 24)) / (60 * 60);
    int minutes = (uptime_seconds % (60 * 60)) / 60;
    
    if (days > 0) {
        printf("%dd ", days);
    }
    if (hours > 0) {
        printf("%dh ", hours);
    }
    printf("%dm\n", minutes);
}

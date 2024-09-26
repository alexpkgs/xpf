#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <unistd.h>

#define write_str(fd, str) write(fd, str, strlen(str))

char ascii_logo[] = 
    "     ___    \n"
    "     (. ·|   \n"
    "     (<> |   \n"
    "    / __  \\  \n"
    "   ( /  \\ /|  \n"
    "  _/\\ __)/_)  \n"
    "  \\/-____\\/   \n";

int main() {
    struct utsname kernel;
    uname(&kernel);

    int fd = STDOUT_FILENO;

    // Print ASCII logo
    write_str(fd, ascii_logo);

    // dist
    int osrelease = open("/etc/os-release", O_RDONLY);
    char osline[256];
    ssize_t n;
    while ((n = read(osrelease, osline, sizeof(osline) - 1)) > 0) {
        osline[n] = '\0';
        char *line = strtok(osline, "\n");
        while (line != NULL) {
            if (strstr(line, "ID=") && !strstr(line, "\"")) {
                char *distro = strtok(line, "=");
                distro = strtok(NULL, "=");
                write_str(fd, "\n");
                write_str(fd, "Distro: ");
                write_str(fd, distro);
                break;
            }
            line = strtok(NULL, "\n");
        }
    }
    close(osrelease);

    // Kernel version
    write_str(fd, "\nKernel: ");
    write_str(fd, kernel.release);
    write_str(fd, "\n");

    // Uptime
    write_str(fd, "Uptime: ");
    int puptime = open("/proc/uptime", O_RDONLY);
    char uptime_buf[128];
    double uptime;
    read(puptime, uptime_buf, sizeof(uptime_buf));
    sscanf(uptime_buf, "%lf", &uptime);
    close(puptime);

    uint8_t days = (int)uptime / (60 * 60 * 24);
    uint8_t hours = ((int)uptime % (60 * 60 * 24)) / (60 * 60);
    uint8_t minutes = ((int)uptime % (60 * 60)) / 60;
    char uptime_str[64];
    if (days > 0) {
        snprintf(uptime_str, sizeof(uptime_str), "%dd ", days);
        write_str(fd, uptime_str);
    }
    if (hours > 0) {
        snprintf(uptime_str, sizeof(uptime_str), "%dh ", hours);
        write_str(fd, uptime_str);
    }
    snprintf(uptime_str, sizeof(uptime_str), "%dm\n", minutes);
    write_str(fd, uptime_str);

    // Memory info
    int meminfo = open("/proc/meminfo", O_RDONLY);
    char line[256];
    int total_memory = 0;
    int free_memory = 0;
    while ((n = read(meminfo, line, sizeof(line) - 1)) > 0) {
        line[n] = '\0';
        char *line_ptr = strtok(line, "\n");
        while (line_ptr != NULL) {
            if (strncmp(line_ptr, "MemTotal:", 9) == 0) {
                sscanf(line_ptr + 9, "%d", &total_memory);
            } else if (strncmp(line_ptr, "MemAvailable:", 13) == 0) {
                sscanf(line_ptr + 13, "%d", &free_memory);
            }
            line_ptr = strtok(NULL, "\n");
        }
    }
    close(meminfo);

    char mem_str[128];
    snprintf(mem_str, sizeof(mem_str), "Memory: %.2fGiB / %.2fGiB\n\n\x1b[0m", 
             (double)(total_memory - free_memory) / 1048576, 
             (double)total_memory / 1048576);
    write_str(fd, mem_str);

    return 0;
}

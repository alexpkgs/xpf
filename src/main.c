#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "config.h" // Include the config header

// ASCII Art for Tux
const char *tuxLogo =
"      ___\n"
"     (. ·|\n"
"     (<> |\n"
"    / __  \\\n"
"   ( /  \\ /|\n"
"  _/\\ __)/_)\n"
"  \\/-____\\/";

// ASCII Art for FreeBSD
const char *freebsdLogo =
"⢿⣷⣦⣶⣶⣶⣖⣤⣶⡿\n"
"⢨⣾⣿⣿⣿⣿⣷⣟⡿⡅\n"
"⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
"⠘⣿⣿⣿⣿⣿⣿⣿⣿⠏\n"
"⠀⠈⠻⢿⣿⣿⡿⠟⠋⠀\n";

// ASCII Art for macOS
const char *macLogo[] = {
    "          ",
    "\033[38;5;034m     .    \033[0m\n",
    "\033[38;5;226m    ..    \033[0m\n",
    "\033[38;5;208m........  \033[0m\n",
    "\033[38;5;196m.......   \033[0m\n",
    "\033[38;5;129m........  \033[0m\n",
    "\033[38;5;038m ......   \033[0m\n"
};

// Function to execute a shell command and return its output
char* execCommand(const char *command) {
    FILE *fp;
    char *result = malloc(256);
    char buffer[256];
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        free(result);
        exit(EXIT_FAILURE);
    }

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strncpy(result, buffer, 255);
        result[255] = '\0'; // Ensure null termination
    } else {
        strcpy(result, "Unknown");
    }

    pclose(fp);
    return result;
}

// Function to trim whitespace from a string
char* trimWhitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate after the last non-space character
    *(end + 1) = '\0';

    return str;
}

// Function to retrieve the OS name
char* checkOS() {
    char *osRelease = execCommand("cat /etc/os-release 2>/dev/null");
    char *osName = strstr(osRelease, "NAME=");
    if (osName) {
        osName += 6; // Move past "NAME=\""
        char *endQuote = strchr(osName, '"');
        if (endQuote) *endQuote = '\0';
        char *trimmed = strdup(trimWhitespace(osName));
        free(osRelease);
        return trimmed;
    }
    free(osRelease);
    return strdup("Unknown OS");
}

// Function to check if the OS is macOS
int isMacOS() {
    return strstr(checkOS(), "macOS") != NULL;
}

// Function to retrieve the shell being used
char* checkShell() {
    char *shell = execCommand("basename $SHELL");
    return shell; // Could be NULL or a valid string
}

// Function to retrieve the terminal type
char* checkTerminal() {
    char *term = getenv("TERM");
    return term ? strdup(trimWhitespace(term)) : NULL; // Return NULL if not found
}

// Function to retrieve the window manager
char* checkWindowManager() {
    char *wm = getenv("XDG_CURRENT_DESKTOP");
    return wm ? strdup(trimWhitespace(wm)) : NULL; // Return NULL if not found
}

// Function to retrieve CPU information
char* checkCPU() {
    char *cpuInfo = execCommand("lscpu | grep 'Model name:'");
    char *modelName = strstr(cpuInfo, ":");
    if (modelName) {
        modelName += 2; // Move past ": "
        char *trimmed = strdup(trimWhitespace(modelName));
        free(cpuInfo);
        return trimmed;
    }
    free(cpuInfo);
    return NULL; // Return NULL if not found
}

// Function to print the label and value
void printInfo(const char *label, const char *value, int show) {
    if (show) {
        if (value) {
            printf("%s: %s\n", label, value);
            free((char *)value); // Free the allocated memory for the value
        } else {
            // Fake error messages for specific cases
            if (strcmp(label, "sh") == 0) {
                printf("Error (00x02): how on earth\n");
            } else if (strcmp(label, "cpu") == 0) {
                printf("Error (023x9): why you not using an cpu i will blow your computer up\n");
            } else if (strcmp(label, "te") == 0) {
                printf("Error (9x02): okay how on earth you running this fetch without an terminal? eh\n");
            } else if (strcmp(label, "wm") == 0) {
                printf("Error (212x92): no wm?\n");
            } else {
                printf("%s: Unknown\n", label);
            }
        }
    }
}

// Main function
int main() {
    // Display the appropriate logo
    char *os = checkOS();
    if (strcmp(os, "FreeBSD") == 0) {
        printf("%s", freebsdLogo);
    } else if (isMacOS()) {
        for (int i = 0; i < sizeof(macLogo) / sizeof(macLogo[0]); i++) {
            printf("%s", macLogo[i]);
        }
    } else {
        printf("%s", tuxLogo);
    }

    // Free the OS string after checking
    free(os);

    // Check and print system information based on config
    printInfo("os", checkOS(), Config.ShowOS);
    
    // Call to checkShell that may return NULL
    char *shell = checkShell();
    printInfo("sh", shell, Config.ShowShell);
    
    // Call to checkTerminal that may return NULL
    char *terminal = checkTerminal();
    printInfo("te", terminal, Config.ShowTerminal);
    
    // Call to checkWindowManager that may return NULL
    char *wm = checkWindowManager();
    printInfo("wm", wm, Config.ShowWM);
    
    // Call to checkCPU that may return NULL
    char *cpu = checkCPU();
    printInfo("cpu", cpu, Config.ShowCPU);

    return 0;
}

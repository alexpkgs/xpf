#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Configuration structure
typedef struct {
    bool ShowOS;
    bool ShowShell;
    bool ShowTerminal;
    bool ShowWM;
    bool ShowCPU;
} Config;

// Function to return the default configuration
Config DefaultConfig() {
    Config config;
    config.ShowOS = false;
    config.ShowShell = true;
    config.ShowTerminal = false;
    config.ShowWM = true;
    config.ShowCPU = true;
    return config;
}

#endif // CONFIG_H


#include "config/config_manager.h"

#include <iostream>

#define CONFIG_PATH "./config.txt"

int main() {
    // init config manager
    ConfigManager &config_manager = ConfigManager::get_instance();
    if (config_manager.config_read(CONFIG_PATH) != CONFIG_OK) {
        return 0;
    }

    std::cout << config_manager.read_int("num") << std::endl;
    std::cout << config_manager.read_string("path") << std::endl;
    return 0;
}

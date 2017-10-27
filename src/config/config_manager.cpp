#include "config/config_manager.h"

static std::string trim(const std::string& str) {
    size_t l_index = 0;  
    for (l_index = 0; l_index < str.size(); l_index++) {
        if (str[l_index] != ' ') {
            break;
        }
    }   
    size_t r_index = 0;
    for (r_index = str.size() - 1; r_index >= 0; r_index--) {
        if (str[r_index] != ' ') {
            break;
        }
    }   

    if (l_index > r_index) {
        return ""; 
    }  
    return str.substr(l_index, r_index + 1); 
}


int ConfigManager::config_read(const char *path) {
    FILE *file;
    if ((file = fopen(path, "r")) == NULL) {
        return CONFIG_PATH_NO_EXIST;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#') {
            continue;
        }
        std::string content(line);       
        std::size_t index = content.find('=', 0);
        if (index == std::string::npos) {
            continue;
        }
        std::string key = content.substr(0, index); 
        std::string value = content.substr(index + 1);
        if (value.size() <= 0) {
            continue;
        }
        if (value[value.size() - 1] == '\n') {
            value.pop_back();
            //value.erase(value.size() - 1);
        }
        config_map_.insert(std::pair<std::string, std::string>(trim(key), trim(value))); 
    }

    fclose(file);
    return CONFIG_OK;
}

std::string ConfigManager::read_string(const std::string &key, std::string default_value) {
    std::map<std::string, std::string>::iterator it = config_map_.find(key);
    if (it != config_map_.end()) {
        return it->second;
    }
    return default_value;
}

int ConfigManager::read_int(const std::string &key, int default_value) {
    return read_value(key, default_value);
}

float ConfigManager::read_float(const std::string &key, float default_value) {
    return read_value(key, default_value);
}

double ConfigManager::read_double(const std::string &key, double default_value) {
    return read_value(key, default_value);
}

long ConfigManager::read_long(const std::string &key, long default_value) {
    return read_value(key, default_value);
}


bool ConfigManager::read_bool(const std::string &key, bool default_value) {
    std::map<std::string, std::string>::iterator it = config_map_.find(key);
    if (it != config_map_.end()) {
        std::stringstream ss; 
        ss << std::boolalpha << it->second;
        bool result;
        ss >> result;
        return result;
    }   
    return default_value;
}


#ifndef CONFIG_CONFIG_MANAGER_H_
#define CONFIG_CONFIG_MANAGER_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>

typedef enum {
    CONFIG_OK = 0,
    CONFIG_PATH_NO_EXIST,
    CONFIG_ERROR
} CONFIG_STATE;
/**
 * 配置管理器
 */
class ConfigManager {
 public:
    ConfigManager() {}
    virtual ~ConfigManager() {}
    
    static ConfigManager& get_instance() {
        static ConfigManager instance;
        return instance;
    }

    int config_read(const char *path);
    std::string read_string(const std::string &key, std::string default_value="");
    int read_int(const std::string &key, const int default_value=0);
    float read_float(const std::string &key, const float default_value=0.0f);
    double read_double(const std::string &key, const double default_value=0.0);
    bool read_bool(const std::string &key, const bool default_value=false);
    long read_long(const std::string &key, const long default_value=0l);

 private:
    template<typename T> T read_value(const std::string &key, T default_value);
    std::map<std::string, std::string> config_map_;
    
};

template<typename T> T ConfigManager::read_value(const std::string &key, T default_value) {
    std::map<std::string, std::string>::iterator it = config_map_.find(key);
    if (it != config_map_.end()) {
        std::stringstream ss; 
        ss << it->second;
        T result = 0;
        ss >> result;
        return result;
    }   
    return default_value;
}

#endif

#ifndef JSON_JSON_H_
#define JSON_JSON_H_

#include <jansson.h>

#include <map>
#include <vector>
#include <string>

typedef enum {
    JSON_ERROR = -1,
    JSON_OK = 0,
    JSON_EXIST_ROOT,
    JSON_NODE_NO_EXIST
} JSON_STATE;

typedef int json_error_type;

#define array_foreach(array, index, value) \
    for(index = 0; \
        index < array->array_size() && (value = array->json_array(index)); \
        index++)

class JsonObject {
 public:
    JsonObject() : root_(NULL), is_root_(false), next_(NULL){};
    ~JsonObject() {
        if (is_root_) {
            if (root_) json_decref(root_);
        }

        if (next_) {
            delete next_;
        }
    }

    /**
     * loads content with json format
     * success with 0 (JSON_Ok)
     */
    int loads(const std::string &content) {
        if (root_) return JSON_EXIST_ROOT;

        json_error_t error;
        root_ = json_loads(content.c_str(), 0, &error);
        if (root_ == NULL) {
            return -1;
        }
        is_root_ = true;
        return 0;
    }


    JsonObject *json_object(const std::string &key) {
        if (root_ == NULL) return NULL;
        json_t *node = json_object_get(root_, key.c_str());
        if (node == NULL) {
            return NULL;
        }
        JsonObject *new_json_object = new JsonObject();
        new_json_object->root_ = node;
        new_json_object->next_ = next_; 
        next_ = new_json_object;
        return new_json_object;
    }

    JsonObject *json_array(const int index) {
        if (root_ == NULL) return NULL;
        json_t *node = json_array_get(root_, index);
        if (node == NULL) {
            return NULL;
        }

        JsonObject *new_json_object = new JsonObject();
        new_json_object->root_ = node;
        new_json_object->next_ = next_;
        next_ = new_json_object;
        return new_json_object;
    }

    const int array_size() {
        if (root_ == NULL) {
            return -1;
        }
        return json_array_size(root_);
    }

    /**
     * on success, type value is 0, on error, type value is -1 
     */
    const char *string_value(json_error_type *type) {
        if (root_ == NULL) {
            if (type) *type = JSON_ERROR;
            return NULL;
        }

        if (type) *type = JSON_OK;
        return json_string_value(root_);
    }

    /**
     * on success, type value is 0, on error, type value is -1 
     */
    const char *string_value(const std::string &key, json_error_type *type) {
        if (root_ == NULL) { 
            if (type) *type = JSON_ERROR;
            return NULL;
        }
        json_t *node = json_object_get(root_, key.c_str());
        if (node == NULL) {
            if (type) *type = JSON_ERROR;
            return NULL;
        }
    
        if (type) *type = JSON_OK;
        return json_string_value(node);
    }

    /**
     * on success, type value is 0, on error, type value is -1
     */
    const int int_value(const std::string &key, json_error_type *type) {
        if (root_ == NULL) {
            if (type) *type = JSON_ERROR;
            return -1;
        }
        json_t *node = json_object_get(root_, key.c_str());
        if (node == NULL) {
            if (type) *type = JSON_ERROR;
            return -1;
        }
    
        if (type) *type = JSON_OK;
        return json_integer_value(node);
    }

 private:
    json_t *root_;
    bool is_root_;
    JsonObject *next_;
};

typedef enum {
    JSONDATA_OBJECT,
    JSONDATA_ARRAY,
    JSONDATA_INT,
    JSONDATA_STRING
} JSONDATA_TYPE;

class JsonData {
 public:
    JsonData() {
        new(this) JsonData(NULL, "root");
    } 
    JsonData(json_t *parent, std::string key)
        : key_(key),
          index_(0),
          is_array_item_(false) {
        init_other_param(parent);
    }
    JsonData(json_t *parent, size_t index)
        : key_(""),
          index_(index),
          is_array_item_(true) {
        init_other_param(parent);
    }
    ~JsonData() {
        if (is_root_ && root_) json_decref(root_);
        /* free dumps content list  */
        for (int i = 0; i < dumps_content_list_.size(); i++) {
            free(dumps_content_list_[i]);
        }
        
        reset_data();
    }

    void init_other_param(json_t *parent) {
        if (parent) {
            /* not root */
            is_root_ = false;
            parent_ = parent;
        } else {
            is_root_ = true;
            parent_ = NULL;
        }
        root_ = json_object();
        json_type_ = JSONDATA_OBJECT;
        str_ = "";
        num_ = 0;
    }

    JsonData &operator[](const std::string &key) {
        std::map<std::string, JsonData*>::iterator it; 
        it = jsondata_map_.find(key);
        if (it == jsondata_map_.end()) {
            /* if current is not object type, change object type */
            if (json_type_ != JSONDATA_OBJECT) {
                json_t *old_root = root_;
                root_ = json_object();
                if (parent_) {
                    /* not root */
                    json_object_set_new(parent_, key_.c_str(), root_);
                } else {
                    /* root */
                    json_decref(old_root);
                }
                json_type_ = JSONDATA_OBJECT;
                reset_data();
            }

            /* new node */
            JsonData *new_data = new JsonData(root_, key); 
            jsondata_map_.insert(std::pair<std::string, JsonData*>(key, new_data));
            json_object_set_new(root_, key.c_str(), new_data->root_);
            return *new_data;
        }

        return *(it->second);
    }

    JsonData &operator[](const size_t index) {
        /* if current is not array type, change array type */
        if (json_type_ != JSONDATA_ARRAY) {
            json_t *old_root = root_;
            root_ = json_array();
            if (parent_) {
                /* not root */
                json_object_set_new(parent_, key_.c_str(), root_);
            } else {
                /* root */
                json_decref(old_root);
            }
            json_type_ = JSONDATA_ARRAY;
            reset_data();
        }

        /* ensure array size */
        resize_jsonarray(index);
       
        if (jsondata_list_.size() <= index) {
            JsonData *data = new JsonData(root_, index);
            jsondata_list_.push_back(data);
            json_array_append_new(root_, data->root_);
        }
        return *jsondata_list_[index];
    }

    JsonData &operator=(const std::string &value) {
        json_type_ = JSONDATA_STRING;
        str_ = value;
        root_ = json_string(value.c_str());
        if (!is_array_item()) {
            json_object_set_new(parent_, key_.c_str(), root_);
        } else {
            json_array_set_new(parent_, index_, root_);
        }
        return *this;
    } 

    JsonData &operator=(const int value) {
        json_type_ = JSONDATA_INT;
        num_ = value;
        root_ = json_integer(value);
        if (!is_array_item()) {
            json_object_set_new(parent_, key_.c_str(), root_);
        } else {
            json_array_set_new(parent_, index_, root_);
        } 
        return *this;
    }

    const char *dumps() {
        char *dumps_content = json_dumps(root_, JSON_INDENT(0)); 
        if (dumps_content) {
            dumps_content_list_.push_back(dumps_content);
        }
        return dumps_content;
    }

 private:
    JsonData(const JsonData&);  
    JsonData &operator=(const JsonData&);   

 protected:
    void free_jsondata_map() {
        /* free jsonData map*/
        std::map<std::string, JsonData*>::iterator it;
        for (it = jsondata_map_.begin(); it != jsondata_map_.end(); it++) {
            if (it->second) {
                delete it->second;
            }
        }   
        jsondata_map_.clear();
    }

    void free_jsondata_list() {
        /* free jsondata list */
        for (int i = 0; i < jsondata_list_.size(); i++) {
            if (jsondata_list_[i]) {
                delete jsondata_list_[i];
            }
        }
    }
    
    void reset_data() {
        free_jsondata_map();
        free_jsondata_list();
    }

    void resize_jsonarray(size_t size) {
        if (!root_) return;
    
        size_t csize = json_array_size(root_);
        if (csize < size) {
            for (size_t i = csize; i < size; i++) {
                json_array_append_new(root_, json_integer(0));
                jsondata_list_.push_back(NULL);
            }
        }
    }

    bool is_array_item() {
        return is_array_item_;
    }

    json_t *parent_;
    json_t *root_;
    bool is_root_;
    JSONDATA_TYPE json_type_;
    std::string key_;
    size_t index_;
    bool is_array_item_;
    std::string str_;
    int num_;
    std::map<std::string, JsonData*> jsondata_map_;
    std::vector<JsonData*> jsondata_list_;
    std::vector<char*> dumps_content_list_;

};

#endif

#ifndef __JSON_ITEM_H__
#define __JSON_ITEM_H__

#include <string>
#include <memory>
#include <json/json.h>

namespace JSON_SERVICE{
    class JsonItem;
    
    typedef std::shared_ptr<JsonItem> SJsonItem;
    class JsonItem
    {
        public:
            JsonItem();
            ~JsonItem();

            SJsonItem add(std::string fieldName, int fieldValue);  
            SJsonItem add(std::string fieldName, float fieldValue);  
            SJsonItem add(std::string fieldName, double fieldValue);  
            SJsonItem add(std::string fieldName, long fieldValue);
            SJsonItem add(std::string fieldName, bool fieldValue);  
            SJsonItem add(std::string fieldName, std::string fieldValue);  
            SJsonItem add(std::string fieldName, const char* fieldValue);  
            SJsonItem add(std::string fieldName, SJsonItem fieldValue);
            void dump();
        private:
            std::shared_ptr<Json::Value> SJsonRoot;
    };


}

#endif
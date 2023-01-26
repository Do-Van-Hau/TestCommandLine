#ifndef _JSON_FILE_LIB_
#define _JSON_FILE_LIB_

#include <json-for-modern-cpp/json.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"
#include "Error.h"
#include <utils/RefBase.h>
#include <services/ConfigurationManagerService/IConfigurationManagerServiceType.h>

#include "ConfigManagerDefine.h"

class JsonFileLib : public android::RefBase {
public:
    JsonFileLib();
    virtual ~JsonFileLib();
    error_t readJsonFile(std::string fileName);
    ConfigResponse getJsonValueSet(std::string dataName);
    ConfigResponse getJsonDataSet(std::string dataName);
    bool ckeckJsonFile(std::string fileName);
private :
    nlohmann::json mJsonData;
    std::string objName;
    std::string objPath;
};

#endif // _JSON_FILE_LIB_
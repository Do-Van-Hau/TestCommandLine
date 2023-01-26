#include "include/JsonFileLib.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "ConfigMgrJson"
#endif

JsonFileLib::JsonFileLib() {
    LOGI("%s ctr", __func__);
}

JsonFileLib::~JsonFileLib() {}

error_t JsonFileLib::readJsonFile(std::string fileName) {
    using json = nlohmann::json;
    int32_t err = E_ERROR;

    if(!ckeckJsonFile(fileName)) {
        LOGI("%s ckeckJsonFile ERROR", __func__);
        return err;
    }

    objPath = fileName;
    std::ifstream is;
    is.open(fileName, std::ios::in | std::ios::out);

    try {
        json j;
        j = json::parse(is);
        // maybe madatory field
        auto count = static_cast<std::size_t>(0);
        count += j.count("globalServerLink");
        count += j.count("mqttInfo");
        count += j.count("mqttHSF");
        count += j.count("servicePackage");
        if(count != 4) {
            LOGE("%s, mandatory field error", __func__);
       }
        int32_t FILE_SIZE = 256;
        char path[FILE_SIZE] = {0,};
        int dirPath = fileName.rfind("/") + 1;
        char* defaultPath = CONFIGMGR_DIR_PATH;
        std::string namePath = fileName.substr(dirPath, fileName.length() - dirPath);
        snprintf(path, FILE_SIZE, "%s%s", defaultPath, namePath.c_str());
        if( access( path, F_OK ) != -1 ) {
            LOGV("%s %s's file exist!(need update)", __func__, path);
        } else {
            LOGV("%s %s's file doesn't exist!(create this file)", __func__, path);
        }
        mJsonData = j;
        std::ofstream fout;
        fout.open(path, std::ios::in | std::ios::out);
        fout << j;
        fout.close();
        err = E_OK;
    } catch(std::logic_error& e) {
        LOGE("%s, %s path error", __func__, fileName.c_str());
        goto finished;
    } catch(...) {
        LOGE("%s, parsing error!!!", __func__);
        goto finished;
    }
finished:
    is.close();
    LOGI("%s end", __func__);
    return err;
}

ConfigResponse JsonFileLib::getJsonValueSet(std::string dataName) {
    ConfigResponse r;
    r.sourceType = DataFrom::ProvisionedData;
    r.result = ConfigResult::ObjectError;
    std::string jsonPath = PROVISIONING_JSON_PATH;
    if (mJsonData == nullptr) {
       bool checkReadJsonFile = readJsonFile(jsonPath);
    }
    if (checkReadJsonFile) {
        r.result = ConfigResult::DataNotFound;
        try {
            json j = mJsonData;
            r.mJsonData = mJsonData;

            if((dataName.compare("globalServerLink"))==0) {
                if(j.count("globalServerLink") > 0) {
                    auto depth1_globalServerLink = j.at("globalServerLink");
                    r.mJsonData = depth1_globalServerLink;
                    r.data[dataName] = depth1_globalServerLink;
                    r.result = ConfigResult::NoError;
                }
            }
            if((dataName.compare("mqttInfo"))==0) {
                if(j.count("mqttInfo") > 0) {
                    auto depth1_mqttInfo = j.at("mqttInfo");
                    auto depth2_mqttInfo_host = j.at("mqttInfo").at("host");
                    int32_t depth2_mqttInfo_port = j.at("mqttInfo").at("port");
                    int32_t depth2_mqttInfo_keepalive = j.at("mqttInfo").at("keepalive");
                    auto depth3_mqttInfo_topic_event = j.at("mqttInfo").at("topic").at("event");
                    auto depth3_mqttInfo_topic_shoulderTap = j.at("mqttInfo").at("topic").at("shoulderTap");
                    auto depth3_mqttInfo_topic_hsf = j.at("mqttInfo").at("topic").at("hsf");
                    r.mJsonData = depth1_mqttInfo;
                    r.data["host"] = depth2_mqttInfo_host;
                    r.data["port"] = std::to_string(depth2_mqttInfo_port);
                    r.data["keepalive"] = std::to_string(depth2_mqttInfo_keepalive);
                    r.data["topic"] = "";
                    r.data["topic>event"] = depth3_mqttInfo_topic_event;
                    r.data["topic>shoulderTap"] = depth3_mqttInfo_topic_shoulderTap;
                    r.data["topic>hsf"] = depth3_mqttInfo_topic_hsf;
                    r.result = ConfigResult::NoError;
                }
            }
            if((dataName.compare("mqttHSF"))==0) {
                if(j.count("mqttHSF") > 0) {
                    auto depth1_mqttHSF = j.at("mqttHSF");
                    auto depth2_mqttHSF_command = j.at("mqttHSF").at("command");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_CWL = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_CWL");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_IG = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_IG");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_TIGCY = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_TIGCY");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_IGONA = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_IGONA");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLA = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_GNSSLA");
                    auto depth4_mqttHSF_cyclicData_uploadRate_NAVI_LA = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("NAVI_LA");
                    auto depth4_mqttHSF_cyclicData_uploadRate_YOPN_LA = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("YOPN_LA");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLO = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_GNSSLO");
                    auto depth4_mqttHSF_cyclicData_uploadRate_NAVI_LO = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("NAVI_LO");
                    auto depth4_mqttHSF_cyclicData_uploadRate_YOPN_LO = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("YOPN_LO");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_GNSSDI = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_GNSSDI");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_GNSSSP = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_GNSSSP");
                    auto depth4_mqttHSF_cyclicData_uploadRate_VSPEED = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("VSPEED");
                    auto depth4_mqttHSF_cyclicData_uploadRate_T_GNSSAC = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("T_GNSSAC");
                    auto depth4_mqttHSF_cyclicData_uploadRate_BR_PRES = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("BR_PRES");
                    auto depth4_mqttHSF_cyclicData_uploadRate_APDL_POS = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("APDL_POS");
                    auto depth4_mqttHSF_cyclicData_uploadRate_AC_TEMP = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("AC_TEMP");
                    auto depth4_mqttHSF_cyclicData_uploadRate_SUN_VLM = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("SUN_VLM");
                    auto depth4_mqttHSF_cyclicData_uploadRate_AT_PRES = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("AT_PRES");
                    auto depth4_mqttHSF_cyclicData_uploadRate_CR_DI2E = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("CR_DI2E");
                    auto depth4_mqttHSF_cyclicData_uploadRate_CR_DI2EU = j.at("mqttHSF").at("cyclicData").at("uploadRate").at("CR_DI2EU");
                    auto depth4_mqttHSF_eventData_captureRate_T_IGONTI = j.at("mqttHSF").at("eventData").at("captureRate").at("T_IGONTI");
                    auto depth4_mqttHSF_eventData_captureRate_T_IGOFTI = j.at("mqttHSF").at("eventData").at("captureRate").at("T_IGOFTI");
                    auto depth4_mqttHSF_eventData_captureRate_PARK_BR = j.at("mqttHSF").at("eventData").at("captureRate").at("PARK_BR");
                    auto depth4_mqttHSF_eventData_captureRate_AIRBAG = j.at("mqttHSF").at("eventData").at("captureRate").at("AIRBAG");
                    auto depth4_mqttHSF_eventData_captureRate_BR_EBB = j.at("mqttHSF").at("eventData").at("captureRate").at("BR_EBB");
                    auto depth4_mqttHSF_eventData_captureRate_LIT_HZSW = j.at("mqttHSF").at("eventData").at("captureRate").at("LIT_HZSW");
                    auto depth4_mqttHSF_eventData_captureRate_VSA_TCS = j.at("mqttHSF").at("eventData").at("captureRate").at("VSA_TCS");
                    auto depth4_mqttHSF_eventData_captureRate_RAIN_BCM = j.at("mqttHSF").at("eventData").at("captureRate").at("RAIN_BCM");
                    auto depth4_mqttHSF_eventData_captureRate_RAIN_RLS = j.at("mqttHSF").at("eventData").at("captureRate").at("RAIN_RLS");
                    auto depth4_mqttHSF_eventData_captureRate_FUEL_LW = j.at("mqttHSF").at("eventData").at("captureRate").at("FUEL_LW");
                    r.mJsonData = depth1_mqttHSF;
                    r.data["command"] = depth2_mqttHSF_command;
                    r.data["cyclicData"] = "";
                    r.data["cyclicData>uploadRate"] = "";
                    r.data["cyclicData>uploadRate>T_CWL"] = (depth4_mqttHSF_cyclicData_uploadRate_T_CWL.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_CWL : "";
                    r.data["cyclicData>uploadRate>T_IG"] = (depth4_mqttHSF_cyclicData_uploadRate_T_IG.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_IG : "";
                    r.data["cyclicData>uploadRate>T_TIGCY"] = (depth4_mqttHSF_cyclicData_uploadRate_T_TIGCY.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_TIGCY : "";
                    r.data["cyclicData>uploadRate>T_IGONA"] = (depth4_mqttHSF_cyclicData_uploadRate_T_IGONA.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_IGONA : "";
                    r.data["cyclicData>uploadRate>T_GNSSLA"] = (depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLA.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLA : "";
                    r.data["cyclicData>uploadRate>NAVI_LA"] = (depth4_mqttHSF_cyclicData_uploadRate_NAVI_LA.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_NAVI_LA : "";
                    r.data["cyclicData>uploadRate>YOPN_LA"] = (depth4_mqttHSF_cyclicData_uploadRate_YOPN_LA.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_YOPN_LA : "";
                    r.data["cyclicData>uploadRate>T_GNSSLO"] = (depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLO.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_GNSSLO : "";
                    r.data["cyclicData>uploadRate>NAVI_LO"] = (depth4_mqttHSF_cyclicData_uploadRate_NAVI_LO.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_NAVI_LO : "";
                    r.data["cyclicData>uploadRate>YOPN_LO"] = (depth4_mqttHSF_cyclicData_uploadRate_YOPN_LO.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_YOPN_LO : "";
                    r.data["cyclicData>uploadRate>T_GNSSDI"] = (depth4_mqttHSF_cyclicData_uploadRate_T_GNSSDI.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_GNSSDI : "";
                    r.data["cyclicData>uploadRate>T_GNSSSP"] = (depth4_mqttHSF_cyclicData_uploadRate_T_GNSSSP.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_GNSSSP : "";
                    r.data["cyclicData>uploadRate>VSPEED"] = (depth4_mqttHSF_cyclicData_uploadRate_VSPEED.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_VSPEED : "";
                    r.data["cyclicData>uploadRate>T_GNSSAC"] = (depth4_mqttHSF_cyclicData_uploadRate_T_GNSSAC.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_T_GNSSAC : "";
                    r.data["cyclicData>uploadRate>BR_PRES"] = (depth4_mqttHSF_cyclicData_uploadRate_BR_PRES.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_BR_PRES : "";
                    r.data["cyclicData>uploadRate>APDL_POS"] = (depth4_mqttHSF_cyclicData_uploadRate_APDL_POS.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_APDL_POS : "";
                    r.data["cyclicData>uploadRate>AC_TEMP"] = (depth4_mqttHSF_cyclicData_uploadRate_AC_TEMP.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_AC_TEMP : "";
                    r.data["cyclicData>uploadRate>SUN_VLM"] = (depth4_mqttHSF_cyclicData_uploadRate_SUN_VLM.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_SUN_VLM : "";
                    r.data["cyclicData>uploadRate>AT_PRES"] = (depth4_mqttHSF_cyclicData_uploadRate_AT_PRES.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_AT_PRES : "";
                    r.data["cyclicData>uploadRate>CR_DI2E"] = (depth4_mqttHSF_cyclicData_uploadRate_CR_DI2E.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_CR_DI2E : "";
                    r.data["cyclicData>uploadRate>CR_DI2EU"] = (depth4_mqttHSF_cyclicData_uploadRate_CR_DI2EU.size() > 0) ? depth4_mqttHSF_cyclicData_uploadRate_CR_DI2EU : "";
                    r.data["eventData"] = "";
                    r.data["eventData>captureRate"] = "";
                    r.data["eventData>captureRate>T_IGONTI"] = (depth4_mqttHSF_eventData_captureRate_T_IGONTI.size() > 0) ? depth4_mqttHSF_eventData_captureRate_T_IGONTI : "";
                    r.data["eventData>captureRate>T_IGOFTI"] = (depth4_mqttHSF_eventData_captureRate_T_IGOFTI.size() > 0) ? depth4_mqttHSF_eventData_captureRate_T_IGOFTI : "";
                    r.data["eventData>captureRate>PARK_BR"] = (depth4_mqttHSF_eventData_captureRate_PARK_BR.size() > 0) ? depth4_mqttHSF_eventData_captureRate_PARK_BR : "";;
                    r.data["eventData>captureRate>AIRBAG"] = (depth4_mqttHSF_eventData_captureRate_AIRBAG.size() > 0) ? depth4_mqttHSF_eventData_captureRate_AIRBAG : "";
                    r.data["eventData>captureRate>BR_EBB"] = (depth4_mqttHSF_eventData_captureRate_BR_EBB.size() > 0) ? depth4_mqttHSF_eventData_captureRate_BR_EBB : "";
                    r.data["eventData>captureRate>LIT_HZSW"] = (depth4_mqttHSF_eventData_captureRate_LIT_HZSW.size() > 0) ? depth4_mqttHSF_eventData_captureRate_LIT_HZSW : "";
                    r.data["eventData>captureRate>VSA_TCS"] = (depth4_mqttHSF_eventData_captureRate_VSA_TCS.size() > 0) ? depth4_mqttHSF_eventData_captureRate_VSA_TCS : "";
                    r.data["eventData>captureRate>RAIN_BCM"] = (depth4_mqttHSF_eventData_captureRate_RAIN_BCM.size() > 0) ? depth4_mqttHSF_eventData_captureRate_RAIN_BCM : "";
                    r.data["eventData>captureRate>RAIN_RLS"] = (depth4_mqttHSF_eventData_captureRate_RAIN_RLS.size() > 0) ? depth4_mqttHSF_eventData_captureRate_RAIN_RLS : "";
                    r.data["eventData>captureRate>FUEL_LW"] = (depth4_mqttHSF_eventData_captureRate_FUEL_LW.size() > 0) ? depth4_mqttHSF_eventData_captureRate_FUEL_LW : "";
                    r.result = ConfigResult::NoError;
                }
            }
            if((dataName.compare("servicePackage"))==0) {
                if(j.count("servicePackage") > 0) {
                    auto depth1_servicePackage = j.at("servicePackage");
                    auto depth2_servicePackage_categories = j.at("servicePackage").at("categories");
                    auto depth2_servicePackage_name = j.at("servicePackage").at("name");
                    auto depth2_servicePackage_services = j.at("servicePackage").at("services");
                    r.mJsonData = depth1_servicePackage;
                    r.data["categories"] = depth2_servicePackage_categories.dump();
                    r.data["name"] = depth2_servicePackage_name;
                    r.data["services"] = depth2_servicePackage_services.dump();
                    r.result = ConfigResult::NoError;
                }
            }
        } catch(std::logic_error& e) {
            LOGE("%s, %s find search error", __func__, dataName.c_str());
            r.result = ConfigResult::ObjectError;
        } catch(...) {
            LOGE("%s, parsing error!!!", __func__);
            r.result = ConfigResult::ObjectError;
        }
        is.close();
    } else {
        LOGI("%s readJsonFile else null", __func__);
        r.result = ConfigResult::DataNotFound;
    }
    if( ConfigResult::ObjectError == r.result ) {
        LOGI("%s readJsonFile json file read fail(error)", __func__);
    }
    return r;
}

ConfigResponse JsonFileLib::getJsonDataSet(std::string dataName) {
    ConfigResponse r;
    r.sourceType = DataFrom::ProvisionedData;
    r.result = ConfigResult::ObjectError;
    int32_t checkReadJsonFile = readJsonFile(dataName);
    if( E_OK == checkReadJsonFile ) {
        LOGI("%s readJsonFile result success %d", __func__, checkReadJsonFile);
        r.result = ConfigResult::NoError;
        using json = nlohmann::json;
        std::ifstream is;
        is.open(dataName, std::ios::in | std::ios::out);
        try {
            json j;
            j = json::parse(is);
            mJsonData = j;
            r.mJsonData = j;
        } catch(std::logic_error& e) {
            LOGE("%s, %s path error", __func__, dataName);
        } catch(...) {
            LOGE("%s, parsing error!!!", __func__);
        }
        is.close();
    } else {
        LOGI("%s readJsonFile result fail %d", __func__, checkReadJsonFile);
        r.result = ConfigResult::DataNotFound;
    }
    if( ConfigResult::ObjectError == r.result ) {
        LOGI("%s readJsonFile json file read fail(error)", __func__);
    }
    return r;
}

bool JsonFileLib::ckeckJsonFile(std::string fileName) {
    bool err = false;
    std::string path = ((fileName.compare(PROVISIONING_JSON_PATH)) == 0) ? PROVISIONING_JSON_PATH : fileName;
    if( access( path.c_str(), F_OK ) != -1 ) {
        LOGI("%s %s's file exist!", __func__, path.c_str());
        err = true;
    } else {
        LOGE("%s %s's file doesn't exist!", __func__, path.c_str());
        err = false;
    }
    return err;
}

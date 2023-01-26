/**
 * @copyright
 * Copyright (c) 2015 by LG Electronics Inc.
 * This program or software including the accompanying associated documentation
 * ("Software") is the proprietary software of LG Electronics Inc. and or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate written license agreement between you
 * and LG Electronics Inc. ("Authorized License"). Except as set forth in an
 * Authorized License, LG Electronics Inc. grants no license (express or implied),
 * rights to use, or waiver of any kind with respect to the Software, and LG
 * Electronics Inc. expressly reserves all rights in and to the Software and all
 * intellectual property therein. If you have no Authorized License, then you have
 * no rights to use the Software in any ways, and should immediately notify LG
 * Electronics Inc. and discontinue all use of the Software.
 * @file        DBManager.cpp
 * @brief       class for DBManager file process
 * @author
 *    copyright (c) 2018  Jaeho Choi <elliott.choi@lge.com>
 * @version     1.0.00
*/

#include "include/DBManager.h"
#include "include/DBBase.h"
#include "include/ConfigManager.h"
#include "Log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "ConfigMgrDBMgr"
#endif // LOG_TAG

DBManager::DBManager(android::sp<ConfigManager> _mConfigManager) {
    LOGV("%s", __func__);
    mConfigManager = _mConfigManager;
    mIsProvisioned = false;
    mDBBase = new DBBase(this);
#if USE_ONE_FILE == 1
    mXmlFileLib = new XmlFileLib();
    mJsonFileLib = new JsonFileLib();
#else //USE_ONE_FILE
    for(const auto sourceType : {DataFrom::General, DataFrom::ProvisionedData}) {
        LOGV("created %s", getStringFromDataFrom(sourceType).c_str());
        mXmlFileLib.push_back(new XmlFileLib(getStringFromDataFrom(sourceType)));
    }
#endif //USE_ONE_FILE
    mIsJsonFile = false;
}

DBManager::~DBManager() {}


ConfigResponse DBManager::getData(const DataFrom sourceType, const std::string& name) {
    ConfigResponse r;
    r.sourceType = sourceType;
    if(sourceType == DataFrom::ProvisioningData) {
        r = getDataSet(DataFrom::ProvisionedData, name);
        r.sourceType = DataFrom::ProvisionedData;
        LOGI("Result except default: %d", (int)r.result);
        if(ConfigResult::NoError != r.result) {
            LOGI("XML logic defaultProvData::%s ", __func__);
            r = getDataSet(DataFrom::DefaultProvData, name);
            r.sourceType = DataFrom::DefaultProvData;
        }
    } else {
        r = getDataSet(sourceType, name);
    }
    if(ConfigResult::NoError == r.result) {
        //LOGV("DBManager::%s getDataValueSet success name : %s", __func__, name.c_str());
    } else {
        LOGV("DBManager::%s getDataValueSet fail name : %s", __func__, name.c_str());
    }
    return r;
}

ConfigResponse DBManager::getDataSet(const DataFrom sourceType, const std::string& name) {
    ConfigResponse r;
    mIsJsonFile = false;
    if(mIsJsonFile && (sourceType == DataFrom::ProvisionedData)) {
        LOGI("DBManager::%s json logic", __func__);
        r = mJsonFileLib->getJsonValueSet(name);
    } else {
        LOGI("XML logic provisionedData::%s ", __func__);
    r = mXmlFileLib->getXmlValueSet(
        mXmlFileLib->makeNewDataName(getStringFromDataFrom(sourceType), name));
    }
    r.sourceType = sourceType;
    return r;
}

ConfigResult DBManager::getConfigData(
    const DataFrom sourceType, const std::string& name, std::string& value
) {
#if USE_ONE_FILE == 1
    ConfigResult result = mXmlFileLib->getXmlValueSingle
        (mXmlFileLib->makeNewDataName(getStringFromDataFrom(sourceType), name), value);
#else //USE_ONE_FILE
    ConfigResult result = mXmlFileLib[static_cast<int32_t>(sourceType)]->
        getXmlValueSingle(name, value);
#endif //USE_ONE_FILE
    if(ConfigResult::NoError == result) {
        //LOGV("DBManager::%s getXmlValueSingle success name : %s value : %s",
        //    __func__, name.c_str(), value.c_str());
    } else {
        LOGV("DBManager::%s getXmlValueSingle fail : %s value : %s",
            __func__, name.c_str(), value.c_str());
    }
    return result;
}

int32_t DBManager::setData(
    const DataFrom sourceType,
    const std::vector<std::pair<std::string, std::string>>& storeData
) {
    int32_t r = 0;
    mXmlFileLib->readXmlFile(mPathOfXmlFile);

    for (auto &a : storeData) {
        LOGI("received gen data is %s : %s", a.first.c_str(), a.second.c_str());
#if USE_ONE_FILE == 1
        if(E_OK == mXmlFileLib->addFromExternal(mXmlFileLib->
            makeNewDataName(getStringFromDataFrom(sourceType), a.first), a.second))
#else //USE_ONE_FILE
        if(E_OK == mXmlFileLib[static_cast<int32_t>(sourceType)]->
            addFromExternal(getStringFromDataFrom(sourceType), a.first), a.second))
#endif //USE_ONE_FILE
        {
            r++;
        }
    }

    if(0 != r) {
        if(DataFrom::ProvisionedData == sourceType) {
            mIsProvisioned = true;
        }
#if USE_ONE_FILE == 1
        mXmlFileLib->saveXmlFile(mPathOfXmlFile);
#else //USE_ONE_FILE
        mXmlFileLib[static_cast<int32_t>(sourceType)]->
        saveXmlFile(mPathOfXmlFile[static_cast<int32_t>(sourceType)]);
#endif //USE_ONE_FILE
    }
    postUpdateDB(getStringFromDataFrom(sourceType), true);
    return r;
}

error_t DBManager::setConfigData(
    const DataFrom sourceType, const std::string name, const std::string value
) {
    error_t result = E_OK;
#if USE_ONE_FILE == 1
    result = mXmlFileLib->addFromExternal(
        mXmlFileLib->makeNewDataName(getStringFromDataFrom(sourceType), name), value);
    LOGI("DBManager::%s addFromExternal name %s value %s parent %s(%d) result %d",
        __func__, name.c_str(), value.c_str(), getStringFromDataFrom(sourceType).c_str(),
        (int32_t) sourceType, result);
    mXmlFileLib->saveXmlFile(mPathOfXmlFile);
#else //USE_ONE_FILE
    result = mXmlFileLib[static_cast<int32_t>(sourceType)]->
        addFromExternal(name, value);
    mXmlFileLib[static_cast<int32_t>(sourceType)]->
        saveXmlFile(mPathOfXmlFile[static_cast<int32_t>(sourceType)]);
#endif //USE_ONE_FILE
    if(DataFrom::ProvisionedData == sourceType) {
        if(E_OK == result) {
            mIsProvisioned = true;
        }
    }
    return result;
}

bool DBManager::isProvisioned(void) {
    if(mIsProvisioned) {
        error_t result = mJsonFileLib->ckeckJsonFile(PROVISIONING_JSON_PATH);
        mIsProvisioned = (result == true) ? true : false;
    }
    return mIsProvisioned;
}

void DBManager::createDB() {
    mXmlFileLib->readXmlFile(mPathOfXmlFile);
    mIsJsonFile = mJsonFileLib->ckeckJsonFile(mPathOfJsonFile);
    if(mIsJsonFile) {
        mJsonFileLib->readJsonFile(mPathOfJsonFile);
        mIsProvisioned = true;
    }
    if(true == mXmlFileLib->isValueAvailable(
        getStringFromDataFrom(DataFrom::ProvisionedData)
    )) {
        mIsProvisioned = true;
    }
}

void DBManager::postUpdateDB(std::string name, bool updateFromSetData) {
    (void) updateFromSetData;
    DataFrom sourceType = DataFrom::Default;
    if((name.compare("General"))==0) { sourceType = DataFrom::General; }
    else if((name.compare("DefaultProvData"))==0) { sourceType = DataFrom::DefaultProvData; }
    else if((name.compare("ProvisionedData"))==0) { sourceType = DataFrom::ProvisionedData; }
    LOGI("DBManager::%s sourceType: %s(%d)", __func__, name.c_str(), sourceType);
    ConfigResult result;
    result = dbReloadNeeded(sourceType, Reload::Init, mPathOfXmlFile);
}

ConfigResult DBManager::dbReloadNeeded(
    const DataFrom sourceType, const Reload reloadReason, const std::string& location
) {
    (void) reloadReason;
    ConfigResult r = ConfigResult::ObjectError;
    if(DataFrom::ProvisionedData == sourceType) {
        error_t result = mJsonFileLib->readJsonFile(location);
        mIsJsonFile = (result ==  E_OK) ? true : false;
        if(mIsJsonFile) {
            mIsProvisioned = true;
            r = ConfigResult::NoError;
        } else {
            mIsProvisioned = false;
            r = ConfigResult::ObjectError;
        }
    } else {
        mXmlFileLib->readXmlFile(location);
        r = ConfigResult::NoError;
    }
    return r;
}

void DBManager::setMnoData(const Mno mnoType, const std::string& mnoData) {
    (void) mnoType;
    (void) mnoData;
}

dbStatus DBManager::dbStatusQuery(
    const DataFrom sourceType, const Query queryReason, const std::string& value
) {
    (void) sourceType;
    (void) queryReason;
    (void) value;

    dbStatus r;

    //It is initialized to a random number as an API that is not used in honda.
    r.rst1 = 0;
    r.rst2 = 0;
    r.rst3 = 0;

    return r;
}

void DBManager::checkSecurity() {

}

std::string DBManager::getStringFromDataFrom(DataFrom sourceType) {
    std::string stringSource;
    if(DataFrom::General == sourceType) {
        stringSource = "General";
    } else if(DataFrom::DefaultProvData == sourceType) {
        stringSource = "DefaultProvData";
    } else if(DataFrom::ProvisionedData == sourceType) {
        stringSource = "ProvisionedData";
    } else if(DataFrom::ProvisioningData == sourceType) {
        stringSource = "ProvisioningData";
    }
    return stringSource;
}

error_t DBManager::initializePath(DataFrom sourceType) {
    (void) sourceType;

    return E_OK;
}

ConfigResponse DBManager::getJsonData(const DataFrom sourceType, const std::string& name) {
    ConfigResponse r;
    r.sourceType = sourceType;
    if(sourceType == DataFrom::ProvisionedData) {
        LOGI("DBManager::%s getJsonData arg is ProvisionedData", __func__);
        //dataName = file path >> 1 basic path same 2 different 3different exist
        std::string path = (name == "") ? mPathOfJsonFile : name;
        r = getJsonDataSet(sourceType, path);
    } else {
        LOGI("DBManager::%s Applicable only to ProvisionedData.", __func__);
        r.result = ConfigResult::Busy;
    }
    if(ConfigResult::NoError == r.result) {
        //LOGI("DBManager::%s getJsonDataSet success name : %s", __func__, name.c_str());
    } else {
        LOGI("DBManager::%s getJsonDataSet fail name : %s", __func__, name.c_str());
    }
    return r;
}

ConfigResponse DBManager::getJsonDataSet(const DataFrom sourceType, const std::string& name) {
    ConfigResponse r;
    r.sourceType = sourceType;
    r = mJsonFileLib->getJsonDataSet(name);
    return r;
}
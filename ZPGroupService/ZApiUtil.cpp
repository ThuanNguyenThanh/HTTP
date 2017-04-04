/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ZApiUtil.h"
#include "ZLogUtil.h"
#include "ZStringDefine.h"

#include <iostream>

#include <Poco/NumberFormatter.h>
#include <Poco/Timestamp.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/VarHolder.h>

std::string ZApiUtil::GetResponseString(uint32_t u32Code, const std::string& strMessage)
{
    return ("{\"code\":" + Poco::NumberFormatter::format(u32Code) + ",\"message\":\"" + strMessage + "\"}");
}

#if 0
/**
 * 
 * @param u64Deadline timestamp in millisecond
 * @return expire time in second
 */
uint64_t ZApiUtil::GetExpireTime(uint64_t u64Deadline)
{
    if (!u64Deadline)
        return 0;

    // Poco::Timestamp is in microsecond => * 1000
    Poco::Timestamp tsDeadline(u64Deadline * 1000);
    Poco::Timestamp tsNow; // Creates a timestamp with the current time

    if (tsDeadline <= tsNow)
        return 0;

    uint64_t u64ExpireTime = (tsDeadline - tsNow);
    u64ExpireTime /= (1000 * 1000); // to second

    return u64ExpireTime;
}
#endif

bool ZApiUtil::GetIntegerValueFromJSonString(const std::string& strJSonData, const std::string& strKey, uint64_t& u64Value)
{
    if (strJSonData.empty() || strKey.empty())
        return false;

    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(strJSonData);

        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        if (!object->has(strKey)) {
            return false;
        }
        
        u64Value = object->getValue<uint64_t>(strKey);

        return true;
    }
    catch (Poco::Exception &ex) {

    }
    catch (std::exception &e) {

    }
    return false;
}


bool ZApiUtil::GetStringValueFromJSonString(const std::string& strJSonData, const std::string& strKey, std::string& strData)
{
    if (strJSonData.empty() || strKey.empty())
        return false;

    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(strJSonData);

        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        if (!object->has(strKey)) {
            return false;
        }
        
        strData = object->getValue<std::string>(strKey);

        return true;
    }
    catch (Poco::Exception &ex) {

    }
    catch (std::exception &e) {

    }
    return false;
}

std::string ZApiUtil::HandleResult(uint64_t u64Code, const std::string& strMessage, bool bIsError)
{
    if (bIsError) {
        ZLogUtil::instance().Error("Response code=" + Poco::NumberFormatter::format(u64Code) + " mes=" + strMessage);
    }
    else {
        ZLogUtil::instance().Debug("Response code=" + Poco::NumberFormatter::format(u64Code) + " mes=" + strMessage);
    }
    std::string strRet = ZApiUtil::GetResponseString(u64Code, strMessage);
    return strRet;
}
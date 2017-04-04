/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZRedisProcess.cpp
 * Author: root
 * 
 * Created on March 24, 2017, 3:34 PM
 */

#include <stdint.h>

#include "ZRedisProcess.h"

ZRedisProcess::ZRedisProcess() {
}

ZRedisProcess::ZRedisProcess(const ZRedisProcess& orig) {
}

ZRedisProcess::~ZRedisProcess() {
}

ZRedisProcess& ZRedisProcess::GetInstance() {
    static ZRedisProcess zRds;
    return zRds;
}

bool ZRedisProcess::Init(const std::string& host, uint32_t port) {
    return m_zcluster.Init(host, port);
}

uint64_t ZRedisProcess::IncrKey(const std::string& strKey, int64_t& nErrorCode) {
    nErrorCode = 0;
    if (strKey.empty()) {
        nErrorCode = -9999;
        return 0;
    }
    uint64_t uResult = m_zcluster.Incr(strKey);
    if (!uResult) {
        nErrorCode = -9998;
        return 0;
    }
    return uResult;
}

bool ZRedisProcess::HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue) {
    if (strHash.empty() || strField.empty() || strValue.empty())
        return false;

    if (m_zcluster.HSet(strHash, strField, strValue) < 0) {
        return false;
    }

    return true;
}

// SenderID send to UserID, MsgID

bool ZRedisProcess::SetUserIDAndSenderIDInfo(const std::string& strHash, const std::string& strFieldSID,
        const std::string& strFieldUID, const std::string& strSenderID, const std::string& strUserID) {

    if (strHash.empty() || strFieldSID.empty() || strFieldUID.empty() || strSenderID.empty() || strUserID.empty())
        return false;

    if (m_zcluster.SAdd("senderid:" + strSenderID, "userid:" + strUserID) < 0)
        return false;
    //Save userID of SenderID
    if (m_zcluster.SAdd("senderid:" + strSenderID, "msgid:" + strHash) < 0)
        return false; //Save msgID of SenderID

    if (m_zcluster.SAdd("userid:" + strUserID, "senderid:" + strSenderID) < 0)
        return false; //Save SenderID of userID

    if (m_zcluster.SAdd("userid:" + strUserID, "msgid:" + strHash) < 0)
        return false; //Save msgID of userID

    return true;
}

bool ZRedisProcess::IncreaseResult(const std::string& strHash, const std::string& strField, int64_t& uErrCode) {
    if (strHash.empty() || strField.empty())
        return false;

    if (IncrKey("countsumrequest", uErrCode) == 0)
        return false;

    if (m_zcluster.HGetInteger(strHash, strField)) {
        if (IncrKey("countreqsuccess", uErrCode) == 0)
            return false;
    } else {
        if (IncrKey("countreqfail", uErrCode) == 0)
            return false;
    }

    return true;
}

bool ZRedisProcess::GetAverageTimeProccess(const std::string& strHash, const std::string& strField, const string& strGetTimeProcess) {
    if (strHash.empty() || strField.empty() || strGetTimeProcess.empty())
        return false;

    REDISPROCCESS RdPro;

    Poco::NumberParser::tryParseUnsigned64(strHash, RdPro.u64NumOfMsg);
    Poco::NumberParser::tryParseUnsigned64(strGetTimeProcess, RdPro.u64TimeProccess);


    if (RdPro.u64NumOfMsg == 1) {
        RdPro.u64Avg = RdPro.u64Max = RdPro.u64Min = RdPro.u64TimeProccess;
        
        if (m_zcluster.Set("maxtimepro", std::to_string(RdPro.u64Max)) == false)
            return false;

        if (m_zcluster.Set("mintimepro", std::to_string(RdPro.u64Min)) == false)
            return false;
        
    } else {
        uint64_t uTempAvgTime = m_zcluster.GetInteger("avgtimepro");
        RdPro.u64Avg = (RdPro.u64TimeProccess + uTempAvgTime * (RdPro.u64NumOfMsg - 1)) / RdPro.u64NumOfMsg;
    }

    if (m_zcluster.Set("avgtimepro", std::to_string(RdPro.u64Avg)) == false)
        return false;

    if (m_zcluster.GetInteger("maxtimepro") < RdPro.u64TimeProccess) {
        RdPro.u64Max = RdPro.u64TimeProccess;
        m_zcluster.Set("maxtimepro", std::to_string(RdPro.u64Max));
        return true;
    }

    if (m_zcluster.GetInteger("mintimepro") > RdPro.u64TimeProccess) {
        RdPro.u64Min = RdPro.u64TimeProccess;
        m_zcluster.Set("mintimepro", std::to_string(RdPro.u64Min));
        return true;
    }

    return true;
}

bool ZRedisProcess::SumOfSenderID(const std::string& strField, const std::string& strValue, int64_t& i64ErrCode) {
    if (strField.empty() || strValue.empty())
        return false;

    int64_t uCheckSenderIDExist = m_zcluster.SAdd("senderidexist", strValue);

    if (uCheckSenderIDExist > 0) {
        if (IncrKey("countsenderid", i64ErrCode) == 0)
            return false;
    }

    return true;
}

bool ZRedisProcess::SumOfUserID(const std::string & strField, const std::string & strValue, int64_t& i64ErrCode) {
    if (strField.empty() || strValue.empty())
        return false;

    int64_t uCheckUserIDExist = m_zcluster.SAdd("useridexist", strValue); //ZSCORE UserID UserIDExist

    if (uCheckUserIDExist > 0) {
        if (IncrKey("countuserid", i64ErrCode) == 0)
            return false;
    }

    return true;
}
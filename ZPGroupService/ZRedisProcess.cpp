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

bool ZRedisProcess::SetMsgIDInfo(uint64_t u64MsgID, const std::string& strField, const std::string& strValue) {
    std::string strMsgID = std::to_string(u64MsgID);
    
    if (strMsgID.empty() || strField.empty() || strValue.empty())
        return false;

    if (m_zcluster.HSet(strMsgID, strField, strValue) < 0) {
        return false;
    }

    return true;
}

// SenderID send to UserID, MsgID

bool ZRedisProcess::SetUserIDAndSenderIDInfo(uint64_t u64MsgID, uint64_t u64SenderID, uint64_t u64UserID) {

    std::string strMsgID = std::to_string(u64MsgID);
    std::string strSenderID = std::to_string(u64SenderID);
    std::string strUserID = std::to_string(u64UserID);

    if (strMsgID.empty() || std::to_string(u64SenderID).empty() || std::to_string(u64UserID).empty())
        return false;

    if (m_zcluster.SAdd("ns:listmsgsenderid:" + strSenderID + ":list", strMsgID) < 0)//List msgID of SenderID
        return false;

    if (m_zcluster.SAdd("ns:listmsguserid:" + strUserID + ":list", strMsgID) < 0)//List msgID of UserID
        return false;

    if (m_zcluster.SAdd("ns:listuserid:" + strSenderID, ":list" + strUserID) < 0) //List UserID of SenderID
        return false;

    if (m_zcluster.SAdd("ns:listsenderid:" + strUserID + ":list", strSenderID) < 0) //List Sender of UserID
        return false;

    return true;
}

bool ZRedisProcess::IncreaseResult(uint64_t u64MsgID, const std::string& strFieldResult, int64_t& uErrCode) {
    std::string strMsgID = std::to_string(u64MsgID);

    if (strMsgID.empty() || strFieldResult.empty())
        return false;

    if (IncrKey("countsumrequest", uErrCode) == 0)
        return false;

    if (m_zcluster.HGetInteger(strMsgID, strFieldResult)) {
        if (IncrKey("countreqsuccess", uErrCode) == 0)
            return false;
    } else {
        if (IncrKey("countreqfail", uErrCode) == 0)
            return false;
    }

    return true;
}

bool ZRedisProcess::GetAverageTimeProccess(uint64_t u64MsgID, uint64_t u64TimeProccess) {
    std::string strMsgID = std::to_string(u64MsgID);

    if (strMsgID.empty())
        return false;

    REDISPROCCESS RdPro;

    if (u64MsgID == 1) {
        RdPro.u64Avg = RdPro.u64Max = RdPro.u64Min = u64TimeProccess;

        if (m_zcluster.Set("maxtimepro", std::to_string(RdPro.u64Max)) == false)
            return false;

        if (m_zcluster.Set("mintimepro", std::to_string(RdPro.u64Min)) == false)
            return false;

    } else {
        uint64_t uTempAvgTime = m_zcluster.GetInteger("avgtimepro");
        RdPro.u64Avg = (u64TimeProccess + uTempAvgTime * (u64MsgID - 1)) / u64MsgID;
    }

    if (m_zcluster.Set("avgtimepro", std::to_string(RdPro.u64Avg)) == false)
        return false;

    if (m_zcluster.GetInteger("maxtimepro") < u64TimeProccess) {
        RdPro.u64Max = u64TimeProccess;
        m_zcluster.Set("maxtimepro", std::to_string(RdPro.u64Max));
        return true;
    }

    if (m_zcluster.GetInteger("mintimepro") > u64TimeProccess) {
        RdPro.u64Min = u64TimeProccess;
        m_zcluster.Set("mintimepro", std::to_string(RdPro.u64Min));
        return true;
    }

    return true;
}

bool ZRedisProcess::SumOfSenderID(const std::string& strFieldSenderID, uint64_t uSenderID, int64_t& i64ErrCode) {
    if (strFieldSenderID.empty() || std::to_string(uSenderID).empty())
        return false;

    int64_t uCheckSenderIDExist = m_zcluster.SAdd("senderidexist", std::to_string(uSenderID));

    if (uCheckSenderIDExist > 0) {
        if (IncrKey("countsenderid", i64ErrCode) == 0)
            return false;
    }

    return true;
}

bool ZRedisProcess::SumOfUserID(const std::string & strFieldUserID, uint64_t uUserID, int64_t& i64ErrCode) {
    if (strFieldUserID.empty() || std::to_string(uUserID).empty())
        return false;

    int64_t uCheckUserIDExist = m_zcluster.SAdd("useridexist", std::to_string(uUserID)); //ZSCORE UserID UserIDExist

    if (uCheckUserIDExist > 0) {
        if (IncrKey("countuserid", i64ErrCode) == 0)
            return false;
    }

    return true;
}
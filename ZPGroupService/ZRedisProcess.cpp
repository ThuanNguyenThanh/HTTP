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
#include "ZMsgDefine.h"

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
        nErrorCode = -1;
        return 0;
    }
    uint64_t uResult = m_zcluster.Incr(strKey);
    if (!uResult) {
        nErrorCode = -2;
        return 0;
    }
    return uResult;
}

bool ZRedisProcess::SetMsgIDInfo(uint64_t u64MsgID, const std::string& strField, const std::string& strValue) {
    std::string strMsgID = std::to_string(u64MsgID);

    if ((u64MsgID == 0) || strField.empty() || strValue.empty()) {
        //iErrCode = EMPTY_DATA_ERROR_CODE;
        return false;
    }
    if (m_zcluster.HSet(strMsgID, strField, strValue) < 0) {
        //iErrCode = SET_DATA_ERROR;
        return false;
    }

    return true;
}

bool ZRedisProcess::SetMsgIDInfo(uint64_t u64MsgID, const std::string& strField, uint64_t u64Value) {
    if ((u64MsgID == 0) || strField.empty()) {
        //iErrCode = EMPTY_DATA_ERROR_CODE;
        return false;
    }
    if (m_zcluster.HSet(std::to_string(u64MsgID), strField, std::to_string(u64Value)) < 0) {
        //iErrCode = SET_DATA_ERROR;
        return false;
    }

    return true;
}

// SenderID send to UserID, MsgID

bool ZRedisProcess::SetUserIDAndSenderIDInfo(uint64_t u64MsgID, uint64_t u64SenderID, uint64_t u64UserID) {

    std::string strMsgID = std::to_string(u64MsgID);
    std::string strSenderID = std::to_string(u64SenderID);
    std::string strUserID = std::to_string(u64UserID);

    if ((u64MsgID == 0) || (u64SenderID == 0) || (u64UserID == 0)) {
        //iErrCode = EMPTY_DATA_ERROR;
        return false;
    }

    if (m_zcluster.SAdd("ns:listmsgofsenderid:" + strSenderID + ":list", strMsgID) < 0) //List msgID of SenderID
        return false;

    if (m_zcluster.SAdd("ns:listmsgofuserid:" + strUserID + ":list", strMsgID) < 0) //List msgID of UserID
        return false;

    if (m_zcluster.SAdd("ns:listuseridofsenderid:" + strSenderID + ":list", strUserID) < 0) //List UserID of SenderID
        return false;

    if (m_zcluster.SAdd("ns:listsenderidofuserid:" + strUserID + ":list", strSenderID) < 0) //List Sender of UserID
        return false;

    return true;
}

bool ZRedisProcess::IncreaseResult(uint64_t u64MsgID, int64_t& uErrCode) {
    if (u64MsgID == 0)
        return false;

    std::string strMsgID = std::to_string(u64MsgID);

    if (IncrKey("ns:countsumrequest", uErrCode) == 0)
        return false;

    if (m_zcluster.HGetInteger(strMsgID, RDS_MSG_INFO_FIELD_RESULT)) {
        if (IncrKey("ns:countreqsuccess", uErrCode) == 0)
            return false;
    } else {
        if (IncrKey("ns:countreqfail", uErrCode) == 0)
            return false;
    }

    return true;
}

bool ZRedisProcess::GetAverageTimeProccess(uint64_t u64MsgID, uint64_t u64TimeProccess) {

    if (u64MsgID == 0 || u64TimeProccess == 0)
        return false;

    uint64_t u64Avg = 0;

    if (u64MsgID == 1) {
        u64Avg = u64TimeProccess;

        if (!m_zcluster.Set("ns:maxtimepro", std::to_string(u64TimeProccess)))
            return false;

        if (!m_zcluster.Set("ns:mintimepro", std::to_string(u64TimeProccess)))
            return false;

    } else {
        uint64_t uTempAvgTime = m_zcluster.GetInteger("ns:avgtimepro");

        if (uTempAvgTime == 0)
            return false;

        u64Avg = (u64TimeProccess + uTempAvgTime * (u64MsgID - 1)) / u64MsgID;
    }

    if (!m_zcluster.Set("ns:avgtimepro", std::to_string(u64Avg)))
        return false;

    if (m_zcluster.GetInteger("ns:maxtimepro") < u64TimeProccess) {
        if (!m_zcluster.Set("ns:maxtimepro", std::to_string(u64TimeProccess)))
            return false;

        return true;
    }

    if (m_zcluster.GetInteger("ns:mintimepro") > u64TimeProccess) {
        if (!m_zcluster.Set("ns:mintimepro", std::to_string(u64TimeProccess)))
            return false;

        return true;
    }

    return true;
}

bool ZRedisProcess::CountSenderID(uint64_t u64SenderID) {
    if (u64SenderID == 0)
        return false;

    if(m_zcluster.SAdd("ns:senderidexist", std::to_string(u64SenderID)) < 0)
        return false;
    
    return true;
}

bool ZRedisProcess::CountUserID(uint64_t u64UserID) {
    if (u64UserID == 0)
        return false;

    if(m_zcluster.SAdd("ns:useridexist", std::to_string(u64UserID)) < 0)
        return false;

    return true;
}
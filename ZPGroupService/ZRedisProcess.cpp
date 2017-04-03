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

#include "ZRedisProcess.h"
#include "redis/zcluster.h"

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

uint16_t ZRedisProcess::Init(const std::string& host, uint32_t port) {
    return m_zcluster.Init(host, port);
}

uint64_t ZRedisProcess::IncrKey(const std::string& strKey) {
    if (strKey.empty())
        return None;

    return m_zcluster.Incr(strKey);
}

uint16_t ZRedisProcess::HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue) {
    //m_zcluster.Del(m_strData);
    if (strHash.empty() || strField.empty() || strValue.empty())
        return ErrEmptyData;

    m_zcluster.HSet(strHash, strField, strValue);

    return None;
}

// SenderID send to UserID, MsgID

uint16_t ZRedisProcess::ListUserIDAndSenderIDInfo(const std::string& strHash, const std::string& strFieldSID,
        const std::string& strFieldUID, const std::string& strSenderID, const std::string& strUserID) {

    if (strHash.empty() || strFieldSID.empty() || strFieldUID.empty() || strSenderID.empty() || strUserID.empty())
        return ErrEmptyData;

    //Proccess for 2 if condition
    if (m_zcluster.HGetString(strHash, strFieldSID).compare(strSenderID))
        return ErrCmpSenderID; // SMEMBERS SenderID: {ID}

    m_zcluster.SAdd("SenderID:" + strSenderID, "UserID:" + strUserID); //Save userID of SenderID
    m_zcluster.SAdd("SenderID:" + strSenderID, "MsgID:" + strHash); //Save msgID of SenderID

    // UserID send to SenderID, MsgID
    if (m_zcluster.HGetString(strHash, strFieldUID).compare(strUserID)) // SMEMBERS UserID: {ID}
        return ErrCmpUserID;

    m_zcluster.SAdd("UserID:" + strUserID, "SenderID:" + strSenderID); //Save SenderID of userID
    m_zcluster.SAdd("UserID:" + strUserID, "MsgID:" + strHash); //Save msgID of userID

    return None;
}

uint16_t ZRedisProcess::ReqSuccessAndFail(const std::string& strHash, const std::string& strField) {
    if (strHash.empty() || strField.empty())
        return ErrEmptyData;

    //Sum of Request success: ZSCORE Result SumSuccess
    if (!m_zcluster.HGetString(strHash, strField).compare("Success")) {
        uint64_t u64IncrSuccess = IncrKey("CountReqSuccess");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrSuccess), "SumSuccess");
        SumOfRequest(strField);
        return None;
    }

    //Sum of Request fail: ZSCORE Result SumFail
    if (!m_zcluster.HGetString(strHash, strField).compare("Fail")) {
        uint64_t u64IncrFail = IncrKey("CountReqFail");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrFail), "SumFail");
        SumOfRequest(strField);
        return None;
    }
}

uint16_t ZRedisProcess::SumOfRequest(const std::string& strField) {
    if (strField.empty())
        return ErrEmptyData;

    uint64_t u64IncrSuccess = IncrKey("CountSumRequest");

    m_zcluster.ZAdd(strField, std::to_string(u64IncrSuccess), "SumRequest"); // ZSCORE Result SumRequest

    return None;
}

//ZSCORE TimeProccess MaxTimePro
//ZSCORE TimeProccess MinTimePro
//ZSCORE TimeProccess AvgTimePro

uint16_t ZRedisProcess::AverageTimeProccess(const std::string& strHash, const std::string& strField, const string& strGetTimeProcess) {
    if (strHash.empty() || strField.empty() || strGetTimeProcess.empty())
        return ErrEmptyData;

    RADISPROCCESS RdPro;

    uint64_t u64NumOfMsg = std::stoi(strHash);

    if (u64NumOfMsg == 1) {
        RdPro.u64AvgTimeProccess = RdPro.u64MaxTimeProccess = RdPro.u64MinTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MaxTimeProccess), "MaxTimePro");
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MinTimeProccess), "MinTimePro");
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64AvgTimeProccess), "AvgTimePro");

        m_zcluster.ZAdd(strField, strGetTimeProcess, strHash);
        return None;
    }

    RdPro.u64AvgTimeProccess = (std::stoi(strGetTimeProcess) + (m_zcluster.ZScore(strField, "AvgTimePro")) * (u64NumOfMsg - 1)) / u64NumOfMsg;
    m_zcluster.ZAdd(strField, std::to_string(RdPro.u64AvgTimeProccess), "AvgTimePro");

    m_zcluster.ZAdd(strField, strGetTimeProcess, strHash);

    if (m_zcluster.ZScore(strField, "MaxTimePro") < std::stoi(strGetTimeProcess)) {
        RdPro.u64MaxTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MaxTimeProccess), "MaxTimePro");
        return None;
    }

    if (m_zcluster.ZScore(strField, "MinTimePro") > std::stoi(strGetTimeProcess)) {
        RdPro.u64MinTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MinTimeProccess), "MinTimePro");
        return None;
    }

    return None;
}

//Sum of SenderID
// Command: ZSCORE SenderID SumSenderID

uint16_t ZRedisProcess::SumOfSenderID(const std::string& strField, const std::string& strValue) {
    if (strField.empty() || strValue.empty())
        return ErrEmptyData;

    uint16_t uCheckSenderIDExist = m_zcluster.SAdd("SenderIDExist", strValue); //ZSCORE SenderID SenderIDExist

    if (!strField.compare("SenderID") && uCheckSenderIDExist) {
        uint64_t u64IncrSenderID = IncrKey("CountSenderID");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrSenderID), "SumSenderID");
       // m_zcluster.ZAdd(strField, strValue, "ListSenderID");
    }

    return None;
}

/*Sum of UserID
Command: 
    ZSCORE UserID SumUserID
    ZSCORE UserID ListUserID

*/
uint16_t ZRedisProcess::SumOfUserID(const std::string & strField, const std::string & strValue) {
    if (strField.empty() || strValue.empty())
        return ErrEmptyData;

    uint16_t uCheckUserIDExist = m_zcluster.SAdd("UserIDExist", strValue); //ZSCORE UserID UserIDExist

    if (!strField.compare("UserID") && uCheckUserIDExist) {
        uint64_t u64IncrUserID = IncrKey("CountUserID");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrUserID), "SumUserID");
       // m_zcluster.ZAdd(strField, strValue, "ListUserID");
    }

    return None;
}
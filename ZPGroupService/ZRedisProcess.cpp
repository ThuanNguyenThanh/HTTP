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

bool ZRedisProcess::Init(const std::string& host, uint32_t port) {
    return m_zcluster.Init(host, port);
}

uint64_t ZRedisProcess::IncrKey(const std::string& strKey) {
    if (strKey.empty())
        return 0;

    return m_zcluster.Incr(strKey);
}

bool ZRedisProcess::HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue) {
    //m_zcluster.Del(m_strData);
    uint16_t a = m_zcluster.HSet(strHash, strField, strValue);

    std::cout << "hset?: " << a << endl << "strValue: " << strValue << endl;

    std::string strOut = m_zcluster.HGetString(strHash, strField);

    std::cout << "strOut: " << strOut << endl;

    if (strOut.compare(strValue) != 0) {
        std::cerr << "TestHSet_HGet Failed" << std::endl;
        return false;
    }

    std::cout << "TestHSet_HGet Passed" << std::endl;
    return true;
}

// SenderID send to UserID, MsgID

bool ZRedisProcess::ListUserIDAndSenderIDInfo(const std::string& strHash, const std::string& strFieldSID,
        const std::string& strFieldUID, const std::string& strSenderID, const std::string& strUserID) {

    if (strHash.empty() || strFieldSID.empty() || strFieldUID.empty() || strSenderID.empty() || strUserID.empty())
        return false;

    //Proccess for 2 if condition
    if (!m_zcluster.HGetString(strHash, strFieldSID).compare(strSenderID)) { // SMEMBERS SenderID: {ID}
        m_zcluster.SAdd("SenderID:" + strSenderID, strUserID); //Save userID of SenderID
        m_zcluster.SAdd("SenderID:" + strSenderID, strHash); //Save msgID of SenderID
    }

    // UserID send to SenderID, MsgID
    if (!m_zcluster.HGetString(strHash, strFieldUID).compare(strUserID)) { // SMEMBERS UserID: {ID}
        m_zcluster.SAdd("UserID:" + strUserID, strSenderID); //Save SenderID of userID
        m_zcluster.SAdd("UserID:" + strUserID, strHash); //Save msgID of userID
    }

    return true;
}

bool ZRedisProcess::ReqSuccessAndFail(const std::string& strHash, const std::string& strField) {
    if (strHash.empty() || strField.empty())
        return false;

    //Sum of Request success: ZSCORE Result SumSuccess
    if (!m_zcluster.HGetString(strHash, strField).compare("Success")) {
        uint64_t u64IncrSuccess = IncrKey("CountReqSuccess");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrSuccess), "SumSuccess");
        SumOfRequest(strField);
        return true;
    }

    //Sum of Request fail: ZSCORE Result SumFail
    if (!m_zcluster.HGetString(strHash, strField).compare("Fail")) {
        uint64_t u64IncrFail = IncrKey("CountReqFail");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrFail), "SumFail");
        SumOfRequest(strField);
        return true;
    }
}

bool ZRedisProcess::SumOfRequest(const std::string& strField) {
    if (strField.empty())
        return false;

    uint64_t u64IncrSuccess = IncrKey("CountSumRequest");

    m_zcluster.ZAdd(strField, std::to_string(u64IncrSuccess), "SumRequest"); // ZSCORE Result SumRequest

    return true;
}

//List time proccess: ZRANGEBYSCORE TimeProcess -inf +inf WITHSCORES

bool ZRedisProcess::ListTimeProcess(const std::string& strHash, const std::string& strField) {
    if (strHash.empty() || strField.empty())
        return false;

    std::string strGetTimeProcess = m_zcluster.HGetString(strHash, strField);
    m_zcluster.ZAdd(strField, strGetTimeProcess, strHash);

    return true;
}

bool ZRedisProcess::AverageTimeProccess(const std::string& strHash, const std::string& strField, const string& strGetTimeProcess) {
    if (strHash.empty() || strField.empty() || strGetTimeProcess.empty())
        return false;

    RADISPROCCESS RdPro;

    uint64_t u64NumOfMsg = std::stoi(strHash);

    // m_zcluster.ZAdd(strField, strGetTimeProcess, strHash);

    if (m_zcluster.ZScore(strField, "AvgTimePro") == -1) {
        RdPro.u64AvgTimeProccess = RdPro.u64MaxTimeProccess = RdPro.u64MinTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MaxTimeProccess), "MaxTimePro");
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MinTimeProccess), "MinTimePro");
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64AvgTimeProccess), "AvgTimePro");
        return true;
    }

    RdPro.u64AvgTimeProccess = (std::stoi(strGetTimeProcess) + (m_zcluster.ZScore(strField, "AvgTimPro")) * (u64NumOfMsg - 1)) / u64NumOfMsg;
    m_zcluster.ZAdd(strField, std::to_string(RdPro.u64AvgTimeProccess), "AvgTimePro");

    if (m_zcluster.ZScore(strField, "MaxTimePro") < std::stoi(strGetTimeProcess)) {
        RdPro.u64MaxTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MaxTimeProccess), "MaxTimePro");
        return true;
    }

    if (m_zcluster.ZScore(strField, "MinTimePro") > std::stoi(strGetTimeProcess)) {
        RdPro.u64MinTimeProccess = std::stoi(strGetTimeProcess);
        m_zcluster.ZAdd(strField, std::to_string(RdPro.u64MinTimeProccess), "MinTimePro");
        return true;
    }
}

//Sum of SenderID: ZSCORE SenderID SumSenderID

bool ZRedisProcess::SumOfSenderID(const std::string& strHash, const std::string& strField, const std::string& strValue) {
    if (strHash.empty() || strField.empty() || strValue.empty())
        return false;

    uint16_t uCheckSenderIDExist = m_zcluster.ZAdd(strField, strValue, "SenderIDExist"); //ZSCORE SenderID SenderIDExist

    if (!strField.compare("SenderID") && !uCheckSenderIDExist) {
        uint64_t u64IncrSenderID = IncrKey("CountSenderID");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrSenderID), "SumSenderID");
    }

    return true;
}

//Sum of UserID : ZSCORE UserID SumUserID

bool ZRedisProcess::SumOfUserID(const std::string& strHash, const std::string & strField, const std::string& strValue) {
    if (strHash.empty() || strField.empty() || strValue.empty())
        return false;

    uint16_t uCheckUserIDExist = m_zcluster.ZAdd(strField, strValue, "UserIDExist"); //ZSCORE UserID UserIDExist

    if (!strField.compare("UserID") && !uCheckUserIDExist) {
        uint64_t u64IncrUserID = IncrKey("CountUserID");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrUserID), "SumUserID");
    }

    return true;
}


//bool ZRedisProcess::ListMsgIDOfSenderID(const std::string& strHash, const std::string& strField, const std::string& strSenderID)//, std::vector<uint64_t>& vtMsgIDs)
//{
//    if (strHash.empty() || strField.empty() || strSenderID.empty())
//        return false;
//
//    for (uint16_t i = 0; i < std::stoi(strHash); i++) {
//        if (m_zcluster.HGetString(strHash, strField).compare(strSenderID))
//            return false;
//        //vtMsgIDs.push_back(std::stoi(strHash));
//        m_zcluster.SAdd("SenderID:" + strSenderID, strHash);
//    }
//    return true;
//}
//
//bool ZRedisProcess::ListMsgIDOfUserID(const std::string& strHash, const std::string& strField, const std::string& strUserID)//, std::vector<uint64_t>& vtMsgIDs)
//{
//    if (strHash.empty() || strField.empty() || strUserID.empty())
//        return false;
//
//    for (uint16_t i = 0; i < std::stoi(strHash); i++) {
//        if (m_zcluster.HGetString(strHash, strField).compare(strUserID))
//            return false;
//        //vtMsgIDs.push_back(std::stoi(strHash));
//        m_zcluster.SAdd("UserID:" + strUserID, strHash);
//    }
//    return true;
//}



// UserID send to SenderID, MsgID
//
//bool ZRedisProcess::ListSenderIDOfUserID(const std::string& strHash, const std::string& strField,
//        const std::string& strUserID, const std::string& strSenderID) {//, std::vector<uint64_t>& vtMsgIDs) {
//    if (strHash.empty() || strField.empty() || strUserID.empty())
//        return false;
//
//    for (uint16_t i = 0; i < std::stoi(strHash); i++) {
//        if (!m_zcluster.HGetString(strHash, strField).compare(strUserID)) {
//            //vtMsgIDs.push_back(std::stoi(strHash));
//            m_zcluster.SAdd("UserID:" + strUserID, strSenderID);
//            m_zcluster.SAdd("UserID:" + strUserID, strHash);
//        }
//    }
//    return true;
//}




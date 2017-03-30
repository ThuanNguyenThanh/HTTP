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

    for (uint32_t i = 0; i < std::stoi(strHash); i++) {
        // SenderID send to UserID, MsgID
        if (!m_zcluster.HGetString(strHash, strFieldSID).compare(strSenderID)) {
            m_zcluster.SAdd("SenderID:" + strSenderID, strUserID);
            m_zcluster.SAdd("SenderID:" + strSenderID, strHash);
        }

        // UserID send to SenderID, MsgID
        if (!m_zcluster.HGetString(strHash, strFieldUID).compare(strUserID)) {
            m_zcluster.SAdd("UserID:" + strUserID, strSenderID);
            m_zcluster.SAdd("UserID:" + strUserID, strHash);
        }
    }
    return true;
}

bool ZRedisProcess::SumOfReq(const std::string& strHash, const std::string& strField) {
    if (strHash.empty() || strField.empty())
        return false;
    
    if (!m_zcluster.HGetString(strHash, strField).compare("Success")) {
        uint64_t u64IncrSuccess = IncrKey("0");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrSuccess), "Success");
    }

    if (!m_zcluster.HGetString(strHash, strField).compare("Fail")) {
        uint64_t u64IncrFail = IncrKey("0");
        m_zcluster.ZAdd(strField, std::to_string(u64IncrFail), "Fail");
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




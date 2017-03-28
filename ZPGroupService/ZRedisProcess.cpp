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

void ZRedisProcess::HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue) {
    //m_zcluster.Del(m_strData);
    uint16_t a = m_zcluster.HSet(strHash, strField, strValue);

    std::cout << "hset?: " << a << endl<< "strValue: " << strValue << endl;

    std::string strOut = m_zcluster.HGetString(strHash, strField);

    std::cout << "strOut: " << strOut << endl;

    if (strOut.compare(strValue) != 0) {
        std::cerr << "TestHSet_HGet Failed" << std::endl;
        return;
    }

    std::cout << "TestHSet_HGet Passed" << std::endl;
}



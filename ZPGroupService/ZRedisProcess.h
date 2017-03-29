/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZRedisProcess.h
 * Author: root
 *
 * Created on March 24, 2017, 3:34 PM
 */

#ifndef ZREDISPROCESS_H
#define ZREDISPROCESS_H

#include "ZApiCalcHandler.h"
#include "redis/zcluster.h"

class ZRedisProcess {
private:
    //Poco::FastMutex m_mtSync;
    ZCluster m_zcluster;

public:
    ZRedisProcess();
    ZRedisProcess(const ZRedisProcess& orig);
    virtual ~ZRedisProcess();

    static ZRedisProcess& GetInstance();
    bool Init(const std::string& host, uint32_t port);
    uint64_t IncrKey(const std::string& strKey);
    bool HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue);

    bool MsgIDOfSenderID(const std::string& strHash, const std::string& strField, const std::string& uSenderID);

    bool ListMsgIDOfSenderID(const std::string&, const std::string&, const std::string&); //std::vector<uint64_t>& vtMsgIDs);
    bool ListMsgIDOfUserID(const std::string&, const std::string&, const std::string&);
    bool ListUserIDOfSenderID(const std::string& uSenderID, std::vector<uint64_t>& vtUserIDs);
    bool ListSenderIDOfUserID(const std::string& uUserID, std::vector<uint64_t>& vtSenderIDs);

    bool ListUserIDOfSenderID(const std::string&, const std::string&,
            const std::string&, const std::string&);
};

#endif /* ZREDISPROCESS_H */


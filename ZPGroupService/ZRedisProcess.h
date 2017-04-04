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

typedef struct RedisProccess {
    uint64_t u64Max = 0;
    uint64_t u64Min = 0;
    uint64_t u64Avg = 0;
} REDISPROCCESS;

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

    uint64_t IncrKey(const std::string& strKey, int64_t& i64ErrCode);

    bool SetMsgIDInfo(uint64_t u64MsgID, const std::string& strField, const std::string& strValue);

    bool SetUserIDAndSenderIDInfo(uint64_t u64MsgID, uint64_t u64SenderID, uint64_t u64UserID);

    bool IncreaseResult(uint64_t u64MsgID, const std::string& strFieldResult, int64_t& uErrCode);

    bool GetAverageTimeProccess(uint64_t u64MsgID, uint64_t u64TimeProccess);

    bool SumOfSenderID(const std::string& strFieldSenderID, uint64_t uSenderID, int64_t& i64ErrCode);

    bool SumOfUserID(const std::string & strFieldUserID, uint64_t uUserID, int64_t& i64ErrCode);
};

#endif /* ZREDISPROCESS_H */


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
    uint64_t u64Max;
    uint64_t u64Min;
    uint64_t u64Avg;
    int64_t i64NumOfMsg;
    int64_t i64TimeProccess;
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

    bool HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue);

    bool SetUserIDAndSenderIDInfo(const std::string&, const std::string&,
            const std::string&, const std::string&, const std::string&);

    bool IncreaseResult(const std::string& strHash, const std::string& strField, int64_t& uErrCode);

    //uint16_t ListTimeProcess(const std::string& strHash, const std::string& strField);

    bool GetAverageTimeProccess(const std::string&, const std::string&, const std::string&);

    bool SumOfSenderID(const std::string & strField, const std::string& strValue, int64_t& i64ErrCode);

    bool SumOfUserID(const std::string & strField, const std::string& strValue, int64_t& i64ErrCode);
};

#endif /* ZREDISPROCESS_H */


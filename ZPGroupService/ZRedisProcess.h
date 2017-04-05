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

//#define EMPTY_DATA_ERROR      -1
//#define INCR_KEY_ERROR        -2
//#define SET_DATA_ERROR        -3

#include "ZApiCalcHandler.h"
#include "redis/zcluster.h"

typedef struct TimeProccess {
    uint64_t u64Max = 0;
    uint64_t u64Min = 0;
    uint64_t u64Avg = 0;
} TIMEPROCCESS;

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
    bool SetMsgIDInfo(uint64_t u64MsgID, const std::string& strField, uint64_t u64Value);

    bool SetUserIDAndSenderIDInfo(uint64_t u64MsgID, uint64_t u64SenderID, uint64_t u64UserID);

    bool IncreaseResult(uint64_t u64MsgID, int64_t& uErrCode);

    bool GetAverageTimeProccess(uint64_t u64MsgID, uint64_t u64TimeProccess);

    bool CountSenderID(uint64_t uSenderID);

    bool CountUserID(uint64_t uUserID);
};

#endif /* ZREDISPROCESS_H */


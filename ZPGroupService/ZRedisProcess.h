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

typedef struct tagRadisProccess {
    uint64_t u64MaxTimeProccess;
    uint64_t u64MinTimeProccess;
    uint64_t u64AvgTimeProccess;
    uint64_t u64SumReqSuccess;
    uint64_t u64SumReqFail;
    uint64_t u64SumReq;

} RADISPROCCESS;

enum ListError {
    None,
    ErrEmptyData,
    ErrCmpSenderID,
    ErrCmpUserID,
    ErrSumofSenderID,
    ErrSumofUserID
   // ErrCmpUserID,

};

class ZRedisProcess {
private:
    //Poco::FastMutex m_mtSync;
    ZCluster m_zcluster;

public:
    ZRedisProcess();
    ZRedisProcess(const ZRedisProcess& orig);
    virtual ~ZRedisProcess();

    static ZRedisProcess& GetInstance();

    uint16_t Init(const std::string& host, uint32_t port);

    uint64_t IncrKey(const std::string& strKey);

    uint16_t HSetMsgID(const std::string& strHash, const std::string& strField, const std::string& strValue);

    uint16_t ListUserIDAndSenderIDInfo(const std::string&, const std::string&,
            const std::string&, const std::string&, const std::string&);

    uint16_t ReqSuccessAndFail(const std::string& strHash, const std::string& strField);

    uint16_t SumOfRequest(const std::string& strHash);

    uint16_t ListTimeProcess(const std::string& strHash, const std::string& strField);

    uint16_t AverageTimeProccess(const std::string&, const std::string&, const std::string&);

    uint16_t SumOfSenderID(const std::string & strField, const std::string& strValue);

    uint16_t SumOfUserID(const std::string & strField, const std::string& strValue);
};

#endif /* ZREDISPROCESS_H */


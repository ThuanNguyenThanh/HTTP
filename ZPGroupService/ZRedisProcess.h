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
typedef struct tagRadisProccess
{
    uint64_t u64MaxTimeProccess = 0;
    uint64_t u64MinTimeProccess = 0;
    uint64_t u64AvgTimeProccess = 0;
    
}RADISPROCCESS;

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
    
    bool ListUserIDAndSenderIDInfo(const std::string&, const std::string&,
            const std::string&, const std::string&, const std::string&);

    bool ReqSuccessAndFail(const std::string& strHash, const std::string& strField);
    
    bool SumOfRequest(const std::string& strHash, const std::string& strField);
    
    bool ListTimeProcess(const std::string& strHash, const std::string& strField);
    
    bool AverageTimeProccess(const std::string& , const std::string& , const std::string& );
    
    bool SumOfSenderID(const std::string& strHash, const std::string & strField, const std::string& strValue);
    
    bool SumOfUserID(const std::string& strHash, const std::string & strField, const std::string& strValue);
};

#endif /* ZREDISPROCESS_H */


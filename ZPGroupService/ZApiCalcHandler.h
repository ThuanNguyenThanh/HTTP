/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiAddHandler.h
 * Author: root
 *
 * Created on March 7, 2017, 6:08 PM
 */
#ifndef ZAPICALHANDLER_H
#define ZAPICALHANDLER_H

#include "ZApiDefine.h"
#include "ZApiBroadcastMsgHandler.h"
#include "ZLogUtil.h"
#include "ZRedisProcess.h"
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct tagMessageInfo {
    //SenderID
    std::string strKeySenderID = "SenderID";
    uint32_t uSenderID = 0;
    std::string strSenderID = "SenderID:" + std::to_string(uSenderID);

    //UserID
    std::string strKeyUserID = "UserID";
    uint32_t uUserID = 0;
    //std::string strSenderID = "SenderID:" + std::to_string(uSenderID);

    //Data
    std::string strKeyData = "Data";
    std::string strData = "";

    //Time start process
    std::string strKeyTimeStart = "TimeStart";
    unsigned long long ullTimeStart = 0;
    unsigned long long ullTimeEnd = 0;
    unsigned long long ullTimeProcess = 0;

    //Time process
    std::string strKeyTimeProcess = "TimeProccess";
    
    //Result process
    std::string strKeyResult = "Result";
    std::string strResult = RandomResult();

    std::string RandomResult() {
        std::string strResult = "";
        srand(time(NULL)); //Randomize seed initialization
        uint16_t uValueRand = rand() % 2; // Generate a random number between 0 and 1

        if (uValueRand == 1)
            strResult = "Success";
        else
            strResult = "Fail";
        return strResult;
    }


} MSGINFO;

class ZApiCalcHandler : public Poco::Net::HTTPRequestHandler {
protected:

public:
    ZApiCalcHandler();
    ~ZApiCalcHandler();

    void HandleStringRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, std::string& strJSonData, std::ostream& respStream);
    std::string ProcessData(const MSGINFO&);
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void GetJson(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, std::ostream& respStream, MSGINFO&);
};



#endif


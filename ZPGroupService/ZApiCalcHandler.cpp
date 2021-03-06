/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdint.h>
#include "ZMsgDefine.h"
#include"ZApiCalcHandler.h"
#include "ZApiUtil.h"
#include "ZMsgDefine.h"


ZApiCalcHandler::ZApiCalcHandler() {

}

ZApiCalcHandler::~ZApiCalcHandler() {

}

void ZApiCalcHandler::HandleStringRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, std::string& strJSonData, std::ostream& respStream) {

    std::istream& reqStream = request.stream();
    char buffer[MAX_REQ_BUFFER_SIZE + 1] = {0};

    uint64_t u64CountBytes = 0;
    try {
        while (!reqStream.eof() && reqStream.good()) {
            uint64_t u64BytesToRead = MAX_REQ_BUFFER_SIZE - u64CountBytes;
            if (u64BytesToRead < 1) {
                respStream << ZApiUtil::HandleResult(API_RES_REQ_SIZE_TOO_BIG, "Request content size too big", true);
                return;
            }

            reqStream.read(buffer + u64CountBytes, u64BytesToRead);

            u64CountBytes += reqStream.gcount();
            if (u64CountBytes > MAX_REQ_BUFFER_SIZE) {
                respStream << ZApiUtil::HandleResult(API_RES_REQ_SIZE_TOO_BIG, "Request content size too big", true);
                return;
            }
        }
    } catch (std::exception& e) {
        ZLogUtil::instance().Debug("Read req data has exception: " + std::string(e.what()));
        respStream << ZApiUtil::HandleResult(API_RES_READ_REQ_FAILED, "Read request data failed", true);
        return;
    }
    strJSonData = std::string(buffer, u64CountBytes);
}

void ZApiCalcHandler::GetJson(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, std::ostream& respStream, MSGINFO& msg) {
    string strJSonData = "";
    HandleStringRequest(request, response, strJSonData, respStream);

    if (ZApiUtil::GetIntegerValueFromJSonString(strJSonData, RDS_MSG_INFO_FIELD_SENDERID, msg.uSenderID) == false)
        return;

    if (ZApiUtil::GetIntegerValueFromJSonString(strJSonData, RDS_MSG_INFO_FIELD_USERID, msg.uUserID) == false)
        return;

    if (ZApiUtil::GetStringValueFromJSonString(strJSonData, RDS_MSG_INFO_FIELD_DATA, msg.strData) == false)
        return;
}

std::string ZApiCalcHandler::ProcessData(const MSGINFO& msg, const ERRCODE& errCode) {

    std::string strProcess = msg.strData + ": data is processed. " + "Error Code: " + std::to_string(errCode.uErrCodeIncr);
    return strProcess;
}

bool bCheckInit = ZRedisProcess::GetInstance().Init("127.0.0.1", 8000);

void ZApiCalcHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::ostream& respStream = response.send();
    struct timeval tv, tv1;
    MSGINFO msg;
    ERRCODE errCode; 

    GetJson(request, response, respStream, msg);

    gettimeofday(&tv, NULL);
    msg.u64TimeStart = tv.tv_usec;

    if (!bCheckInit) {
        std::cerr << "Can not connect to redis cluster!!!" << std::endl;
        return;
    }

    uint64_t u64MsgID = ZRedisProcess::GetInstance().IncrKey("MsgID", errCode.uErrCodeIncr);

    // Save Message Infor
    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID, RDS_MSG_INFO_FIELD_SENDERID, msg.uSenderID))
        return;

    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID, RDS_MSG_INFO_FIELD_USERID, msg.uUserID))
        return;

    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID,RDS_MSG_INFO_FIELD_DATA, msg.strData))
        return;

    gettimeofday(&tv1, NULL);
    msg.u64TimeEnd = tv1.tv_usec;
    msg.u64TimeProcess = msg.u64TimeEnd - msg.u64TimeStart;

    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID,RDS_MSG_INFO_FIELD_TIME_START, msg.u64TimeStart))
        return;

    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID,RDS_MSG_INFO_FIELD_TIME_PROCCESS, msg.u64TimeProcess))
        return;

    if (!ZRedisProcess::GetInstance().SetMsgIDInfo(u64MsgID,RDS_MSG_INFO_FIELD_RESULT, msg.uResult))
        return;

    //Statistics SenderID, UserID
    if (!ZRedisProcess::GetInstance().SetUserIDAndSenderIDInfo(u64MsgID, msg.uSenderID, msg.uUserID))
        return;

    if (!ZRedisProcess::GetInstance().IncreaseResult(u64MsgID, errCode.uErrCodeIncr))
        return;

    if (!ZRedisProcess::GetInstance().GetAverageTimeProccess(u64MsgID,  msg.u64TimeProcess))
        return;

    if (!ZRedisProcess::GetInstance().CountSenderID(msg.uSenderID))
        return;

    if (!ZRedisProcess::GetInstance().CountUserID(msg.uUserID))
        return;

    respStream << ZApiCalcHandler::ProcessData(msg, errCode);
}


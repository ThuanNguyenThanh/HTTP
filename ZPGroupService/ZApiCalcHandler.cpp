/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"ZApiCalcHandler.h"
#include "ZApiUtil.h"

ZApiCalcHandler::ZApiCalcHandler() {

}

ZApiCalcHandler::~ZApiCalcHandler() {

}

//uint32_t ZApiCalcHandler::GetSenderID() {
//    return m_uSenderID;
//}
//
//uint32_t ZApiCalcHandler::GetUserID() {
//    return m_uUserID;
//}
//
//std::string ZApiCalcHandler::GetData() {
//    return m_strData;
//}

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

    if (ZApiUtil::GetIntegerValueFromJSonString(strJSonData, msg.strKey1, msg.uSenderID) == false)
        return;

    if (ZApiUtil::GetIntegerValueFromJSonString(strJSonData, msg.strKey2, msg.uUserID) == false)
        return;

    if (ZApiUtil::GetStringValueFromJSonString(strJSonData, msg.strKey3, msg.strData) == false)
        return;
}

std::string ZApiCalcHandler::ProcessData(const MSGINFO& msg) {

    std::string strProcess = msg.strData + ": data is processed";
    return strProcess;
}

bool bCheckInit = ZRedisProcess::GetInstance().Init("127.0.0.1", 8000);

void ZApiCalcHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::ostream& respStream = response.send();
    struct timeval tv, tv1;
    MSGINFO msg;

    GetJson(request, response, respStream, msg);

    gettimeofday(&tv, NULL);
    msg.ullTimeStart = tv.tv_usec;

    if (!bCheckInit) {
        std::cerr << "Can not connect to redis cluster!!!" << std::endl;
        return;
    }

    std::string strHash = std::to_string(ZRedisProcess::GetInstance().IncrKey("0"));

    ZRedisProcess::GetInstance().HSetMsgID(strHash, msg.strKey1, std::to_string(msg.uSenderID));
    ZRedisProcess::GetInstance().HSetMsgID(strHash, msg.strKey2, std::to_string(msg.uUserID));
    ZRedisProcess::GetInstance().HSetMsgID(strHash, msg.strKey3, msg.strData);

    gettimeofday(&tv1, NULL);
    msg.ullTimeStart = tv1.tv_usec;
    msg.ullTimeProcess = msg.ullTimeEnd - msg.ullTimeStart;
    ZRedisProcess::GetInstance().HSetMsgID(strHash, msg.strKey4, std::to_string(msg.ullTimeStart));

    if (msg.RandomResult() == 1)
        msg.strResult = "Success";
    else
        msg.strResult = "Fail";

    ZRedisProcess::GetInstance().HSetMsgID(strHash, msg.strKey5, msg.strResult);
    respStream << ZApiCalcHandler::ProcessData(msg);
}


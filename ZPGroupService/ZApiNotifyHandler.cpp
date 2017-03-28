/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiAddStringHandler.cpp
 * Author: root
 * 
 * Created on March 13, 2017, 11:20 AM
 */

#include "ZApiNotifyHandler.h"

ZApiNotifyHandler::ZApiNotifyHandler() {
}

ZApiNotifyHandler::ZApiNotifyHandler(const ZApiNotifyHandler& orig) {
}

ZApiNotifyHandler::~ZApiNotifyHandler() {
}

std::string ZApiNotifyHandler::AddString()
{
    return this->m_strData;
}

void ZApiNotifyHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    std::ostream& respStream = response.send();
    GetJson(request, response, respStream);
    
//    if (!ZRedisProcess::GetInstance().Init("127.0.0.1", 8000)) {
//        std::cerr << "Can not connect to redis cluster!!!" << std::endl;
//        return;
//    }
//
//    ZRedisProcess::GetInstance().HSetMsgID();
    
    respStream << ZApiNotifyHandler::AddString();
}


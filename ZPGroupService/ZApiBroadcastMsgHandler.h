/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZBroadcastMessageHandler.h
 * Author: danhcc
 *
 * Created on October 13, 2016, 3:05 PM
 */

#ifndef ZBROADCASTMESSAGEHANDLER_H
#define ZBROADCASTMESSAGEHANDLER_H


#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class ZApiBroadcastMsgHandler : public Poco::Net::HTTPRequestHandler
{
public:
    ZApiBroadcastMsgHandler();
    ~ZApiBroadcastMsgHandler();

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
    {
    };

protected:
    void HandleStringRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    void HandleProtobufRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    

    bool CheckSignature(const std::string& strSignature);
};

#endif /* ZBROADCASTMESSAGEHANDLER_H */


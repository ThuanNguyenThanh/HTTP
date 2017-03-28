/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiMTGProtobufHandler.h
 * Author: danhcc
 *
 * Created on October 25, 2016, 10:34 AM
 */

#ifndef ZAPIMTGPROTOBUFHANDLER_H
#define ZAPIMTGPROTOBUFHANDLER_H

#include "ZApiBroadcastMsgHandler.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class ZApiMTGProtobufHandler : public ZApiBroadcastMsgHandler
{
public:
    ZApiMTGProtobufHandler();
    ~ZApiMTGProtobufHandler();

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};

#endif /* ZAPIMTGPROTOBUFHANDLER_H */


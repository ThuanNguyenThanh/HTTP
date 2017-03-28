/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ZApiMTGProtobufHandler.h"
#include "ZApiDefine.h"
#include "ZApiUtil.h"
#include "ZConfigReader.h"
#include "ZLogUtil.h"

#include <Poco/NumberFormatter.h>

ZApiMTGProtobufHandler::ZApiMTGProtobufHandler() : ZApiBroadcastMsgHandler()
{

}

ZApiMTGProtobufHandler::~ZApiMTGProtobufHandler()
{

}

void ZApiMTGProtobufHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    HandleProtobufRequest(request, response);
}



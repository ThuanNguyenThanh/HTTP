/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ZApiMTGStringHandler.h"
#include "ZApiDefine.h"
#include "ZApiUtil.h"
#include "ZConfigReader.h"
#include "ZLogUtil.h"

#include <Poco/NumberFormatter.h>

ZApiMTGStringHandler::ZApiMTGStringHandler() : ZApiBroadcastMsgHandler()
{

}

ZApiMTGStringHandler::~ZApiMTGStringHandler()
{

}

void ZApiMTGStringHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    HandleStringRequest(request, response);
}


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ZHandlerFactory.h"
#include "ZApiMTGProtobufHandler.h"
#include "ZApiMTGStringHandler.h"
#include "ZApiDefine.h"
#include "ZApiExceptionHandler.h"
#include "ZLogUtil.h"
#include "ZApiCalcHandler.h"
#include <Poco/StringTokenizer.h>
#include <Poco/NumberFormatter.h>

ZHandlerFactory::ZHandlerFactory()
{

}

Poco::Net::HTTPRequestHandler* ZHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    ZLogUtil::instance().Debug("Accept connection from " + request.clientAddress().toString()
                               + " URI " + request.getURI() + " "
                               + Poco::NumberFormatter::format(request.getContentLength64()) + " bytes");

    if (request.getMethod() == "GET") {
        std::string strMsg = "Wrong method";
        return new ZApiExceptionHandler(API_RES_WRONG_METHOD, strMsg);
    }

    std::string strURI = request.getURI();
    Poco::StringTokenizer strTokens(strURI, "/");

    // strTokens[0] is always empty
    std::string strMsg = "Wrong API";
    if (strTokens.count() < 4) {
        return new ZApiExceptionHandler(API_RES_WRONG_API, strMsg);
    }

    if (strTokens[1].compare(API_PATH_BROADCAST) != 0)
        return new ZApiExceptionHandler(API_RES_WRONG_API, strMsg);

    if (strTokens[2].compare(API_PATH) != 0)
        return new ZApiExceptionHandler(API_RES_WRONG_API, strMsg);


    if (strTokens[3].compare(API_PATH_BROADCAST_PROTOBUF) == 0) {
        return new ZApiMTGProtobufHandler();
    }

    if (strTokens[3].compare(API_PATH_BROADCAST_STRING) == 0) {
        return new ZApiMTGStringHandler();
    }

    if (strTokens[3].compare(API_PATH_NOTIFICATION) == 0) {
        return new ZApiCalcHandler();
    }
    
    return new ZApiExceptionHandler(API_RES_WRONG_API, strMsg);
}
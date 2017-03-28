/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiAddStringHandler.h
 * Author: root
 *
 * Created on March 13, 2017, 11:20 AM
 */

#ifndef ZAPINOTIFYHANDLER_H
#define ZAPINOTIFYHANDLER_H

#include "ZApiCalcHandler.h"
#include "ZRedisProcess.h"
class ZApiNotifyHandler : public ZApiCalcHandler{
public:
    ZApiNotifyHandler();
    ZApiNotifyHandler(const ZApiNotifyHandler& orig);
    virtual ~ZApiNotifyHandler();
    std::string AddString();
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    
private:

};

#endif /* ZAPIADDSTRINGHANDLER_H */


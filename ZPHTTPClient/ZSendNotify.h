/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZSendNotify.h
 * Author: root
 *
 * Created on March 23, 2017, 5:31 PM
 */

#ifndef ZSENDNOTIFY_H
#define ZSENDNOTIFY_H


#include <Poco/NumberFormatter.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>

#include <stddef.h>
#include <unistd.h>
#include <string>

class ZSendNotify {
public:
    ZSendNotify();
    ZSendNotify(uint32_t, uint32_t, std::string);
    ZSendNotify(const ZSendNotify& orig);
    virtual ~ZSendNotify();

    void SetSenderID(uint32_t);
    void SetUserID(uint32_t);
    void SetData(std::string);

    uint32_t GetSenderID();
    uint32_t GetUserID();
    std::string GetData();

    std::string SetMessageID(uint32_t, uint32_t, std::string);
    std::string MessageID(uint32_t, uint32_t, std::string);
    std::string SendPostRequest(const std::string& strURI, const std::string& strBody);
private:
    uint32_t m_uSenderID;
    uint32_t m_uUserID;
    std::string m_strData;
};

#endif /* ZSENDNOTIFY_H */


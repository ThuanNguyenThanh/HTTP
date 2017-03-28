/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZSendNotify.cpp
 * Author: root
 * 
 * Created on March 23, 2017, 5:31 PM
 */

#include "ZSendNotify.h"

ZSendNotify::ZSendNotify() {

}

ZSendNotify::ZSendNotify(const ZSendNotify& orig) {
}

ZSendNotify::~ZSendNotify() {
}

void ZSendNotify::SetSenderID(uint32_t uSenderID)
{
    m_uSenderID = uSenderID;
}

void ZSendNotify::SetUserID(uint32_t uUserID)
{
    m_uUserID = uUserID;
}

void ZSendNotify::SetData(std::string strData)
{
    m_strData = strData;
}

uint32_t ZSendNotify::GetSenderID()
{
    return m_uSenderID;
}

uint32_t ZSendNotify::GetUserID()
{
    return m_uUserID;
}

std::string ZSendNotify::GetData()
{
    return m_strData;
}

ZSendNotify::ZSendNotify(uint32_t uSenderID, uint32_t uUserID, std::string strData)
{
    m_uSenderID = uSenderID;
    m_uUserID = uUserID;
    m_strData = strData;
}

std::string ZSendNotify::MessageID(uint32_t uSenderID, uint32_t uUserID, std::string strData)
{
    if(uSenderID > 4294967293 || uSenderID > 4294967293 || strData == "")
        return "";
    std::string strMsgID = "{\"SenderID\":" + std::to_string(uSenderID) + ", " + "\"UserID\":" + std::to_string(uUserID) + ", " + "\"Data\":" + "\"" + strData + "\"}";
    return strMsgID;
}

std::string ZSendNotify::SendPostRequest(const std::string& strURI, const std::string& strBody)
{
    if (strURI.empty()) {
        return "";
    }

    std::string strResp = "";
    try {
        Poco::URI uri(strURI);
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

        std::string strPath(uri.getPathAndQuery());
        if (strPath.empty())
            strPath = "/";

        // send request
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, strPath, Poco::Net::HTTPMessage::HTTP_1_1);
        req.setContentLength(strBody.length());
        session.sendRequest(req) << strBody;

        // get response
        Poco::Net::HTTPResponse resp;
        std::istream& rs = session.receiveResponse(resp);

        Poco::StreamCopier::copyToString(rs, strResp);
    }
    catch (Poco::Exception &ex) {
    }
    catch (std::exception &e) {
    }

    return strResp;
}
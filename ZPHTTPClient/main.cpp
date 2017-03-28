/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on March 7, 2017, 2:15 PM
 */
#include "ZSendNotify.h"

int main(int argc, char** argv) {
    uint32_t uSenderID, uUserID;
    ZSendNotify notify;
    
    std::string strData = "";
    std::cout << "Enter SenderID: " << std::endl;
    std::cin >> uSenderID;
    notify.SetSenderID(uSenderID);
    
    std::cout << "Enter UserID: " << std::endl;
    std::cin >> uUserID;
    notify.SetUserID(uUserID);
    
    std::cout << "Enter Data: " << std::endl;
    std::cin.ignore();
    std::getline(std::cin, strData);
    notify.SetData(strData);
    
    //ZSendNotify(uSenderID, uUserID, strData);
    std::string strMsgID = notify.MessageID(notify.GetSenderID(), notify.GetUserID(), notify.GetData());
    if(strMsgID == "")    
    {
        std::cout<<"Data not suitable";
        return 0;
    }
    std::string strResp = notify.SendPostRequest("http://localhost:9999/group/api/notification", strMsgID);
    std::cout<< strResp <<std::endl;
    return 0;
}



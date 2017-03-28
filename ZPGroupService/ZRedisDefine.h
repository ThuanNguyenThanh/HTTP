/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZRedisDefine.h
 * Author: danhcc
 *
 * Created on September 29, 2016, 8:41 AM
 */

#ifndef ZREDISDEFINE_H
#define ZREDISDEFINE_H

#define RDS_MTG_COUNTER                     "ns:mtg:counter"
#define RDS_MTG_INFO_FIELD_DATA             "data"
#define RDS_MTG_INFO_FIELD_IS_PRIVATE       "isprivate"
#define RDS_MTG_INFO_FIELD_MSG_TYPE         "msgtype"
#define RDS_MTG_INFO_FIELD_PUSH_DATA        "pushdata"
#define RDS_MTG_INFO_FIELD_PUSH_TITLE       "pushtitle"
#define RDS_MTG_INFO_FIELD_SOURCE_ID        "sourceid"
#define RDS_MTG_INFO_FIELD_EXPIRE_TIME      "expiretime"
#define RDS_MTG_SET                         "ns:mtg:set"

#define MAX_TIME_EXPIRE_KEY                 172800 // seconds (48h)

#endif /* ZREDISDEFINE_H */


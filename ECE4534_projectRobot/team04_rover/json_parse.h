/*
 * json_parse.h
 *
 *  Created on: Oct 12, 2019
 *      Author: mmcgr
 */

#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define JSMN_HEADER
#include "jsmn.h"
#include "control_queue.h"
#include "navigation_queue.h"
#include <ti/drivers/dpl/HwiP.h>
/* Defining Number of subscription topics                                    */
#define SUBSCRIPTION_TOPIC_COUNT 4

#define PUBLISH_PAYLOAD_BUFFER_SIZE         512
#define LUT_SIZE                            256
#define INT_BUFFER_SIZE 8

#define JSON_INITIAL_KEY        "{"
#define JSON_SEPERATOR_KEY      ", "
#define JSON_MSGID_KEY          "\"msgID\": "
#define JSON_COUNT_KEY          "\"count\": "
#define JSON_SUM_KEY          "\"sum\": "
#define JSON_FINAL_KEY          "}"
#define JSON_NUMP_KEY          "\"published\": "
#define JSON_NUMR_KEY          "\"received\": "
#define JSON_NUMM_KEY          "\"missed\": "
#define JSON_WHEEL1_KEY         "\"wheel1\": "
#define JSON_WHEEL2_KEY         "\"wheel2\": "
#define JSON_WHEEL3_KEY         "\"wheel3\": "
#define JSON_SPEED_KEY          "\"speed\": "
#define JSON_STILL_KEY          "\"inStill\": "


// parse
#define JSON_parse_msgID        "Message_ID"
#define JSON_parse_count        "count"
#define JSON_parse_LUT          "LUT"
#define JSON_parse_dir          "direction"
#define JSON_parse_distance     "distance"
#define JSON_parse_moveType     "type"
#define JSON_parse_speed        "speed"
#define JSON_parse_wheel        "wheel"
#define JSON_parse_forward      "forward"
#define JSON_parse_back         "back"
#define JSON_parse_left         "left"
#define JSON_parse_right        "right"
#define JSON_parse_stop         "stop"
#define JSON_parse_cw           "clockwise"
#define JSON_parse_ccw          "counterclockwise"
#define JSON_parse_cw90         "cwTime"
#define JSON_parse_ccw90        "ccwTime"
#define JSON_parse_rotate       "rotate"
#define JSON_parse_degree       "angle"
#define JSON_parse_msg_type     "command"
#define JSON_parse_key_dir1     "dir1"
#define JSON_parse_key_dir2     "dir2"
#define JSON_parse_key_dir3     "dir3"
#define JSON_parse_key_speed1     "s1"
#define JSON_parse_key_speed2     "s2"
#define JSON_parse_key_speed3     "s3"
#define JSON_parse_key_time       "time"

#define Movement_MSG            0x0
#define Wheel_Test_MSG             0x1
#define Rotate_MSG              0x2
#define Stop_MSG              0x3
#define Wheels_Move_MSG       0x4


extern int8_t LUT_array[LUT_SIZE];

typedef void (*topic_func_type)(const char * payload_p);

topic_func_type topic_functions[SUBSCRIPTION_TOPIC_COUNT];

void json_parse_topic_functions_init(topic_func_type *tfuncs);

void json_parse_big(const char *payload_p, uint16_t *msgID_p);

void json_parse_command(const char *payload_p, uint16_t *msgID_p);

void json_parse_topic1(const char *payload_p, uint16_t *msgID_p);

void json_parse_topic2(const char *payload_p);

void json_parse_topic3(const char *payload_p);

void json_parse_recvd(const char *payload_p);

void json_stringify_topic0(char *payload_p, uint16_t msgID, uint16_t published_num, uint16_t received_num, uint16_t missed_num);


void json_stringify_topic1(char *payload_p, uint16_t msgID, uint16_t testVal);

void json_stringify_topic2(char *payload_p, uint16_t msgID, uint16_t testVal);

void json_stringify_wheelSpeeds(char *payload_p, uint16_t msgID, uint32_t speed1, uint32_t speed2, uint32_t speed3);

void json_stringify_still(char *payload_p, uint16_t msgID, uint16_t isStill);

#endif /* JSON_PARSE_H_ */

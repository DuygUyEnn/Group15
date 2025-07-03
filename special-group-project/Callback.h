//
// Created by Admin on 12/15/2024.
//
#include "Devices.h"
#include "Gateways.h"
#ifndef SPECIAL_GROUP_PROJECT_CALLBACK_H
#define SPECIAL_GROUP_PROJECT_CALLBACK_H
struct CallbackContext {
    Devices device_data; // represent for Devices table in database
    Gateways gateway_data; // represent for Gateways table in database
    Messages messages_data; // represent for Messages table in database
    bool message_received = false; // State when received message or not
};

#endif //SPECIAL_GROUP_PROJECT_CALLBACK_H

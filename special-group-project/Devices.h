//
// Created by Admin on 12/13/2024.
//
#include "string"
#include "iostream"
#include "MQTTClient.h"
#ifndef SPECIAL_GROUP_PROJECT_DEVICES_H
#define SPECIAL_GROUP_PROJECT_DEVICES_H

//create Devices object as a table that store data in variable form
class Devices {
public:
    Devices();

    Devices(const std::string& device_id, const std::string& gateway_id, const std::string& model, const std::string& band_id, const float& battery_status )
            : Device_id(device_id), Gateway_id(gateway_id), Model(model), Band_id(band_id), Battery_status(battery_status) {};

    //function for retrieving data from school sensors
    Devices Getting_device_data(void* context, char* topic, int topic_len, MQTTClient_message* message);
    //function for retrieving data from team's sensor
    Devices getting_device_data(const std::string string);
    std::string Device_id_() const;
    std::string Gateway_id_() const;
    std::string Model_() const;
    std::string Band_id_() const;
    float Battery_status_() const;
    void Print() const;
private:
    std::string Device_id;
    std::string Gateway_id;
    std::string Model;
    std::string Band_id;
    float Battery_status;
};
#endif //SPECIAL_GROUP_PROJECT_DEVICES_H

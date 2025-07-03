//
// Created by Admin on 12/13/2024.
//
#include "iostream"
#include "string"
#include "MQTTClient.h"
#ifndef SPECIAL_GROUP_PROJECT_MESSAGES_H
#define SPECIAL_GROUP_PROJECT_MESSAGES_H

//create Messages object as a table that store data in variable form
class Messages {
public:
    Messages();
    Messages(const std::string& time, const std::string& device_id, const unsigned int& timestamp,
             const float& humidity, const float& light, const float& pressure, const float& temperature)
             :Time(time), Device_id(device_id), Timestamp(timestamp), Humidity(humidity), Light(light), Pressure(pressure), Temperature(temperature) {};

    //function for retrieving data from school sensor
    Messages Getting_messages_data(void* context, char* topic, int topic_len, MQTTClient_message* message);
    //function for retrieving data from team's sensor
    Messages getting_device_data(const std::string string);
    std::string Time_() const;
    std::string Device_id_() const;
    unsigned int Timestamp_() const;
    float Humidity_() const;
    float Light_() const;
    float Pressure_() const;
    float Temperature_() const;
    void Print() const;
private:
    std::string Time;
    std::string Device_id;
    unsigned int Timestamp;
    float Humidity;
    float Light;
    float Pressure;
    float Temperature;
};
#endif //SPECIAL_GROUP_PROJECT_MESSAGES_H

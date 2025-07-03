//
// Created by Admin on 12/13/2024.
//
#include "iostream"
#include "string"
#include "MQTTClient.h"
#ifndef SPECIAL_GROUP_PROJECT_GATEWAYS_H
#define SPECIAL_GROUP_PROJECT_GATEWAYS_H

//create Gateways object as a table that store data in variable form
class Gateways {
public:
    Gateways();

    Gateways(const std::string& gateway_id, const std::string& eui, const float& latitude, const float& longitude, const float& altitude)
            : Gateway_id(gateway_id), Eui(eui), Latitude(latitude), Longitude(longitude), Altitude(altitude) {};

    // function for retrieving data from school sensors
    Gateways Getting_gateway_data(void* context, char* topic, int topic_len, MQTTClient_message* message);
    // function for retrieving data from team's sensor
    Gateways getting_gateway_data(const std::string string);
    std::string Gateway_id_() const;
    std::string Eui_() const;
    float Latitude_() const;
    float Longitude_() const;
    float Altitude_() const;
    void Print() const;
private:
    std::string Gateway_id;
    std::string Eui;
    float Latitude;
    float Longitude;
    float Altitude;
};
#endif //SPECIAL_GROUP_PROJECT_GATEWAYS_H
#include "Gateways.h"
#include <MQTTClient.h>
#include <nlohmann/json.hpp>
// Định nghĩa constructor mặc định
using json = nlohmann::json;
Gateways::Gateways()
        : Gateway_id("Unknown"), Eui("Unknown"), Latitude(-1), Longitude(-1), Altitude(-1) {}


Gateways Gateways::Getting_gateway_data (void *context, char *topic, int topic_Len, MQTTClient_message *message) {
    Gateways gateways;
    std::string payload(static_cast<char*>(message->payload), message-> payloadlen);
    try {
        // Parse payload JSON
        json received_json = json::parse(payload);
        // gateway-> eui -> latitude -> longitude -> altitude
        Gateways gateways_temp = {
                received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("gateway_id") &&
                !received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].is_null()
                ? received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].get<std::string>()
                : "Unknown Gateway",

                received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("eui") &&
                !received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["eui"].is_null()
                ? received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["eui"].get<std::string>()
                : "Unknown EUI",

                received_json["uplink_message"]["rx_metadata"][0]["location"].contains("latitude") &&
                !received_json["uplink_message"]["rx_metadata"][0]["location"]["latitude"].is_null()
                ?received_json["uplink_message"]["rx_metadata"][0]["location"]["latitude"].get<float>()
                : -1,

                received_json["uplink_message"]["rx_metadata"][0]["location"].contains("longitude") &&
                !received_json["uplink_message"]["rx_metadata"][0]["location"]["longitude"].is_null()
                ?received_json["uplink_message"]["rx_metadata"][0]["location"]["longitude"].get<float>()
                : -1,

                received_json["uplink_message"]["rx_metadata"][0]["location"].contains("altitude") &&
                !received_json["uplink_message"]["rx_metadata"][0]["location"]["altitude"].is_null()
                ?received_json["uplink_message"]["rx_metadata"][0]["location"]["altitude"].get<float>()
                : -1

        };


        gateways = gateways_temp;

    } catch (const std::exception& e) { // if parsing error, throw exception and print error message
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return gateways;
}

Gateways Gateways::getting_gateway_data(const std::string string) {
    Gateways gateways;
    try {
        // Parse payload JSON
        json received_json = json::parse(string);
        // gateway-> eui -> latitude -> longitude -> altitude
        Gateways gateways_temp = {
                received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("gateway_id") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].is_null()
                ? received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].get<std::string>()
                : "Unknown Gateway",

                received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("eui") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["eui"].is_null()
                ? received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["eui"].get<std::string>()
                : "Unknown EUI",

                received_json["data"]["uplink_message"]["rx_metadata"][0]["location"].contains("latitude") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["latitude"].is_null()
                ?received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["latitude"].get<float>()
                : -1,

                received_json["data"]["uplink_message"]["rx_metadata"][0]["location"].contains("longitude") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["longitude"].is_null()
                ?received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["longitude"].get<float>()
                : -1,

                received_json["data"]["uplink_message"]["rx_metadata"][0]["location"].contains("altitude") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["altitude"].is_null()
                ?received_json["data"]["uplink_message"]["rx_metadata"][0]["location"]["altitude"].get<float>()
                : -1

        };


        gateways = gateways_temp;

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return gateways;
}

std::string Gateways::Gateway_id_() const {
    return Gateway_id;
}

std::string Gateways::Eui_() const {
    return Eui;
}

float Gateways::Latitude_() const {
    return Latitude;
}

float Gateways::Longitude_() const {
    return Longitude;
}

float Gateways::Altitude_() const {
    return Altitude;
}

void Gateways::Print() const {
    std::cout << "Gateway Information:" << std::endl;
    std::cout << "  Gateway ID: " << Gateway_id << std::endl;
    std::cout << "   EUI: " << Eui << std::endl;
    std::cout << "  Latitude: " << Latitude << std::endl;
    std::cout << "  Longitude: " << Longitude << std::endl;
    std::cout << "  Altitude: " << Altitude << std::endl;
}


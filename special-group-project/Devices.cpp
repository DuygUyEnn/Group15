#include "Devices.h"
#include <MQTTClient.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// Default constructor (the same with Gateways and Messages object)
Devices::Devices()
        : Device_id("Unknown"), Gateway_id("Unknown"), Model("Unknown"), Band_id("Unknown"), Battery_status(-1) {}

//Function for retrieving message and data from school sensors
Devices Devices::Getting_device_data (void *context, char *topic, int topic_Len, MQTTClient_message *message) {
    Devices devices_data;
    std::string payload(static_cast<char*>(message->payload), message-> payloadlen);
    try {
        // Parse payload JSON
        json received_json = json::parse(payload);
        std::string device_type = received_json["end_device_ids"]["device_id"].get<std::string>();
        //baterry for mkr not exist
        if (device_type.find("mkr") != std::string::npos) {
            //Filtering Devices
            Devices devices_data_temp = {
                    received_json["end_device_ids"].contains("device_id") &&
                    !received_json["end_device_ids"]["device_id"].is_null()
                    ? received_json["end_device_ids"]["device_id"].get<std::string>()
                    : "Unknown Device",

                    received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("gateway_id") &&
                    !received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].is_null()
                    ? received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].get<std::string>()
                    : "Unknown Gateway",

                    received_json["uplink_message"]["version_ids"].contains("model_id") &&
                    !received_json["uplink_message"]["version_ids"]["model_id"].is_null()
                    ? received_json["uplink_message"]["version_ids"]["model_id"].get<std::string>()
                    : "Unknown Model",

                    received_json["uplink_message"]["version_ids"].contains("band_id") &&
                    !received_json["uplink_message"]["version_ids"]["band_id"].is_null()
                    ? received_json["uplink_message"]["version_ids"]["band_id"].get<std::string>()
                    : "Unknown Band",

                    // battery status = -1 mean not exist
                    received_json["uplink_message"]["decoded_payload"].contains("Bat_status") &&
                    !received_json["uplink_message"]["decoded_payload"]["Bat_status"].is_null()
                    ? received_json["uplink_message"]["decoded_payload"]["Bat_status"].get<float>()
                    : -1
            };
            devices_data = devices_data_temp;
        }else if (device_type.find("lht") != std::string::npos) { //checking if Device is lht type
            Devices devices_data_temp = {
                    received_json["end_device_ids"].contains("device_id") &&
                    !received_json["end_device_ids"]["device_id"].is_null()
                    ? received_json["end_device_ids"]["device_id"].get<std::string>()
                    : "Unknown Device",

                    received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("gateway_id") &&
                    !received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].is_null()
                    ? received_json["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].get<std::string>()
                    : "Unknown Gateway",

                    received_json["uplink_message"]["version_ids"].contains("model_id") &&
                    !received_json["uplink_message"]["version_ids"]["model_id"].is_null()
                    ? received_json["uplink_message"]["version_ids"]["model_id"].get<std::string>()
                    : "Unknown Model",

                    received_json["uplink_message"]["version_ids"].contains("band_id") &&
                    !received_json["uplink_message"]["version_ids"]["band_id"].is_null()
                    ? received_json["uplink_message"]["version_ids"]["band_id"].get<std::string>()
                    : "Unknown Band",


                    received_json["uplink_message"]["decoded_payload"].contains("Bat_status") &&
                    !received_json["uplink_message"]["decoded_payload"]["Bat_status"].is_null()
                    ? received_json["uplink_message"]["decoded_payload"]["Bat_status"].get<float>()
                    : -1

            };
            devices_data = devices_data_temp;
        } else {
            std::cout << "Unknown device \n";
        }
        //device -> gateway-> model -> band_id


    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return devices_data;
}

// Function for retrieving team's sensor
Devices Devices::getting_device_data(const std::string string) {
    Devices devices;
    try {
        // Parse payload JSON
        json received_json = json::parse(string);
        // gateway-> eui -> latitude -> longitude -> altitude
        Devices devices_temp = {
                received_json["data"]["end_device_ids"].contains("device_id") &&
                !received_json["data"]["end_device_ids"]["device_id"].is_null()
                ? received_json["data"]["end_device_ids"]["device_id"].get<std::string>()
                : "Unknown Device",

                received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"].contains("gateway_id") &&
                !received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].is_null()
                ? received_json["data"]["uplink_message"]["rx_metadata"][0]["gateway_ids"]["gateway_id"].get<std::string>()
                : "Unknown Gateway",

                received_json["data"]["uplink_message"]["version_ids"].contains("model_id") &&
                !received_json["data"]["uplink_message"]["version_ids"]["model_id"].is_null()
                ? received_json["data"]["uplink_message"]["version_ids"]["model_id"].get<std::string>()
                : "Unknown Model",

                    received_json["data"]["uplink_message"]["version_ids"].contains("band_id") &&
                    !received_json["data"]["uplink_message"]["version_ids"]["band_id"].is_null()
                    ? received_json["data"]["uplink_message"]["version_ids"]["band_id"].get<std::string>()
                    : "Unknown Band",


                received_json["data"]["uplink_message"]["decoded_payload"].contains("Bat_status") &&
                !received_json["data"]["uplink_message"]["decoded_payload"]["Bat_status"].is_null()
                ? received_json["data"]["uplink_message"]["decoded_payload"]["Bat_status"].get<float>()
                : -1
        };


        devices = devices_temp;

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return devices;
}

std::string Devices::Device_id_() const {
    return Device_id;
}

std::string Devices::Gateway_id_() const {
    return Gateway_id;
}

std::string Devices::Model_() const {
    return Model;
}

std::string Devices::Band_id_() const {
    return Band_id;
}

float Devices::Battery_status_() const {
    return Battery_status;
}

//Function for print result after each message
void Devices::Print() const {
    std::cout << "Device Information:" << std::endl;
    std::cout << "  Device ID: " << Device_id << std::endl;
    std::cout << "  Gateway ID: " << Gateway_id << std::endl;
    std::cout << "  Model: " << Model << std::endl;
    std::cout << "  Band ID: " << Band_id << std::endl;
    std::cout << "  Battery status: " << Battery_status << std::endl;
}
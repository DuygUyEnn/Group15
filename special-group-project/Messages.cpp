#include "Messages.h"
#include <MQTTClient.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
Messages::Messages()
         :  Time("Unknown"), Device_id("Unknown"), Timestamp(-1), Humidity(-1), Light(-1), Pressure(-1), Temperature(-1)  {}


Messages Messages::Getting_messages_data(void *context, char *topic, int topic_len, MQTTClient_message *message) {
    Messages messages;
    std::string payload(static_cast<char*>(message->payload), message-> payloadlen);
    try {
        // Parse payload JSON
        json received_json = json::parse(payload);
        std::string device_type = received_json["end_device_ids"]["device_id"].get<std::string>();
        if (device_type.find("mkr") != std::string::npos) {
            Messages messages_temp = {
                     received_json["uplink_message"]["received_at"],
                     received_json["end_device_ids"]["device_id"],

                     received_json["uplink_message"]["rx_metadata"][0].contains("timestamp") &&
                              !received_json["uplink_message"]["rx_metadata"][0]["timestamp"].is_null()
                              ? received_json["uplink_message"]["rx_metadata"][0]["timestamp"].get<unsigned int>()
                              : -1 ,

                     received_json["uplink_message"]["decoded_payload"].contains("humidity") &&
                     !received_json["uplink_message"]["decoded_payload"]["humidity"].is_null()
                     ?received_json["uplink_message"]["decoded_payload"]["humidity"].get<float>()
                     : -1,
                     received_json["uplink_message"]["decoded_payload"].contains("light") &&
                     !received_json["uplink_message"]["decoded_payload"]["light"].is_null()
                     ?received_json["uplink_message"]["decoded_payload"]["light"].get<float>()
                     : -1,
                     received_json["uplink_message"]["decoded_payload"].contains("pressure") &&
                     !received_json["uplink_message"]["decoded_payload"]["pressure"].is_null()
                     ?received_json["uplink_message"]["decoded_payload"]["pressure"].get<float>()
                     : -1,
                     received_json["uplink_message"]["decoded_payload"].contains("temperature") &&
                     !received_json["uplink_message"]["decoded_payload"]["temperature"].is_null()
                     ?received_json["uplink_message"]["decoded_payload"]["temperature"].get<float>()
                     : std::numeric_limits<float>::quiet_NaN()
            };
            messages = messages_temp;

        }else if (device_type.find("lht") != std::string::npos) {
            Messages messages_temp = {
                    received_json["uplink_message"]["received_at"],
                    received_json["end_device_ids"]["device_id"],

                    received_json["uplink_message"]["rx_metadata"][0].contains("timestamp") &&
                    !received_json["uplink_message"]["rx_metadata"][0]["timestamp"].is_null()
                    ? received_json["uplink_message"]["rx_metadata"][0]["timestamp"].get<unsigned int>()
                    : -1 ,
                    received_json["uplink_message"]["decoded_payload"].contains("Hum_SHT") &&
                    !received_json["uplink_message"]["decoded_payload"]["Hum_SHT"].is_null()
                    ?received_json["uplink_message"]["decoded_payload"]["Hum_SHT"].get<float>()
                    : -1,
                    received_json["uplink_message"]["decoded_payload"].contains("ILL_lx") &&
                    !received_json["uplink_message"]["decoded_payload"]["ILL_lx"].is_null()
                    ?received_json["uplink_message"]["decoded_payload"]["ILL_lx"].get<float>()
                    : -1,
                    received_json["uplink_message"]["decoded_payload"].contains("pressure") &&
                    !received_json["uplink_message"]["decoded_payload"]["pressure"].is_null()
                    ?received_json["uplink_message"]["decoded_payload"]["pressure"].get<float>()
                    : -1,
                    received_json["uplink_message"]["decoded_payload"].contains("TempC_SHT") &&
                    !received_json["uplink_message"]["decoded_payload"]["TempC_SHT"].is_null()
                    ?received_json["uplink_message"]["decoded_payload"]["TempC_SHT"].get<float>()
                    : std::numeric_limits<float>::quiet_NaN()
            };
            messages = messages_temp;

        }else {
            std::cout << "Unknown device \n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return messages;
}

Messages Messages::getting_device_data(const std::string string) {
    Messages messages;
    try {
        // Parse payload JSON
        json received_json = json::parse(string);
        std::string device_type = received_json["data"]["end_device_ids"]["device_id"].get<std::string>();
        if (device_type.find("mkr") != std::string::npos) {
            Messages messages_temp = {
                    received_json["data"]["uplink_message"]["received_at"],
                    received_json["data"]["end_device_ids"]["device_id"],

                    received_json["data"]["uplink_message"]["rx_metadata"][0].contains("timestamp") &&
                    !received_json["data"]["uplink_message"]["rx_metadata"][0]["timestamp"].is_null()
                    ? received_json["data"]["uplink_message"]["rx_metadata"][0]["timestamp"].get<unsigned int>()
                    : -1 ,

                    received_json["data"]["uplink_message"]["decoded_payload"].contains("humidity") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["humidity"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["humidity"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("light") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["light"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["light"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("pressure") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["pressure"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["pressure"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("temperature") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["temperature"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["temperature"].get<float>()
                    : std::numeric_limits<float>::quiet_NaN()
            };
            messages = messages_temp;

        }else if (device_type.find("lht") != std::string::npos) {
            Messages messages_temp = {
                    received_json["data"]["uplink_message"]["received_at"],
                    received_json["data"]["end_device_ids"]["device_id"],

                    received_json["data"]["uplink_message"]["rx_metadata"][0].contains("timestamp") &&
                    !received_json["data"]["uplink_message"]["rx_metadata"][0]["timestamp"].is_null()
                    ? received_json["data"]["uplink_message"]["rx_metadata"][0]["timestamp"].get<unsigned int>()
                    : -1 ,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("Hum_SHT") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["Hum_SHT"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["Hum_SHT"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("ILL_lx") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["ILL_lx"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["ILL_lx"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("pressure") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["pressure"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["pressure"].get<float>()
                    : -1,
                    received_json["data"]["uplink_message"]["decoded_payload"].contains("TempC_SHT") &&
                    !received_json["data"]["uplink_message"]["decoded_payload"]["TempC_SHT"].is_null()
                    ?received_json["data"]["uplink_message"]["decoded_payload"]["TempC_SHT"].get<float>()
                    : std::numeric_limits<float>::quiet_NaN()
            };
            messages = messages_temp;

        }else {
            std::cout << "Unknown device \n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON or processing data: " << e.what() << "\n";
    }
    return messages;
}

std::string Messages::Time_() const {
    return Time;
}

std::string Messages::Device_id_() const {
    return Device_id;
}

unsigned int Messages::Timestamp_() const {
    return Timestamp;
}

float Messages::Humidity_() const {
    return Humidity;
}

float Messages::Light_() const {
    return Light;
}

float Messages::Pressure_() const {
    return Pressure;
}

float Messages::Temperature_() const {
    return Temperature;
}

void Messages::Print() const {
    if(Device_id.find("mkr") != std::string::npos) {
        std::cout << "Message Information:" << std::endl;
        std::cout << "  Time: " << Time << std::endl;
        std::cout << "  Device_id: " << Device_id << std::endl;
        std::cout << "  Timestamp: " << Timestamp << std::endl;
        std::cout << "  Humidity: " << Humidity << std::endl;
        std::cout << "  Light: " << Light << std::endl;
        std::cout << "  Pressure: " << Pressure << std::endl;
        std::cout << "  Inside temp: " << Temperature << std::endl;
    } else if (Device_id.find("lht") != std::string::npos) {
        std::cout << "Message Information:" << std::endl;
        std::cout << "  Time: " << Time << std::endl;
        std::cout << "  Device_id: " << Device_id << std::endl;
        std::cout << "  Timestamp: " << Timestamp << std::endl;
        std::cout << "  Humidity: " << Humidity << std::endl;
        std::cout << "  Light: " << Light << std::endl;
        std::cout << "  Pressure: " << Pressure << std::endl;
        std::cout << "  Outside temp: " << Temperature << std::endl;
    }
}

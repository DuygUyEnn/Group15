    #include <iostream>
    #include <MQTTClient.h>
    #include "stdio.h"
    #include "unistd.h"
    #include "string.h"
    #include "stdlib.h"
    #include <nlohmann/json.hpp>
    #include "Devices.h"
    #include "Gateways.h"
    #include "Messages.h"
    #include "Callback.h"
    #include <pqxx/pqxx>
    #include "Postgre_connection.h"



    using json = nlohmann::json;
    #define ADDRESS1 "eu1.cloud.thethings.network"
    #define PASSWORD1 "NNSXS.DTT4HTNBXEQDZ4QYU6SG73Q2OXCERCZ6574RVXI.CQE6IG6FYNJOO2MOFMXZVWZE4GXTCC2YXNQNFDLQL4APZMWU6ZGA"
    #define ADDRESS2 "eu1.cloud.thethings.network"
    #define PASSWORD2 "NNSXS.LJI7N7UF4HXBTB3NIIKCL7L4ORKOFJNCZKZ2NBA.ORARE2PNW6DYPEC2YBXICKGL5BNWY7JCA4A3OFU76GIMIBH5TD2A"
    #define TOPIC "#"
    #define CLIENT_ID1 "project-software-engineering@ttn"
    #define CLIENT_ID2 "specialgroup15-2024"
    #define QOS 2
    #define TIME_OUT 10000L



    MQTTClient_deliveryToken token;

    //Checking whether connection is_lost
    void connection_lost(void * context, char *cause) {
        std::cout << "Connection lost\n";
    }

    //this can be use for sub but currently is unnecessary
    void message_delivered(void *context, MQTTClient_deliveryToken tc) {
        std::cout << "Message delivered\n";
        token = tc;
    }

    //function that getting a message and filtering the data for 3 objects Devices, Gateways and Messages
    int message_received (void *context, char *topic, int topic_Len, MQTTClient_message *message, Devices& device_data, Gateways& gateway_data, Messages& messages_data) {
        std::string payload(static_cast<char*>(message->payload), message-> payloadlen);
        std::cout << "Message received, topic: " << topic << " content: " << (char*)message->payload << "\n";
        try {
            device_data = device_data.Getting_device_data(context,topic,topic_Len,message);
            gateway_data = gateway_data.Getting_gateway_data(context,topic,topic_Len,message);
            messages_data = messages_data.Getting_messages_data(context,topic,topic_Len,message);

        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON or applying filter: " << e.what() << "\n";
        }
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topic);
        return 1;
    }

    int message_callback(void *context, char *topic, int topicLen, MQTTClient_message *message) {
        // Casts the type context to a `CallbackContext` pointer
        CallbackContext *callbackContext = static_cast<CallbackContext*>(context);
        callbackContext->message_received = true; // if received new message, it will be true
        // Call the message_received function with a reference to the data in the context
        return message_received(context, topic, topicLen, message, callbackContext->device_data, callbackContext->gateway_data, callbackContext->messages_data);
    }


    int main(int argc, char ** argv) {
        pqxx::connection conn = connectToPostgres();

        MQTTClient Client1;
        MQTTClient Client2;
        MQTTClient_connectOptions options1 = MQTTClient_connectOptions_initializer;
        MQTTClient_connectOptions options2 = MQTTClient_connectOptions_initializer;
        MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;
        int rc;

        CallbackContext callbackContext;
        //Create MQTTClient
        MQTTClient_create(&Client1, ADDRESS1, CLIENT_ID1, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
        MQTTClient_create(&Client2, ADDRESS2, CLIENT_ID2, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);

        // Checking if connection is error
        if ((rc = MQTTClient_setCallbacks(Client1, &callbackContext, connection_lost, message_callback, message_delivered)) != MQTTCLIENT_SUCCESS) {
            std::cout << "Callback set error...\n";
            exit(EXIT_FAILURE);
        }

        if ((rc = MQTTClient_setCallbacks(Client2, &callbackContext, connection_lost, message_callback, message_delivered)) != MQTTCLIENT_SUCCESS) {
            std::cout << "Callback set error for Client2...\n";
            exit(EXIT_FAILURE);
        }


        options1.keepAliveInterval = 25; // Set the keep-alive interval to 25 seconds
        options1.cleansession = 1; //enable clean session
        options1.password = PASSWORD1; // set password for gateway
        options1.ssl = &sslOptions; // use SSL for secure communication and assign SSL options.
        // path to certificate
        // options.ssl->trustStore = "E:\\Project\\Project software\\specialRepos\\special-group-project\\certificate.txt";
        options1.username = CLIENT_ID1; // set username
        options1.serverURIcount = 0;
        options1.serverURIs = NULL;

        options2.keepAliveInterval = 25; // Set the keep-alive interval to 25 seconds
        options2.cleansession = 1; //enable clean session
        options2.password = PASSWORD2; // set password for gateway
        options2.ssl = &sslOptions; // use SSL for secure communication and assign SSL options.
        // path to certificate
        // options.ssl->trustStore = "E:\\Project\\Project software\\specialRepos\\special-group-project\\certificate.txt";
        options2.username = CLIENT_ID2; // set username
        options2.serverURIcount = 0;
        options2.serverURIs = NULL;

        //checking if connecting successfully
        if ((rc = MQTTClient_connect(Client1, &options1)) != MQTTCLIENT_SUCCESS) {
            std::cout << "Failed to connect with return code " << rc << "\n";
            exit(EXIT_FAILURE);
        }

        if ((rc = MQTTClient_subscribe(Client1, TOPIC, QOS)) != MQTTCLIENT_SUCCESS) {
            std::cout << "Failed to subscribe with return code " << rc << "\n";
            exit(EXIT_FAILURE);
        }

        if ((rc = MQTTClient_connect(Client2, &options2 )) != MQTTCLIENT_SUCCESS) {
            std::cout << "Failed to connect with return code " << rc << "\n";
            exit(EXIT_FAILURE);
        }

        if ((rc = MQTTClient_subscribe(Client2, TOPIC, QOS)) != MQTTCLIENT_SUCCESS) {
            std::cout << "Failed to subscribe with return code " << rc << "\n";
            exit(EXIT_FAILURE);
        }



        // print the result to check if getting data correctly
        while (true) {
            if(callbackContext.message_received) {
                sleep(2);
                insertGateways(conn, callbackContext.gateway_data);
                insertDevices(conn, callbackContext.device_data);
                insertMessages(conn, callbackContext.messages_data);
                callbackContext.messages_data.Print();
                callbackContext.message_received = false; // set message_received back to false for the next message
            }
        }
    }
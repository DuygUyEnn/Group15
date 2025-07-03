//
// Created by duynguyen on 15/03/2025.
//

#include "Postgre_connection.h"


pqxx::connection connectToPostgres() {
    std::string conn_str = "dbname=group15 user=duynguyen password=Newocean2005@ host=127.0.0.1 port=5432";
    try {
        pqxx::connection conn(conn_str);
        if (conn.is_open()) std::cout << "Connected" << std::endl;
        else std::cout << "Disconnected" << std::endl;
        return conn;
    } catch (const std::exception &e) {
        std::cout << "Connecting failed" << e.what() << std::endl;
        exit(1);
    }
}

void insertGateways(pqxx::connection &conn, const Gateways& gateway) {
    try {
        pqxx::work txn(conn);
        std::string query = "INSERT INTO gateways (Gateway_ID, Eui, Latitude, Longitude, Altitude) VALUES (" +
                            txn.quote(gateway.Gateway_id_()) + ", " +
                            txn.quote(gateway.Eui_()) + ", " +
                            std::to_string(gateway.Latitude_()) + ", " +
                            std::to_string(gateway.Longitude_()) + ", " +
                            std::to_string(gateway.Altitude_()) + ")";
        txn.exec(query);
        txn.commit();
        std::cout << "Inserted data into gateways table." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error inserting data: " << e.what() << std::endl;
    }
}

void insertDevices(pqxx::connection &conn, const Devices& device) {
    try {
        pqxx::work txn(conn);
        std::string query = "INSERT INTO devices (Device_ID, Gateway_ID, Model_ID, Band_ID, Battery_status) VALUES (" +
                            txn.quote(device.Device_id_()) + ", " +
                            txn.quote(device.Gateway_id_()) + ", " +
                            txn.quote(device.Model_()) + ", " +
                            txn.quote(device.Band_id_()) + ", " +
                            std::to_string(device.Battery_status_()) + ")";
        txn.exec(query);
        txn.commit();
        std::cout << "Inserted data into devices table." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error inserting data: " << e.what() << std::endl;
    }
}


void insertMessages(pqxx::connection &conn, const Messages &msg) {
    try {
        pqxx::work txn(conn);
        std::string query = "INSERT INTO messages (Time, Device_ID, Timestamp, Humidity, Light, Pressure, Temp) VALUES (" +
                            txn.quote(msg.Time_()) + ", " +
                            txn.quote(msg.Device_id_()) + ", " +
                            std::to_string(msg.Timestamp_()) + ", " +
                            std::to_string(msg.Humidity_()) + ", " +
                            std::to_string(msg.Light_()) + ", " +
                            std::to_string(msg.Pressure_()) + ", " +
                            std::to_string(msg.Temperature_()) + ")";
        txn.exec(query);
        txn.commit();
        std::cout << "Inserted data into messages table." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error inserting data: " << e.what() << std::endl;
    }
}



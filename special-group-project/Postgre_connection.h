//
// Created by duynguyen on 15/03/2025.
//

#ifndef POSTGRE_CONNECTION_H
#define POSTGRE_CONNECTION_H

#include <pqxx/pqxx>
#include "Messages.h"
#include "Devices.h"
#include "Gateways.h"

pqxx::connection connectToPostgres();
void insertGateways(pqxx::connection &conn, const Gateways& gateway);
void insertDevices(pqxx::connection &conn, const Devices& device);
void insertMessages(pqxx::connection &conn, const Messages &msg);



#endif //POSTGRE_CONNECTION_H

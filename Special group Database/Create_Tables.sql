-----Setting up the Database-----
USE master;
GO
DROP DATABASE Special_DB;
GO
CREATE DATABASE Special_DB;
GO
USE Special_DB;
GO

-----Creating Tables-----
CREATE TABLE Sensors (
Device_ID VARCHAR(20) NOT NULL PRIMARY KEY,
Gateway_ID VARCHAR(20) NOT NULL,
Latitude FLOAT NOT NULL,
Longitude FLOAT NOT NULL,
Altitude FLOAT NOT NULL,
Model VARCHAR(20) NOT NULL,
Band_ID VARCHAR(20) NOT NULL
)

CREATE TABLE Messages (
Time TIME NOT NULL,
Device_ID VARCHAR(20) NOT NULL,
Timestamp INT NOT NULL,
Frequency INT NOT NULL,
Humidity FLOAT NOT NULL,
Light FLOAT NOT NULL,
Pressure FLOAT NOT NULL,
Temperature FLOAT NOT NULL
PRIMARY KEY (Time, Device_ID)
);

CREATE TABLE Gateways (
Gateway_ID VARCHAR(20) NOT NULL PRIMARY KEY,
EUI VARCHAR(20) NOT NULL
);

-----Creating the FOREIGN KEYS-----
ALTER TABLE Sensors
ADD FOREIGN KEY (Gateway_ID) REFERENCES Gateways(Gateway_ID)

ALTER TABLE Messages
ADD FOREIGN KEY (Device_ID) REFERENCES Sensors(Device_ID)

SELECT * FROM Messages;
SELECT * FROM Sensors;
SELECT * FROM Gateways;
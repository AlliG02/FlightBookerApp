#pragma once
#include "Flight.h"
#include "Database.h"
#include <vector>

class FlightManager {
private:
    Database db;
    std::vector<Flight> flights;

    void createTable();
    void loadFlights();

public:
    FlightManager(const std::string& dbName);
    void listFlights() const;
    Flight* getFlightById(int id);
    bool reduceSeats(int flightId);
    bool addFlight(const Flight& flight);
    bool flightExists(int flightId) const;
    void seedFlights(); 
};

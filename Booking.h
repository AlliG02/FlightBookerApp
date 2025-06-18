#pragma once
#include <string>

class Booking {
public:
    int id;
    std::string passengerName;
    int flightId;

    Booking(int id, const std::string& name, int flightId);
    void display() const;
};

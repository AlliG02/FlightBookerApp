#pragma once
#include <string>

class Flight {
public:
    int id;
    std::string origin;
    std::string destination;
    std::string departureTime;
    int availableSeats;

    Flight(int id, const std::string& origin, const std::string& destination,
           const std::string& departureTime, int availableSeats);

    void display() const;
};

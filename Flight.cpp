#include "Flight.h"
#include <iostream>

Flight::Flight(int id, const std::string& origin, const std::string& destination,
               const std::string& departureTime, int availableSeats)
    : id(id), origin(origin), destination(destination),
      departureTime(departureTime), availableSeats(availableSeats) {}

void Flight::display() const {
    std::cout << "Flight " << id << ": " << origin << " -> " << destination
              << " at " << departureTime << " | Seats: " << availableSeats << "\n";
}

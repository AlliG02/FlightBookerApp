#include "Booking.h"
#include <iostream>

Booking::Booking(int id, const std::string& name, int flightId)
    : id(id), passengerName(name), flightId(flightId) {}

void Booking::display() const {
    std::cout << "Booking ID: " << id << ", Name: " << passengerName << ", Flight ID: " << flightId << "\n";
}

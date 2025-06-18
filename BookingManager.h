#pragma once
#include "Booking.h"
#include "Database.h"
#include "FlightManager.h"
#include <vector>

class BookingManager {
private:
    Database db;
    std::vector<Booking> bookings;

    void createTable();
    void loadBookings();

public:
    BookingManager(const std::string& dbName);
    bool addBooking(const Booking& booking);
    void listBookings() const;
    bool bookFlight(FlightManager& flightManager, const std::string& name, int flightId);
    bool cancelBooking(int bookingId);

};

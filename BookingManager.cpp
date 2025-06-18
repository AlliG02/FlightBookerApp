#include "BookingManager.h"
#include <iostream>
#include <sstream>

BookingManager::BookingManager(const std::string& dbName) : db(dbName) {
    if (!db.open()) {
        std::cerr << "Failed to open booking database.\n";
        return;
    }
    createTable();
    loadBookings();
}

void BookingManager::createTable() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS bookings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            passenger_name TEXT,
            flight_id INTEGER
        );
    )";
    db.execute(sql);
}

void BookingManager::loadBookings() {
    bookings.clear();
    std::string sql = "SELECT id, passenger_name, flight_id FROM bookings;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch bookings: " << sqlite3_errmsg(db.getDB()) << "\n";
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int flightId = sqlite3_column_int(stmt, 2);

        bookings.emplace_back(id, name, flightId);
    }

    sqlite3_finalize(stmt);
}

bool BookingManager::addBooking(const Booking& booking) {
    std::stringstream ss;
    ss << "INSERT INTO bookings (passenger_name, flight_id) VALUES ('"
       << booking.passengerName << "', "
       << booking.flightId << ");";

    if (!db.execute(ss.str())) {
        return false;
    }

    loadBookings(); // refresh local list
    return true;
}

void BookingManager::listBookings() const {
    if (bookings.empty()){
        std::cout << "There are no bookings yet!\n";
    }
    for (const auto& booking : bookings) {
        booking.display();
    }
}

bool BookingManager::bookFlight(FlightManager& flightManager, const std::string& name, int flightId) {
    if (!flightManager.flightExists(flightId)) {
        std::cerr << "Flight ID " << flightId << " does not exist.\n";
        return false;
    }

    std::stringstream ss;
    ss << "INSERT INTO bookings (passenger_name, flight_id) VALUES ('"
       << name << "', " << flightId << ");";

    if (!db.execute(ss.str())) {
        std::cerr << "Failed to insert booking.\n";
        return false;
    }

    flightManager.reduceSeats(flightId);

    std::cout << "Booking successful!\n";
    loadBookings();
    return true;
}

bool BookingManager::cancelBooking(int bookingId) {
    std::stringstream ss;
    ss << "DELETE FROM bookings WHERE id = " << bookingId << ";";

    if (!db.execute(ss.str())) {
        std::cerr << "Failed to cancel booking.\n";
        return false;
    }

    std::cout << "Booking canceled.\n";
    loadBookings();
    return true;
}
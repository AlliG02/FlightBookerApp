#include "FlightManager.h"
#include <iostream>
#include <sstream>

FlightManager::FlightManager(const std::string& dbName) : db(dbName) {
    if (!db.open()) {
        std::cerr << "Failed to open database.\n";
        return;
    }
    createTable();

    // Only seed flughts if the database is empty
    std::string check = "SELECT COUNT(*) FROM flights;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db.getDB(), check.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) == 0) {
            std::cout << "Seeding initial flights...\n";
            seedFlights();
        }
    }
    sqlite3_finalize(stmt);

    loadFlights();
}

void FlightManager::createTable() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS flights (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            origin TEXT,
            destination TEXT,
            departure_time TEXT,
            available_seats INTEGER
        );
    )";
    db.execute(sql);
}

void FlightManager::seedFlights() {
    std::vector<std::string> inserts = {
        "INSERT INTO flights (origin, destination, departure_time, available_seats) VALUES ('London', 'New York', '2025-07-01 08:00', 50);",
        "INSERT INTO flights (origin, destination, departure_time, available_seats) VALUES ('Paris', 'Tokyo', '2025-07-02 10:30', 60);",
        "INSERT INTO flights (origin, destination, departure_time, available_seats) VALUES ('Berlin', 'Toronto', '2025-07-03 16:00', 70);"
    };

    for (const auto& sql : inserts) {
        db.execute(sql);
    }
}

void FlightManager::loadFlights() {
    flights.clear();
    std::string sql = "SELECT id, origin, destination, departure_time, available_seats FROM flights;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch flights: " << sqlite3_errmsg(db.getDB()) << "\n";
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string origin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string departureTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int seats = sqlite3_column_int(stmt, 4);

        flights.emplace_back(id, origin, destination, departureTime, seats);
    }
    sqlite3_finalize(stmt);
}

void FlightManager::listFlights() const {
    for (const auto& f : flights) {
        f.display();
    }
}

Flight* FlightManager::getFlightById(int id) {
    for (auto& f : flights) {
        if (f.id == id) return &f;
    }
    return nullptr;
}

bool FlightManager::reduceSeats(int flightId) {
    Flight* flight = getFlightById(flightId);
    if (!flight || flight->availableSeats <= 0)
        return false;

    flight->availableSeats--;

    std::stringstream ss;
    ss << "UPDATE flights SET available_seats = " << flight->availableSeats
       << " WHERE id = " << flightId << ";";

    return db.execute(ss.str()); // <-- must return true
}


bool FlightManager::addFlight(const Flight& flight) {
    std::stringstream ss;
    ss << "INSERT INTO flights (origin, destination, departure_time, available_seats) VALUES ("
       << "'" << flight.origin << "', "
       << "'" << flight.destination << "', "
       << "'" << flight.departureTime << "', "
       << flight.availableSeats << ");";

    if (!db.execute(ss.str()))
        return false;

    loadFlights(); // Reload to refresh local cache
    return true;
}

bool FlightManager::flightExists(int flightId) const {
    std::string sql = "SELECT COUNT(*) FROM flights WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, flightId);
    rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0);
    sqlite3_finalize(stmt);
    return exists;
}
#include "FlightManager.h"
#include "BookingManager.h"
#include <iostream>

// need to compile each cpp file

int main() {
    FlightManager flightManager("flights.db");
    BookingManager bookingManager("flights.db");

    int choice;
    while (true) {
        std::cout << "\n--- Flight Booking CLI ---\n";
        std::cout << "1. List Flights\n2. Book Flight\n3. View Bookings\n4. Cancel Booking\n5. Exit\nChoice: ";
        std::cin >> choice;

        if (choice == 1) {
            flightManager.listFlights();
        } else if (choice == 2) {
            std::string name;
            int flightId;
            std::cout << "Enter your name: ";
            std::cin >> name;
            std::cout << "Enter flight ID: ";
            std::cin >> flightId;
            bookingManager.bookFlight(flightManager, name, flightId);
        } else if (choice == 3) {
            bookingManager.listBookings();
        } else if (choice == 4) {
            int bookingId;
            std::cout << "Enter booking ID to cancel: ";
            std::cin >> bookingId;
            bookingManager.cancelBooking(bookingId);
        } else {
            break;
        }
    }

    return 0;
}

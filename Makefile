# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source files
SRCS = main.cpp Flight.cpp FlightManager.cpp Booking.cpp BookingManager.cpp Database.cpp
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = FlightBookingApp

# Libraries
LIBS = -lsqlite3

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

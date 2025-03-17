#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Exception Handling
class BookingException : public exception {
public:
    virtual const char* what() const throw() {
        return "Booking exception occurred";
    }
};

class FlightNotFoundException : public BookingException {
public:
    virtual const char* what() const throw() {
        return "Flight not found";
    }
};

// Observer Design Pattern
class IObserver {
public:
    virtual void update(const string& message) = 0;
};

class ISubject {
public:
    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notify(const string& message) = 0;
};

// Base class for Person
class Person {
protected:
    string name;
    string email;
    string phone_number;

public:
    // Constructor to initialize a person's details
    Person(const string& name, const string& email, const string& phone_number)
        : name(name), email(email), phone_number(phone_number) {}

    // Method to display person's details
    virtual void get_details() const {
        cout << "Name: " << name << ", Email: " << email << ", Phone Number: " << phone_number << endl;
    }

    string get_name() const {
        return name;
    }

    void set_email(const string& new_email) {
        email = new_email;
    }

    void set_phone_number(const string& new_phone_number) {
        phone_number = new_phone_number;
    }
};

// Passenger class inheriting Person and implementing IObserver
class Passenger : public Person, public IObserver {
private:
    string passport_number;

public:
    // Constructor to initialize a passenger's details
    Passenger(const string& name, const string& email, const string& phone_number, const string& passport_number)
        : Person(name, email, phone_number), passport_number(passport_number) {}

    // Method to display passenger's details
    void get_passenger_info() const {
        get_details();
        cout << "Passport Number: " << passport_number << endl;
    }

    // Method to receive updates
    void update(const string& message) override {
        cout << "Passenger received update: " << message << endl;
    }

    string get_passport_number() const {
        return passport_number;
    }
};

// Abstract Flight class implementing ISubject
class Flight : public ISubject {
protected:
    string flight_number;
    string origin;
    string destination;
    string departure_time;
    string arrival_time;
    vector<IObserver*> observers;

public:
    // Constructor to initialize flight details
    Flight(const string& flight_number, const string& origin, const string& destination, const string& departure_time, const string& arrival_time)
        : flight_number(flight_number), origin(origin), destination(destination), departure_time(departure_time), arrival_time(arrival_time) {}

    // Method to display flight details
    virtual void get_flight_info() const {
        cout << "Flight Number: " << flight_number << ", Origin: " << origin << ", Destination: " << destination
            << ", Departure Time: " << departure_time << ", Arrival Time: " << arrival_time << endl;
    }

    // Pure virtual methods for availability and fare calculation
    virtual bool check_availability() const = 0;
    virtual double calculate_fare() const = 0;

    // Attach an observer
    void attach(IObserver* observer) override {
        observers.push_back(observer);
    }

    // Detach an observer
    void detach(IObserver* observer) override {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Notify all observers
    void notify(const string& message) override {
        for (auto* observer : observers) {
            observer->update(message);
        }
    }

    string get_origin() const { return origin; }
    string get_destination() const { return destination; }
    string get_flight_number() const { return flight_number; }
};

// DomesticFlight class inheriting Flight
class DomesticFlight : public Flight {
public:
    // Constructor to initialize domestic flight details
    DomesticFlight(const string& flight_number, const string& origin, const string& destination, const string& departure_time, const string& arrival_time)
        : Flight(flight_number, origin, destination, departure_time, arrival_time) {}

    // Method to check availability
    bool check_availability() const override {
        return true; // Simplified for demonstration
    }

    // Method to calculate fare
    double calculate_fare() const override {
        double base_fare = 50.0; // Base fare for domestic flights
        return base_fare;
    }
};

// InternationalFlight class inheriting Flight
class InternationalFlight : public Flight {
public:
    // Constructor to initialize international flight details
    InternationalFlight(const string& flight_number, const string& origin, const string& destination, const string& departure_time, const string& arrival_time)
        : Flight(flight_number, origin, destination, departure_time, arrival_time) {}

    // Method to check availability
    bool check_availability() const override {
        return true; // Simplified for demonstration
    }

    // Method to calculate fare
    double calculate_fare() const override {
        double base_fare = 200.0; // Base fare for international flights
        return base_fare;
    }
};

// FlightFactory class for creating flight instances
class FlightFactory {
public:
    // Static method to create flight instances based on flight type
    static Flight* create_flight(const string& flight_type, const string& flight_number, const string& origin, const string& destination, const string& departure_time, const string& arrival_time) {
        if (flight_type == "Domestic") {
            return new DomesticFlight(flight_number, origin, destination, departure_time, arrival_time);
        } else if (flight_type == "International") {
            return new InternationalFlight(flight_number, origin, destination, departure_time, arrival_time);
        }
        return nullptr;
    }
};

// Booking class to manage flight bookings
class Booking {
private:
    string booking_id;
    Passenger* passenger;
    Flight* flight;
    string seat_number;
    string booking_status;

public:
    // Constructor to initialize booking details
    Booking(const string& booking_id, Passenger* passenger, Flight* flight, const string& seat_number, const string& booking_status)
        : booking_id(booking_id), passenger(passenger), flight(flight), seat_number(seat_number), booking_status(booking_status) {}

    // Method to confirm booking
    void confirm_booking() {
        booking_status = "Confirmed";
        flight->notify("Booking confirmed for flight " + flight->get_origin() + " to " + flight->get_destination() + " for passenger " + passenger->get_name());
    }

    // Method to cancel booking
    void cancel_booking() {
        booking_status = "Cancelled";
        flight->notify("Booking cancelled for flight " + flight->get_origin() + " to " + flight->get_destination() + " for passenger " + passenger->get_name());
    }

    // Get flight associated with the booking
    Flight* get_flight() const {
        return flight;
    }

    // Get booking status
    string get_booking_status() const {
        return booking_status;
    }

    // Set booking status and notify observers
    void set_booking_status(const string& status) {
        booking_status = status;
        flight->notify("Booking status changed for flight " + flight->get_origin() + " to " + flight->get_destination() + " for passenger " + passenger->get_name() + " to " + status);
    }

    // Display booking details
    void get_booking_info() const {
        cout << "Booking ID: " << booking_id << ", Seat Number: " << seat_number << ", Status: " << booking_status << endl;
        flight->get_flight_info();
    }

    // Get booking ID
    string get_booking_id() const {
        return booking_id;
    }

    Passenger* get_passenger() const {
        return passenger;
    }
};

// Itinerary class to manage bookings for a passenger
class Itinerary {
private:
    string itinerary_id;
    vector<Booking*> bookings;

public:
    // Constructor to initialize itinerary details
    Itinerary(const string& itinerary_id) : itinerary_id(itinerary_id) {}

    // Method to display itinerary details
    void get_itinerary_details() const {
        cout << "Itinerary ID: " << itinerary_id << endl;
        for (const auto& booking : bookings) {
            booking->get_booking_info();
        }
    }

    // Add booking to itinerary
    void add_booking(Booking* booking) {
        bookings.push_back(booking);
    }

    // Remove booking from itinerary
    void remove_booking(Booking* booking) {
        bookings.erase(remove(bookings.begin(), bookings.end(), booking), bookings.end());
    }
};

// Schedule class to manage flights on a specific date
class Schedule {
private:
    string schedule_id;
    string date;
    vector<Flight*> flights;

public:
    // Constructor to initialize schedule details
    Schedule(const string& schedule_id, const string& date)
        : schedule_id(schedule_id), date(date) {}

    // Add flight to schedule
    void add_flight(Flight* flight) {
        flights.push_back(flight);
    }

    // Remove flight from schedule
    void remove_flight(Flight* flight) {
        flights.erase(remove(flights.begin(), flights.end(), flight), flights.end());
    }

    // Display schedule details
    void get_schedule_details() const {
        cout << "Schedule ID: " << schedule_id << ", Date: " << date << endl;
        for (const auto& flight : flights) {
            flight->get_flight_info();
        }
    }

    vector<Flight*> get_flights() const {
        return flights;
    }
};

// Function to find a flight by flight number in a schedule
Flight* find_flight_by_number(Schedule& schedule, const string& flight_number) {
    for (const auto& flight : schedule.get_flights()) {
        if (flight->get_flight_number() == flight_number) {
            return flight;
        }
    }
    throw FlightNotFoundException();
}

// Function to display all passengers on a given flight
void display_passengers(Flight* flight, const vector<Booking*>& bookings) {
    cout << "Passengers on flight " << flight->get_flight_number() << ":" << endl;
    for (const auto& booking : bookings) {
        if (booking->get_flight() == flight) {
            Passenger* passenger = booking->get_passenger();
            if (passenger) {
                passenger->get_passenger_info();
            }
        }
    }
}

// Function to display all passengers on domestic and international flights separately
void display_passengers_by_flight_type(const vector<Flight*>& flights, const vector<Booking*>& bookings) {
    cout << "Passengers on Domestic Flights:" << endl;
    for (const auto& flight : flights) {
        if (dynamic_cast<DomesticFlight*>(flight)) {
            display_passengers(flight, bookings);
        }
    }

    cout << "Passengers on International Flights:" << endl;
    for (const auto& flight : flights) {
        if (dynamic_cast<InternationalFlight*>(flight)) {
            display_passengers(flight, bookings);
        }
    }
}

// Function to modify passenger information
void modify_passenger_info(Passenger* passenger) {
    int choice;
    cout << "Modify Passenger Information for " << passenger->get_name() << endl;
    cout << "1. Email" << endl;
    cout << "2. Phone Number" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    string new_info;
    switch (choice) {
    case 1:
        cout << "Enter new email: ";
        cin >> new_info;
        passenger->set_email(new_info);
        break;
    case 2:
        cout << "Enter new phone number: ";
        cin >> new_info;
        passenger->set_phone_number(new_info);
        break;
    default:
        cout << "Invalid choice" << endl;
    }
}

// Function to add a new passenger
Passenger* add_passenger() {
    string name, email, phone_number, passport_number;
    cout << "Enter passenger details:" << endl;
    cout << "Name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Email: ";
    getline(cin, email);
    cout << "Phone Number: ";
    getline(cin, phone_number);
    cout << "Passport Number: ";
    getline(cin, passport_number);
    return new Passenger(name, email, phone_number, passport_number);
}

// Function to book a new flight for a passenger
Booking* book_flight(Passenger* passenger, Schedule& schedule) {
    string flight_number, seat_number, booking_id;
    cout << "Enter flight number: ";
    cin >> flight_number;
    try {
        Flight* flight = find_flight_by_number(schedule, flight_number);
        cout << "Enter seat number: ";
        cin >> seat_number;
        cout << "Enter booking ID: ";
        cin >> booking_id;
        Booking* new_booking = new Booking(booking_id, passenger, flight, seat_number, "Pending");
        flight->attach(passenger);
        return new_booking;
    } catch (const FlightNotFoundException& e) {
        cout << e.what() << endl;
        return nullptr;
    }
}

// Function to view a specific passenger's details
void view_passenger_details(const vector<Passenger*>& passengers) {
    string passport_number;
    cout << "Enter passport number: ";
    cin >> passport_number;
    for (const auto& passenger : passengers) {
        if (passenger->get_passport_number() == passport_number) {
            passenger->get_passenger_info();
            return;
        }
    }
    cout << "Passenger not found" << endl;
}

// Function to view flight details
void view_flight_details(const vector<Flight*>& flights) {
    string flight_number;
    cout << "Enter flight number: ";
    cin >> flight_number;
    for (const auto& flight : flights) {
        if (flight->get_flight_number() == flight_number) {
            flight->get_flight_info();
            return;
        }
    }
    cout << "Flight not found" << endl;
}

int main() {
    // Creating initial passengers
    Passenger* passenger1 = new Passenger("John Doe", "john@example.com", "1234567890", "P12345");
    Passenger* passenger2 = new Passenger("Jane Smith", "jane@example.com", "0987654321", "P54321");

    // Creating initial flights
    Flight* flight1 = FlightFactory::create_flight("Domestic", "FL123", "New York", "Los Angeles", "2023-06-15 10:00", "2023-06-15 14:00");
    Flight* flight2 = FlightFactory::create_flight("International", "FL456", "New York", "London", "2023-06-16 18:00", "2023-06-17 06:00");

    // Creating initial bookings
    Booking* booking1 = new Booking("B123", passenger1, flight1, "12A", "Confirmed");
    Booking* booking2 = new Booking("B456", passenger2, flight2, "14B", "Confirmed");

    // Attach passengers to flights as observers
    flight1->attach(passenger1);
    flight2->attach(passenger2);

    // Creating itinerary
    Itinerary* itinerary = new Itinerary("I123");
    itinerary->add_booking(booking1);
    itinerary->add_booking(booking2);

    // Creating schedule
    Schedule* schedule = new Schedule("S123", "2023-06-15");
    schedule->add_flight(flight1);
    schedule->add_flight(flight2);

    vector<Booking*> bookings = { booking1, booking2 };
    vector<Passenger*> passengers = { passenger1, passenger2 };

    int choice;
    do {
        cout << "Menu:" << endl;
        cout << "1. Display Itinerary" << endl;
        cout << "2. Display Schedule" << endl;
        cout << "3. Display Passengers by Flight Type" << endl;
        cout << "4. Modify Passenger Information" << endl;
        cout << "5. Notify Passengers" << endl;
        cout << "6. Add New Passenger" << endl;
        cout << "7. Book New Flight for a Passenger" << endl;
        cout << "8. Cancel Booking" << endl;
        cout << "9. View Specific Passenger's Details" << endl;
        cout << "10. View Flight Details" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            itinerary->get_itinerary_details();
            break;
        case 2:
            schedule->get_schedule_details();
            break;
        case 3:
            display_passengers_by_flight_type(schedule->get_flights(), bookings);
            break;
        case 4:
            {
                string passport_number;
                cout << "Enter passport number of the passenger to modify: ";
                cin >> passport_number;
                bool found = false;
                for (auto* passenger : passengers) {
                    if (passenger->get_passport_number() == passport_number) {
                        modify_passenger_info(passenger);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Passenger not found" << endl;
                }
            }
            break;
        case 5:
            flight1->notify("This is a notification for domestic flight.");
            flight2->notify("This is a notification for international flight.");
            break;
        case 6:
            {
                Passenger* new_passenger = add_passenger();
                if (new_passenger) {
                    passengers.push_back(new_passenger);
                    cout << "Passenger added successfully!" << endl;
                } else {
                    cout << "Failed to add passenger." << endl;
                }
            }
            break;
        case 7:
            {
                string passport_number;
                cout << "Enter passport number of the passenger to book flight for: ";
                cin >> passport_number;
                Passenger* passenger = nullptr;
                for (auto* p : passengers) {
                    if (p->get_passport_number() == passport_number) {
                        passenger = p;
                        break;
                    }
                }
                if (passenger) {
                    Booking* new_booking = book_flight(passenger, *schedule);
                    if (new_booking) {
                        bookings.push_back(new_booking);
                        itinerary->add_booking(new_booking);
                        new_booking->confirm_booking();
                        cout << "Flight booked successfully!" << endl;
                    } else {
                        cout << "Failed to book flight." << endl;
                    }
                } else {
                    cout << "Passenger not found." << endl;
                }
            }
            break;
        case 8:
            {
                string booking_id;
                cout << "Enter booking ID to cancel: ";
                cin >> booking_id;
                bool found = false;
                for (auto* booking : bookings) {
                    if (booking->get_booking_id() == booking_id) {
                        booking->cancel_booking();
                        itinerary->remove_booking(booking);
                        bookings.erase(remove(bookings.begin(), bookings.end(), booking), bookings.end());
                        delete booking;
                        found = true;
                        cout << "Booking cancelled successfully!" << endl;
                        break;
                    }
                }
                if (!found) {
                    cout << "Booking not found" << endl;
                }
            }
            break;
        case 9:
            view_passenger_details(passengers);
            break;
        case 10:
            view_flight_details(schedule->get_flights());
            break;
        case 11:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 11);

    // Clean up dynamically allocated memory
    for (auto* passenger : passengers) {
        delete passenger;
    }
    for (auto* booking : bookings) {
        delete booking;
    }
    for (auto* flight : schedule->get_flights()) {
        delete flight;
    }
    delete itinerary;
    delete schedule;

    return 0;
}

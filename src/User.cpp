#include "Person.cpp"
#include <string>

class User : public Person {
public:
    User(string username = "", string password = "")
        : Person(move(username), move(password)) {}
};
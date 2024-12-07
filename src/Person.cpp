#pragma once
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string username;
    string password;

public:
    Person(string username = "", string password = "") : username(username), password(password) {}

    virtual string getUsername() const { return username; }
    virtual string getPassword() const { return password; }

    virtual void setUsername(string username) { this->username = username; }
    virtual void setPassword(string password) { this->password = password; }
};

#include<iostream>
#include <string>

using namespace std;

class ExamSystemException : public runtime_error {
public:
    explicit ExamSystemException(const string& message) : runtime_error(message) {}
};

#include <iostream>
#include "Display.cpp"

int main() {
    try {
        Admin admin;
        Display display(admin);
        display.showMenu();
        return 0;
    } catch (const exception& e) {
        cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
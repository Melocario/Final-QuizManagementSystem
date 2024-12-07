#include <iostream>
#include "UserSystem.cpp"

class Display {
private:
    Admin& admin;
    UserSystem userSystem;

    static int getMenuChoice(const vector<string>& options) {
        string input;
        int choice;

        do {
            for (size_t i = 0; i < options.size(); i++) {
                cout << i << ". " << options[i] << "\n";
            }
            cout << "Enter your choice: ";
            
            getline(cin, input);
            try {
                choice = stoi(input);
                if (choice >= 0 && choice < static_cast<int>(options.size())) {
                    return choice;
                }
            } catch (...) {
                // Invalid input, will prompt again
            }
            cout << "Invalid choice. Please try again.\n";
        } while (true);
    }

    void showUserManagementMenu() {
        vector<string> options = {
            "Back",
            "Add User",
            "View Users",
            "Edit User",
            "Delete User",
            "Search User",
            "Sort Users"
        };

        while (true) {
            cout << "\nUser Management Menu:\n";
            int choice = getMenuChoice(options);
            
            if (choice == 0) return;
            
            try {
                switch (choice) {
                    case 1: admin.addUser(); break;
                    case 2: admin.viewUsers(); break;
                    case 3: admin.editUser(); break;
                    case 4: admin.deleteUser(); break;
                    case 5: admin.searchUser(); break;
                    case 6: admin.sortUsers(); break;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }

    void showQuizManagementMenu() {
        vector<string> options = {
            "Back",
            "Add Quiz",
            "View Quizzes",
            "Edit Quiz",
            "Delete Quiz",
            "Search Quiz",
            "Sort Quizzes"
        };

        while (true) {
            cout << "\nQuiz Management Menu:\n";
            int choice = getMenuChoice(options);
            
            if (choice == 0) return;
            
            try {
                switch (choice) {
                    case 1: admin.addQuiz(); break;
                    case 2: admin.viewQuizzes(); break;
                    case 3: admin.editQuiz(); break;
                    case 4: admin.deleteQuiz(); break;
                    case 5: admin.searchQuiz(); break;
                    case 6: admin.sortQuizzes(); break;
                    default: cout << "Feature not implemented yet.\n";
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }

public:
    explicit Display(Admin& admin) : admin(admin), userSystem(admin) {}

    void showMenu() {
        vector<string> options = {
            "Exit",
            "Admin Menu",
            "User Login"
        };

        while (true) {
            cout << "\nExam Management System\n";
            int choice = getMenuChoice(options);
            
            if (choice == 0) return;
            
            try {
                switch (choice) {
                    case 1: showAdminMenu(); break;
                    case 2: userSystem.login(); break;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }

    void showAdminMenu() {
        vector<string> options = {
            "Back",
            "User Management",
            "Quiz Management"
        };

        while (true) {
            cout << "\nAdmin Menu:\n";
            int choice = getMenuChoice(options);
            
            if (choice == 0) return;
            
            try {
                switch (choice) {
                    case 1: showUserManagementMenu(); break;
                    case 2: showQuizManagementMenu(); break;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }
};
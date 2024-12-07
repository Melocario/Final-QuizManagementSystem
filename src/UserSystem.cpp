#include <iostream>
#include <chrono>
#include <iomanip>
#include "Admin.cpp"

using namespace std;

class UserSystem {
private:
    Admin& admin;
    User* currentUser;
    
    bool authenticate(const string& username, const string& password) {
        const User* user = admin.getUserByUsername(username);
        if (user && user->getPassword() == password) {
            currentUser = const_cast<User*>(user);
            return true;
        }
        return false;
    }

    void takeQuiz() {
        const vector<Quiz>& quizzes = admin.getQuizzes();
        if (quizzes.empty()) {
            cout << "No quizzes available.\n";
            return;
        }

        // Display available quizzes
        cout << "\nAvailable Quizzes:\n";
        for (const auto& quiz : quizzes) {
            cout << quiz.id << ". " << quiz.title << "\n";
        }

        // Select quiz
        string quizIdStr;
        cout << "Enter quiz ID to take: ";
        getline(cin, quizIdStr);
        int quizId = stoi(quizIdStr);

        // Find selected quiz
        auto it = find_if(quizzes.begin(), quizzes.end(),
            [quizId](const Quiz& q) { return q.id == quizId; });
        
        if (it == quizzes.end()) {
            cout << "Quiz not found.\n";
            return;
        }

        const Quiz& quiz = *it;
        int score = 0;
        vector<int> userAnswers;

        // Record start time
        auto startTime = chrono::steady_clock::now();

        // Take quiz
        cout << "\nQuiz: " << quiz.title << "\n";
        cout << "----------------------------------------\n";
        
        for (size_t i = 0; i < quiz.questions.size(); i++) {
            const auto& question = quiz.questions[i];
            
            cout << "\nQuestion " << (i + 1) << ": " << question.questionText << "\n";
            for (size_t j = 0; j < question.choices.size(); j++) {
                cout << (j + 1) << ". " << question.choices[j] << "\n";
            }

            string answerStr;
            int answer;
            do {
                cout << "Your answer (1-" << question.choices.size() << "): ";
                getline(cin, answerStr);
                try {
                    answer = stoi(answerStr);
                    if (answer >= 1 && answer <= static_cast<int>(question.choices.size())) {
                        break;
                    }
                } catch (...) {
                    // Invalid input, will prompt again
                }
                cout << "Invalid choice. Please try again.\n";
            } while (true);

            userAnswers.push_back(answer - 1);
            if (answer - 1 == question.correctAnswerIndex) {
                score++;
            }
        }

        // Record end time and calculate duration
        auto endTime = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime);

        // Display results
        cout << "\nQuiz Complete!\n";
        cout << "----------------------------------------\n";
        cout << "Score: " << score << "/" << quiz.questions.size() << "\n";
        cout << "Percentage: " << fixed << setprecision(2) 
             << (score * 100.0 / quiz.questions.size()) << "%\n";
        cout << "Time taken: " << duration.count() << " seconds\n\n";

        // Show detailed review
        cout << "Review:\n";
        for (size_t i = 0; i < quiz.questions.size(); i++) {
            const auto& question = quiz.questions[i];
            cout << "Q" << (i + 1) << ": ";
            if (userAnswers[i] == question.correctAnswerIndex) {
                cout << "Correct\n";
            } else {
                cout << "Incorrect. Correct answer was: " 
                     << question.choices[question.correctAnswerIndex] << "\n";
            }
        }
    }

    void viewQuizzes() const {
        const vector<Quiz>& quizzes = admin.getQuizzes();
        if (quizzes.empty()) {
            cout << "No quizzes available.\n";
            return;
        }

        cout << "\nAvailable Quizzes:\n";
        for (const auto& quiz : quizzes) {
            cout << "ID: " << quiz.id << "\n";
            cout << "Title: " << quiz.title << "\n";
            cout << "Questions: " << quiz.questions.size() << "\n";
            cout << "----------------------------------------\n";
        }
    }

    void showUserMenu() {
        vector<string> options = {
            "Logout",
            "Take Quiz",
            "View Available Quizzes",
            "View Profile"
        };

        while (true) {
            cout << "\nWelcome, " << currentUser->getUsername() << "!\n";
            for (size_t i = 0; i < options.size(); i++) {
                cout << i << ". " << options[i] << "\n";
            }

            string input;
            cout << "Enter your choice: ";
            getline(cin, input);

            try {
                int choice = stoi(input);
                if (choice == 0) {
                    currentUser = nullptr;
                    return;
                }

                switch (choice) {
                    case 1: takeQuiz(); break;
                    case 2: viewQuizzes(); break;
                    case 3:
                        cout << "\nProfile Information:\n";
                        cout << "Username: " << currentUser->getUsername() << "\n";
                        break;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } catch (...) {
                cout << "Invalid input. Please try again.\n";
            }
        }
    }

public:
    explicit UserSystem(Admin& admin) : admin(admin), currentUser(nullptr) {}

    void login() {
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        if (authenticate(username, password)) {
            cout << "Login successful!\n";
            showUserMenu();
        } else {
            cout << "Invalid username or password.\n";
        }
    }
};
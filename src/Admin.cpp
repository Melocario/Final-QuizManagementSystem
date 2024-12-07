#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include "Constants.cpp"
#include "ExamSystemException.cpp"
#include "User.cpp"
#include "Quiz.cpp"
#include "AdminInterface.cpp"

using namespace std;

class Admin : public AdminInterface {
private:
    vector<User> users;
    vector<Quiz> quizzes;
    
    // File operations
    void saveUsersToTextFile() const {
        ofstream outFile(Constants::USERS_TEXT_FILE);
        if (!outFile) {
            throw ExamSystemException("Could not open users text file for writing");
        }
        
        for (const auto& user : users) {
            outFile << user.getUsername() << " " << user.getPassword() << "\n";
        }
    }

    void loadUsersFromTextFile() {
        ifstream inFile(Constants::USERS_TEXT_FILE);
        if (!inFile) {
            return; // Silent return if file doesn't exist yet
        }

        users.clear();
        string username, password;
        while (inFile >> username >> password) {
            users.emplace_back(username, password);
        }
    }

    // Binary file operations
    void saveUsersToBinaryFile() const {
        ofstream outFile(Constants::USERS_BINARY_FILE, ios::binary);
        if (!outFile) {
            throw ExamSystemException("Could not open users binary file for writing");
        }

        for (const auto& user : users) {
            // Write username length and username
            string username = user.getUsername();
            size_t usernameLen = username.length();
            outFile.write(reinterpret_cast<const char*>(&usernameLen), sizeof(usernameLen));
            outFile.write(username.c_str(), usernameLen);

            // Write password length and password
            string password = user.getPassword();
            size_t passwordLen = password.length();
            outFile.write(reinterpret_cast<const char*>(&passwordLen), sizeof(passwordLen));
            outFile.write(password.c_str(), passwordLen);
        }
    }

    void loadUsersFromBinaryFile() {
        ifstream inFile(Constants::USERS_BINARY_FILE, ios::binary);
        if (!inFile) {
            return; // Silent return if file doesn't exist yet
        }

        users.clear();
        while (inFile.peek() != EOF) {
            // Read username
            size_t usernameLen;
            inFile.read(reinterpret_cast<char*>(&usernameLen), sizeof(usernameLen));
            string username(usernameLen, '\0');
            inFile.read(&username[0], usernameLen);

            // Read password
            size_t passwordLen;
            inFile.read(reinterpret_cast<char*>(&passwordLen), sizeof(passwordLen));
            string password(passwordLen, '\0');
            inFile.read(&password[0], passwordLen);

            users.emplace_back(username, password);
        }
    }

    void saveQuizzesToTextFile() const {
        ofstream outFile(Constants::QUIZZES_TEXT_FILE);
        if (!outFile) {
            throw ExamSystemException("Could not open quizzes text file for writing");
        }

        for (const auto& quiz : quizzes) {
            outFile << quiz.id << "\n" << quiz.title << "\n";
            outFile << quiz.questions.size() << "\n";
            
            for (const auto& question : quiz.questions) {
                outFile << question.questionText << "\n";
                for (const auto& choice : question.choices) {
                    outFile << choice << "\n";
                }
                outFile << question.correctAnswerIndex << "\n";
            }
        }
    }

    void loadQuizzesFromTextFile() {
    ifstream inFile(Constants::QUIZZES_TEXT_FILE);
    if (!inFile) {
        return; // Silent return if file doesn't exist yet
    }

    quizzes.clear();
    string line;
    while (getline(inFile, line)) {
        Quiz quiz;
        quiz.id = stoi(line);
        
        getline(inFile, quiz.title);
        getline(inFile, line);
        int numQuestions = stoi(line);

        for (int i = 0; i < numQuestions; i++) {
            Quiz::Question question;
            getline(inFile, question.questionText);
            
            for (int j = 0; j < Constants::NUM_QUIZ_CHOICES; j++) {
                string choice;
                getline(inFile, choice);
                question.choices.push_back(choice);
            }

            getline(inFile, line);
            question.correctAnswerIndex = stoi(line);
            quiz.questions.push_back(question);
        }
        quizzes.push_back(quiz);
    }
}

    void saveQuizzesToBinaryFile() const {
        ofstream outFile(Constants::QUIZZES_BINARY_FILE, ios::binary);
        if (!outFile) {
            throw ExamSystemException("Could not open quizzes binary file for writing");
        }

        for (const auto& quiz : quizzes) {
            // Write quiz ID
            outFile.write(reinterpret_cast<const char*>(&quiz.id), sizeof(quiz.id));

            // Write quiz title
            size_t titleLen = quiz.title.length();
            outFile.write(reinterpret_cast<const char*>(&titleLen), sizeof(titleLen));
            outFile.write(quiz.title.c_str(), titleLen);

            // Write number of questions
            size_t numQuestions = quiz.questions.size();
            outFile.write(reinterpret_cast<const char*>(&numQuestions), sizeof(numQuestions));

            for (const auto& question : quiz.questions) {
                // Write question text
                size_t questionLen = question.questionText.length();
                outFile.write(reinterpret_cast<const char*>(&questionLen), sizeof(questionLen));
                outFile.write(question.questionText.c_str(), questionLen);

                // Write choices
                for (const auto& choice : question.choices) {
                    size_t choiceLen = choice.length();
                    outFile.write(reinterpret_cast<const char*>(&choiceLen), sizeof(choiceLen));
                    outFile.write(choice.c_str(), choiceLen);
                }

                // Write correct answer index
                outFile.write(reinterpret_cast<const char*>(&question.correctAnswerIndex), 
                            sizeof(question.correctAnswerIndex));
            }
        }
    }

    void loadQuizzesFromBinaryFile() {
        ifstream inFile(Constants::QUIZZES_BINARY_FILE, ios::binary);
        if (!inFile) {
            return; // Silent return if file doesn't exist yet
        }

        quizzes.clear();
        while (inFile.peek() != EOF) {
            Quiz quiz;

            // Read quiz ID
            inFile.read(reinterpret_cast<char*>(&quiz.id), sizeof(quiz.id));

            // Read quiz title
            size_t titleLen;
            inFile.read(reinterpret_cast<char*>(&titleLen), sizeof(titleLen));
            quiz.title.resize(titleLen);
            inFile.read(&quiz.title[0], titleLen);

            // Read number of questions
            size_t numQuestions;
            inFile.read(reinterpret_cast<char*>(&numQuestions), sizeof(numQuestions));

            for (size_t i = 0; i < numQuestions; i++) {
                Quiz::Question question;

                // Read question text
                size_t questionLen;
                inFile.read(reinterpret_cast<char*>(&questionLen), sizeof(questionLen));
                question.questionText.resize(questionLen);
                inFile.read(&question.questionText[0], questionLen);

                // Read choices
                for (int j = 0; j < Constants::NUM_QUIZ_CHOICES; j++) {
                    size_t choiceLen;
                    inFile.read(reinterpret_cast<char*>(&choiceLen), sizeof(choiceLen));
                    string choice(choiceLen, '\0');
                    inFile.read(&choice[0], choiceLen);
                    question.choices.push_back(choice);
                }

                // Read correct answer index
                inFile.read(reinterpret_cast<char*>(&question.correctAnswerIndex),
                          sizeof(question.correctAnswerIndex));

                quiz.questions.push_back(question);
            }
            quizzes.push_back(quiz);
        }
    }


    // Validation methods
    static bool isValidUsername(const string& username) {
        regex usernameRegex("^[a-zA-Z0-9]{" + 
            to_string(Constants::MIN_USERNAME_LENGTH) + "," + 
            to_string(Constants::MAX_USERNAME_LENGTH) + "}$");
        return regex_match(username, usernameRegex);
    }

    static bool isValidPassword(const string& password) {
        regex passwordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[A-Za-z\\d@$!%*?&]{" + 
            to_string(Constants::MIN_PASSWORD_LENGTH) + ",}$");
        return regex_match(password, passwordRegex);
    }

public:
     Admin() {
        try {
            // Try loading from binary files first
            loadUsersFromBinaryFile();
            loadQuizzesFromBinaryFile();

            // If binary files don't exist, try text files
            if (users.empty()) {
                loadUsersFromTextFile();
            }
            if (quizzes.empty()) {
                loadQuizzesFromTextFile();
            }
        } catch (const exception& e) {
            throw ExamSystemException("Failed to initialize Admin: " + string(e.what()));
        }
    }

    ~Admin() override {
        try {
            // Save to both formats
            saveUsersToTextFile();
            saveUsersToBinaryFile();
            saveQuizzesToTextFile();
            saveQuizzesToBinaryFile();
        } catch (const exception& e) {
            cerr << "Error saving data: " << e.what() << endl;
        }
    }

    // User management methods
    void addUser() override {
        string username, password;
        
        do {
            cout << "Enter username (" << Constants::MIN_USERNAME_LENGTH 
                 << "-" << Constants::MAX_USERNAME_LENGTH << " alphanumeric characters): ";
            getline(cin, username);
        } while (!isValidUsername(username) || isUserExist(username));

        do {
            cout << "Enter password (minimum " << Constants::MIN_PASSWORD_LENGTH 
                 << " characters, 1 uppercase, 1 number): ";
            getline(cin, password);
        } while (!isValidPassword(password));

        users.emplace_back(username, password);
        cout << "User added successfully.\n";
    }

    void viewUsers() const override {
        if (users.empty()) {
            cout << "No users found.\n";
            return;
        }

        for (const auto& user : users) {
            cout << "Username: " << user.getUsername() << "\n";
        }
    }

    void editUser() override {
        string username;
        cout << "Enter username to edit: ";
        getline(cin, username);

        auto user = getUserByUsername(username);
        if (!user) {
            cout << "User not found.\n";
            return;
        }

        string newPassword;
        do {
            cout << "Enter new password: ";
            getline(cin, newPassword);
        } while (!isValidPassword(newPassword));

        user->setPassword(newPassword);
        cout << "Password updated successfully.\n";
    }

    void deleteUser() override {
        string username;
        cout << "Enter username to delete: ";
        getline(cin, username);

        auto it = remove_if(users.begin(), users.end(),
            [&username](const User& user) { return user.getUsername() == username; });

        if (it != users.end()) {
            users.erase(it, users.end());
            cout << "User deleted successfully.\n";
        } else {
            cout << "User not found.\n";
        }
    }

    void searchUser() const override {
        string username;
        cout << "Enter username to search: ";
        getline(cin, username);

        auto it = find_if(users.begin(), users.end(),
            [&username](const User& user) { return user.getUsername() == username; });

        if (it != users.end()) {
            cout << "User found: " << it->getUsername() << "\n";
        } else {
            cout << "User not found.\n";
        }
    }

    void sortUsers() override {
        sort(users.begin(), users.end(),
            [](const User& a, const User& b) { return a.getUsername() < b.getUsername(); });
        cout << "Users sorted alphabetically.\n";
    }

    // Quiz management methods
    void addQuiz() {
        Quiz quiz;
        cout << "Enter quiz ID: ";
        string idStr;
        getline(cin, idStr);
        quiz.id = stoi(idStr);

        cout << "Enter quiz title: ";
        getline(cin, quiz.title);

        cout << "Enter number of questions: ";
        string numQuestionsStr;
        getline(cin, numQuestionsStr);
        int numQuestions = stoi(numQuestionsStr);

        for (int i = 0; i < numQuestions; i++) {
            Quiz::Question question;
            cout << "Question " << (i + 1) << ": ";
            getline(cin, question.questionText);

            for (int j = 0; j < Constants::NUM_QUIZ_CHOICES; j++) {
                cout << "Choice " << (j + 1) << ": ";
                string choice;
                getline(cin, choice);
                question.choices.push_back(choice);
            }

            cout << "Enter correct choice (1-4): ";
            string correctStr;
            getline(cin, correctStr);
            question.correctAnswerIndex = stoi(correctStr) - 1;

            quiz.questions.push_back(question);
        }

        quizzes.push_back(quiz);
        cout << "Quiz added successfully.\n";
    }

        // View all quizzes
    void viewQuizzes() const {
        if (quizzes.empty()) {
            cout << "No quizzes available.\n";
            return;
        }

        for (const auto& quiz : quizzes) {
            cout << "Quiz ID: " << quiz.id << "\n"
                 << "Title: " << quiz.title << "\n"
                 << "Number of Questions: " << quiz.questions.size() << "\n\n";
        }
    }

    // Search for a quiz by ID
    void searchQuiz() const {
        cout << "Enter quiz ID to search: ";
        string idStr;
        getline(cin, idStr);
        int id = stoi(idStr);

        auto it = find_if(quizzes.begin(), quizzes.end(),
            [id](const Quiz& quiz) { return quiz.id == id; });

        if (it != quizzes.end()) {
            cout << "Quiz Found:\n";
            cout << "ID: " << it->id << "\n"
                 << "Title: " << it->title << "\n"
                 << "Number of Questions: " << it->questions.size() << "\n";
        } else {
            cout << "Quiz not found.\n";
        }
    }

    // Edit a quiz by ID
    void editQuiz() {
        cout << "Enter quiz ID to edit: ";
        string idStr;
        getline(cin, idStr);
        int id = stoi(idStr);

        auto it = find_if(quizzes.begin(), quizzes.end(),
            [id](const Quiz& quiz) { return quiz.id == id; });

        if (it != quizzes.end()) {
            cout << "Editing Quiz ID: " << it->id << "\n";
            cout << "Enter new title: ";
            getline(cin, it->title);

            cout << "Enter new number of questions: ";
            string numQuestionsStr;
            getline(cin, numQuestionsStr);
            int numQuestions = stoi(numQuestionsStr);

            it->questions.clear();
            for (int i = 0; i < numQuestions; i++) {
                Quiz::Question question;
                cout << "Question " << (i + 1) << ": ";
                getline(cin, question.questionText);

                for (int j = 0; j < Constants::NUM_QUIZ_CHOICES; j++) {
                    cout << "Choice " << (j + 1) << ": ";
                    string choice;
                    getline(cin, choice);
                    question.choices.push_back(choice);
                }

                cout << "Enter correct choice (1-4): ";
                string correctStr;
                getline(cin, correctStr);
                question.correctAnswerIndex = stoi(correctStr) - 1;

                it->questions.push_back(question);
            }

            cout << "Quiz updated successfully.\n";
        } else {
            cout << "Quiz not found.\n";
        }
    }

    // Delete a quiz by ID
    void deleteQuiz() {
        cout << "Enter quiz ID to delete: ";
        string idStr;
        getline(cin, idStr);
        int id = stoi(idStr);

        auto it = remove_if(quizzes.begin(), quizzes.end(),
            [id](const Quiz& quiz) { return quiz.id == id; });

        if (it != quizzes.end()) {
            quizzes.erase(it, quizzes.end());
            cout << "Quiz deleted successfully.\n";
        } else {
            cout << "Quiz not found.\n";
        }
    }

    // Sort quizzes by title
    void sortQuizzes() {
        sort(quizzes.begin(), quizzes.end(),
            [](const Quiz& a, const Quiz& b) { return a.title < b.title; });
        cout << "Quizzes sorted by title.\n";
    }

    // Helper methods
    bool isUserExist(const string& username) const {
        return any_of(users.begin(), users.end(),
            [&username](const User& user) { return user.getUsername() == username; });
    }

    User* getUserByUsername(const string& username) {
        auto it = find_if(users.begin(), users.end(),
            [&username](const User& user) { return user.getUsername() == username; });
        return it != users.end() ? &(*it) : nullptr;
    }

    const vector<Quiz>& getQuizzes() const { return quizzes; }

    const User* getUserByUsername(const string& username) const {
    auto it = find_if(users.begin(), users.end(),
        [&username](const User& user) { return user.getUsername() == username; });
    return it != users.end() ? &(*it) : nullptr;
}
};

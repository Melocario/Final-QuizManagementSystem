#include <vector>
#include <string>

using namespace std;

struct Quiz {
    struct Question {
        string questionText;
        vector<string> choices;
        int correctAnswerIndex;

        Question() : correctAnswerIndex(-1) {}
    };

    int id;
    string title;
    vector<Question> questions;

    Quiz(int id = 0, string title = "") : id(id), title(move(title)) {}
};

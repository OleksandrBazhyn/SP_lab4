#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <windows.h>

using namespace std;

// Структура для представлення скінченого автомата
struct Automaton {
    set<int> states;                      // Множина станів
    set<char> alphabet;                   // Вхідний алфавіт
    int initialState;                     // Початковий стан
    set<int> finalStates;                 // Фінальні стани
    map<pair<int, char>, int> transitions; // Функція переходів
};

// Функція для читання автомата з файлу
bool readAutomaton(const string& filepath, Automaton& automaton) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Не вдалося відкрити файл: " << filepath << endl;
        return false;
    }

    int alphabetSize, numStates, numFinalStates;
    file >> alphabetSize; // Розмір алфавіту
    file >> numStates;    // Кількість станів
    automaton.states = set<int>();
    for (int i = 0; i < numStates; i++) {
        automaton.states.insert(i);
    }

    file >> automaton.initialState; // Початковий стан

    file >> numFinalStates; // Кількість фінальних станів
    for (int i = 0; i < numFinalStates; i++) {
        int finalState;
        file >> finalState;
        automaton.finalStates.insert(finalState);
    }

    // Зчитування переходів
    int fromState, toState;
    char symbol;
    while (file >> fromState >> symbol >> toState) {
        automaton.transitions[{fromState, symbol}] = toState;
        automaton.alphabet.insert(symbol);
    }

    file.close();
    return true;
}

// Функція для визначення неприйнятих символів
set<char> findUnacceptedSymbols(const Automaton& automaton) {
    set<char> unacceptedSymbols = automaton.alphabet;

    // Проходимо всі переходи і видаляємо символи, для яких є переходи
    for (const auto& [transition, toState] : automaton.transitions) {
        unacceptedSymbols.erase(transition.second); // Видалення символу з множини
    }

    return unacceptedSymbols; // Залишаються тільки ті символи, яких немає в переходах
}


int main() {
    SetConsoleOutputCP(CP_UTF8);  // Установити UTF-8 для виводу
    SetConsoleCP(CP_UTF8);        // Установити UTF-8 для вводу
    string filepath;
    cout << "Введіть шлях до файлу з автоматом (наприклад, ./data/automaton.txt): ";
    cin >> filepath;

    Automaton automaton;
    if (!readAutomaton(filepath, automaton)) {
        return 1;
    }

    while (true) {
        string word;
        cout << "Введіть слово (або 'exit' для виходу): ";
        cin >> word;

        if (word == "exit") {
            break;
        }

        // Перевіряємо кожен символ слова
        bool allAccepted = true;
        for (char c : word) {
            if (automaton.alphabet.find(c) == automaton.alphabet.end()) {
                cout << "Символ '" << c << "' не входить до алфавіту автомата." << endl;
                allAccepted = false;
            }
        }

        if (allAccepted) {
            cout << "Усі символи слова приймаються автоматом." << endl;
        }
        else {
            cout << "Слово містить неприйняті символи." << endl;
        }
    }

    return 0;
}

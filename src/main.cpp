#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const string FILE_NAME = "../data/users.csv";


// check if email/username + password are correct
bool validCredentials(const string& emailOrUsername, const string& password) {
    ifstream file(FILE_NAME);
    string line, email, username, pass;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, email, ',');
        getline(ss, username, ',');
        getline(ss, pass, ',');

        if ((emailOrUsername == email || emailOrUsername == username) && password == pass) {
            return true;
        }
    }
    return false;
}


// check if email exists
bool emailExists(const string& email) {
    ifstream file(FILE_NAME);
    string line, existingEmail, username, pass;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, existingEmail, ',');
        getline(ss, username, ',');
        getline(ss, pass, ',');

        if (existingEmail == email) {
            return true;
        }
    }
    return false;
}

// check if username exists
bool usernameExists(const string& username) {
    ifstream file(FILE_NAME);
    string line, email, existingUsername, pass;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, email, ',');
        getline(ss, existingUsername, ',');
        getline(ss, pass, ',');

        if (existingUsername == username) {
            return true;
        }
    }
    return false;
}


// register new user
void registerUser() {
    string email, username, password, confirmPassword;
    
    cout << "Enter email: ";
    getline(cin, email);

    while(emailExists(email)){
        cout<<"\nThis email is already registered. Try another.\n";
        getline(cin, email);
    }

    cout << "\nEnter username: ";
    getline(cin, username);
    while(usernameExists(username)){
        cout<<"\nThis username is already taken. Try another.\n";
        getline(cin, username);
    }
    


    cout << "\nEnter password: ";
    getline(cin, password);

    cout << "\nConfirm password: ";
    getline(cin, confirmPassword);

    while (password != confirmPassword) {
        cout << "\nPasswords do not match. Try again.\n";
        cout << "Confirm password: ";
        getline(cin, confirmPassword);
    }

    ofstream file(FILE_NAME, ios::app);
    if (!file.is_open()) {
    cout << " Could not open " << FILE_NAME << endl;
    return;
}
    file << email << "," << username << "," << password << "\n";
    file.close();

    cout << "\nRegistration successful!\n";
}

// login user
void loginUser() {
    string emailOrUsername, password;

    cout << "\nEnter email or username: ";
    getline(cin, emailOrUsername);

    while (!(emailExists(emailOrUsername) || usernameExists(emailOrUsername))) {
        cout << "\nNo account found with that email/username.\n";
        cout << "\nEnter email or username: ";
        getline(cin, emailOrUsername);
    }
    cout << "\nEnter password: ";
    getline(cin, password);

    while(!validCredentials(emailOrUsername, password)) {
        cout << "\nWrong password.\n";
         cout << "Try again! ";
         getline(cin, password);
    }

    if (validCredentials(emailOrUsername, password)) {
        cout << "\nLogin successful!\n";
        cout << "====================================\n";
        cout << "         Welcome " << emailOrUsername << "!\n";
        cout << "====================================\n";
    }
}

int main() {
    int choice;

        cout << "\n====================================\n";
        cout << "          Spotify Recommender      \n";
        cout << "====================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loginUser();
        } else if (choice == 3) {
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    
    return 0;
}

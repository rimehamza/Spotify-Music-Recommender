#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // For std::transform

using namespace std;

// --- User Management Configuration ---
const string USER_FILE_NAME = "../data/users.csv";

// --- Track Recommendation Configuration ---
const string TRACKS_FILE_NAME = "tracks.csv";

// Struct to hold track data
struct Track {
    string genre;
    string mood;
    string artist;
    string trackName;
};

// Helper function to convert a string to lowercase
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// --- User Management Functions (from original file) ---

// check if email/username + password are correct
bool validCredentials(const string& emailOrUsername, const string& password) {
    ifstream file("data/users.csv");
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

// NOTE: Uncommented the emailExists function as it's used in registerUser
// check if email exists
bool emailExists(const string& email) {
    ifstream file("data/users.csv");
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
    ifstream file("data/users.csv");
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

    ofstream file("data/users.csv", ios::app);
    if (!file.is_open()) {
    cout << " Could not open " << "data/users.csv"<< endl;
    return;
}
    file << email << "," << username << "," << password << "\n";
    file.close();

    cout << "\nRegistration successful!\n";
}

// --- Music Recommendation Helper Functions ---

// Loads all tracks from the CSV file into a vector of Track structs
vector<Track> loadTracks() {
    vector<Track> tracks;
    ifstream file("data/tracks.csv");
    string line;
    
    if (!file.is_open()) {
        cerr << "Error: Could not open " << "data/tracks.csv" << endl;
        return tracks;
    }
    
    // Skip the header line (genre,mood,artist,track)
    getline(file, line); 

    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        Track t;

        // NOTE: The original tracks.csv has some text formatting issues.
        // We are assuming a simple comma-separated format for basic parsing.

        // Get Genre
        if (!getline(ss, segment, ',')) continue;
        t.genre = segment;

        // Get Mood
        if (!getline(ss, segment, ',')) continue;
        t.mood = segment;

        // Get Artist
        if (!getline(ss, segment, ',')) continue;
        t.artist = segment;

        // Get Track Name (rest of the line)
        if (!getline(ss, segment)) continue;
        t.trackName = segment;
        
        tracks.push_back(t);
    }
    return tracks;
}

// Gathers all unique genres or moods for validation
vector<string> getUniqueAttributes(const vector<Track>& tracks, bool getGenre) {
    vector<string> uniqueAttributes;
    for (const auto& track : tracks) {
        string attribute = getGenre ? track.genre : track.mood;
        if (find(uniqueAttributes.begin(), uniqueAttributes.end(), attribute) == uniqueAttributes.end()) {
            uniqueAttributes.push_back(attribute);
        }
    }
    return uniqueAttributes;
}

// Prompts the user for a preference until a valid one is entered
string getValidPreference(const string& promptType, const vector<string>& validOptions) {
    string input;
    string lowerInput;
    
    cout << "\nAvailable " << promptType << "s: ";
    for (size_t i = 0; i < validOptions.size(); ++i) {
        cout << validOptions[i] << (i == validOptions.size() - 1 ? "" : ", ");
    }
    cout << "\n";

    while (true) {
        cout << "Enter your preferred " << promptType << ": ";
        getline(cin, input);
        lowerInput = toLower(input);

        // Check if the input matches any valid option (case-insensitive)
        bool found = false;
        for (const string& option : validOptions) {
            if (toLower(option) == lowerInput) {
                // Return the correctly cased option from the list
                return option; 
            }
        }

        cout << "Invalid " << promptType << ". Please choose from the available list.\n";
    }
}

// Main recommendation logic
void recommendMusic(const string& username) {
    vector<Track> allTracks = loadTracks();

    if (allTracks.empty()) {
        cout << "\nError: No tracks available for recommendation.\n";
        return;
    }
    
    // Get unique genres and moods for user selection and validation
    vector<string> uniqueGenres = getUniqueAttributes(allTracks, true);
    vector<string> uniqueMoods = getUniqueAttributes(allTracks, false);

    
    // Get user preferences
    string preferredGenre = getValidPreference("Genre", uniqueGenres);
    string preferredMood = getValidPreference("Mood", uniqueMoods);

    cout << "\nGenerating playlist for " << preferredGenre << " and " << preferredMood << "...\n";
    cout << "------------------------------------\n";
    
    // Find matching tracks
    vector<Track> playlist;
    for (const auto& track : allTracks) {
        if (track.genre == preferredGenre && track.mood == preferredMood) {
            playlist.push_back(track);
        }
    }
    
    // Display playlist
    if (playlist.empty()) {
        cout << "No tracks found matching " << preferredGenre << " and " << preferredMood << ".\n";
    } else {
        cout << "  Your Custom Playlist:\n";
        cout << "------------------------------------\n";
        for (size_t i = 0; i < playlist.size(); ++i) {
            // Display as: 1. [Track Name] by [Artist]
            cout << i + 1 << ". " << playlist[i].trackName << " by " << playlist[i].artist << "\n";
        }
    }
    cout << "====================================\n";
}

// login user
void loginUser() {
    string username, password;

    cout << "\nEnter username: ";
    getline(cin, username);

    // NOTE: Changed this to only check username since the prompt implies username login
    while (!(usernameExists(username))) {
        cout << "\nNo account found with that username.\n";
        cout << "\nEnter username: ";
        getline(cin, username);
    }
    
    cout << "\nEnter password: ";
    getline(cin, password);

    // Using a do-while loop for password retry logic
    do {
        if (!validCredentials(username, password)) {
            cout << "\nWrong password. Try again: ";
            getline(cin, password);
        }
    } while (!validCredentials(username, password));


    // If we exit the loop, credentials are valid
    cout << "\nLogin successful!\n";
    cout << "====================================\n";
    cout << "         Welcome " << username << "!\n";
    cout << "====================================\n";
    
    // *** NEW: Call the music recommendation function after successful login ***
    recommendMusic(username);
}

int main() {
    int choice;
    bool running = true;

    while (running) {
        cout << "\n====================================\n";
        cout << "          Spotify Recommender      \n";
        cout << "====================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "------------------------------------\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loginUser();
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            running = false;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    
    return 0;
}
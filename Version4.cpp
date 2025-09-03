#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

// ===== Helper =====
string getTableType(const string &tableName) {
    ifstream meta("tables.txt");
    string line;
    while (getline(meta, line)) {
        stringstream ss(line);
        string name, type, fields;
        getline(ss, name, '|');
        getline(ss, type, '|');
        if (name == tableName) return type;
    }
    return "";
}

// ===== Create Table =====
void createTable() {
    string name, type;
    cout << "Enter table name: ";
    cin >> name;
    cout << "Enter type (TEXT / SONG): ";
    cin >> type;
    cin.ignore();

    ofstream meta("tables.txt", ios::app);
    if (type == "TEXT") {
        string fields;
        cout << "Enter fields (comma-separated): ";
        getline(cin, fields);
        meta << name << "|" << type << "|" << fields << endl;
    } else if (type == "SONG") {
        meta << name << "|SONG|id,title,artist,filepath,lyrics" << endl;
    }
    meta.close();

    ofstream fout(name + ".txt"); // empty file for table
    fout.close();
    cout << "Table created!\n";
}

// ===== Insert =====
void insertRecord(const string &table) {
    string type = getTableType(table);
    if (type == "") {
        cout << "Table not found!\n"; return;
    }

    ofstream fout(table + ".txt", ios::app);
    if (type == "TEXT") {
        cout << "Enter record (comma-separated): ";
        string record;
        cin.ignore();
        getline(cin, record);
        fout << record << endl;
    } else if (type == "SONG") {
        int id; string title, artist, path, lyrics;
        cout << "Enter Song ID: "; cin >> id;
        cin.ignore();
        cout << "Enter Title: "; getline(cin, title);
        cout << "Enter Artist: "; getline(cin, artist);
        cout << "Enter File Path: "; getline(cin, path);
        cout << "Enter Lyrics (one line): "; getline(cin, lyrics);
        fout << id << "," << title << "," << artist << "," 
             << path << "," << lyrics << endl;
    }
    fout.close();
    cout << "Record inserted!\n";
}

// ===== View Records =====
void viewRecords(const string &table) {
    ifstream fin(table + ".txt");
    if (!fin) { cout << "No records!\n"; return; }
    string line;
    cout << "\n--- Records in " << table << " ---\n";
    while (getline(fin, line)) cout << line << endl;
    fin.close();
}

// ===== Play Song =====
void playSong(const string &table) {
    if (getTableType(table) != "SONG") {
        cout << "Not a SONG table!\n"; return;
    }
    int searchId;
    cout << "Enter Song ID: ";
    cin >> searchId;

    ifstream fin(table + ".txt");
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string idStr, title, artist, filepath, lyrics;
        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, artist, ',');
        getline(ss, filepath, ',');
        getline(ss, lyrics, ',');

        if (stoi(idStr) == searchId) {
            cout << "🎵 Playing: " << title << " by " << artist << endl;
            cout << "Lyrics: " << lyrics << endl;
            string command = "start \"\" \"" + filepath + "\"";
            system(command.c_str());
            break;
        }
    }
    fin.close();
}

// ===== Main Menu =====
int main() {
    int choice;
    string table;
    do {
        cout << "\n===== Mini DBMS =====\n";
        cout << "1. Create Table\n";
        cout << "2. Insert Record\n";
        cout << "3. View Records\n";
        cout << "4. Play Song (if SONG table)\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createTable(); break;
            case 2: cout << "Enter table: "; cin >> table; insertRecord(table); break;
            case 3: cout << "Enter table: "; cin >> table; viewRecords(table); break;
            case 4: cout << "Enter SONG table: "; cin >> table; playSong(table); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid!\n";
        }
    } while (choice != 5);
    return 0;
}

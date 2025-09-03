#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void addRecord() {
    ofstream fout("data.txt", ios::app);  // open in append mode
    int id;
    string name;

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Name: ";
    cin >> name;

    fout << id << "," << name << endl;  // save as "id,name"
    fout.close();
    cout << "Record added successfully!\n";
}

void showAll() {
    ifstream fin("data.txt");
    if (!fin) {
        cout << "No records found!\n";
        return;
    }

    string line;
    cout << "\n--- All Records ---\n";
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, name;
        getline(ss, id, ',');   // split by comma
        getline(ss, name, ',');
        cout << "ID: " << id << " | Name: " << name << endl;
    }
    fin.close();
    cout << "-------------------\n";
}

void searchRecord() {
    ifstream fin("data.txt");
    if (!fin) {
        cout << "No records found!\n";
        return;
    }

    int searchId;
    cout << "Enter ID to search: ";
    cin >> searchId;

    string line;
    bool found = false;
    while (getline(fin, line)) {
        stringstream ss(line);
        int id;
        string name;
        char comma;
        ss >> id >> comma >> name;  // read "id,name"

        if (id == searchId) {
            cout << "Record Found -> ID: " << id << ", Name: " << name << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "Record not found!\n";
    fin.close();
}

int main() {
    int choice;
    while (true) {
        cout << "\n===== Mini Database Menu =====\n";
        cout << "1. Add Record\n";
        cout << "2. Show All Records\n";
        cout << "3. Search Record by ID\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRecord(); break;
            case 2: showAll(); break;
            case 3: searchRecord(); break;
            case 4: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice! Try again.\n";
        }
    }
}

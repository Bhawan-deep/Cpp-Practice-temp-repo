#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// ========== CREATE TABLE ==========
void createTable() {
    string tableName;
    int fieldCount;
    cout << "Enter table name: ";
    cin >> tableName;
    cout << "How many fields? ";
    cin >> fieldCount;

    vector<string> fields(fieldCount);
    for (int i = 0; i < fieldCount; i++) {
        cout << "Enter field " << (i + 1) << ": ";
        cin >> fields[i];
    }

    // Save schema
    ofstream schema(tableName + "_schema.txt");
    for (int i = 0; i < fieldCount; i++) {
        schema << fields[i];
        if (i != fieldCount - 1) schema << ",";
    }
    schema.close();

    // Create table file with header
    ofstream table(tableName + ".txt");
    for (int i = 0; i < fieldCount; i++) {
        table << fields[i];
        if (i != fieldCount - 1) table << ",";
    }
    table << endl;
    table.close();

    cout << "Table '" << tableName << "' created!\n";
}

// ========== LOAD SCHEMA ==========
vector<string> loadSchema(string tableName) {
    ifstream schema(tableName + "_schema.txt");
    vector<string> fields;
    string field;
    while (getline(schema, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

// ========== INSERT ==========
void insertRecord() {
    string tableName;
    cout << "Enter table name: ";
    cin >> tableName;

    vector<string> fields = loadSchema(tableName);
    if (fields.empty()) {
        cout << "Table not found!\n";
        return;
    }

    ofstream table(tableName + ".txt", ios::app);
    for (int i = 0; i < fields.size(); i++) {
        string value;
        cout << "Enter value for " << fields[i] << ": ";
        cin >> value;
        table << value;
        if (i != fields.size() - 1) table << ",";
    }
    table << endl;
    table.close();
    cout << "Record inserted!\n";
}

// ========== VIEW ==========
void viewTable(string tableName) {
    ifstream table(tableName + ".txt");
    if (!table) {
        cout << "Table not found!\n";
        return;
    }
    string line;
    while (getline(table, line)) {
        cout << line << endl;
    }
    table.close();
}

// ========== SEARCH ==========
void searchRecord(string tableName) {
    vector<string> fields = loadSchema(tableName);
    if (fields.empty()) { cout << "Table not found!\n"; return; }

    cout << "Search by which field? (0 to " << fields.size()-1 << ")\n";
    for (int i=0;i<fields.size();i++) cout << i << ". " << fields[i] << endl;

    int fIndex;
    cin >> fIndex;
    string value;
    cout << "Enter value to search: ";
    cin >> value;

    ifstream table(tableName + ".txt");
    string line;
    getline(table, line); // skip header
    bool found = false;
    while (getline(table, line)) {
        stringstream ss(line);
        vector<string> values;
        string val;
        while (getline(ss, val, ',')) values.push_back(val);
        if (values[fIndex] == value) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) cout << "No record found!\n";
}

// ========== DELETE ==========
void deleteRecord(string tableName) {
    vector<string> fields = loadSchema(tableName);
    if (fields.empty()) { cout << "Table not found!\n"; return; }

    cout << "Delete by which field? (0 to " << fields.size()-1 << ")\n";
    for (int i=0;i<fields.size();i++) cout << i << ". " << fields[i] << endl;

    int fIndex;
    cin >> fIndex;
    string value;
    cout << "Enter value to match: ";
    cin >> value;

    ifstream table(tableName + ".txt");
    ofstream temp("temp.txt");
    string line;
    getline(table, line); // copy header
    temp << line << endl;

    bool deleted = false;
    while (getline(table, line)) {
        stringstream ss(line);
        vector<string> values;
        string val;
        while (getline(ss, val, ',')) values.push_back(val);

        if (values[fIndex] == value) {
            cout << "Deleted: " << line << endl;
            deleted = true;
            continue;
        }
        temp << line << endl;
    }
    table.close();
    temp.close();
    remove((tableName + ".txt").c_str());
    rename("temp.txt", (tableName + ".txt").c_str());
    if (!deleted) cout << "No matching record found!\n";
}

// ========== UPDATE ==========
void updateRecord(string tableName) {
    vector<string> fields = loadSchema(tableName);
    if (fields.empty()) { cout << "Table not found!\n"; return; }

    cout << "Update which field? (0 to " << fields.size()-1 << ")\n";
    for (int i=0;i<fields.size();i++) cout << i << ". " << fields[i] << endl;

    int fIndex;
    cin >> fIndex;
    string oldValue, newValue;
    cout << "Enter old value: ";
    cin >> oldValue;
    cout << "Enter new value: ";
    cin >> newValue;

    ifstream table(tableName + ".txt");
    ofstream temp("temp.txt");
    string line;
    getline(table, line); // copy header
    temp << line << endl;

    bool updated = false;
    while (getline(table, line)) {
        stringstream ss(line);
        vector<string> values;
        string val;
        while (getline(ss, val, ',')) values.push_back(val);

        if (values[fIndex] == oldValue) {
            values[fIndex] = newValue;
            updated = true;
        }

        for (int i=0;i<values.size();i++) {
            temp << values[i];
            if (i != values.size()-1) temp << ",";
        }
        temp << endl;
    }
    table.close();
    temp.close();
    remove((tableName + ".txt").c_str());
    rename("temp.txt", (tableName + ".txt").c_str());
    if (updated) cout << "Record updated!\n"; 
    else cout << "No matching record found!\n";
}

// ========== TABLE MENU ==========
void tableMenu(string tableName) {
    int choice;
    do {
        cout << "\n=== Operations on table '" << tableName << "' ===\n";
        cout << "1. View Records\n";
        cout << "2. Search Record\n";
        cout << "3. Update Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Back\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: viewTable(tableName); break;
            case 2: searchRecord(tableName); break;
            case 3: updateRecord(tableName); break;
            case 4: deleteRecord(tableName); break;
            case 5: cout << "Back to main menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

// ========== MAIN ==========
int main() {
    int choice;
    do {
        cout << "\n==== Mini DB ====\n";
        cout << "1. Create Table\n";
        cout << "2. Insert into Table\n";
        cout << "3. Select Table (operations)\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) createTable();
        else if (choice == 2) insertRecord();
        else if (choice == 3) {
            string tableName;
            cout << "Enter table name: ";
            cin >> tableName;
            tableMenu(tableName);
        }
        else if (choice == 4) cout << "Exiting...\n";
        else cout << "Invalid choice!\n";
    } while (choice != 4);

    return 0;
}

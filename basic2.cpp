#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

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

    // Create empty table file with header
    ofstream table(tableName + ".txt");
    for (int i = 0; i < fieldCount; i++) {
        table << fields[i];
        if (i != fieldCount - 1) table << ",";
    }
    table << endl;
    table.close();

    cout << "Table '" << tableName << "' created!\n";
}

void insertRecord() {
    string tableName;
    cout << "Enter table name: ";
    cin >> tableName;

    // Load schema
    ifstream schema(tableName + "_schema.txt");
    if (!schema) {
        cout << "Table not found!\n";
        return;
    }

    vector<string> fields;
    string field;
    while (getline(schema, field, ',')) {
        fields.push_back(field);
    }
    schema.close();

    // Insert values
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

void viewTable() {
    string tableName;
    cout << "Enter table name: ";
    cin >> tableName;

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

int main() {
    int choice;
    do {
        cout << "\n==== Mini DB ====\n";
        cout << "1. Create Table\n";
        cout << "2. Insert into Table\n";
        cout << "3. View Table\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createTable(); break;
            case 2: insertRecord(); break;
            case 3: viewTable(); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
using std::left;
#define MAX_NUM_STU 20

// TASK1: Merge two files into a new file.
void mergeFile(const string ip_filename1, const string ip_filename2, const string op_filename){
    ifstream record1(ip_filename1);
    ifstream record2(ip_filename2);
    ofstream merged(op_filename);

    char c; record1.get(c);
    while(!record1.eof()) {
        merged.put(c);
        record1.get(c);
    }
    while(!record2.eof()) {
        merged.put(c);
        record2.get(c);
    }

    record1.close(); record2.close(); merged.close();
}

// TASK2: Clean a given file and save it to a new file.
void cleanFile(const string ip_filename, const string op_filename){
    string data[20][2];
    int i = 0;
    bool previouslyWritten = false;
    
    ifstream in(ip_filename);
    ofstream out(op_filename);

    for (int j = 0; j < 20; j++) {
        data[j][0] = "";
        data[j][1] = "";
    }
    
    string line;
    while (!in.eof()) {
        getline(in, line);
        for (int j = 0; j < line.length() + 1; j++) {
            char c = line[j];
            int ascii = int(line[j]);
            if (((ascii >= 65) && (ascii <= 90)) || ((ascii >= 97) && (ascii <= 122)) || ((ascii >= 48) && (ascii <= 57))) {
                data[i/2][i%2] += c;
                previouslyWritten = true;
            } else {
                if (previouslyWritten) {
                    previouslyWritten = false;
                    i++;
                }
            }
        }
        if (previouslyWritten) {
            previouslyWritten = false;
            i++;
        }
    }

    int maxNamelen = 0;
    for (int j = 0; j < i/2; j++) {
        if (data[j][0].length() > maxNamelen) {
            maxNamelen = data[j][0].length();
        }
    }

    out << left;
    for (int j = 0; j < i/2; j++) {
        out << setw(maxNamelen+1) << data[j][0] << data[j][1] << endl;
    }

    in.close(); out.close();
}

// TASK3: Analyze a file, that is, to find the two students who have got the lowest and the highest grade, respectively.
// If two or more students got the same lowest/highest grade, you only need to return the first-appeared ones.
void analyzeFile(const string filename, string &low_name, string &high_name, int &low_grade, int &high_grade){
    string data[20][2];
    string word;
    int i = 0;

    ifstream file(filename);

    while (file >> word) {
        data[i/2][i%2] = word;
        i++;
    }

    low_grade  = stoi(data[0][1]); high_grade = stoi(data[0][1]);
    low_name = data[0][0]; high_name = data[0][0];
    for (int j = 0; j < i/2; j++) {
        if (stoi(data[j][1]) < low_grade) {
            low_grade = stoi(data[j][1]);
            low_name = data[j][0];
        } else if (stoi(data[j][1]) > high_grade) {
            high_grade = stoi(data[j][1]);
            high_name = data[j][0];
        }
    }
}

// Display a given file.
void displayFile(const string filename){
    string temp;
    ifstream ifs(filename);
    if (!ifs){
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }
    
    cout << filename << endl;
    while (!ifs.eof()){
        getline(ifs, temp);
        cout << temp << endl;
    }
    cout << endl;
    ifs.close();
    return;
}

int main(){
    string low_name, high_name;
    int low_grade = 0, high_grade = 0;

    displayFile("record1.txt");
    displayFile("record2.txt");

    mergeFile("record1.txt", "record2.txt", "record.txt");
    displayFile("record.txt");

    cleanFile("record.txt", "record_clean.txt");
    displayFile("record_clean.txt");

    analyzeFile("record_clean.txt", low_name, high_name, low_grade, high_grade);
    cout << low_name << " got the lowest grade: " << low_grade << endl;
    cout << high_name << " got the highest grade: " << high_grade << endl;
    return 0;
}
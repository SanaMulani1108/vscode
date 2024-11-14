#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

vector<vector<string>> readCSV(const string& fileName) {
    vector<vector<string>> data;
    ifstream file(fileName);
    string line;
    
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;
        
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        
        data.push_back(row);
    }
    
    return data;
}

void writeCSV(const string& fileName, const vector<vector<string>>& data) {
    ofstream file(fileName);
    
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); i++) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
}

double findPearsonCorrelation(const vector<vector<string>>& data, int tid1, int tid2) {
    vector<int> x, y;

    for (size_t j = 1; j < data[0].size(); j++) {
        x.push_back(stoi(data[tid1][j]));
        y.push_back(stoi(data[tid2][j]));
    }

    int n = x.size();
    int sumX = accumulate(x.begin(), x.end(), 0);
    int sumY = accumulate(y.begin(), y.end(), 0);
    int sumX2 = accumulate(x.begin(), x.end(), 0, [](int acc, int val) { return acc + val * val; });
    int sumY2 = accumulate(y.begin(), y.end(), 0, [](int acc, int val) { return acc + val * val; });
    int sumXY = 0;

    for (size_t i = 0; i < n; i++) {
        sumXY += x[i] * y[i];
    }

    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    return (denominator == 0) ? 0 : numerator / denominator;
}

string getVerdict(double correlation) {
    if (correlation == 0) return "No relationship";
    if (correlation > 0) return "Positive correlation";
    return "Negative correlation";
}

int main() {
    string inputFile = "ass9_student_data.csv";
    vector<vector<string>> data = readCSV(inputFile);

    int totalRows = data.size() - 1;
    vector<vector<string>> results = {{"Item 1 (Activity)", "Item 2 (Activity)", "Pearson Correlation Coefficient", "Type of Correlation"}};

    for (int i = 1; i <= totalRows; i++) {
        for (int j = i + 1; j <= totalRows; j++) {
            double correlation = findPearsonCorrelation(data, i, j);
            string verdict = getVerdict(correlation);
            string item1 = data[i][0];
            string item2 = data[j][0];
            results.push_back({item1, item2, to_string(correlation), verdict});
        }
    }

    writeCSV("PearsonCorrelation_output.csv", results);

    return 0;
}

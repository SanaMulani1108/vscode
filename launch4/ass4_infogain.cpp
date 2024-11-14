#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;

double calculateEntropy(int yesCount, int total) {
    if (total == 0) return 0;
    double pYes = static_cast<double>(yesCount) / total;
    double pNo = 1 - pYes;
    return -pYes * log2(pYes) - pNo * log2(pNo);
}

double log2(double value) {
    if (value == 0) return 0;
    return log(value) / log(2);
}

int main() {
    string csvFile = "ass4_cricket_data.csv";
    string line;
    char csvSplitBy = ',';
    int total = 0;
    int yesCount = 0;
    
    ifstream file(csvFile);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<string> columnHeaders;
    if (getline(file, line)) {
        stringstream ss(line);
        string header;
        while (getline(ss, header, csvSplitBy)) {
            columnHeaders.push_back(header);
        }
    }

    vector<vector<string>> data;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> values;
        string value;
        while (getline(ss, value, csvSplitBy)) {
            values.push_back(value);
        }
        if (values.size() == columnHeaders.size()) {
            data.push_back(values);
            total++;
            if (values.back() == "Yes") {
                yesCount++;
            }
        }
    }

    file.close();

    double parentEntropy = calculateEntropy(yesCount, total);

    cout << "\n=== Calculation Steps for All Columns ===\n";
    cout << "Parent Entropy: " << fixed << setprecision(4) << parentEntropy << endl;

    for (size_t choice = 0; choice < columnHeaders.size() - 1; ++choice) {
        map<string, vector<int>> childCount;
        for (const auto& row : data) {
            string childName = row[choice];
            if (childCount.find(childName) == childCount.end()) {
                childCount[childName] = vector<int>(2, 0);
            }
            if (row.back() == "Yes") {
                childCount[childName][0]++;
            } else {
                childCount[childName][1]++;
            }
        }

        double childEntropy = 0;
        cout << "\nColumn: " << columnHeaders[choice] << "\n";
        cout << "| Child Node | Yes Count | No Count | Total | Entropy |\n";
        cout << "-----------------------------------------------------\n";
        for (const auto& entry : childCount) {
            const vector<int>& counts = entry.second;
            int childTotal = counts[0] + counts[1];
            double childNodeEntropy = calculateEntropy(counts[0], childTotal);
            childEntropy += (static_cast<double>(childTotal) / total) * childNodeEntropy;

            cout << "| " << setw(10) << left << entry.first << " | " 
                 << setw(9) << counts[0] << " | " 
                 << setw(8) << counts[1] << " | " 
                 << setw(5) << childTotal << " | " 
                 << setw(8) << fixed << setprecision(4) << childNodeEntropy << " |\n";
        }

        cout << "-----------------------------------------------------\n";
        cout << "Total Child Entropy: " << fixed << setprecision(4) << childEntropy << endl;

        double infoGain = parentEntropy - childEntropy;
        cout << "Information Gain for column " << columnHeaders[choice] << ": " << fixed << setprecision(4) << infoGain << endl;
    }

    return 0;
}

// Information Gain in data mining measures the reduction in entropy (uncertainty) when a dataset is split based on a particular feature. It helps identify the most informative feature for splitting the data.

// Steps to Calculate Information Gain:

// Calculate Parent Entropy: Compute the entropy for the entire dataset before the split, based on the distribution of the target variable (e.g., "Yes"/"No").

// Formula for entropy:
// Entropy(S)=-(pyesl og2(pyes)+pno log2(no))


// Calculate Child Entropy: For each feature, compute the entropy of the resulting subgroups (child nodes) after the split. Each child group will have its own "Yes" and "No" counts.

// Child entropy formula for each subgroup:
// Entropy(S)=-(pyesl og2(pyes)+pno log2(no))

// Calculate Information Gain: The information gain is the difference between the parent entropy and the weighted sum of the child entropies.

// Formula for Information Gain:
// infogain=entro(parent)-sum(schild/s * entro(child))

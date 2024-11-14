#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

struct AttributeResults {
    int attributeIndex;
    double parentGini;
    double childGini;

    AttributeResults(int idx, double pGini, double cGini)
        : attributeIndex(idx), parentGini(pGini), childGini(cGini) {}
};

double calculateGini(int yesCount, int total) {
    if (total == 0) return 0;
    double pYes = (double) yesCount / total;
    double pNo = 1 - pYes;
    return 1 - (pYes * pYes + pNo * pNo);
}

int main() {
    string csvFile = "ass12_play_dataset.csv";
    vector<vector<string>> dataset;
    int total = 0, yesCount = 0;

    ifstream file(csvFile);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    getline(file, line);  // Skip header
    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> values;
        string token;
        while (getline(iss, token, ',')) {
            values.push_back(token);
        }
        dataset.push_back(values);
        total++;

        if (values[4] == "Play") {
            yesCount++;
        }
    }
    file.close();

    double parentGini = calculateGini(yesCount, total);

    cout << fixed << setprecision(4);
    cout << "Parent Gini Index: " << parentGini << endl;
    cout << "\n";

    vector<AttributeResults> resultsList;

    for (int attributeIndex = 0; attributeIndex < 4; attributeIndex++) {
        total = 0;
        yesCount = 0;
        map<string, vector<int>> childCount;

        for (auto &values : dataset) {
            total++;

            if (values[4] == "Play") {
                yesCount++;
            }

            if (attributeIndex == 2) {
                int humidity = stoi(values[2]);
                values[2] = (humidity > 80) ? "High" : "Normal";
            } else if (attributeIndex == 1) {
                int temperature = stoi(values[1]);
                values[1] = (temperature > 80) ? "High" : (temperature >= 70) ? "Mild" : "Low";
            }

            string childName = values[attributeIndex];
            if (childCount.find(childName) == childCount.end()) {
                childCount[childName] = {0, 0};
            }

            if (values[4] == "Play") {
                childCount[childName][0]++;
            } else {
                childCount[childName][1]++;
            }
        }

        double childGini = 0;

        cout << left << setw(15) << "Attribute Value" << setw(5) << "Play" << setw(10) << "Don't Play" 
             << setw(6) << "Total" << setw(10) << "Gini Index" << endl;
        cout << "------------------------------------------------------------" << endl;

        int grandTotal = 0;
        for (const auto &entry : childCount) {
            string key = entry.first;
            int playCount = entry.second[0];
            int dontPlayCount = entry.second[1];
            int childTotal = playCount + dontPlayCount;
            grandTotal += childTotal;

            double gini = calculateGini(playCount, childTotal);
            double proportion = (double) childTotal / total;

            childGini += proportion * gini;

            cout << setw(15) << key << setw(5) << playCount << setw(10) << dontPlayCount
                 << setw(6) << childTotal << setw(10) << gini << endl;
        }

        cout << "------------------------------------------------------------" << endl;
        cout << setw(15) << "Total" << setw(5) << "" << setw(10) << "" << setw(6) << grandTotal 
             << setw(10) << "" << endl;
        cout << "\nChild Gini: " << childGini << endl;
        cout << "Gini Gain: " << parentGini - childGini << endl;
        cout << "\n\n";

        resultsList.push_back(AttributeResults(attributeIndex, parentGini, childGini));
    }

    auto bestAttribute = min_element(resultsList.begin(), resultsList.end(), 
        [](const AttributeResults &a, const AttributeResults &b) {
            return a.childGini < b.childGini;
        });

    if (bestAttribute != resultsList.end()) {
        cout << "Best splitting criterion is Attribute " << bestAttribute->attributeIndex 
             << " with Gini Gain: " << parentGini - bestAttribute->childGini << endl;
    }

    return 0;
}




// Attribute Classification
// In decision tree algorithms, attributes (or features) of the dataset are analyzed to determine the best attribute to split the data, creating the most homogenous subsets.
// The algorithm evaluates each attribute and determines its potential to split the dataset into groups with minimized impurity (i.e., each subset should ideally contain records with similar outcomes, like all records indicating "Play" or "Don't Play").






// Information Gain in data mining measures the reduction in entropy (uncertainty) when a dataset is split based on a particular feature. It helps identify the most informative feature for splitting the data.

// Formula for entropy:
// Entropy(S)=-(pyesl og2(pyes)+pno log2(no))

// child entropy formula for each subgroup:
// Entropy(S)=-(pyesl og2(pyes)+pno log2(no))

// Formula for Information Gain:
// infogain=entro(parent)-sum(schild/s * entro(child))



// Gini Index
// The Gini Index is an alternative metric to entropy for measuring impurity. It is widely used in the CART (Classification and Regression Trees) algorithm.
// The Gini Index of a dataset measures the probability that a randomly selected record will be misclassified if it were labeled according to the overall distribution of classes in the subset:
//  giniindex=1- (pyes2 + pno2)


// A lower Gini Index indicates less impurity, making the split more desirable for achieving homogeneous groups in the tree.

// Gini Gain
// Gini Gain is similar to Information Gain but based on the Gini Index. It measures the reduction in the Gini Index after a split compared to the Gini Index before the split.
// The Gini Gain for an attribute is calculated as:

// Gini Gain=Parent Gini−∑(proportion of subset)×Gini Index of subset
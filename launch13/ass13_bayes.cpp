#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Data {
    string outlook;
    string temperature;
    string humidity;
    string wind;
    string play;
};

double calculateAttributeLikelihood(const string& attribute, const vector<Data>& dataset, int attributeIndex, const string& classLabel);
double calculateLikelihood(const vector<string>& newCase, const vector<Data>& dataset, const string& classLabel);

int main() {
    string csvFile = "ass13_cricket_play_data.csv";
    vector<Data> dataset;
    int total = 0, yesCount = 0, noCount = 0;

    // Read CSV file
    ifstream file(csvFile);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file." << endl;
        return 1;
    }

    string line;
    getline(file, line); // Skip header line
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> values;

        while (getline(ss, token, ',')) {
            values.push_back(token);
        }

        if (values.size() == 5) {
            dataset.push_back({values[0], values[1], values[2], values[3], values[4]});
            total++;
            if (values[4] == "Yes") {
                yesCount++;
            } else {
                noCount++;
            }
        }
    }
    file.close();

    double pYes = static_cast<double>(yesCount) / total;
    double pNo = static_cast<double>(noCount) / total;

    cout << "Calculated Prior Probabilities:\n";
    cout << "P(Yes) = " << fixed << setprecision(4) << pYes << endl;
    cout << "P(No) = " << fixed << setprecision(4) << pNo << endl << endl;

    cout << "Enter the weather conditions for a new case:\n";
    vector<string> newCase(4);
    cout << "Outlook (Sunny, Overcast, Rain): ";
    cin >> newCase[0];
    cout << "Temperature (Hot, Mild, Cool): ";
    cin >> newCase[1];
    cout << "Humidity (High, Normal): ";
    cin >> newCase[2];
    cout << "Wind (Weak, Strong): ";
    cin >> newCase[3];

    cout << "\nLikelihoods for Each Attribute:\n";
    cout << "| " << left << setw(11) << "Attribute" << " | " << setw(16) << "Likelihood (Yes)" << " | " << setw(15) << "Likelihood (No)" << " |\n";
    cout << "---------------------------------------------------\n";

    for (int i = 0; i < newCase.size(); i++) {
        double likelihoodYes = calculateAttributeLikelihood(newCase[i], dataset, i, "Yes");
        double likelihoodNo = calculateAttributeLikelihood(newCase[i], dataset, i, "No");

        cout << "| " << setw(11) << newCase[i] << " | " << setw(16) << fixed << setprecision(6) << likelihoodYes 
             << " | " << setw(15) << likelihoodNo << " |\n";
    }

    double yesPosterior = pYes * calculateLikelihood(newCase, dataset, "Yes");
    double noPosterior = pNo * calculateLikelihood(newCase, dataset, "No");

    cout << "\nCalculated Posterior Probabilities:\n";
    cout << "P(Yes|New Case) = " << fixed << setprecision(6) << yesPosterior << endl;
    cout << "P(No|New Case) = " << fixed << setprecision(6) << noPosterior << endl;

    if (yesPosterior > noPosterior) {
        cout << "\nClassified as: Yes (Football will be played)" << endl;
    } else {
        cout << "\nClassified as: No (Football will not be played)" << endl;
    }

    return 0;
}

double calculateAttributeLikelihood(const string& attribute, const vector<Data>& dataset, int attributeIndex, const string& classLabel) {
    int totalInClass = 0;
    int attributeMatchCount = 0;

    for (const auto& data : dataset) {
        if (data.play == classLabel) {
            totalInClass++;
            switch (attributeIndex) {
                case 0: if (data.outlook == attribute) attributeMatchCount++; break;
                case 1: if (data.temperature == attribute) attributeMatchCount++; break;
                case 2: if (data.humidity == attribute) attributeMatchCount++; break;
                case 3: if (data.wind == attribute) attributeMatchCount++; break;
            }
        }
    }

    return (totalInClass == 0) ? 0.0 : static_cast<double>(attributeMatchCount) / totalInClass;
}

double calculateLikelihood(const vector<string>& newCase, const vector<Data>& dataset, const string& classLabel) {
    double likelihood = 1.0;
    for (int i = 0; i < newCase.size(); i++) {
        likelihood *= calculateAttributeLikelihood(newCase[i], dataset, i, classLabel);
    }
    return likelihood;
}



// **Theory:**

// **Bayes’ theorem:**  
// Bayes’ theorem calculates the probability of a hypothesis given observed data. It is widely used in classification problems and can be expressed as:
// P(H|E) = P(E|H) * P(H)  / P(E)

// - **P(H|E):** Posterior probability  
// - **P(H):** Prior probability  
// - **P(E|H):** Likelihood (probability of observing E given H)  
// - **P(E):** Marginal likelihood (probability of E)

// **Naive Bayes Classifier:**  
// The Naive Bayes classifier assumes that the attributes are conditionally independent given the class. The classification is done by calculating the posterior probability for each class and choosing the class with the highest probability.

// **Formula for Classification:**
// P(C|x) = P(x|C)* P(C) / P(x)

// **Conclusion (Data Mining Perspective):**

// In data mining, the Naive Bayes classifier is a simple yet powerful algorithm for predictive modeling. It performs well with large datasets and is effective for applications like spam detection and sentiment analysis. Its assumption of conditional independence makes it computationally efficient and applicable even with limited data.
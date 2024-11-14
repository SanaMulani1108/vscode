#include <bits/stdc++.h>
using namespace std;

double calSupport(const vector<string>& transactions, const vector<int>& combination) {
    int cnt = 0;
    for (const auto& it : transactions) {
        bool valid = true;
        for (int idx : combination) {
            if (it[idx] == '0') {
                valid = false;
                break;
            }
        }
        if (valid) cnt++;
    }
    double support = static_cast<double>(cnt) / transactions.size();
    return support;
}

void generateCombinations(int n, int k, int start, vector<int>& current, vector<vector<int>>& Combinations) {
    if (current.size() == k) {
        Combinations.push_back(current);
        return;
    }
    for (int i = start; i < n; i++) {
        current.push_back(i);
        generateCombinations(n, k, i + 1, current, Combinations);
        current.pop_back();
    }
}


void generateSubsets(const vector<int>& set, vector<vector<int>>& subsets) {
    int n = set.size();
    for (int i = 1; i < (1 << n); i++) {
        vector<int> subset;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                subset.push_back(set[j]);
            }
        }
        subsets.push_back(subset);
    }
}

pair<double, double> calculateRuleSupportConfidence(const vector<string>& transactions, const vector<int>& antecedent, const vector<int>& consequent) {
    vector<int> combined = antecedent;
    combined.insert(combined.end(), consequent.begin(), consequent.end());

    double supportXY = calSupport(transactions, combined);
    double supportX = calSupport(transactions, antecedent);
    double confidence = supportX > 0 ? supportXY / supportX : 0;

    return {supportXY, confidence};
}

int getHeaderIndex(const vector<string>& headers, const string& name) {
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == name) {
            return i;
        }
    }
    return -1; // return -1 if not found
}

void findAssociationRules(const vector<string>& transactions, const vector<vector<int>>& frequentItemSets, const vector<string>& headers, double minConfidence) {
    ofstream out("results.txt", ios::app); 
    out << "\n--- Association Rule Calculations ---\n";
    
    for (const auto& itemSet : frequentItemSets) {
        vector<vector<int>> subsets;
        generateSubsets(itemSet, subsets);

        for (const auto& subset : subsets) {
            if (subset.size() == itemSet.size()) continue; 

            vector<int> remaining;
            for (int idx : itemSet) {
                if (find(subset.begin(), subset.end(), idx) == subset.end()) {
                    remaining.push_back(idx);
                }
            }
            double supportXY = calSupport(transactions, itemSet);
            double supportX = calSupport(transactions, subset);
            double confidence = supportX > 0 ? supportXY / supportX : 0;

            out << "Rule: { ";
            for (int idx : subset) out << headers[idx] << " ";
            out << "} -> { ";
            for (int idx : remaining) out << headers[idx] << " ";
            out << "} -> Confidence: " << confidence << endl;

            if (confidence >= minConfidence) {
                out << "Valid Rule (Conf >= " << minConfidence << "): { ";
                for (int idx : subset) out << headers[idx] << " ";
                out << "} -> { ";
                for (int idx : remaining) out << headers[idx] << " ";
                out << "} with Confidence: " << confidence << endl;
            }
        }
    }
    out.close();
}

int main() {
    ifstream in("input.csv");
    vector<string> transactions;
    vector<string> headers;
    string line;

    if (getline(in, line)) {
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            headers.push_back(token);
        }
    }

    while (getline(in, line)) {
        stringstream ss(line);
        string token, transaction = "";
        while (getline(ss, token, ',')) {
            if (token == "TRUE")
                transaction += '1';
            else if (token == "FALSE")
                transaction += '0';
        }
        if (transaction != "")
            transactions.push_back(transaction);
    }

    double minSupport = 0 , minConfidence = 0 ;
    

    
    ofstream out("results.txt", ios::app); 

    cout << "Enter antecedent items (space-separated, -1 to end): ";
    vector<int> antecedent, consequent;
    string item;
    while (cin >> item && item != "-1") {
        int idx = getHeaderIndex(headers, item);
        if (idx != -1) {
            antecedent.push_back(idx);
        } else {
            cout << "Item " << item << " not found in headers." << endl;
        }
    }

    cout << "Enter consequent items (space-separated, -1 to end): ";
    while (cin >> item && item != "-1") {
        int idx = getHeaderIndex(headers, item);
        if (idx != -1) {
            consequent.push_back(idx);
        } 
        else {
            cout << "Item " << item << " not found in headers." << endl;
        }
    }

    pair<double, double> result = calculateRuleSupportConfidence(transactions, antecedent, consequent);
    double support = result.first;
    double confidence = result.second;

    out << "\n--- Rule Support and Confidence ---\n";
    out << "Support: " << support << ", Confidence: " << confidence << endl;
    out.close();

    return 0;
}
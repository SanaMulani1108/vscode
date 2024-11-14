#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<set<string>> readCSV(const string& filename);
int calculateSupportCount(const vector<set<string>>& transactions, const set<string>& itemset);
vector<set<string>> generateCandidates(const vector<set<string>>& frequentItemsets, int k);
void generateAssociationRules(const vector<vector<set<string>>>& allFrequentItemsets, const vector<set<string>>& transactions, double min_support_percentage, double min_confidence);

vector<set<string>> readCSV(const string& filename) {
    vector<set<string>> transactions;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        set<string> transaction;
        size_t start = line.find('{');
        size_t end = line.find('}');
        if (start != string::npos && end != string::npos) {
            string itemsStr = line.substr(start + 1, end - start - 1);
            stringstream ss(itemsStr);
            string item;
            while (getline(ss, item, ',')) {
                transaction.insert(item);
            }
            transactions.push_back(transaction);
        }
    }
    return transactions;
}

int calculateSupportCount(const vector<set<string>>& transactions, const set<string>& itemset) {
    int count = 0;
    for (const auto& transaction : transactions) {
        if (includes(transaction.begin(), transaction.end(), itemset.begin(), itemset.end())) {
            count++;
        }
    }
    return count;
}

vector<set<string>> generateCandidates(const vector<set<string>>& frequentItemsets, int k) {
    vector<set<string>> candidates;
    int n = frequentItemsets.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            set<string> candidate = frequentItemsets[i];
            candidate.insert(frequentItemsets[j].begin(), frequentItemsets[j].end());
            if (candidate.size() == k) {
                candidates.push_back(candidate);
            }
        }
    }
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    return candidates;
}

void generateAssociationRules(const vector<vector<set<string>>>& allFrequentItemsets, const vector<set<string>>& transactions, double min_support_percentage, double min_confidence) {
    int numTransactions = transactions.size();
    cout << "\nAssociation Rules with Confidence >= " << min_confidence * 100 << "%:" << endl;

    for (const auto& frequentItemsets : allFrequentItemsets) {
        for (const auto& itemset : frequentItemsets) {
            int itemsetSupportCount = calculateSupportCount(transactions, itemset);
            double itemsetSupport = (double)(itemsetSupportCount) / numTransactions;
            if (itemset.size() < 2) continue;

            for (auto it = itemset.begin(); it != itemset.end(); ++it) {
                set<string> antecedent = itemset;
                antecedent.erase(*it);
                set<string> consequent = { *it };

                int antecedentSupportCount = calculateSupportCount(transactions, antecedent);
                double antecedentSupport = (double)(antecedentSupportCount) / numTransactions;

                if (antecedentSupport > 0) {
                    double confidence = itemsetSupport / antecedentSupport;
                    if (confidence >= min_confidence) {
                        cout << "{ ";
                        for (const auto& a : antecedent) {
                            cout << a << " ";
                        }
                        cout << "} => { " << *it << " }";
                        cout << " (Support: " << fixed << setprecision(2) << itemsetSupport * 100
                             << "%, Confidence: " << fixed << setprecision(2) << confidence * 100 << "%)" << endl;
                    }
                }
            }
        }
    }
}

int main() {
    string filename = "sample.csv";
    double min_support_percentage, min_confidence;

    cout << "Enter minimum support percentage (e.g., 0.2 for 20%): ";
    cin >> min_support_percentage;
    cout << "Enter minimum confidence (e.g., 0.5 for 50%): ";
    cin >> min_confidence;

    vector<set<string>> transactions = readCSV(filename);
    int numTransactions = transactions.size();

    cout << "Minimum Support Percentage: " << min_support_percentage * 100 << "%" << endl;
    map<string, int> itemSupport;

    for (const auto& transaction : transactions) {
        for (const auto& item : transaction) {
            itemSupport[item]++;
        }
    }

    vector<set<string>> frequentItemsets;
    vector<vector<set<string>>> allFrequentItemsets;

    cout << "\nFrequent 1-itemsets:" << endl;
    for (const auto& item : itemSupport) {
        double support = (double)(item.second) / numTransactions;
        if (support >= min_support_percentage) {
            frequentItemsets.push_back({item.first});
            cout << "Item: " << item.first << ", Support: " << fixed << setprecision(2) << support * 100 << "%" << endl;
        }
    }
    allFrequentItemsets.push_back(frequentItemsets);

    int k = 2;
    while (!frequentItemsets.empty()) {
        vector<set<string>> candidates = generateCandidates(frequentItemsets, k);
        map<set<string>, int> candidateSupport;

        for (const auto& candidate : candidates) {
            int supportCount = calculateSupportCount(transactions, candidate);
            candidateSupport[candidate] = supportCount;
        }

        frequentItemsets.clear();
        if (!candidateSupport.empty()) {
            cout << "\nFrequent " << k << "-itemsets:" << endl;
            for (const auto& candidate : candidateSupport) {
                double support = (double)(candidate.second) / numTransactions;
                if (support >= min_support_percentage) {
                    frequentItemsets.push_back(candidate.first);
                    cout << "Itemset: {";
                    for (const auto& item : candidate.first) {
                        cout << item << " ";
                    }
                    cout << "}, Support: " << fixed << setprecision(2) << support * 100 << "%" << endl;
                }
            }
            allFrequentItemsets.push_back(frequentItemsets);
        }
        k++;
    }

    generateAssociationRules(allFrequentItemsets, transactions, min_support_percentage, min_confidence);

    return 0;
}



// Association rules are a fundamental concept in data mining used to discover interesting relationships or patterns among a set of items in large datasets, particularly in transactional data. These rules are often used in market basket analysis, where the goal is to find associations between products that customers frequently purchase together.

// Support measures how frequently an itemset appears in the dataset and is calculated using the formula:

// {Support}(A) = {Frequency of itemset A}/{Total number of transactions}

// Confidence measures how often items in the right-hand side of a rule appear when the left-hand side is present, and is calculated using:

// {Confidence}(A -> B) = A U B/ A


// Using a minimum support threshold, we first identify all frequent itemsets, which are item combinations whose support exceeds the threshold. Then, we generate association rules from these frequent itemsets and calculate their confidence. Only the rules with confidence above a specified minimum threshold are considered strong association rules.

// InAssociation rules provide valuable insights into the relationships between items in a dataset. By mining these patterns, businesses can make data-driven decisions, such as identifying which products to bundle together or which items are frequently bought together, thereby enhancing marketing strategies, improving inventory management, and increasing sales.

// csv-collectioncol-bitvector-asr
// weka
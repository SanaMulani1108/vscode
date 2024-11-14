#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

vector<set<string>> transactions;

map<set<string>, int> getItemsetCounts(int setSize);
void printFrequentItemsets(map<set<string>, int>& itemsetCount, int totalTransactions, double minSupportThreshold);
set<set<string>> getCombinations(const set<string>& transaction, int setSize);
void generateCombinations(set<set<string>>& combinations, set<string>& currentSet, const vector<string>& items, int index, int setSize);
map<set<string>, int> generateNextItemsets(const set<set<string>>& previousItemsets, int setSize);
int main() {
    string csvFile = "ass7_freq_item_set.csv"; 
    string line;
    char csvSplitBy = ',';
    double minSupportThreshold = 0.3;

    ifstream file(csvFile);

    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        set<string> items;
        int index = 0;
        while (getline(ss, item, csvSplitBy)) {
            if (index > 0 && !item.empty()) { 
                items.insert(item);
            }
            index++;
        }
        transactions.push_back(items);
    }
    file.close();

    int k = 1;
    map<set<string>, int> currentItemsetCount = getItemsetCounts(1);
    map<set<string>, int> frequentItemsets;

    while (!currentItemsetCount.empty()) {
        cout << "\n" << k << "-itemsets with support >= " << minSupportThreshold << ":\n";
        printFrequentItemsets(currentItemsetCount, transactions.size(), minSupportThreshold);
        frequentItemsets.insert(currentItemsetCount.begin(), currentItemsetCount.end());

      
        set<set<string>> previousItemsets;
        for (const auto& item : frequentItemsets) {
            previousItemsets.insert(item.first);
        }
        currentItemsetCount = generateNextItemsets(previousItemsets, k + 1);
        k++;
    }

    return 0;
}


map<set<string>, int> getItemsetCounts(int setSize) {
    map<set<string>, int> itemsetCount;

    for (const auto& transaction : transactions) {
        if (transaction.size() >= setSize) {
            set<set<string>> combinations = getCombinations(transaction, setSize);
            for (const auto& combination : combinations) {
                itemsetCount[combination]++;
            }
        }
    }

    return itemsetCount;
}

void printFrequentItemsets(map<set<string>, int>& itemsetCount, int totalTransactions, double minSupportThreshold) {
    for (const auto& entry : itemsetCount) {
        double support = static_cast<double>(entry.second) / totalTransactions;
        if (support >= minSupportThreshold) {
            cout << "Item set: { ";
            for (const string& item : entry.first) {
                cout << item << " ";
            }
            cout << "} | Support: " << support << " | Count: " << entry.second << endl;
        }
    }
}

set<set<string>> getCombinations(const set<string>& transaction, int setSize) {
    set<set<string>> combinations;
    vector<string> transactionList(transaction.begin(), transaction.end());
    set<string> currentSet;
    generateCombinations(combinations, currentSet, transactionList, 0, setSize);
    return combinations;
}

void generateCombinations(set<set<string>>& combinations, set<string>& currentSet, const vector<string>& items, int index, int setSize) {
    if (currentSet.size() == setSize) {
        combinations.insert(currentSet);
        return;
    }

    for (int i = index; i < items.size(); i++) {
        currentSet.insert(items[i]);
        generateCombinations(combinations, currentSet, items, i + 1, setSize);
        currentSet.erase(items[i]);
    }
}

map<set<string>, int> generateNextItemsets(const set<set<string>>& previousItemsets, int setSize) {
    map<set<string>, int> nextItemsetCount;

    for (const auto& transaction : transactions) {
        if (transaction.size() >= setSize) {
            set<set<string>> combinations = getCombinations(transaction, setSize);
            for (const auto& combination : combinations) {
                nextItemsetCount[combination]++;
            }
        }
    }

    return nextItemsetCount;
}

// frequent itemsets are sets of items that appear together in a dataset with a frequency (support) greater than or equal to a user-defined threshold. The support of an itemset is calculated as the ratio of the number of transactions containing that itemset to the total number of transactions. The formula for support is:
// Support(X)= Count of transactions containing X/Total number of transactions

 
// For example, given a dataset of transactions with items like Milk, Bread, Butter, Eggs, Apples, and a minimum support threshold of 30% (0.30), the goal is to find all itemsets (combinations of items) whose support exceeds this threshold.

// Steps:
// Extract the transactions: From the CSV dataset, represent each transaction as a set of items, indicating whether the item is present (True) or absent (False).
// Count itemsets: For each size of itemsets (1-itemsets, 2-itemsets, etc.), count how many transactions contain the itemset.
// Calculate support: For each itemset, calculate the support percentage using the formula mentioned above.
// Filter frequent itemsets: Only retain itemsets with support greater than or equal to the minimum threshold.

// Frequent itemsets are key in identifying associations between items in transactions. By finding these itemsets, businesses can gain insights into which items are often bought together, aiding in decisions like product bundling or targeted marketing.

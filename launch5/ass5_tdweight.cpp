#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

int main() {
    string inputFilePath = "ass5_region_product_quantity.csv"; 
    string outputFilePath = "output_region_product_quantity.csv"; 

    map<string, int> regionRiceMap;
    map<string, int> regionWheatMap;
    int totalRice = 0;
    int totalWheat = 0;
    map<string, int> regionTotalMap;

    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    string line;
    bool isHeader = true;

    while (getline(inputFile, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        stringstream ss(line);
        string region, product, quantityStr;
        getline(ss, region, ',');
        getline(ss, product, ',');
        getline(ss, quantityStr, ',');

        int quantity = stoi(quantityStr);

        region = region.substr(0, region.find_last_not_of(" \t\n\r") + 1); 
        product = product.substr(0, product.find_last_not_of(" \t\n\r") + 1);

        if (product == "Rice") {
            regionRiceMap[region] += quantity;
            totalRice += quantity;
        } else if (product == "Wheat") {
            regionWheatMap[region] += quantity;
            totalWheat += quantity;
        }

        regionTotalMap[region] += quantity;
    }

    regionTotalMap["Total"] = totalRice + totalWheat;
    inputFile.close();

    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return 1;
    }

    outputFile << "Region,Rice (Count),Rice (TWeight %),Rice (DWeight %),"
               << "Wheat (Count),Wheat (TWeight %),Wheat (DWeight %),Total (Count)" << endl;

    for (const auto& region : regionTotalMap) {
        if (region.first != "Total") {
            int riceCount = regionRiceMap[region.first];
            int wheatCount = regionWheatMap[region.first];
            int totalCount = region.second;

            double riceTWeight = totalRice > 0 ? (static_cast<double>(riceCount) / totalRice) * 100 : 0;
            double wheatTWeight = totalWheat > 0 ? (static_cast<double>(wheatCount) / totalWheat) * 100 : 0;
            double riceDWeight = totalCount > 0 ? (static_cast<double>(riceCount) / totalCount) * 100 : 0;
            double wheatDWeight = totalCount > 0 ? (static_cast<double>(wheatCount) / totalCount) * 100 : 0;

            outputFile << region.first << "," 
                       << riceCount << "," << fixed << setprecision(2) << riceTWeight << "%," 
                       << riceDWeight << "%," << wheatCount << "," 
                       << wheatTWeight << "%," << wheatDWeight << "%," << totalCount << endl;
        }
    }

    outputFile << "Total," << totalRice << ",100%,100%," << totalWheat << ",100%,100%,"
               << regionTotalMap["Total"] << endl;

    outputFile.close();

    cout << "Output written to: " << outputFilePath << endl;

    return 0;
}

// In data mining, TWeight (Total Weight) and DWeight (Distribution Weight) are measures that provide insights into the proportion of each product category (e.g., Smartphones, Laptops) relative to the total quantity or the total quantity within each region.

// TWeight (Total Weight): This is the percentage of the product's quantity relative to the total quantity of that product across all regions. It shows how a region's sales of a product contribute to the overall sales of that product.

// Formula:
// TWeight= Product Count in Region/Total Product Count (across all regions) ×100

// DWeight (Distribution Weight): This is the percentage of the product's quantity relative to the total quantity in that specific region. It shows how the sales of a product are distributed within a region.

// Formula:
// TWeight= Product Count in Region/Total Count in region) ×100

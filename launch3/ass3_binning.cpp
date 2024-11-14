#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <numeric> 
using namespace std;

vector<double> readDataFromCsv(const string &csvFile, int columnIndex);
vector<vector<double>> equalFrequencyBinning(const vector<double> &dataValues, int numBins);
vector<vector<double>> equalWidthBinning(const vector<double> &dataValues, int numBins);
double smoothByMean(const vector<double> &bin);
double smoothByMedian(vector<double> bin);
vector<double> smoothByBoundaries(const vector<double> &bin);

int main() {
    string csvFile = "ass3_Prices_data.csv";
    int columnIndex = 0;
    int numBins;

    cout << "Enter the number of bins you want to create: ";
    cin >> numBins;

    cout << "\nChoose the type of binning you want to perform:\n";
    cout << "1. Equal Frequency Binning\n";
    cout << "2. Equal Width Binning\n";
    cout << "Your choice (1 or 2): ";
    int binningChoice;
    cin >> binningChoice;
    cout << endl;

    vector<double> dataValues = readDataFromCsv(csvFile, columnIndex);
    if (dataValues.empty()) {
        cout << "No data available for binning." << endl;
        return 0;
    }

    sort(dataValues.begin(), dataValues.end());
    vector<vector<double>> bins;

    if (binningChoice == 1) {
        bins = equalFrequencyBinning(dataValues, numBins);
    } else if (binningChoice == 2) {
        bins = equalWidthBinning(dataValues, numBins);
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 0;
    }

    cout << "Binning results:\n";
    for (int binCount = 0; binCount < bins.size(); ++binCount) {
        vector<double> bin = bins[binCount];
        cout << "Bin " << (binCount + 1) << " contents: ";
        for (double value : bin) cout << value << " ";
        cout << "\nSmoothing by Bin Mean: " << smoothByMean(bin);
        cout << "\nSmoothing by Bin Median: " << smoothByMedian(bin);
        cout << "\nSmoothing by Boundaries: ";
        vector<double> boundarySmoothBin = smoothByBoundaries(bin);
        for (double value : boundarySmoothBin) cout << value << " ";
        cout << "\n--------------------------------------------\n";
    }
    return 0;
}

vector<double> readDataFromCsv(const string &csvFile, int columnIndex) {
    vector<double> dataValues;
    ifstream file(csvFile);
    string line;
    
    if (file.is_open()) {
        getline(file, line); 
        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            int col = 0;
            while (getline(ss, value, ',')) {
                if (col == columnIndex) {
                    try {
                        dataValues.push_back(stod(value));
                    } catch (invalid_argument &) {
                        cerr << "Invalid number format in line: " << line << endl;
                    }
                    break;
                }
                col++;
            }
        }
        file.close();
    } else {
        cerr << "Error reading file: " << csvFile << endl;
    }
    return dataValues;
}

vector<vector<double>> equalFrequencyBinning(const vector<double> &dataValues, int numBins) {
    vector<vector<double>> bins;
    int binSize = dataValues.size() / numBins;
    for (size_t i = 0; i < dataValues.size(); i += binSize) {
        int end = min(i + binSize, dataValues.size());
        bins.emplace_back(dataValues.begin() + i, dataValues.begin() + end);
    }
    return bins;
}

vector<vector<double>> equalWidthBinning(const vector<double> &dataValues, int numBins) {
    vector<vector<double>> bins;
    double minVal = *min_element(dataValues.begin(), dataValues.end());
    double maxVal = *max_element(dataValues.begin(), dataValues.end());
    double binWidth = (maxVal - minVal) / numBins;

    for (int i = 0; i < numBins; ++i) {
        double binLowerBound = minVal + i * binWidth;
        double binUpperBound = minVal + (i + 1) * binWidth;
        vector<double> bin;
        for (double value : dataValues) {
            if (value >= binLowerBound && value < binUpperBound) {
                bin.push_back(value);
            }
        }
        bins.push_back(bin);
    }
    bins.back().push_back(maxVal); 
    return bins;
}

double smoothByMean(const vector<double> &bin) {
    double sum = accumulate(bin.begin(), bin.end(), 0.0);
    return bin.empty() ? 0.0 : sum / bin.size();
}

double smoothByMedian(vector<double> bin) {
    sort(bin.begin(), bin.end());
    size_t size = bin.size();
    return size % 2 == 0 ? (bin[size / 2 - 1] + bin[size / 2]) / 2.0 : bin[size / 2];
}

vector<double> smoothByBoundaries(const vector<double> &bin) {
    double minVal = *min_element(bin.begin(), bin.end());
    double maxVal = *max_element(bin.begin(), bin.end());
    vector<double> boundarySmoothBin;
    for (double value : bin) {
        boundarySmoothBin.push_back(abs(value - minVal) < abs(value - maxVal) ? minVal : maxVal);
    }
    return boundarySmoothBin;
}

// Binning is a data preprocessing technique used to smooth or group a continuous set of data values into a smaller number of "bins" or intervals. This approach helps reduce noise, manage large datasets, and improve the performance of machine learning algorithms by reducing the dimensionality of continuous features.

// Types of Binning
// The code above uses two main binning techniques:

// Equal Frequency Binning:

// Divides data into bins that each contain approximately the same number of data points. This method ensures each bin has a balanced amount of data but may result in bins with varying ranges (intervals) in the data values.
// Formula: Divide sorted data points into 𝑛 bins, each containing approximately 
// total number of data points/𝑛

// For example, if you have 100 values and 4 bins, each bin will contain about 25 values.

// Equal Width Binning:
// Divides the range of data values into bins of equal width, resulting in bins with the same interval size but potentially differing numbers of data points.
// Formula: 
// Bin Width=Max Value−Min Value/Number of Bins

// For example, with a data range of 0 to 100 and 4 bins, each bin would have a width of 25.
// Smoothing Techniques
// The code provides three methods to smooth the values within each bin:

// Smoothing by Mean: Replaces all values in a bin with the mean of that bin.
// Smoothing by Median: Replaces all values in a bin with the median value of that bin.
// Smoothing by Boundaries: Replaces each value in the bin with the closest boundary value (either the bin’s minimum or maximum).

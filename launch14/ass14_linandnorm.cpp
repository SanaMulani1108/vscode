#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    string csvFile = "ass14_work.csv";
    string outputCsvFile = "normalized_values.csv";
    string line;

    int n = 0;
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    vector<double> xValues, yValues;

    ifstream file(csvFile);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    getline(file, line); // Skip header

    // Read data
    while (getline(file, line)) {
        istringstream iss(line);
        string xStr, yStr;

        if (getline(iss, xStr, ',') && getline(iss, yStr, ',')) {
            double x = stod(xStr);
            double y = stod(yStr);

            xValues.push_back(x);
            yValues.push_back(y);

            n++;
            sumX += x;
            sumY += y;
            sumXY += (x * y);
            sumX2 += (x * x);
        }
    }
    file.close();

    // Linear regression calculation
    double m = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double c = (sumY - m * sumX) / n;

    cout << fixed << setprecision(4);
    cout << "Slope (m): " << m << endl;
    cout << "Intercept (c): " << c << endl;
    cout << "Linear Regression Equation: y = " << m << "x + " << c << endl;

    // Min-Max Normalization
    double minX = *min_element(xValues.begin(), xValues.end());
    double maxX = *max_element(xValues.begin(), xValues.end());

    // Z-score Normalization
    double meanX = sumX / n;
    double varianceX = 0;
    for (double x : xValues) {
        varianceX += pow(x - meanX, 2);
    }
    double stdDevX = sqrt(varianceX / n);

    // Open output file for writing normalized values
    ofstream outputFile(outputCsvFile);
    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    // Write header to output file
    outputFile << "Original_X,Original_Y,Min_Max_Normalized_X,Z_Score_Normalized_X\n";

    // Write normalized values to file
    for (size_t i = 0; i < xValues.size(); i++) {
        double x = xValues[i];
        double y = yValues[i];
        
        // Min-Max Normalization
        double minMaxNorm = (x - minX) / (maxX - minX);
        
        // Z-score Normalization
        double zScore = (x - meanX) / stdDevX;
        
        // Write original values and normalized values to the output file
        outputFile << fixed << setprecision(4)
                   << x << "," << y << "," << minMaxNorm << "," << zScore << "\n";
    }

    outputFile.close();
    cout << "Normalized values saved to " << outputCsvFile << endl;

    return 0;
}


// Linear Regression is a statistical method used to model the relationship between a dependent variable 
// y and one or more independent variables x. It assumes a linear relationship between the variables, which means that changes in the independent variable(s) result in proportional changes in the dependent variable. The goal of linear regression is to find the best-fitting straight line, known as the regression line, that minimizes the sum of squared differences (errors) between the actual data points and the predicted values.

// In the context of simple linear regression (like the code provided), the equation for the line is:


// y=mx+c
// Where:


// y is the dependent variable (e.g., exam score),
// x is the independent variable (e.g., study hours),
// m is the slope (representing the rate of change of y with respect to x),
// c is the intercept (the value of y when x=0).

// m=nsumxy -sumxsumy/n (sum(xsq) -(sumx)sq)

// c=sumy*sum(xsq) -sumx sumxy/n (sum(xsq) -(sumx)sq)



// Prediction and Analysis: Linear regression is a powerful tool for predicting and understanding the relationship between variables, especially when the relationship is approximately linear.
// Simple yet Effective: It is simple to implement and can provide valuable insights with minimal computation, making it widely used in data mining and predictive analytics.


// LinearRegressionlearner

// Min-max normalization is a technique used to scale data to a specific range, usually between 0 and 1. This method adjusts the values in a dataset so that they fit within this range while preserving the relationships between them. It's useful in data mining and machine learning to prevent certain features from dominating others due to differences in scale.

// The formula for min-max normalization is:
// X′ =(X-minx/maxx- minx)*(new_max−new_min)+new_min


// Z-score normalization (also called standardization) is a technique used to rescale data by centering it around the mean and adjusting it based on the standard deviation. This method transforms data into a distribution with a mean of 0 and a standard deviation of 1, which is useful for comparing data points that come from different distributions or scales. Z-score normalization is widely used in data mining and machine learning, especially when the algorithm assumes normally distributed data.

// The formula for Z-score normalization is:
// Z= X−μ / σ


// X is the original value,
// μ is the mean of the data,
// σ is the standard deviation of the data
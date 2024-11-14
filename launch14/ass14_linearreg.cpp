#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    string csvFile = "ass14_work.csv"; 
    string line;
    string cvsSplitBy = ",";
    
    int n = 0;
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    ifstream file(csvFile);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    
    getline(file, line);

    while (getline(file, line)) {
        istringstream iss(line);
        string xStr, yStr;

        if (getline(iss, xStr, ',') && getline(iss, yStr, ',')) {
            double x = stod(xStr);
            double y = stod(yStr);

            n++;
            sumX += x;
            sumY += y;
            sumXY += (x * y);
            sumX2 += (x * x);
        }
    }
    file.close();

    double m = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double c = (sumY - m * sumX) / n;

    cout << fixed << setprecision(4);
    cout << "Slope (m): " << m << endl;
    cout << "Intercept (c): " << c << endl;
    cout << "Linear Regression Equation: y = " << m << "x + " << c << endl;

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
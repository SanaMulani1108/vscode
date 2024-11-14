// #include<bits/stdc++.h>
// #include<fstream>
// using namespace std;

// double cal_median(vector<double> v) {
//     int n = v.size();
//     return (n % 2 == 0) ? ((v[(n / 2) - 1] + v[n / 2]) / 2.0) : v[n / 2];
// }

// int main() {
//     ifstream in("ass6_fivenum.csv");
//     vector<double> data;
//     double value;

//     while (in >> value) {
//         data.push_back(value);
//     }
//     sort(data.begin(), data.end());

//     cout << "sorted data: ";
//     for (auto it : data)
//         cout << it << " ";
//     cout << "\n";

//     double min, q1, median, q3, max, iqr;
//     int n = data.size();
//     min = data[0];
//     max = data[n - 1];
//     median = cal_median(data);

//     if (n % 2 == 0) { // even
//         vector<double> v(data.begin(), data.begin() + (n / 2));
//         q1 = cal_median(v);
//         vector<double> p(data.begin() + (n / 2), data.end());
//         q3 = cal_median(p);
//     } else { // odd
//         vector<double> v(data.begin(), data.begin() + (n / 2));
//         q1 = cal_median(v);
//         vector<double> p(data.begin() + (n / 2) + 1, data.end());
//         q3 = cal_median(p);
//     }

//     iqr = q3 - q1; // Calculating IQR

//     double lower_bound = q1 - 1.5 * iqr;
//     double upper_bound = q3 + 1.5 * iqr;

//     cout << "min: " << min << "\n";
//     cout << "q1: " << q1 << "\n";
//     cout << "median: " << median << "\n";
//     cout << "q3: " << q3 << "\n";
//     cout << "max: " << max << "\n";
//     cout << "IQR: " << iqr << "\n";
//     cout << "Lower Bound: " << lower_bound << "\n";
//     cout << "Upper Bound: " << upper_bound << "\n";

//     // Detecting outliers
//     cout << "Outliers: ";
//     bool outlier_found = false;
//     for (auto it : data) {
//         if (it < lower_bound || it > upper_bound) {
//             cout << it << " ";
//             outlier_found = true;
//         }
//     }
//     if (!outlier_found) {
//         cout << "None";
//     }
//     cout << "\n";

//     return 0;
// }
#include<bits/stdc++.h>
#include<fstream>
using namespace std;

double cal_median(vector<double> v) {
    int n = v.size();
    return (n % 2 == 0) ? ((v[(n / 2) - 1] + v[n / 2]) / 2.0) : v[n / 2];
}

int main() {
    ifstream in("ass6_fivenum.csv");
    ofstream out("results.csv");  // Open output file
    vector<double> data;
    double value;

    while (in >> value) {
        data.push_back(value);
    }
    sort(data.begin(), data.end());

    // Output sorted data to the CSV
    out << "Sorted Data\n";
    for (auto it : data)
        out << it << ",";
    out << "\n";

    double min, q1, median, q3, max, iqr;
    int n = data.size();
    min = data[0];
    max = data[n - 1];
    median = cal_median(data);

    if (n % 2 == 0) { // even
        vector<double> v(data.begin(), data.begin() + (n / 2));
        q1 = cal_median(v);
        vector<double> p(data.begin() + (n / 2), data.end());
        q3 = cal_median(p);
    } else { // odd
        vector<double> v(data.begin(), data.begin() + (n / 2));
        q1 = cal_median(v);
        vector<double> p(data.begin() + (n / 2) + 1, data.end());
        q3 = cal_median(p);
    }

    iqr = q3 - q1; // Calculating IQR

    double lower_bound = q1 - 1.5 * iqr;
    double upper_bound = q3 + 1.5 * iqr;

    // Output five-number summary, IQR, and bounds to CSV
    out << "Summary\n";
    out << "Min," << min << "\n";
    out << "Q1," << q1 << "\n";
    out << "Median," << median << "\n";
    out << "Q3," << q3 << "\n";
    out << "Max," << max << "\n";
    out << "IQR," << iqr << "\n";
    out << "Lower Bound," << lower_bound << "\n";
    out << "Upper Bound," << upper_bound << "\n";

    // Detect and output outliers to the CSV
    out << "Outliers\n";
    bool outlier_found = false;
    for (auto it : data) {
        if (it < lower_bound || it > upper_bound) {
            out << it << ",";
            outlier_found = true;
        }
    }
    if (!outlier_found) {
        out << "None";
    }
    out << "\n";

    out.close();
    return 0;
}

// The 5-number summary is a set of descriptive statistics that provides a quick overview of a dataset. It includes the following five values:

// Minimum (Min): The smallest value in the dataset.
// First Quartile (Q1): The median of the lower half of the dataset (25th percentile).
// Median: The middle value of the dataset (50th percentile).
// Third Quartile (Q3): The median of the upper half of the dataset (75th percentile).
// Maximum (Max): The largest value in the dataset.
// To calculate the 5-number summary, you need to:

// Sort the data.
// Find the Median: This splits the data into two halves.
// Find Q1 and Q3: These are the medians of the lower and upper halves, respectively.
// Find the Min and Max: The smallest and largest values.
// Formula for Outliers (based on IQR):
// IQR (Interquartile Range) = Q3 - Q1
// Lower Bound for Outliers = Q1 - 1.5 * IQR
// Upper Bound for Outliers = Q3 + 1.5 * IQR
// Any data points outside these bounds are considered outliers.
// This allows you to summarize the data's spread and identify potential outliers.
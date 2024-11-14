#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

// Function to calculate Euclidean distance between two points
double calculateDistance(const vector<double>& point1, const vector<double>& point2) {
    double sum = 0;
    for (int i = 0; i < point1.size(); i++) {
        sum += pow(point1[i] - point2[i], 2);
    }
    return sqrt(sum);
}

// Function to calculate the mean (centroid) of points in a cluster
vector<double> calculateMean(const vector<vector<double>>& points) {
    vector<double> centroid(points[0].size(), 0);
    for (const vector<double>& point : points) {
        for (int i = 0; i < point.size(); i++) {
            centroid[i] += point[i];
        }
    }
    for (int i = 0; i < centroid.size(); i++) {
        centroid[i] /= points.size();
    }
    return centroid;
}

// Function to find the nearest centroid for a given point
int findNearestCentroid(const vector<double>& point, const vector<vector<double>>& centroids) {
    int nearestCentroidIndex = 0;
    double minDistance = calculateDistance(point, centroids[0]);

    for (int i = 1; i < centroids.size(); i++) {
        double distance = calculateDistance(point, centroids[i]);
        if (distance < minDistance) {
            minDistance = distance;
            nearestCentroidIndex = i;
        }
    }
    return nearestCentroidIndex;
}

// Function to get points assigned to a specific cluster
vector<vector<double>> getClusterPoints(const vector<vector<double>>& data, const vector<int>& assignments, int clusterIndex) {
    vector<vector<double>> clusterPoints;
    for (int i = 0; i < data.size(); i++) {
        if (assignments[i] == clusterIndex) {
            clusterPoints.push_back(data[i]);
        }
    }
    return clusterPoints;
}

int main() {
    string csvFile = "ass10_data.csv";  // Your CSV file
    vector<vector<double>> dataPoints;
    ifstream file(csvFile);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    string line;
    getline(file, line); // Skip header line

    // Read the CSV file assuming 2D data (Weight, Height)
    while (getline(file, line)) {
        vector<double> dataPoint;
        size_t start = 0, end;
        while ((end = line.find(",", start)) != string::npos) {
            dataPoint.push_back(stod(line.substr(start, end - start)));
            start = end + 1;
        }
        dataPoint.push_back(stod(line.substr(start))); // Last value (Height)
        dataPoints.push_back(dataPoint);
    }
    file.close();

    int k;
    cout << "Enter the number of clusters (K): ";
    cin >> k;

   
    vector<vector<double>> centroids(k, vector<double>(dataPoints[0].size()));
    for (int i = 0; i < k; i++) {
        cout << "Enter initial centroid " << (i + 1) << " (Weight and Height): ";
        for (int j = 0; j < dataPoints[0].size(); j++) {
            cin >> centroids[i][j];
        }
        cout << "Centroid " << (i + 1) << ": ";  // Debugging print statement
        for (double value : centroids[i]) {
            cout << value << " ";
        }
        cout << endl;
    }

    vector<int> clusterAssignments(dataPoints.size());
    bool centroidsChanged = true;
    int iterations = 0;

    while (centroidsChanged && iterations < 100) {
        iterations++;
        centroidsChanged = false;

        // Assign points to the nearest centroid
        for (int i = 0; i < dataPoints.size(); i++) {
            int nearestCentroidIndex = findNearestCentroid(dataPoints[i], centroids);
            clusterAssignments[i] = nearestCentroidIndex;
        }

        // Recalculate centroids
        for (int i = 0; i < k; i++) {
            vector<vector<double>> clusterPoints = getClusterPoints(dataPoints, clusterAssignments, i);
            if (!clusterPoints.empty()) {
                vector<double> newCentroid = calculateMean(clusterPoints);
                if (calculateDistance(centroids[i], newCentroid) > 1e-4) {
                    centroids[i] = newCentroid;
                    centroidsChanged = true;
                }
            }
        }

        // Output the current centroids
        cout << "Iteration " << iterations << ": Centroids = " << endl;
        for (const auto& centroid : centroids) {
            for (double value : centroid) {
                cout << value << " ";
            }
            cout << endl;
        }
    }

    // Final cluster assignments
    cout << "Final Cluster Assignments:" << endl;
    for (int i = 0; i < dataPoints.size(); i++) {
        cout << "Point (Weight: " << dataPoints[i][0] << ", Height: " << dataPoints[i][1] << ") -> Cluster: " << clusterAssignments[i] << endl;
    }

    return 0;
}


// partition-based clustering in unsupervised learning, focusing on K-Means clustering. This approach divides data into distinct clusters by minimizing the distance between data points and their cluster centers (in K-Means) or representative points (in K-Medoids).

// K-Means Clustering Summary:
// K-Means is an iterative algorithm that groups data into k clusters by assigning each data point to the nearest centroid.
// Steps include:
// Select a value for k.
// Choose k initial points as centroids.
// Compute distances and assign data points to the nearest centroid.
// Recalculate centroids based on current assignments.
// Repeat until convergence (no further changes in cluster assignments)


// colfilter-kmeans
// clusrering
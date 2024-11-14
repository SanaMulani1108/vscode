#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

double singleLinkage(const vector<double>& c1, const vector<double>& c2);
double completeLinkage(const vector<double>& c1, const vector<double>& c2);
double averageLinkage(const vector<double>& c1, const vector<double>& c2);

int main() {
    string file = "ass11_data.csv";
    vector<double> weights;
    int choice;

    cout << "Choose Linkage Method:" << endl;
    cout << "1. Single Linkage" << endl;
    cout << "2. Complete Linkage" << endl;
    cout << "3. Average Linkage" << endl;
    cin >> choice;

    
    ifstream infile(file);
    if (!infile.is_open()) {
        cerr << "Unable to open file" << endl;
        return 1;
    }
    
    string line;
    getline(infile, line); 
    while (getline(infile, line)) {
        string::size_type sz;
        weights.push_back(stod(line, &sz));
    }
    infile.close();

    
    vector<vector<double>> clusters;
    for (double v : weights) {
        clusters.push_back({v});
    }

   
    while (clusters.size() > 1) {
        double minDist = numeric_limits<double>::max();
        int idx1 = -1, idx2 = -1;

        for (size_t i = 0; i < clusters.size(); ++i) {
            for (size_t j = i + 1; j < clusters.size(); ++j) {
                double dist = 0;

                switch (choice) {
                    case 1:
                        dist = singleLinkage(clusters[i], clusters[j]);
                        break;
                    case 2:
                        dist = completeLinkage(clusters[i], clusters[j]);
                        break;
                    case 3:
                        dist = averageLinkage(clusters[i], clusters[j]);
                        break;
                    default:
                        cout << "Invalid choice. Exiting." << endl;
                        return 1;
                }

                if (dist < minDist) {
                    minDist = dist;
                    idx1 = i;
                    idx2 = j;
                }
            }
        }

        cout << "Merging clusters with minimum distance: " << minDist << endl;
        clusters[idx1].insert(clusters[idx1].end(), clusters[idx2].begin(), clusters[idx2].end());
        clusters.erase(clusters.begin() + idx2);

        
        cout << "Merged clusters: " << endl;
        for (const auto& cluster : clusters) {
            for (double v : cluster) {
                cout << v << " ";
            }
            cout << endl;
        }
    }


    cout << "Final Cluster: " << endl;
    for (double v : clusters[0]) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}

double singleLinkage(const vector<double>& c1, const vector<double>& c2) {
    double minDist = numeric_limits<double>::max();
    for (double v1 : c1) {
        for (double v2 : c2) {
            double dist = fabs(v1 - v2);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

double completeLinkage(const vector<double>& c1, const vector<double>& c2) {
    double maxDist = numeric_limits<double>::min();
    for (double v1 : c1) {
        for (double v2 : c2) {
            double dist = fabs(v1 - v2);
            if (dist > maxDist) {
                maxDist = dist;
            }
        }
    }
    return maxDist;
}

double averageLinkage(const vector<double>& c1, const vector<double>& c2) {
    double totalDist = 0.0;
    int count = 0;
    for (double v1 : c1) {
        for (double v2 : c2) {
            totalDist += fabs(v1 - v2);
            count++;
        }
    }
    return totalDist / count;
}



// Hierarchical clustering is an unsupervised learning method that organizes data into a nested hierarchy of clusters. It has two main types:

// Agglomerative (Bottom-up) Clustering: Starts with each data point as its own cluster, then merges clusters based on similarity until all belong to one cluster.
// Divisive (Top-down) Clustering: Begins with a single cluster containing all data points and splits into smaller clusters based on certain criteria until each data point is its own cluster.
// Linkage Methods in hierarchical clustering:

// Single Linkage: Minimum distance between any two points in different clusters.
// Complete Linkage: Maximum distance between any two points in different clusters.
// Average Linkage: Average distance between all points in different clusters.
// Conclusion
// Hierarchical clustering creates a hierarchy of clusters, allowing for flexible data analysis. It is especially useful when the number of clusters is unknown.


// colfil-HierarchicalClustering
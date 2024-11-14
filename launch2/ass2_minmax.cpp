#include <bits/stdc++.h>
using namespace std;

int main()
{
    fstream inputFile("ass2_dataset.csv", ios::in);

    if (!inputFile.is_open()){
        cout << "Error in opening file";
        return 0;
    }

    string line, w;
    
    vector<double> weight;
    int i = 0;
    while(getline(inputFile,line)){
        if(i==0){
            i++;
            continue;
        }

        stringstream str(line);
        getline(str, w, ',');
        double m =stod(w);
        weight.push_back(m);
    }

    int n = weight.size();

    sort(weight.begin(), weight.end());

    double old_min = weight[0], old_max=weight[n-1];
    double new_min, new_max;

    cout << "Enter New Minimum : ";
    cin >> new_min;

    cout << "Enter New Maximum : ";
    cin >> new_max;

    ofstream fout("min_max_output.csv", ios::out);
    if (!fout.is_open()){
        cout << "Error in creating min-max-normalization file";
        return -1;
    }

    fout << "Values After Min-max Normalization" << endl;
    for (double x : weight){
        double normalized_weight = (((x - old_min) / (old_max - old_min)) * (new_max - new_min)) + new_min;
        fout << normalized_weight << endl;
    }
    fout.close();
    inputFile.close();

    return 1;
}

// Min-max normalization is a technique used to scale data to a specific range, usually between 0 and 1. This method adjusts the values in a dataset so that they fit within this range while preserving the relationships between them. It's useful in data mining and machine learning to prevent certain features from dominating others due to differences in scale.

// The formula for min-max normalization is:
// X′ =(X-minx/maxx- minx)*(new_max−new_min)+new_min
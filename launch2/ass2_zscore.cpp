#include <bits/stdc++.h>
using namespace std;

int main()
{

    fstream file("ass2_zdataset2.csv", ios::in);

    if (!file.is_open())
    {
        cout << "Error in opening file";
        return 0;
    }

    string line, mark;
    vector<double> weight;

    int i = 0;
    while (getline(file, line))
    {

        if (i == 0)
        {
            i++;
            continue;
        }

        stringstream str(line);
        getline(str, mark, ',');
        double mk = stod(mark);
        weight.push_back(mk);
    }

    int n = weight.size();

    sort(weight.begin(), weight.end());

    file.close();

    double sum = 0;
    for (auto mk : weight)
    {
        sum += mk;
    }

    double mean = sum / n;

    double dev_sq = 0;
    for (auto wt : weight)
    {
        dev_sq += (wt - mean) * (wt - mean);
    }

    double std_dev = sqrt(dev_sq / n);

    ofstream fw2("zscore.csv", ios::out);

    if (!fw2.is_open())
    {
        cout << "Error in creating Z-score Normalized data file.";
        return -1;
    }

    fw2 << "Z-Score Normalized Weight " << endl;

    for (auto wt : weight)
    {
        double z_score = (wt - mean) / std_dev;

        fw2 <<z_score << endl;
    }

    fw2.close();
    return 0;
}

// Z-score normalization (also called standardization) is a technique used to rescale data by centering it around the mean and adjusting it based on the standard deviation. This method transforms data into a distribution with a mean of 0 and a standard deviation of 1, which is useful for comparing data points that come from different distributions or scales. Z-score normalization is widely used in data mining and machine learning, especially when the algorithm assumes normally distributed data.

// The formula for Z-score normalization is:
// Z= X−μ / σ


// X is the original value,
// μ is the mean of the data,
// σ is the standard deviation of the data
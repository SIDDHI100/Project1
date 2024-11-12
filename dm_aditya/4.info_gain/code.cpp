#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream file("exp3_input.csv");
    ofstream outputFile("output.txt");
    string line, word;
    string day, level, Routine, playGame, value;
    map<string, int> parent;
    map<string, map<string, int>> child;

    if (!file.is_open())
    {
        perror("Error in opening input file: ");
        return -1;
    }

    int i = 0;
    string childName;
    while (getline(file, line))
    {
        stringstream str(line);
        getline(str, day, ',');
        getline(str, level, ',');
        getline(str, Routine, ',');
        getline(str, playGame, ',');
        getline(str, value, ',');

        int choice;
        if (i == 0)
        {
            i++;
            cout << "Enter Child Column Number: ";
            cin >> choice;
            outputFile << "Enter Child Column Number: " << choice << "\n";  // Store in file
            continue;
        }

        switch (choice)
        {
        case 1:
            childName = day;
            break;
        case 2:
            childName = level;
            break;
        case 3:
            childName = Routine;
            break;
        case 4:
            childName = value;
            break;
        default:
            childName = Routine;
            break;
        }

        parent[playGame]++;
        child[childName][playGame]++;
    }

    double pos = parent["Yes"], neg = parent["No"];
    double total = pos + neg;

    double parent_entropy = 0;
    if (total > 0) {
        parent_entropy = -((pos / total) * log2(pos / total) +
                           (neg / total) * log2(neg / total));
    }
    outputFile << "Parent Entropy: " << parent_entropy << "\n";

    double child_entropy = 0;
    for (auto p : child)
    {
        string val = p.first;
        double pR = child[val]["Yes"], nR = child[val]["No"];
        double tR = pR + nR;
        if (tR > 0) {
            child_entropy += -((pR + nR) / total) * 
                             ((pR / tR) * log2(pR / tR) + (nR / tR) * log2(nR / tR));
        }
    }
    outputFile << "Child Entropy * Their proportion: " << child_entropy << "\n";
    outputFile << "Info gain: " << parent_entropy - child_entropy << "\n";

    // Close files
    file.close();
    outputFile.close();
    
    return 0;
}

#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main()
{
    fstream file("input.csv", ios::in);
    if (!file.is_open())
    {
        cout << "Couldn't open file" << endl;
        return 0;
    }

    string line, row, col, count;
    int val;
    map<string, map<string, int>> classRowColMap;
    map<string, int> colMap;
    map<string, int> rowMap;

    int i = 0;
    while (getline(file, line))
    {
        stringstream str(line);
        if (i == 0)  // Skip header row
        {
            i++;
            continue;
        }
        
        getline(str, row, ',');
        getline(str, col, ',');
        getline(str, count, ',');
        val = stoi(count);

        classRowColMap[row][col] = val;
        colMap[col] += val;
        rowMap[row] += val;
    }
    file.close();

    // Calculate the total column and row sums
    int colSum = 0, rowSum = 0;
    for (const auto& c : colMap)
        colSum += c.second;
    for (const auto& r : rowMap)
        rowSum += r.second;

    // Open output CSV file for writing results
    ofstream fw("output.csv", ios::out);
    if (!fw.is_open()) {
        cout << "Couldn't open output file" << endl;
        return 0;
    }

    // Write the header row
    fw << "Column\\Row,Bollywood,t-weight,d-weight,Tollywood,t-weight,d-weight,Total,t-weight,d-weight\n";

    // Write each row (Biographical, Horror, etc.)
    for (const auto& r : rowMap)
    {
        row = r.first;
        fw << row << ",";

        for (const auto& c : colMap)
        {
            col = c.first;
            int cellCount = classRowColMap[row][col];
            float t_weight = ((float)cellCount / rowMap[row]) * 100;
            float d_weight = ((float)cellCount / colMap[col]) * 100;

            fw << cellCount << "," << t_weight << "%," << d_weight << "%,";
        }

        // Total for each row
        int rowTotal = rowMap[row];
        float row_t_weight = 100.0;  // Total row proportion is always 100% for itself
        float row_d_weight = ((float)rowTotal / colSum) * 100;
        fw << rowTotal << "," << row_t_weight << "%," << row_d_weight << "%" << endl;
    }

    // Write the Total row
    fw << "Total,";
    for (const auto& c : colMap)
    {
        col = c.first;
        int colTotal = colMap[col];
        float col_t_weight = ((float)colTotal / colSum) * 100;
        float col_d_weight = 100.0;  // Each column total itself is 100%

        fw << colTotal << "," << col_t_weight << "%," << col_d_weight << "%,";
    }

    // Grand total
    fw << colSum << ",100%,100%" << endl;
    fw.close();

    cout << "Results written to exp4_output.csv" << endl;
    return 0;
}

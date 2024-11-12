#include <bits/stdc++.h>
#include <fstream>
using namespace std;
int main()
{
    fstream file("exp4_input.csv", ios::in);
    if (!file.is_open())
    {
        cout << "Couldn't Open file";
        return 0;
    }
    string line, word;
    string col, row, count;
    int val;
    map<string, map<string, int>> classrowcolMap;
    map<string, int> colMap;
    map<string, int> rowMap;
    int i = 0;
    while (getline(file, line))
    {
        stringstream str(line);
        if (i == 0)
        {
            i++;
            continue;
        }
        getline(str, row, ',');
        getline(str, col, ',');
        getline(str, count, ',');
        val = stoi(count);
        // cout << col << " " << row << " "<< stoi(count) << " " <<
        "\n";
        classrowcolMap[row][col] = val;
        colMap[col] += val;
        rowMap[row] += val;
    }
    for (auto r : rowMap)
    {
        for (auto c : colMap)
        {
            cout << r.first << "-" << c.first << ":";
            cout << classrowcolMap[r.first][c.first] << endl;
        }
    }
    for (auto r : rowMap)
    {
        cout << r.first << "->" << r.second << endl;
    }
    for (auto c : colMap)
    {
        cout << c.first << "->" << c.second << endl;
    }
    int colSum = 0, rowSum = 0;
    for (auto c : colMap)
    {
        colSum += c.second;
    }
    cout << "colSum : " << colSum << "\n";
    for (auto r : rowMap)
    {
        rowSum += r.second;
    }
    cout << "rowSum : " << rowSum << "\n";
    ofstream fw("exp4_output.csv", ios::out);
    fw << "Column\\row , ,Bollywood ,, ,Tollywood ,, ,Total,,, " << endl;
    fw << " Count, t - weight, d - weight, Count, t - weight, d - weight, Count, t - weight, d - wei ght " << endl;
                                                                                             for (auto r : rowMap)
    {
        row = r.first;
        fw << row << ",";
        for (auto c : colMap)
        {
            col = c.first;
            fw << classrowcolMap[row][col] << ",";
            fw << ((float)classrowcolMap[row][col] / rowMap[row]) *
                      100
               << "%,";
            fw << ((float)classrowcolMap[row][col] / colMap[col]) *
                      100
               << "%,";
        }
        fw << rowMap[row] << "," << ((float)rowMap[row] / rowMap[row]) * 100 << "%," << ((float)rowMap[row] / (colSum)) * 100
           << "%" << endl;
    }
    fw << "Total ,";
    for (auto c : colMap)
    {
        col = c.first;
        fw << colMap[col] << ",";
        fw << ((float)colMap[col] / colSum) * 100 << "%,";
        fw << ((float)colMap[col] / colMap[col]) * 100 << "%,";
    }
    fw << colSum << "," << "100%, 100%" << endl;
    fw.close();
    return 0;
}
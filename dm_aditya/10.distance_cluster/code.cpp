#include <bits/stdc++.h>
#include <limits>
using namespace std;
float distance(float x1, float y1, int x2, int y2)
{
    // float x2 = float(x2);
    // float y2 = float(y2);
    return sqrt(((float)x2 - x1) * ((float)x2 - x1) + ((float)y2 - y1) * ((float)y2 - y1));
}
int main()
{
    string line;
    int mid_point;
    string point, x, y;
    int i = 0;
    int val1;
    int val2;
    vector<pair<int, int>> v;
    fstream in("cluster_input.csv", ios::in);
    if (!in.is_open())
    {
        cout << "couldn't open file";
        return -1;
    }
    while (getline(in, line))
    {
        stringstream str(line);
        if (i == 0)
        {
            i++;
            continue;
        }
        getline(str, point, ',');
        getline(str, x, ',');
        getline(str, y, ',');
        val1 = stoi(x);
        val2 = stoi(y);
        v.push_back({val1, val2});
    }
    int n = v.size();
    for (int i = 0; i < v.size(); i++)
    {
        int first = v[i].first;
        int second = v[i].second;
        // cout<<first<<" "<<second<<endl;
    }
    int x_sum = 0, y_sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        int first = v[i].first;
        int second = v[i].second;
        x_sum += first;
        y_sum += second;
    }
    float mid_x = (float)x_sum / n;
    float mid_y = (float)y_sum / n;
    cout << "Mid Point: " << "(" << mid_x << "," << mid_y << ")" << endl;
    ofstream out("cluster_output.csv");
    out << " , p1 ,p2 ,p3 ,p4,C";
    out << "\n";
    for (int i = 0; i < v.size(); i++)
    {
        if (i < v.size())
            out << "p" << i + 1 << ",";
        for (int j = 0; j <= i; j++)
        {
            int f_x1 = v[i].first;
            int s_y1 = v[i].second;
            int f_x2 = v[j].first;
            int s_y2 = v[j].second;
            if (f_x1 == f_x2 && s_y1 == s_y2)
            {
                out << "0" << ",";
                break;
            }
            float dis = distance(f_x1, s_y1, f_x2, s_y2);
            out << dis << ",";
        }
        out << "\n";
    }
    out << "C" << ",";
    pair<int, int> p;
    int ans = 0;
    float x_new;
    float y_new;
    float nearer = INT_MAX;
    for (int i = 0; i < v.size(); i++)
    {
        int first = v[i].first;
        int second = v[i].second;
        float d = distance(mid_x, mid_y, first, second);
        cout << "Distance of p" << i + 1 << " from centre: " << d << endl;
        if (nearer > d)
        {
            nearer = d;
            ans = i + 1;
            x_new = first;
            y_new = second;
        }
        out << d << ",";
        if (i == v.size() - 1)
            out << "0" << ",";
    }
    cout << "Nearer Distance: " << nearer << endl;
    cout << "\n";
    cout << "Nearest point from Centre is: " << "p" << ans << endl;
    out << ",";
    out << "\n"; // New Centre
    out << " , p1 ,p2 ,p3 ,p4";
    out << "\n";
    for (int i = 0; i < v.size(); i++)
    {
        if (i < v.size())
            out << "p" << i + 1 << ",";
        for (int j = 0; j <= i; j++)
        {
            int f_x1 = v[i].first;
            int s_y1 = v[i].second;
            int f_x2 = v[j].first;
            int s_y2 = v[j].second;
            if (f_x1 == f_x2 && s_y1 == s_y2)
            {
                out << "0" << ",";
                break;
            }
            float dis = distance(f_x1, s_y1, f_x2, s_y2);
            out << dis << ",";
        }
        out << "\n";
    }
    out << "p" << ans << "(New Center)" << ",";
    for (int i = 0; i < v.size(); i++)
    {
        int first = v[i].first;
        int second = v[i].second;
        float d = distance(x_new, y_new, first, second);
        cout << "Distance of p" << i + 1 << " from " << "p" << ans << ":" << d << endl;
        out << d << ",";
        if (i == v.size() - 1)
            out << "0" << ",";
    }
    return 0;
}

#include <bits/stdc++.h>
#include <map>
using namespace std;

ifstream fin;
double minfre;
vector<set<string>> datatable;
set<string> products;
map<string, int> freq;
double confidence;

// Function to split a string by whitespace
vector<string> wordsof(string str) {
    vector<string> tmpset;
    string tmp = "";
    int i = 0;
    while (str[i]) {
        if (isalnum(str[i])) tmp += str[i];
        else {
            if (!tmp.empty()) tmpset.push_back(tmp);
            tmp = "";
        }
        i++;
    }
    if (!tmp.empty()) tmpset.push_back(tmp);
    return tmpset;
}

// Combine vector elements into a string, excluding the element at 'miss' index
string combine(vector<string> &arr, int miss) {
    string str;
    for (int i = 0; i < arr.size(); i++)
        if (i != miss) str += arr[i] + " ";
    return str.substr(0, str.size() - 1);
}

// Clone a set
set<string> cloneit(set<string> &arr) {
    set<string> dup;
    for (auto &it : arr) dup.insert(it);
    return dup;
}

// Generate candidate itemsets of length k
set<string> apriori_gen(set<string> &sets, int k) {
    set<string> set2;
    for (auto it1 = sets.begin(); it1 != sets.end(); it1++) {
        auto it2 = it1;
        it2++;
        for (; it2 != sets.end(); it2++) {
            vector<string> v1 = wordsof(*it1);
            vector<string> v2 = wordsof(*it2);
            bool alleq = true;
            for (int i = 0; i < k - 1 && alleq; i++)
                if (v1[i] != v2[i]) alleq = false;
            if (alleq) {
                v1.push_back(v2[k - 1]);
                sort(v1.begin(), v1.end());
                set2.insert(combine(v1, -1));
            }
        }
    }
    return set2;
}

// Count occurrences of a vector in datatable
int countOccurrences(vector<string> v) {
    int count = 0;
    for (auto &s : datatable) {
        bool present = true;
        for (auto &x : v) {
            if (s.find(x) == s.end()) {
                present = false;
                break;
            }
        }
        if (present) count++;
    }
    return count;
}

ofstream fw1("exp7_output.csv", ios::out);

// Generate subsets and calculate confidence
void subsets(vector<string> items, vector<string> v1, vector<string> v2, int idx) {
    if (idx == items.size()) {
        if (v1.empty() || v2.empty()) return;
        int count1 = countOccurrences(items); // Total support
        int count2 = countOccurrences(v1); // Support of antecedent
        double conf = (count1 > 0) ? (double(count1) / count2) * 100 : 0;
        if (conf >= confidence) {
            fw1 << "{ ";
            for (auto &s : v1) fw1 << s << " ";
            fw1 << "} -> { ";
            for (auto &s : v2) fw1 << s << " ";
            fw1 << "}, " << conf << "%" << endl;
        }
        return;
    }
    v1.push_back(items[idx]);
    subsets(items, v1, v2, idx + 1);
    v1.pop_back();
    v2.push_back(items[idx]);
    subsets(items, v1, v2, idx + 1);
    v2.pop_back();
}

// Generate association rules
void generateAssociationRules(set<string> freqItems) {
    for (auto &it : freqItems) {
        vector<string> items = wordsof(it);
        subsets(items, {}, {}, 0);
    }
}

int main() {
    fin.open("input.csv", ios::in);
    if (!fin.is_open()) {
        perror("Error in opening file");
        return -1;
    }

    cout << "Enter Support %: ";
    cin >> minfre;
    cout << "Enter Confidence %: ";
    cin >> confidence;

    string str;
    while (getline(fin, str)) {
        vector<string> arr = wordsof(str);
        set<string> tmpset;
        for (auto &item : arr) tmpset.insert(item);
        datatable.push_back(tmpset);
        for (auto &item : tmpset) {
            products.insert(item);
            freq[item]++;
        }
    }
    fin.close();

    cout << "No of transactions: " << datatable.size() << endl;
    minfre = minfre * datatable.size() / 100;
    cout << "Min frequency: " << minfre << endl;

    // Remove infrequent items
    queue<string> q;
    for (auto it = products.begin(); it != products.end(); ) {
        if (freq[*it] < minfre) {
            it = products.erase(it);
        } else {
            ++it;
        }
    }

    // Generate frequent itemsets
    int pass = 1;
    cout << "\nFrequent " << pass++ << " -item set: \n";
    for (auto &item : products) cout << "{" << item << "} " << freq[item] << endl;

    int i = 2;
    set<string> prev = cloneit(products);
    while (i) {
        set<string> cur = apriori_gen(prev, i - 1);
        if (cur.empty()) break;
        for (auto &it : cur) {
            vector<string> arr = wordsof(it);
            int tot = countOccurrences(arr);
            if (tot >= minfre) {
                freq[it] = tot;
            } else {
                cur.erase(it);
            }
        }
        if (cur.empty()) break;
        cout << "\nFrequent " << pass++ << " -item set: \n";
        for (auto &it : cur) cout << "{" << it << "} " << freq[it] << endl;
        prev = cloneit(cur);
        i++;
    }

    // Generate association rules and write to file
    generateAssociationRules(prev);
    fw1.close();
    return 0;
}

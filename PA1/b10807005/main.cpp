#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    if (argc != 3) {
        cout << "insufficient input" << endl;
        return 0;
    }

    fstream fin, fout;
    fin.open(argv[1], fstream::in);
    fout.open(argv[2], ios::out);
    if (!fin.is_open()) {
        cout << "input file error. file does not open" << endl;
        exit(1);
    }
    int a, b, n;
    fin >> n;
    vector<vector<int> > mis;
    vector<int> chord_pair, mps_set;

    mis.resize(n);
    chord_pair.resize(n);
    fill(chord_pair.begin(), chord_pair.end(), -1);
    int i = 0;
    while (i < n) {
        mis[i].resize(n);
        fill(mis[i].begin(), mis[i].end(), 0);
        i++;
    }

    int b_last;
    fin >> a >> b;
    while (!fin.eof() && b != b_last) {
        chord_pair[a] = b;
        chord_pair[b] = a;
        b_last = b;
        fin >> a >> b;
    }

    for (int x = 1; x < n; x++) {
        for (int i = 0; i + x < n; i++) {
            int j = i + x;
            if (chord_pair[i + x] != -1) {
                int k = chord_pair[i + x];

                if (k < i || k > j) {
                    mis[i][j] = mis[i][j - 1];

                } else if (k > i && k < j) {
                    if (mis[i][k - 1] + mis[k + 1][j - 1] + 1 > mis[i][j - 1]) {
                        mis[i][j] = mis[i][k - 1] + mis[k + 1][j - 1] + 1;

                    } else {
                        mis[i][j] = mis[i][j - 1];
                    }

                } else if (k == i) {
                    mis[i][j] = mis[i + 1][j - 1] + 1;
                }

            } else {
                mis[i][j] = mis[i][j - 1];
            }
        }
    }
    {
        int i = 0, j = n - 1;
        stack<int> i_stack, j_stack;
        do {
            while (j > i) {
                int k = chord_pair[j];
                if (k == i) {
                    mps_set.push_back(i);
                    i++;
                    j--;
                } else if (k > i && k < j) {
                    if (mis[i][j - 1] < mis[i][k - 1] + 1 + mis[k + 1][j - 1]) {
                        mps_set.push_back(k);
                        i_stack.push(i);
                        j_stack.push(k - 1);
                        i = k + 1;
                        j--;
                    } else {
                        j--;
                    }

                } else if (k == -1) {
                    j--;
                } else if (k > j) {
                    j--;
                } else if (k < i) {
                    j--;
                }
            }

            if (!i_stack.empty()) {
                i = i_stack.top();
                i_stack.pop();
                j = j_stack.top();
                j_stack.pop();
            }

        } while (!i_stack.empty() || j > i);
    }

    sort(mps_set.begin(), mps_set.end());

    fout << mis[0][n - 1] << endl;

    for (auto i = 0; i < mis[0][n - 1]; i++) {
        fout << mps_set[i] << " " << chord_pair[mps_set[i]] << endl;
    }
}

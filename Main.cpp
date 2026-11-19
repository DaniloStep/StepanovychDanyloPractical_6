#include <iostream>
#include <vector>
#include <algorithm> // для std::is_sorted
#include <stdexcept> // для std::runtime_error
#include <string>    // для std::string

using namespace std;

struct Stats {
    long long cmp = 0, sw = 0, calls = 0;
};

int checkHeap(const vector<int>& a, int n) {
    for (int i = 0; i < n / 2; i++) {
        int l = 2 * i + 1, r = l + 1;
        if (l < n && a[i] < a[l]) return i;
        if (r < n && a[i] < a[r]) return i;
    }
    return -1;
}

void heapify(vector<int>& a, int n, int i, Stats& s) {
    s.calls++;
    int m = i, l = 2 * i + 1, r = l + 1;
    if (l < n) {
        s.cmp++;
        if (a[l] > a[m]) m = l;
    }
    if (r < n) {
        s.cmp++;
        if (a[r] > a[m]) m = r;
    }
    if (m != i) {
        swap(a[i], a[m]);
        s.sw++;
        heapify(a, n, m, s);
    }
}

void buildHeap(vector<int>& a, Stats& s, bool trace = false) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i, s);
        if (trace && n <= 20) {
            cout << "Node: " << i << " | Size: " << n << " | ";
            for (int x : a) cout << x << " ";
            cout << "\n";
        }
    }
}

bool sortedPart(const vector<int>& a, int start) {
    for (int i = start + 1; i < (int)a.size(); i++) {
        if (a[i - 1] > a[i]) return false;
    }
    return true;
}

void heapSort(vector<int>& a, Stats& build, Stats& extract, bool trace = false) {
    buildHeap(a, build, trace);
    if (checkHeap(a, a.size()) != -1) {
        throw runtime_error("Heap construction failed");
    }
    for (int n = (int)a.size() - 1; n > 0; n--) {
        swap(a[0], a[n]);
        extract.sw++;
        heapify(a, n, 0, extract);
        if (checkHeap(a, n) != -1 || !sortedPart(a, n)) {
            throw runtime_error("Sorting validation failed");
        }
    }
}

bool sorted(const vector<int>& a) {
    return is_sorted(a.begin(), a.end());
}

bool sameData(const vector<int>& a, const vector<int>& b) {
    if (a.size() != b.size()) return false;
    long long s1 = 0, s2 = 0, q1 = 0, q2 = 0;
    for (int x : a) {
        s1 += x;
        q1 += 1LL * x * x;
    }
    for (int x : b) {
        s2 += x;
        q2 += 1LL * x * x;
    }
    return s1 == s2 && q1 == q2;
}

void printStats(string name, Stats s) {
    cout << name << ": " << s.cmp << " | " << s.sw << " | " << s.calls << "\n";
}

void testSort(vector<int> a, int id) {
    vector<int> original = a;
    Stats b, e;
    heapSort(a, b, e);
    cout << "Test " << id << ": " << (sorted(a) && sameData(original, a) ? "PASS" : "FAIL") << "\n";
    if (original.size() >= 5) {
        vector<int> h = original;
        Stats s;
        buildHeap(h, s);
        cout << "Heap: " << (checkHeap(h, h.size()) == -1 ? "PASS" : "FAIL") << "\n";
    }
}

void propertyTests() {
    vector<vector<int>> good = {
        {10, 8, 9, 4, 5},
        {20, 15, 18, 7, 10, 12},
        {44, 40, 43, 33, 28, 42, 31}
    };
    vector<vector<int>> bad = {
        {1, 8, 9},
        {10, 15, 8},
        {5, 4, 9, 2, 3}
    };
    for (auto a : good) {
        cout << "Valid heap: " << (checkHeap(a, a.size()) == -1 ? "PASS" : "FAIL") << "\n";
    }
    for (auto a : bad) {
        cout << "Invalid heap: " << (checkHeap(a, a.size()) != -1 ? "PASS" : "FAIL") << "\n";
    }
}

vector<int> generate(int n, int type) {
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        if (type == 0) a[i] = i;
        if (type == 1) a[i] = n - i;
        if (type == 2) a[i] = ((i * 7919LL + 104729) % 100003) - 50001;
        if (type == 3) a[i] = (i * 17) % 5 - 2;
    }
    return a;
}

int main() {
    try {
        vector<int> a = { 37, 16, 42, 25, 20, 44, 23, 33, 18, 40, 28, 21, 43, 15, 31 };
        cout << "INITIAL ARRAY:\n";
        for (int x : a) cout << x << " ";

        cout << "\n\nBUILD TRACE:\n";
        vector<int> original = a;
        Stats b, e;
        heapSort(a, b, e, true);

        cout << "\nMAX HEAP:\n";
        vector<int> h = original;
        Stats hs;
        buildHeap(h, hs);
        for (int x : h) cout << x << " ";

        cout << "\n\nSORTED ARRAY:\n";
        for (int x : a) cout << x << " ";

        cout << "\n\nSTATISTICS:\n";
        printStats("Build", b);
        printStats("Extraction", e);
        printStats("Total", { b.cmp + e.cmp, b.sw + e.sw, b.calls + e.calls });

        cout << "\nFINAL CHECK: " << (sorted(a) && sameData(original, a) ? "PASS" : "FAIL") << "\n\nPROPERTY TESTS:\n";
        propertyTests();

        cout << "\nSORTING TESTS:\n";
        vector<vector<int>> tests = {
            {}, {7}, {1, 2}, {2, 1}, {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1},
            {4, 1, 5, 2, 3}, {3, 1, 3, 2, 1}, {0, -5, 8, -2, 4}, {6, 6, 6, 6},
            {9, -1, 9, 0, -1, 5}, {10, 7, 8, 4, 2, 1, 3}, {12, 5, 18, 2, 9, 1}, {-10, 0, 10, -10, 5, 5}
        };
        for (int i = 0; i < (int)tests.size(); i++) {
            testSort(tests[i], i + 1);
        }

        cout << "\nEXPERIMENTS:\n";
        string names[] = { "Ascending", "Descending", "Mixed", "Repeated" };
        for (int n : {100, 1000, 10000}) {
            for (int t = 0; t < 4; t++) {
                vector<int> v = generate(n, t);
                Stats x, y;
                heapSort(v, x, y);
                cout << n << " " << names[t] << " | " << x.cmp << "/" << x.sw << " | " << x.cmp + y.cmp << "/" << x.sw + y.sw << "\n";
            }
        }
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

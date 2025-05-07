#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Helper: move head step by step and record each cylinder visited
void travel(int start, int end, vector<int>& order, int& movement) {
    int step = (end > start) ? 1 : -1;
    for (int pos = start + step; pos != end + step; pos += step) {
        movement += 1;
        order.push_back(pos);
    }
}

// FCFS
pair<vector<int>, int> fcfs(int head, const vector<int>& reqs) {
    int movement = 0;
    int cur = head;
    vector<int> order;
    for (int r : reqs) {
        travel(cur, r, order, movement);
        cur = r;
    }
    return {order, movement};
}

// SCAN toward 0 then up
pair<vector<int>, int> scan(int head, const vector<int>& reqs, int disk_size) {
    vector<int> left, right;
    for (int r : reqs) {
        if (r <= head) left.push_back(r);
        else           right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int movement = 0;
    int cur = head;
    vector<int> order;

    // go down toward 0
    if (!left.empty()) {
        travel(cur, left.front(), order, movement);
        cur = left.front();
        for (int i = 1; i < (int)left.size(); ++i) {
            travel(cur, left[i], order, movement);
            cur = left[i];
        }
    }
    // reach cylinder 0 if not already
    if (cur != 0) {
        travel(cur, 0, order, movement);
        cur = 0;
    }
    // go up servicing right side
    for (int r : right) {
        travel(cur, r, order, movement);
        cur = r;
    }

    return {order, movement};
}

// C-SCAN toward 0 with wrap to n-1
pair<vector<int>, int> cscan(int head, const vector<int>& reqs, int disk_size) {
    vector<int> left, right;
    for (int r : reqs) {
        if (r <= head) left.push_back(r);
        else           right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int movement = 0;
    int cur = head;
    vector<int> order;

    // go down toward 0
    if (!left.empty()) {
        travel(cur, left.front(), order, movement);
        cur = left.front();
        for (int i = 1; i < (int)left.size(); ++i) {
            travel(cur, left[i], order, movement);
            cur = left[i];
        }
    }
    // to cylinder 0
    if (cur != 0) {
        travel(cur, 0, order, movement);
        cur = 0;
    }
    // wrap from 0 to n-1
    travel(cur, disk_size - 1, order, movement);
    cur = disk_size - 1;

    // then service right side in descending order
    for (int i = (int)right.size() - 1; i >= 0; --i) {
        travel(cur, right[i], order, movement);
        cur = right[i];
    }

    return {order, movement};
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <initial_head_position>\n";
        return 1;
    }
    int head = stoi(argv[1]);
    if (head < 0) {
        cerr << "Initial head position must be non-negative.\n";
        return 1;
    }

    int n;
    cout << "Enter number of cylinders (0 to n-1): ";
    cin >> n;
    if (n <= head) {
        cerr << "Number of cylinders (" << n << ") must be greater than initial head position (" << head << ").\n";
        return 1;
    }

    cout << "Choose algorithm:\n"
         << "  1) FCFS\n"
         << "  2) SCAN (toward 0 then up)\n"
         << "  3) C-SCAN (toward 0, wrap to n-1)\n"
         << "Enter choice [1-3]: ";
    int alg;
    cin >> alg;

    int numReq;
    cout << "Enter number of requests: ";
    cin >> numReq;

    vector<int> reqs(numReq);
    cout << "Enter the requested cylinder numbers:\n";
    for (int i = 0; i < numReq; ++i) {
        cin >> reqs[i];
        if (reqs[i] < 0 || reqs[i] >= n) {
            cerr << "Request " << reqs[i] << " is out of range 0.." << (n-1) << "\n";
            return 1;
        }
    }

    pair<vector<int>, int> result;
    switch (alg) {
        case 1:
            result = fcfs(head, reqs);
            cout << "\nFCFS full path:\n";
            break;
        case 2:
            result = scan(head, reqs, n);
            cout << "\nSCAN full path (toward 0 then up):\n";
            break;
        case 3:
            result = cscan(head, reqs, n);
            cout << "\nC-SCAN full path (toward 0, wrap to n-1):\n";
            break;
        default:
            cerr << "Invalid algorithm choice\n";
            return 1;
    }

    for (int cyl : result.first) {
        cout << cyl << " ";
    }
    cout << "\nTotal head movement = " << result.second << "\n";

    return 0;
}

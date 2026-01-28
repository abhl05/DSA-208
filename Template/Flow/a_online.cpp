#include <bits/stdc++.h>
using namespace std;
#define int long long
const int INF = 1e18;

// BFS for Finding Augmenting Paths
bool bfs(vector<vector<int>> &adj, int s, int t, vector<int> &parent, int n) {
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && adj[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

// Global variables for transferring data between stages
vector<pair<int, int>> course_needs; 
int total_credits_needed = 0;
int P, Q, R;

// Stage 1: Student -> Course Assignment
// Returns max flow
int edmondsKarp1(vector<vector<int>> &adj, int s, int t, int n) {
    vector<vector<int>> rGraph = adj; // Copy adjacency matrix
    vector<vector<int>> flow(n, vector<int>(n, 0));
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(rGraph, s, t, parent, n)) {
        int path_flow = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            if (adj[u][v] > 0) flow[u][v] += path_flow;
            else flow[v][u] -= path_flow;
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }

    // EXTRACT FLOWS CORRECTLY: Iterate over COURSES (Nodes P+1 to P+R)
    course_needs.clear();
    for (int i = 1; i <= R; i++) {
        int course_node = P + i;
        // Check flow from Course Node to Sink (t)
        if (flow[course_node][t] > 0) {
            // Store {Course Index 1..R, Flow Amount}
            course_needs.push_back({i, flow[course_node][t]});
        }
    }
    return max_flow;
}

// Stage 2: Course -> Teacher Assignment
void edmondsKarp2(vector<vector<int>> &adj, int s, int t, int n) {
    vector<vector<int>> rGraph = adj;
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(rGraph, s, t, parent, n)) {
        int path_flow = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }

    // Validation: Can teachers cover the ENTIRE load from Stage 1?
    // And did Stage 1 cover the ENTIRE student requirement?
    // Since we only call EK2 if EK1 was perfect, we just check if EK2 matches total credits.
    if (max_flow == total_credits_needed) 
        cout << "YES " << max_flow << endl;
    else 
        cout << "NO" << endl;
}

void solve() {
    cin >> P >> Q >> R;
    vector<int> creds(P + 1);
    vector<int> teach_limit(Q + 1);
    vector<int> seat_capacity(R + 1);
    
    // Graph 1 Size: Source(1) + Students(P) + Courses(R) + Sink(1)
    int n1 = P + R + 2;
    vector<vector<int>> scGraph(n1, vector<int>(n1, 0));
    int src1 = 0, dest1 = P + R + 1;

    total_credits_needed = 0;
    for (int i = 1; i <= P; i++) {
        cin >> creds[i];
        total_credits_needed += creds[i];
        scGraph[src1][i] = creds[i]; // Source -> Student
    }

    for (int i = 1; i <= Q; i++) cin >> teach_limit[i];

    for (int i = 1; i <= R; i++) {
        cin >> seat_capacity[i];
        scGraph[P + i][dest1] = seat_capacity[i]; // Course -> Sink
    }

    int K; cin >> K;
    for (int i = 0; i < K; i++) {
        int u, v; cin >> u >> v;
        scGraph[u][P + v] = 1; // Student -> Course (Cap 1)
    }

    // Run Stage 1
    int f1 = edmondsKarp1(scGraph, src1, dest1, n1);

    // If students can't even get courses, fail immediately
    if (f1 != total_credits_needed) {
        cout << "NO" << endl;
        return;
    }

    // Stage 2 Construction
    // Nodes: Source(0) | Courses(1..R) | Teachers(R+1..R+Q) | Sink(R+Q+1)
    int n2 = R + Q + 2;
    vector<vector<int>> ctGraph(n2, vector<int>(n2, 0));
    int src2 = 0, dest2 = R + Q + 1;

    // Source -> Course (Capacity = Students assigned in Stage 1)
    for (auto [course_idx, load] : course_needs) {
        ctGraph[src2][course_idx] = load;
    }

    // Teacher -> Sink (Capacity = Teacher Limit)
    for (int i = 1; i <= Q; i++) {
        ctGraph[R + i][dest2] = teach_limit[i];
    }

    // Course -> Teacher (Capacity = INF)
    int M; cin >> M;
    for (int i = 0; i < M; i++) {
        int t, c; cin >> t >> c; // Teacher t, Course c
        // Edge: Course c -> Teacher t
        ctGraph[c][R + t] = INF; 
    }

    // Run Stage 2
    edmondsKarp2(ctGraph, src2, dest2, n2);
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    solve();
    return 0;
}
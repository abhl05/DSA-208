/*
SECTION C: Multiplayer Board Game
Task: Maximize players with vertex and edge capacities = 1 (Node Splitting)
Pattern: Edmonds-Karp with Adjacency List + Capacity Matrix
*/

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define fastIO() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
#define forn(i, n) for(int i = 0; i < (n); i++)

const int INF = 1e18;

void solve() {
    int N, M;
    cin >> N >> M;

    // Node Splitting:
    // Original nodes 1..N
    // Split into:
    // u_in  = u
    // u_out = u + N
    // Total nodes in flow graph = 2*N
    
    int total_nodes = 2 * N;
    int S = 1;          // Start_in
    int T = N + N;      // Finish_out (since we must leave the final cell)

    vector<vector<int>> adj(total_nodes + 1);
    vector<vector<int>> cap(total_nodes + 1, vector<int>(total_nodes + 1, 0));
    vector<vector<int>> flow(total_nodes + 1, vector<int>(total_nodes + 1, 0));

    // 1. Internal Edges (Vertex Capacities)
    for (int i = 1; i <= N; i++) {
        int u_in = i;
        int u_out = i + N;
        
        int capacity = 1;
        if (i == 1 || i == N) capacity = INF; // Start and Finish are indestructible

        // Edge u_in -> u_out
        adj[u_in].push_back(u_out);
        adj[u_out].push_back(u_in);
        cap[u_in][u_out] = capacity;
    }

    // 2. External Edges (Paths)
    forn(i, M) {
        int u, v;
        cin >> u >> v;
        
        // Path u -> v becomes u_out -> v_in
        int u_out = u + N;
        int v_in = v;

        adj[u_out].push_back(v_in);
        adj[v_in].push_back(u_out);
        cap[u_out][v_in] = 1; // Corridors collapse
    }

    // 3. Edmonds-Karp Max Flow
    int max_flow = 0;
    while (true) {
        vector<int> par(total_nodes + 1, -1);
        queue<int> q;
        q.push(S);
        par[S] = S;

        while (!q.empty() && par[T] == -1) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (par[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                    par[v] = u;
                    q.push(v);
                }
            }
        }

        if (par[T] == -1) break;

        int f = INF;
        for (int v = T; v != S; v = par[v]) {
            int u = par[v];
            f = min(f, cap[u][v] - flow[u][v]);
        }

        for (int v = T; v != S; v = par[v]) {
            int u = par[v];
            flow[u][v] += f;
            flow[v][u] -= f;
        }
        max_flow += f;
    }

    cout << max_flow << endl;
}

signed main() {
    fastIO();
    solve();
    return 0;
}
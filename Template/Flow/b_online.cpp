#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;
using vi = vector<int>;
using vvi = vector<vi>;
using vll = vector<ll>;
using vvll = vector<vll>;

// Edmonds-Karp to compute initial Max Flow
void edmond_karp(const vvi& adj, vvll& flow, const vvll& capacity, int source, int sink, int N) {
    while (true) {
        vi parent(N, -1);
        queue<int> q;
        q.push(source);
        parent[source] = source; // Mark source as visited

        while (!q.empty() && parent[sink] == -1) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                ll rem_capacity = capacity[u][v] - flow[u][v];
                if (parent[v] == -1 && rem_capacity > 0) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (parent[sink] == -1) break; // No augmenting path found

        ll bottleneck = INF;
        for (int node = sink; node != source; node = parent[node]) {
            int u = parent[node];
            bottleneck = min(bottleneck, capacity[u][node] - flow[u][node]);
        }

        for (int node = sink; node != source; node = parent[node]) {
            int u = parent[node];
            flow[u][node] += bottleneck;
            flow[node][u] -= bottleneck;
        }
    }
}

// BFS to find reachability in Residual Graph
// mode 0: Reachable FROM Source (using forward edges)
// mode 1: Can Reach Sink (using backward edges / looking for paths TO sink)
void bfs_check(const vvi& adj, vector<bool>& vis, const vvll &capacity, const vvll &flow, int start_node, int mode) {
    fill(vis.begin(), vis.end(), false);
    vis[start_node] = true;
    queue<int> q;
    q.push(start_node);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (vis[v]) continue;

            ll rem_capacity = 0;
            if (mode == 0) {
                // Forward check: Can we move u -> v?
                rem_capacity = capacity[u][v] - flow[u][v]; 
            } else {
                // Backward check: Can we move v -> u? 
                // (We are searching backwards from Sink, so we look for nodes v that can push flow to u)
                rem_capacity = capacity[v][u] - flow[v][u];
            }

            if (rem_capacity > 0) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vvi adj(N);
    vvll flow(N, vll(N, 0));
    vvll capacity(N, vll(N, 0));

    for (int i = 0; i < M; i++) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        --u, --v; // 0-based indexing
        
        adj[u].push_back(v);
        adj[v].push_back(u); // Add reverse edge for residual graph traversal
        
        capacity[u][v] = cap; // Directed edge capacity
    }

    int source = 0;
    int sink = N - 1; 

    // 1. Run Max Flow on the initial graph
    edmond_karp(adj, flow, capacity, source, sink, N);

    // 2. Precompute reachability arrays
    vector<bool> source_reachable(N, false);
    vector<bool> sink_reachable(N, false);

    // Find all nodes u where S -> ... -> u is possible in residual graph
    bfs_check(adj, source_reachable, capacity, flow, source, 0);
    
    // Find all nodes v where v -> ... -> T is possible in residual graph
    // We do this by searching backwards from T
    bfs_check(adj, sink_reachable, capacity, flow, sink, 1);

    int P;
    cin >> P;
    vector<int> result_indices;

    for (int i = 1; i <= P; i++) {
        int u, v;
        ll c;
        cin >> u >> v >> c;
        --u, --v;
        
        // Condition: If we can reach u from S, AND we can reach T from v,
        // then adding edge u->v creates an augmenting path S->u->v->T.
        if (source_reachable[u] && sink_reachable[v]) {
            result_indices.push_back(i);
        }
    }

    // 3. Handle Output formatting
    if (result_indices.empty()) {
        cout << "None" << endl;
    } else {
        for (size_t i = 0; i < result_indices.size(); i++) {
            cout << result_indices[i] << (i == result_indices.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}
#include <bits/stdc++.h>
using namespace std;
#define int long long
typedef long long ll;
#define vll vector<ll>
#define vllp vector<pair<ll, ll>>
#define endl '\n'
#define fastIO() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
#define vin(x) for (auto &i : x) cin >> i;
#define vout(x) for (auto &i : x) cout << i << " ";
#define all(x) x.begin(), x.end()
#define forn(i, n) for(int i = 0; i < (n); i++)
const int maxn = 1e7 + 43;
const int MOD = 1e9 + 7;
const ll INF = 1e18;
int arr[maxn];
vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool bfs(vector<vector<int>> &adj, int s, int t, vector<int> &parent, int n) {
    vector<bool> visited(n, false);

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            // If v is not visited and there is available capacity (rGraph[u][v] > 0)
            if (!visited[v] && adj[u][v] > 0) {
                // If we found the sink, we are done
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                // Otherwise, mark visited and push to queue
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // We didn't reach the sink
    return false;
}

// Returns the maximum flow from s to t in the given graph
void edmondsKarp(vector<vector<int>> &adj, vector<tuple<int, int, int>> &edges, int s, int t, int n) {
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in the residual graph.
    // rGraph[i][j] indicates residual capacity of edge i-j
    vector<vector<int>> rGraph(n, vector<int>(n));
    for (u = 0; u < n; u++) {
        for (v = 0; v < n; v++) {
            rGraph[u][v] = adj[u][v];
        }
    }

    vector<vector<int>> flow(n, vector<int>(n, 0)); // To store the flow values

    vector<int> parent(n);  // This array is filled by BFS and to store path
    int max_flow = 0;  // There is no flow initially

    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent, n)) {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS.
        int path_flow = INF;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        
        // update residual capacities of the edges and reverse edges
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            if(adj[u][v] > 0) {
                flow[u][v] += path_flow; // Forward edge
            } else {
                flow[v][u] -= path_flow; // Backward edge
            }
            rGraph[u][v] -= path_flow; // Reduce capacity in forward direction
            rGraph[v][u] += path_flow; // Add capacity in reverse direction (residual)
        }
        
        // Add path flow to overall flow
        max_flow += path_flow;
    }
    // cout << "Max Flow: " << max_flow << endl;

    vector <int> s_reachable(n, 0);

    queue<int> q;
    q.push(s);
    s_reachable[s] = 1;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            // If v is not visited and there is available capacity (rGraph[u][v] > 0)
            if (!s_reachable[v] && rGraph[u][v] > 0) {
                // If we found the sink, we are done
                if (v == t) {
                    parent[v] = u;
                }
                // Otherwise, mark visited and push to queue
                q.push(v);
                parent[v] = u;
                s_reachable[v] = 1;
            }
        }
    }
    // cout << "Max Flow: " << max_flow << endl;
    vector <pair<int, int>> min_cut_edges;
    int edge_count = 0;
    for(auto [u, v, w] : edges) {
        if(s_reachable[u] && !s_reachable[v] && flow[u][v] > 0) {
            edge_count++;
            min_cut_edges.push_back({u, v});
        }
    }
    cout << edge_count << endl;
    for(auto [u, v] : min_cut_edges) {
        cout << u << " " << v << endl;
    }
}

void solve() {
    int n, m;
    cin >> n >> m;
    // Create capacity graph
    vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));
    vector<tuple<int, int, int>> edges;

    forn(i, m) {
        int u, v, w;
        cin >> u >> v;
        w = 1;
        edges.push_back({u, v, w});
        edges.push_back({v, u, w});
        capacity[u][v] += w; 
        capacity[v][u] += w; 
    }
    // cout << edges.size() << endl;
    int s = 1; // source
    int t = n; // sink
    edmondsKarp(capacity, edges, s, t, n + 1);
}

signed main() {
    fastIO();
    int tt = 1;
    // cin >> tt;
    while (tt--) {
        solve();
    }
    return 0;
}
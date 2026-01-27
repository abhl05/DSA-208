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

int n, k, m;

bool bfs(vector<vector<int>> &adj, int s, int t, vector<int> &parent, int n) {
    // Create a visited array and mark all vertices as not visited
    vector<bool> visited(n, false);

    // Create a queue, enqueue source vertex and mark source vertex as visited
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
void edmondsKarp(vector<vector<int>> &adj, int s, int t, int n) {
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in the residual graph.
    // rGraph[i][j] indicates residual capacity of edge i-j
    vector<vector<int>> rGraph(n, vector<int>(n));
    for (u = 0; u < n; u++)
        for (v = 0; v < n; v++)
            rGraph[u][v] = adj[u][v];

    vector<int> parent(n);  // This array is filled by BFS and to store path
    int max_flow = 0;  // There is no flow initially

    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent, n)) {
        
        // Find minimum residual capacity of the edges along the
        // path filled by BFS.
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // Add path flow to overall flow
        max_flow += path_flow;

        // update residual capacities of the edges and reverse edges
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow; // Reduce capacity in forward direction
            rGraph[v][u] += path_flow; // Add capacity in reverse direction (residual)
        }
    }

    cout << max_flow << endl;
    // for(int i = 0; i < n; i++) {
    //     for(int j = 0; j < n; j++) {
    //         if(adj[i][j] > rGraph[i][j] && adj[i][j] != 0 && i != s && j != t) { // there was some flow
    //             cout << i - 1 << " " << j - 1 << endl; // flow/capacity
    //         }
    //     }
    // }
    for(int i = 1; i <= k; i++) {  // Officers: 1..k in flow graph
        for(int j = k+1; j <= n-2; j++) {  // Agents: k+1..n-2 in flow graph
            if(adj[i][j] > 0 && adj[i][j] > rGraph[i][j]) {
                // There's flow through this officer-agent edge
                cout << (i-1) << " " << (j-1) << endl;
            }
        }
    }
}

void solve() {
    cin >> n >> k >> m;
    // Create capacity graph
    vector<vector<int>> capacity(n + 2, vector<int>(n + 2, 0));

    int s = 0; // source
    int t = n + 1; // sink
    for(int i = 1; i <= n; i++) {
        if(i <= k) {
            capacity[s][i] = 1; // source to u
        } else {
            capacity[i][t] = 1; // v to sink
        }
    }


    forn(i, m) {
        int u, v;
        cin >> u >> v;
        capacity[u + 1][v + 1] = 1; 
    }
    edmondsKarp(capacity, s, t, n + 2);
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
// #include <bits/stdc++.h>
// using namespace std;
// #define int long long
// typedef long long ll;
// #define vll vector<ll>
// #define vllp vector<pair<ll, ll>>
// #define endl '\n'
// #define fastIO() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
// #define vin(x) for (auto &i : x) cin >> i;
// #define vout(x) for (auto &i : x) cout << i << " ";
// #define all(x) x.begin(), x.end()
// #define forn(i, n) for(int i = 0; i < (n); i++)
// const int maxn = 1e7 + 43;
// const int MOD = 1e9 + 7;
// const ll INF = 1e18;
// int arr[maxn];
// vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// bool bfs(vector<vector<int>> &adj, int s, int t, int parent[], int n) {
//     // Create a visited array and mark all vertices as not visited
//     bool visited[n];
//     fill(visited, visited + n, false);

//     // Create a queue, enqueue source vertex and mark source vertex as visited
//     queue<int> q;
//     q.push(s);
//     visited[s] = true;
//     parent[s] = -1;

//     // Standard BFS Loop
//     while (!q.empty()) {
//         int u = q.front();
//         q.pop();

//         for (int v = 0; v < n; v++) {
//             // If v is not visited and there is available capacity (rGraph[u][v] > 0)
//             if (!visited[v] && adj[u][v] > 0) {
//                 // If we found the sink, we are done
//                 if (v == t) {
//                     parent[v] = u;
//                     return true;
//                 }
//                 // Otherwise, mark visited and push to queue
//                 q.push(v);
//                 parent[v] = u;
//                 visited[v] = true;
//             }
//         }
//     }

//     // We didn't reach the sink
//     return false;
// }

// // Returns the maximum flow from s to t in the given graph
// int fordFulkerson(vector<vector<int>> &adj, int s, int t, int n) {
//     int u, v;

//     // Create a residual graph and fill the residual graph with
//     // given capacities in the original graph as residual capacities
//     // in the residual graph.
//     // rGraph[i][j] indicates residual capacity of edge i-j
//     vector<vector<int>> rGraph(n, vector<int>(n));
//     for (u = 0; u < n; u++)
//         for (v = 0; v < n; v++)
//             rGraph[u][v] = adj[u][v];

//     int parent[n];  // This array is filled by BFS and to store path
//     int max_flow = 0;  // There is no flow initially

//     // Augment the flow while there is a path from source to sink
//     while (bfs(rGraph, s, t, parent, n)) {
        
//         // Find minimum residual capacity of the edges along the
//         // path filled by BFS.
//         int path_flow = INT_MAX;
//         for (v = t; v != s; v = parent[v]) {
//             u = parent[v];
//             path_flow = min(path_flow, rGraph[u][v]);
//         }

//         // Add path flow to overall flow
//         max_flow += path_flow;

//         // update residual capacities of the edges and reverse edges
//         for (v = t; v != s; v = parent[v]) {
//             u = parent[v];
//             rGraph[u][v] -= path_flow; // Reduce capacity in forward direction
//             rGraph[v][u] += path_flow; // Add capacity in reverse direction (residual)
//         }
//     }

//     return max_flow;
// }

// void solve() {
//     int n, m;
//     cin >> n >> m;
//     // Create capacity graph
//     vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));

//     forn(i, m) {
//         int u, v, w;
//         cin >> u >> v >> w;
//         capacity[u][v] += w; 
//     }
//     int s = 1; // source
//     int t = n; // sink
//     int max_flow = fordFulkerson(capacity, s, t, n + 1);
//     cout << max_flow << endl;
// }

// signed main() {
//     fastIO();
//     int tt = 1;
//     // cin >> tt;
//     while (tt--) {
//         solve();
//     }
//     return 0;
// }


// --- dinic's algorithm implementation ---

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// Structure to represent an edge
struct Edge {
    int v;          // Destination vertex
    ll flow;        // Current flow passing through this edge
    ll capacity;    // Maximum capacity of this edge
    int rev;        // Index of the reverse edge in adj[v]
};

// Global variables for the graph
vector<vector<Edge>> adj;
vector<int> level; // Stores level of each node (distance from source)
vector<int> ptr;   // Pointer to the next edge to explore for each node

// Function to add a directed edge with capacity
void add_edge(int u, int v, ll cap) {
    // Forward edge: u -> v with capacity 'cap'
    Edge a = {v, 0, cap, (int)adj[v].size()};
    
    // Backward edge: v -> u with capacity 0 (residual edge)
    // Initially flow is 0, so capacity is 0.
    // As we push flow u->v, we increase capacity v->u.
    Edge b = {u, 0, 0, (int)adj[u].size()}; 
    
    adj[u].push_back(a);
    adj[v].push_back(b);
}

// 1. BFS: Builds the Level Graph
// Returns true if the sink is reachable from the source
bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    
    queue<int> q;
    q.push(s);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (const auto& e : adj[u]) {
            // If there is residual capacity (cap - flow > 0)
            // AND the neighbor hasn't been visited yet
            if (e.capacity - e.flow > 0 && level[e.v] == -1) {
                level[e.v] = level[u] + 1;
                q.push(e.v);
            }
        }
    }
    // Return true if we managed to assign a level to the sink
    return level[t] != -1;
}

// 2. DFS: Pushes flow along valid paths in the Level Graph
// u: current node, t: sink, pushed: max flow we can push from source to u
ll dfs(int u, int t, ll pushed) {
    // If no flow can be pushed, return
    if (pushed == 0) return 0;
    
    // If we reached the sink, we successfully pushed 'pushed' amount
    if (u == t) return pushed;
    
    // Loop through edges starting from ptr[u]
    // ptr[u] avoids re-scanning edges that can't accept more flow
    for (int& cid = ptr[u]; cid < adj[u].size(); ++cid) {
        auto& e = adj[u][cid];
        int tr = e.v;
        
        // We only move to the next level in the level graph
        if (level[u] + 1 != level[tr] || e.capacity - e.flow == 0) continue;
        
        // Recursively try to push flow to the neighbor
        ll tr_pushed = dfs(tr, t, min(pushed, e.capacity - e.flow));
        
        if (tr_pushed == 0) continue; // If neighbor couldn't accept flow, try next edge
        
        // Update flows
        e.flow += tr_pushed;
        adj[tr][e.rev].flow -= tr_pushed;
        
        return tr_pushed;
    }
    
    return 0; // No flow could be pushed from u
}

// Main Dinic's Algorithm Function
ll dinic(int s, int t) {
    ll flow = 0;
    
    // Keep building Level Graphs while the sink is reachable
    while (bfs(s, t)) {
        // Reset edge pointers for the new phase
        fill(ptr.begin(), ptr.end(), 0);
        
        // Push as much flow as possible using DFS
        while (ll pushed = dfs(s, t, INF)) {
            flow += pushed;
        }
    }
    return flow;
}

int main() {
    int n, m; // Nodes, Edges
    // Example usage:
    cin >> n >> m;
    // Resize vectors based on N
    adj.assign(n + 1, vector<Edge>());
    level.resize(n + 1);
    ptr.resize(n + 1);
    // ... read edges using add_edge ...
    for (int i = 0; i < m; i++) {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;
        add_edge(u, v, cap);
    }
    cout << dinic(1, n) << endl;
    
    return 0;
}
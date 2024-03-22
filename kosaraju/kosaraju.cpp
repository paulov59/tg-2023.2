#include <iostream>
#include <vector>
#include <stack>
#include <cstring>
#include <fstream>
using namespace std;

void dfs(int node, vector<int> &visited, vector<int> adj[], stack<int> &stack) {
    visited[node] = 1;
    for (auto neighbor : adj[node]) {
        if (!visited[neighbor])
            dfs(neighbor, visited, adj, stack);
    }
    stack.push(node);
}

void reverse_dfs(int node, vector<int> &visited, vector<int> adjReversed[], vector<int> &component) {
    visited[node] = 1;
    component.push_back(node);
    for (auto neighbor : adjReversed[node]) {
        if (!visited[neighbor])
            reverse_dfs(neighbor, visited, adjReversed, component);
    }
}

void kosaraju(int n, vector<int> adj[], vector<vector<int>> &sccs) {
    vector<int> visited(n, 0);
    stack<int> stack;

    for (int i = 0; i < n; i++) {
        if (!visited[i])
            dfs(i, visited, adj, stack);
    }

    vector<int> adjReversed[n];
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        for (auto j : adj[i])
            adjReversed[j].push_back(i);
    }

    while (!stack.empty()) {
        int node = stack.top();
        stack.pop();
        if (!visited[node]) {
            vector<int> component;
            reverse_dfs(node, visited, adjReversed, component);
            sccs.push_back(component);
        }
    }
}

int main(int argc, char *argv[]) {
    int initial_node;
    bool output = false;
    bool input = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            cout << "Help" << endl;
            cout << "-h: Show help" << endl;
            cout << "-o <file>: Redirect output to file" << endl;
            cout << "-f <file>: Read graph from file" << endl;
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            string output_file = argv[i + 1];
            output = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            string input_file = argv[i + 1];
            input = true;
        }
    }

    if (!input) {
        cout << "No input file specified. Use -f parameter" << endl;
        return 1;
    }

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Couldn't open input file: " << input_file << endl;
        return 1;
    }

    int n, m;
    fin >> n >> m;

    vector<int> adj[n];

    for (int i = 0; i < m; i++) {
        int v1, v2;
        fin >> v1 >> v2;
        v1--; v2--;
        adj[v1].push_back(v2);
    }

    vector<vector<int>> sccs;
    kosaraju(n, adj, sccs);

    if (output_file) {
        ofstream fout(output_file);
        if (!fout) {
            cerr << "Couldn't open output file: " << output_file << endl;
            return 1;
        }
        for (const auto &scc : sccs) {
            int count = 0;
            for (int node : scc) {
                if (count == scc.size() - 1) {
                    fout << node + 1;
                } else {
                    fout << node + 1 << " ";
                }
                count++;
            }
            fout << endl;
        }

        fout.close();
    } else {
        for (const auto &component : sccs) {
            for (int node : component) {
                cout << node+1 << " ";
            }
            cout << endl;
        }

    }
    return 0;
}

#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;

struct Edge {
    int neighbour;
    bool bridge;
};

int n_cities, n_roads, n_cases, capital;
vector<bool> used;
vector<int> tin, fup;
vector<vector<Edge>> graph;
vector<int> component, height;
int timer = 0;
int n_bridges = 0;
vector<vector<int>> parents;
vector<int> difference;


void Read() {
    std::cin >> n_cities >> n_roads;
    std::cin >> capital;
    --capital;

    graph.resize(n_cities, vector<Edge>(1, {0, false}));
    for (int index = 0; index < n_roads; ++index) {
        int from, to;
        std::cin >> from >> to;
        graph[from - 1].push_back({to - 1, false});
        graph[to - 1].push_back({from - 1, false});
    }
}

void DFS(int from, int patron) {
    used[from] = true;
    tin[from] = fup[from] = timer++;
    for (size_t index = 1; index < graph[from].size(); ++index) {
        int to = graph[from][index].neighbour;
        if (to == patron) continue;
        if (used[to]) {
            fup[from] = std::min(fup[from], tin[to]);
        } else {
            DFS(to, from);
            fup[from] = std::min(fup[from], fup[to]);
            if (fup[to] > tin[from]) {
                ++n_bridges;
                graph[from][index].bridge = true;
                for (size_t item = 1; item < graph[to].size(); ++item) {
                    if (graph[to][item].neighbour == from) {
                        graph[to][item].bridge = true;
                    }
                }
            }
        }
    }
}

void BFS() {
    size_t iter, local_iter;
    iter = local_iter = 0;

    vector<int> queue, local_queue;
    queue.reserve(n_bridges + 1);
    local_queue.reserve(n_cities);

    height.resize(n_bridges + 1, 0);
    used.assign(n_cities, false);

    used[capital] = true;
    queue.push_back(capital);

    while (iter < queue.size()) {
        int vert = queue[iter];
        ++iter;
        component[vert] = iter - 1;

        local_queue.push_back(vert);

        while (local_iter < local_queue.size()) {
            int local_vert = local_queue[local_iter];
            ++local_iter;

            for (size_t index = 1; index < graph[local_vert].size(); ++index) {
                int to = graph[local_vert][index].neighbour;
                if (!used[to]) {
                    if (graph[local_vert][index].bridge == true) {
                        queue.push_back(to);
                        parents[queue.size() - 1][0] = component[vert];
                        height[queue.size() - 1] = height[component[vert]] + 1;

                        for (size_t item = 0; item < 16; ++item) {
                            parents[queue.size() - 1][item + 1] =
                                    parents[parents[component[vert]][item]][item];
                        }
                    } else {
                        local_queue.push_back(to);
                        component[to] = iter - 1;
                    }
                    used[to] = true;
                }
            }
        }
    }
}


void Equalizer(int &first, int &second) {
    if (height[first] == height[second]) {
        return;
    }
    if (height[first] < height[second]) {
        std::swap(first, second);
    }
    int num = 16;
    while (num >= 0) {
        if (height[first] >= height[second] + difference[num]) {
            first = parents[first][num];
        } else {
            --num;
        }
    }
}

int LCA(int blue, int yellow) {
    int first = component[blue];
    int second = component[yellow];

    Equalizer(first, second);

    int num = 16;
    while (num > 0) {
        if (parents[first][num] == parents[second][num]) {
            --num;
        } else {
            first = parents[first][num];
            second = parents[second][num];
        }
    }
    while (first != second) {
        first = parents[first].front();
        second = parents[second].front();
    }
    return height[first];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    difference.resize(17, 1);
    for (int item = 0; item < 16; ++item) {
        difference[item + 1] = difference[item] * 2 + 1;
    }

    Read();
    component.resize(n_cities);
    used.resize(n_cities, false);
    tin.resize(n_cities);
    fup.resize(n_cities);

    DFS(0, -1);
    parents.resize(n_bridges + 1, vector<int>(17, 0));
    BFS();

    std::cin >> n_cases;
    for (int index = 0; index < n_cases; ++index) {
        int first, second;
        std::cin >> first >> second;
        std::cout << LCA(first - 1, second - 1) << '\n';
    }
    return 0;
}

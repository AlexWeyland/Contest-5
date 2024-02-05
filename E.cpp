#include <vector>
#include <iostream>
#include <algorithm>

using std::vector;

vector<vector<int>> parents;
vector<int> parent;
vector<int> height;
vector<bool> dead;
vector<int> difference;

void Equalizer(int &first, int &second) {
    if (height[first] == height[second]) {
        return;
    }
    if (height[first] < height[second]) {
        std::swap(first, second);
    }
    int num = 17;
    while (num >= 0) {
        if (height[first] >= height[second] + difference[num]) {
            first = parents[first][num];
        } else {
            --num;
        }
    }
}

int Find_lca(int first, int second) {
    Equalizer(first, second);
    
    int num = 17;
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
    return first;
}

int Find(int guardian) {
    if (dead[guardian] == false) {
        return guardian;
    } else {
        return parent[guardian] = Find(parent[guardian]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    difference.resize(18, 1);
    for (int item = 0; item < 17; ++item) {
        difference[item + 1] = difference[item] * 2 + 1;
    }

    int number;
    std::cin >> number;
    parents.resize(number, vector<int>(18, 0));
    parent.resize(number);
    height.resize(number, 0);
    dead.resize(number, false);
    int new_dino = 1;

    for (int index = 0; index < number; ++index) {
        char command;
        std::cin >> command;

        if (command == '+') {
            int parent_dino;
            std::cin >> parent_dino;
            --parent_dino;

            parents[new_dino][0] = parent_dino;
            for (size_t item = 0; item < 17; ++item) {
                parents[new_dino][item + 1] = parents[parents[parent_dino][item]][item];
            }

            height[new_dino] = height[parent_dino] + 1;
            parent[new_dino] = parent_dino;
            ++new_dino;
        }

        if (command == '-') {
            int dead_dino;
            std::cin >> dead_dino;
            dead[dead_dino - 1] = true;
        }

        if (command == '?') {
            int first, second;
            std::cin >> first >> second;
            int super_guardian = Find_lca(first - 1, second - 1);
            std::cout << Find(super_guardian) + 1 << '\n';
        }
    }
    return 0;
}

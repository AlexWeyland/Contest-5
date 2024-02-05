#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;
using std::string;

vector<int> graph;

int Find(int emp) {
    if (graph[emp] == emp) {
        return emp;
    } else {
        return graph[emp] = Find(graph[emp]);
    }
}

bool Check(int boss, int emp) {
    return Find(boss) != emp && Find(emp) == emp;
}

void Union(int boss, int emp) {
    if (Check(boss, emp)) {
        graph[emp] = boss;
        std::cout << 1;
        std::cout << '\n';
    } else {
        std::cout << 0;
        std::cout << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number;
    int commands;
    std::cin >> number >> commands;

    graph.reserve(number);

    for (int index = 0; index < number; ++index) {
        graph.push_back(index);
    }

    for (int index = 0; index - 1 < commands; ++index) {
        string line;
        std::getline(std::cin, line);
        if (!line.empty()) {
            int space = line.find_first_of(' ');
            if (space == -1) {
                int emp = stoi(line) - 1;
                std::cout << (Find(emp) + 1);
                std::cout << '\n';
            } else {
                int boss = stoi(line.substr(0, space)) - 1;
                int emp = stoi(line.substr(space + 1)) - 1;
                Union(boss, emp);
            }
        }
    }
    return 0;
}

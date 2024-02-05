#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_set>

using std::vector;

struct Edge {
    int left;
    int right;
    int len_of_cover;
    int num_of_cover;
};

struct Request {
    char command;
    int left;
    int right;
};

class Tree {
public:
    vector<Edge> storage;
    int size;

    int Build(const vector<int>& points, int index, int start, int end) {
        int mid = (start + end) / 2;

        if (start == end) {
            storage[index].left = points[start];
            storage[index].right = points[start + 1];
            return (storage[index].right - storage[index].left);
        }

        int answer = Build(points, 2 * index + 1, start, mid) +
                     Build(points, 2 * index + 2, mid + 1, end);

        storage[index].left = storage[2 * index + 1].left;
        storage[index].right = storage[2 * index + 2].right;
        return answer;
    }

    explicit Tree(const vector<int>& points) {
        size = std::pow(2, std::ceil(log2(points.size() - 1)) + 1) - 1;
        storage = vector<Edge>(size);

        Build(points, 0, 0, points.size() - 2);
    }

    void Process(int index) {
        int left_len = 0;
        int right_len = 0;

        if (storage[index].num_of_cover > 0) {
            storage[index].len_of_cover = (storage[index].right - storage[index].left);
            return;
        }

        if (2 * index + 1 < size) {
            left_len = storage[2 * index + 1].len_of_cover;
        }
        if (2 * index + 2  < size)  {
            right_len = storage[2 * index + 2].len_of_cover;
        }
        storage[index].len_of_cover = left_len + right_len;
    }

    int MakeRequest(const Request& request, int index, int start, int end) {
        int mid = (start + end) / 2;
        if (storage[index].left >= request.left &&
                storage[index].right <= request.right) {
            if (request.command == '+') {
                ++storage[index].num_of_cover;
            } else {
                --storage[index].num_of_cover;
            }

            Process(index);
            return storage[index].len_of_cover;
        }

        if (request.right <= storage[index].left ||
            request.left >= storage[index].right) {
            return 0;
        }

        if (2 * index + 1 < size &&
        (storage[2 * index + 1].right - storage[2 * index + 1].left) != 0) {
            MakeRequest(request, 2 * index + 1, start, mid);
        }

        if (2 * index + 2 < size &&
            (storage[2 * index + 2].right - storage[2 * index + 2].left) != 0) {
            MakeRequest(request, 2 * index + 2, mid + 1, end);
        }

        Process(index);
        return storage[index].len_of_cover;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number;
    std::cin >> number;
    vector<Request> requests;
    requests.reserve(number);
    std::unordered_set<int> points_set;
    for (int index = 0; index < number; ++index) {
        char sign;
        int left, right;
        std::cin >> sign >> left >> right;
        requests.push_back({sign, left, right});
        points_set.insert(left);
        points_set.insert(right);
    }
    vector<int> points(points_set.begin(), points_set.end());
    std::sort(points.begin(), points.end());

    Tree tree(points);

    for (auto& command : requests) {
        tree.MakeRequest(command, 0, 0, points.size() - 2);
        std::cout << tree.storage.front().len_of_cover;
        std::cout << '\n';
    }
    return 0;
}

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;
using std::string;

struct Tree {
    Tree* left;
    Tree* right;
    char value;
    int priority;
    int size;
};

void ClearTree(Tree* tree) {
    if (tree->left) {
        ClearTree(tree->left);
    }
    if (tree->right) {
        ClearTree(tree->right);
    }
    delete tree;
}

Tree* Initialize(char symbol) {
    Tree* baby = new Tree;
    baby->left = nullptr;
    baby->right = nullptr;
    baby->value = symbol;
    baby->priority = rand();
    baby->size = 1;
    return baby;
}

int Height(Tree* tree) {
    if (tree == nullptr) {
        return 0;
    }
    return tree->size;
}

void Update(Tree* tree) {
    if (tree == nullptr) {
        return;
    }
    tree->size = Height(tree->left) + Height(tree->right) + 1;
}

void Split(Tree* tree, Tree* &left, Tree* &right, int item) {
    if (tree == nullptr) {
        left = nullptr;
        right = nullptr;
        return;
    }
    if (Height(tree->left) >= item) {
        Split(tree->left, left, tree->left, item);
        right = tree;
        Update(tree);
    } else {
        Split(tree->right, tree->right, right, item - Height(tree->left) - 1);
        left = tree;
        Update(tree);
    }
}

Tree* Merge(Tree* one, Tree* two) {
    if (one == nullptr) {
        return two;
    }
    if (two == nullptr) {
        return one;
    }
    if (one->priority <= two->priority) {
        two->left = Merge(one, two->left);
        Update(two);
        return two;
    } else {
        one->right = Merge(one->right, two);
        Update(one);
        return one;
    }
}

Tree* Create(const string &word) {
    Tree* tree = nullptr;
    for (const auto &symbol : word) {
        tree = Merge(tree, Initialize(symbol));
    }
    return tree;
}

void Output(Tree* tree) {
    if (tree == nullptr) {
        return;
    }
    Output(tree->left);
    std::cout << tree->value;
    Output(tree->right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    string word;
    std::cin >> word;
    Tree* tree = Create(word);
    int number;
    std::cin >> number;
    vector<vector<int>> transpositions;
    transpositions.resize(number, vector<int>(3));

    for (auto &transposition : transpositions) {
        std::cin >> transposition[0];
        --transposition[0];
        std::cin >> transposition[1];
        --transposition[1];
        std::cin >> transposition[2];
        transposition[2] = transposition[2] % (1 + transposition[1] - transposition[0]);
    }

    for (size_t index = number; index > 0; --index) {
        Tree *before, *middle, *after, *one, *two;
        Split(tree, before, middle, transpositions[index - 1][0]);
        Split(middle, middle, after,
              transpositions[index - 1][1] - transpositions[index - 1][0] + 1);
        Split(middle, one, two, transpositions[index - 1][2]);
        tree = Merge(Merge(before, Merge(two, one)), after);
    }
    Output(tree);
    ClearTree(tree);
    return 0;
}

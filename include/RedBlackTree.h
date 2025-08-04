//
// Created by Brenden Chouinard on 8/1/25.
//
#pragma once
#include "StockData.h"
#include <iostream>
#include <string>
using namespace std;

enum Color { RED, BLACK };

template <typename T> class RedBlackTree {
private:
  struct Node {
    T data;
    Color color;
    Node *left;
    Node *right;
    Node *parent;
    // new nodes are always red
    Node(T value)
        : data(value), color(RED), left(nullptr),
          right(nullptr), parent(nullptr) {}
  };

  Node *root;

public:
  RedBlackTree() : root(nullptr) {}

  void insert(const T &item) {
    Node *newNode = new Node(item);
    Node *inserted = insertNode(root, newNode);

    if(inserted != newNode) {
	return;
    }
    root = inserted;

    fixInsert(newNode);
  }

  void printPrefixMatches(const std::string& prefix) {
      int count = 0;
      printPrefixHelper(root, prefix, count);
  }

  void clear() {
      helpClear(root);
      root = nullptr;
  }

private:
    
  void helpClear(Node* node) {
	if(node == nullptr) return;
	helpClear(node->left);
	helpClear(node->right);

	delete node->data;
	delete node;
  }

  void printPrefixHelper(Node* node, const std::string& prefix, int& count) {
      if(node == nullptr || count > 10) return;

      printPrefixHelper(node->left, prefix, count);

      if(count >= 10) return;

      if(node->data->symbol.substr(0, prefix.size()) == prefix) {
	  std::cout << node->data->symbol << " - Return: " << node->data->avg_annual_return
	      << " - Volatility: " << node->data->volatility << std::endl;

	  count++;
      }

      printPrefixHelper(node->right, prefix, count);
  }


  // https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/
  // moves right child to be the parent
  void rotateLeft(Node *&node) {
    if(!node || !node->right) return; // stop dereferencing
    Node *child = node->right;
    node->right = child->left;
    // if subtree exists update the parent
    if (node->right != nullptr)
      node->right->parent = node;
    child->parent = node->parent;

    // if node was the root, then update root
    if (node->parent == nullptr)
      root = child;
    // if not link child's parent to child
    else if (node == node->parent->left)
      node->parent->left = child;
    else
      node->parent->right = child;

    child->left = node;
    node->parent = child;
  }
  // https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/
  // moves left child to be the parent
  void rotateRight(Node *&node) {
    if(!node || !node->left) return;
    Node *child = node->left;
    node->left = child->right;

    if (node->left != nullptr)
      node->left->parent = node;
    child->parent = node->parent;
    // updates the parent's child
    if (node->parent == nullptr)
      root = child;
    else if (node == node->parent->left)
      node->parent->left = child;
    else
      node->parent->right = child;

    child->right = node;
    node->parent = child;
  }

  // https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/
  void fixInsert(Node *node) {
    // fix until we get to the root or the parent is BLACK
    while (node != root && node->parent->color == RED) {
      if (node->parent == node->parent->parent->left) {
        Node *uncle = node->parent->parent->right;

        if (uncle != nullptr && uncle->color == RED) {
          // uncle is red recolor
          node->parent->color = BLACK;
          uncle->color = BLACK;
          node->parent->parent->color = RED;
          node = node->parent->parent;
        } else {
          // right child is node so left rotate
          if (node == node->parent->right) {
            node = node->parent;
            rotateLeft(node);
          }
          // left child is node so right rotate and recolor
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
          rotateRight(node->parent->parent);
        }
      } else {
        // same thing but when the parent is a right child
        Node *uncle = node->parent->parent->left;
        // recolor
        if (uncle != nullptr && uncle->color == RED) {
          node->parent->color = BLACK;
          uncle->color = BLACK;
          node->parent->parent->color = RED;
          node = node->parent->parent;
        } else {
          // right rotation
          if (node == node->parent->left) {
            node = node->parent;
            rotateRight(node);
          }
          // recolor and left rotation
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
          rotateLeft(node->parent->parent);
        }
      }
    }

    root->color = BLACK; // root must be black
  }

  Node *insertNode(Node *root, Node *newNode) {
    // if empty new node goes in
    if (root == nullptr) {
      return newNode;
    }

    // compares alphabetical order
    // goes left if new comes before current
    if (newNode->data->symbol < root->data->symbol) {
      root->left = insertNode(root->left, newNode);
      // sets parent pointer
      root->left->parent = root;
    }
    // goes right if new comes before current
    else if (newNode->data->symbol > root->data->symbol) {
      root->right = insertNode(root->right, newNode);
      // sets parent pointer
      root->right->parent = root;
    }
    // if symbols are equal there is no insert
    else {
	delete newNode;
	return root;
	}

    return root;
  }

  Node *findNode(Node *root, const string &symbol) const {
    if (root == nullptr) {
      return nullptr;
    }
    // checks if we are looking for this
    if (symbol == root->data->symbol) {
      return root;
    }
    // goes left if the one we want comes before this one
    if (symbol < root->data->symbol) {
      return findNode(root->left, symbol);
    }
    // goes right if the one we want comes after this one
    else {
      return findNode(root->right, symbol);
    }
  }

  T *find(const string &symbol) {
    Node *result = findNode(root, symbol);

    if (result != nullptr) {
      return &result->data;
    } else {
      return nullptr;
    }
  }
};

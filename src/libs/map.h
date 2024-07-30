#ifndef MAP_H
#define MAP_H

template<typename Key, typename Value>
struct Pair {
    const Key first;
    Value second;

    Pair(const Key& k, const Value& v) : first(k), second(v) {}
};

template<typename Key, typename Value>
struct Less {
    bool operator()(const Key& lhs, const Key& rhs) const {
        return lhs < rhs;
    }
};

template<typename Key, typename Value, typename Compare = Less<Key, Value>>
class map {
private:
    enum class Color { Red, Black };

    struct Node {
        Pair<Key, Value> data;
        Node* left;
        Node* right;
        Node* parent;
        Color color;

        Node(const Key& key, const Value& value)
            : data(key, value), left(nullptr), right(nullptr), parent(nullptr), color(Color::Red) {}
    };

    Node* root;
    size_t size;
    Compare comp;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;
        x->right = y;
        y->parent = x;
    }

    void insertFixup(Node* z) {
        while (z->parent && z->parent->color == Color::Red) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y && y->color == Color::Red) {
                    z->parent->color = Color::Black;
                    y->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    rotateRight(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y && y->color == Color::Red) {
                    z->parent->color = Color::Black;
                    y->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::Black;
                    z->parent->parent->color = Color::Red;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = Color::Black;
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v) v->parent = u->parent;
    }

    Node* minimum(Node* node) const {
        while (node->left) node = node->left;
        return node;
    }

    void deleteFixup(Node* x) {
        while (x != root && (x == nullptr || x->color == Color::Black)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == Color::Red) {
                    w->color = Color::Black;
                    x->parent->color = Color::Red;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == Color::Black) &&
                    (w->right == nullptr || w->right->color == Color::Black)) {
                    w->color = Color::Red;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == Color::Black) {
                        if (w->left) w->left->color = Color::Black;
                        w->color = Color::Red;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::Black;
                    if (w->right) w->right->color = Color::Black;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == Color::Red) {
                    w->color = Color::Black;
                    x->parent->color = Color::Red;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == Color::Black) &&
                    (w->left == nullptr || w->left->color == Color::Black)) {
                    w->color = Color::Red;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == Color::Black) {
                        if (w->right) w->right->color = Color::Black;
                        w->color = Color::Red;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::Black;
                    if (w->left) w->left->color = Color::Black;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        if (x) x->color = Color::Black;
    }

    Node* findNode(const Key& key) const {
        Node* current = root;
        while (current) {
            if (comp(key, current->data.first)) current = current->left;
            else if (comp(current->data.first, key)) current = current->right;
            else return current;
        }
        return nullptr;
    }

    void clearHelper(Node* node) {
        if (node) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }

public:
    class Iterator {
    private:
        Node* current;
        const map* m;

        Node* successor(Node* x) {
            if (x->right) return m->minimum(x->right);
            Node* y = x->parent;
            while (y && x == y->right) {
                x = y;
                y = y->parent;
            }
            return y;
        }

    public:
        Iterator(Node* node, const map* m) : current(node), m(m) {}

        Pair<Key, Value>& operator*() { return current->data; }
        Pair<Key, Value>* operator->() { return &(current->data); }

        Iterator& operator++() {
            current = successor(current);
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    map() : root(nullptr), size(0) {}

    ~map() { clear(); }

    Iterator begin() const { return Iterator(minimum(root), this); }
    Iterator end() const { return Iterator(nullptr, this); }

    void insert(const Key& key, const Value& value) {
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;

        while (x) {
            y = x;
            if (comp(z->data.first, x->data.first)) x = x->left;
            else if (comp(x->data.first, z->data.first)) x = x->right;
            else {
                delete z;
                return;
            }
        }

        z->parent = y;
        if (!y) root = z;
        else if (comp(z->data.first, y->data.first)) y->left = z;
        else y->right = z;

        insertFixup(z);
        size++;
    }

    void erase(const Key& key) {
        Node* z = findNode(key);
        if (!z) return;

        Node* y = z;
        Node* x;
        Color y_original_color = y->color;

        if (!z->left) {
            x = z->right;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        size--;

        if (y_original_color == Color::Black) {
            deleteFixup(x);
        }
    }

    void clear() {
        clearHelper(root);
        root = nullptr;
        size = 0;
    }

    Value& operator[](const Key& key) {
        Node* node = findNode(key);
        if (node) return node->data.second;
        insert(key, Value());
        return findNode(key)->data.second;
    }

    const Value& at(const Key& key) const {
        Node* node = findNode(key);
        if (node) return node->data.second;
    }

    bool contains(const Key& key) const {
        return findNode(key) != nullptr;
    }

    size_t getSize() const {
        return size;
    }

    bool empty() const {
        return size == 0;
    }
};

#endif // MAP_H

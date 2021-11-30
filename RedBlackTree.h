#pragma once

#include <xtree>

#include "Utility.h"

namespace naive {

// TODO: dependent names

template <typename Key, typename Value>
class TreeNode
{
public:
    using ValueType = Pair<const Key, Value>;

public:
    TreeNode() = default;

    TreeNode(TreeNode* parent, Key&& key, Value&& value) :
        m_parent(parent),
        m_value(std::forward<Key>(key), std::forward<Value>(value))
    {
    }

    template <typename ... Args>
    TreeNode(TreeNode* parent, Args && ... args) :
        m_parent(parent),
        m_value(std::forward<Args>(args)...)
    {
    }

public:
    TreeNode* parent() const
    { return m_parent; }

    TreeNode*& parent()
    { return m_parent; }

    TreeNode* left_child() const
    { return m_left_child; }

    TreeNode*& left_child()
    { return m_left_child; }

    TreeNode* right_child() const
    { return m_right_child; }

    TreeNode*& right_child()
    { return m_right_child; }

    void set_color(bool black)
    { m_black = black; }

    void set_black_color()
    { m_black = true; }

    void set_red_color()
    { m_black = false; }

    bool is_black() const
    { return m_black; }

    bool is_red() const
    { return !m_black; }

    const Key& key() const
    { return m_value.first; }

    const ValueType& value() const
    { return m_value; }

    ValueType& value()
    { return m_value; }

public:
    TreeNode* uncle() const
    {
        const TreeNode* grand_parent = m_parent->m_parent;
        return (grand_parent->m_left_child == m_parent) ? grand_parent->m_right_child
                                                        : grand_parent->m_left_child;
    }

    TreeNode* grandparent() const
    { return m_parent->m_parent; }

    TreeNode* sibling() const
    {
        if (m_parent == nullptr) {
            return nullptr;
        }

        return (m_parent->m_right_child == this) ? m_parent->m_left_child
                                                 : m_parent->m_right_child;
    }

private:
    TreeNode* m_parent = nullptr;
    TreeNode* m_left_child = nullptr;
    TreeNode* m_right_child = nullptr;
    bool m_black = false;

    ValueType m_value;
};

template <typename Key, typename Value>
typename TreeNode<Key, Value>* find_min(TreeNode<Key, Value>* node)
{
    while (node->left_child() != nullptr) {
        node = node->left_child();
    }

    return node;
}

template <typename Key, typename Value>
typename TreeNode<Key, Value>* find_max(TreeNode<Key, Value>* node)
{
    while (node->right_child() != nullptr) {
        node = node->right_child();
    }

    return node;
}


template <typename Key, typename Value>
class BaseIterator
{
public:
    template <typename K, typename V>
    friend class RedBlackTree;

public:
    using RedBlackTree = RedBlackTree<Key, Value>;
    using TreeNode     = TreeNode<Key, Value>;
    using ValueType    = typename TreeNode::ValueType;

public:
    BaseIterator() = default;

    explicit BaseIterator(const RedBlackTree* tree, TreeNode* current) :
        m_tree(tree),
        m_current(current),
        m_end(current == nullptr)
    { }

public:
    const ValueType& operator*() const
    { return m_current->value(); }

    const ValueType* operator->() const
    { return &(m_current->value()); }

    BaseIterator& operator++()
    {
        if (m_current->right_child() != nullptr) {
            m_current = find_min(m_current->right_child());
            return *this;
        }

        while (m_current->parent() != nullptr && m_current == m_current->parent()->right_child()) {
            m_current = m_current->parent();
        }

        if (m_current->parent()) {
            m_current = m_current->parent();
            return *this;
        }

        m_current = nullptr;
        m_end = true;
        return *this;
    }

    BaseIterator& operator--()
    {
        if (m_end) {
            m_current = m_tree->get_last();
            m_end = false;
        }

        if (m_current->left_child() != nullptr) {
            m_current = find_max(m_current->left_child());
            return *this;
        }

        while (m_current->parent() != nullptr && m_current == m_current->parent()->left_child()) {
            m_current = m_current->parent();
        }

        if (m_current->parent() != nullptr) {
            m_current = m_current->parent();
            return *this;
        }

        m_current = nullptr;
        m_end = true;
        return *this;
    }

    bool operator==(const BaseIterator& it)
    { return m_current == it.m_current; }
    bool operator!=(const BaseIterator& it)
    { return !operator==(it); }

protected:
    const RedBlackTree* m_tree = nullptr;
    TreeNode* m_current = nullptr;
    bool      m_end = false;
};

template <typename Key, typename Value>
class Iterator :
    public BaseIterator<Key, Value>
{
public:
    template <typename K, typename V>
    friend class RedBlackTree;

public:
    using RedBlackTree = RedBlackTree<Key, Value>;
    using BaseIterator<Key, Value>::TreeNode;
    using BaseIterator<Key, Value>::ValueType;

public:
    Iterator() = default;

    explicit Iterator(const RedBlackTree* tree, TreeNode* current) :
        BaseIterator<Key, Value>(tree, current)
    { }

public:
    ValueType& operator*()
    { return this->m_current->value(); }

    ValueType* operator->()
    { return &(this->m_current->value()); }

    Iterator& operator++()
    {
        BaseIterator<Key, Value>::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator it = *this;
        BaseIterator<Key, Value>::operator++();
        return it;
    }

    Iterator& operator--()
    {
        BaseIterator<Key, Value>::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator it = *this;
        BaseIterator<Key, Value>::operator--();
        return it;
    }
};

template <typename Key, typename Value>
class ConstIterator :
    public BaseIterator<Key, Value>
{
public:
    template <typename K, typename V>
    friend class RedBlackTree;

    using BaseIterator<Key, Value>::RedBlackTree;
    using BaseIterator<Key, Value>::TreeNode;

public:
    ConstIterator() = default;
    explicit ConstIterator(const RedBlackTree* tree, TreeNode* current) :
        BaseIterator<Key, Value>(tree, current)
    { }
    ConstIterator(const Iterator<Key, Value>& it) :
        BaseIterator<Key, Value>(it)
    { }

public:
    ConstIterator& operator++()
    {
        BaseIterator<Key, Value>::operator++();
        return *this;
    }
    ConstIterator operator++(int)
    {
        ConstIterator it = *this;
        BaseIterator<Key, Value>::operator++();
        return it;
    }

    ConstIterator& operator--()
    {
        BaseIterator<Key, Value>::operator--();
        return *this;
    }
    ConstIterator operator--(int)
    {
        ConstIterator it = *this;
        BaseIterator<Key, Value>::operator--();
        return it;
    }
};

template <typename Key, typename Value>
class ReverseIterator :
    public BaseIterator<Key, Value>
{
public:
    template <typename K, typename V>
    friend class RedBlackTree;

public:
    using RedBlackTree = RedBlackTree<Key, Value>;
    using BaseIterator<Key, Value>::TreeNode;
    using BaseIterator<Key, Value>::ValueType;

public:
    ReverseIterator() = default;

    explicit ReverseIterator(const RedBlackTree* tree, TreeNode* current) :
        BaseIterator<Key, Value>(tree, current)
    { }

public:
    ValueType& operator*()
    { return this->m_current->value(); }

    ValueType* operator->()
    { return &(this->m_current->value()); }

    ReverseIterator& operator++()
    {
        BaseIterator<Key, Value>::operator--();
        return *this;
    }

    ReverseIterator operator++(int)
    {
        ReverseIterator it = *this;
        BaseIterator<Key, Value>::operator--();
        return it;
    }

    ReverseIterator& operator--()
    {
        BaseIterator<Key, Value>::operator++();
        return *this;
    }

    ReverseIterator operator--(int)
    {
        ReverseIterator it = *this;
        BaseIterator<Key, Value>::operator++();
        return it;
    }
};

template <typename Key, typename Value>
class ReverseConstIterator :
    public BaseIterator<Key, Value>
{
public:
    template <typename K, typename V>
    friend class RedBlackTree;

    using BaseIterator<Key, Value>::RedBlackTree;
    using BaseIterator<Key, Value>::TreeNode;

public:
    ReverseConstIterator() = default;
    explicit ReverseConstIterator(const RedBlackTree* tree, TreeNode* current) :
        BaseIterator<Key, Value>(tree, current)
    { }
    ReverseConstIterator(const ReverseIterator<Key, Value>& it) :
        BaseIterator<Key, Value>(it)
    { }

public:
    ReverseConstIterator& operator++()
    {
        BaseIterator<Key, Value>::operator--();
        return *this;
    }
    ReverseConstIterator operator++(int)
    {
        ReverseConstIterator it = *this;
        BaseIterator<Key, Value>::operator--();
        return it;
    }

    ReverseConstIterator& operator--()
    {
        BaseIterator<Key, Value>::operator++();
        return *this;
    }
    ReverseConstIterator operator--(int)
    {
        ReverseConstIterator it = *this;
        BaseIterator<Key, Value>::operator++();
        return it;
    }
};

template <typename Key, typename Value>
class RedBlackTree
{
protected:
    RedBlackTree() = default;

    RedBlackTree(const RedBlackTree& tree) :
        m_root(do_copy(nullptr, tree.m_root)),
        m_size(tree.m_size),
        m_min_node(tree.m_min_node),
        m_max_node(tree.m_max_node)
    { }

    RedBlackTree(RedBlackTree&& tree) noexcept :
        m_root(tree.m_root),
        m_size(tree.m_size),
        m_min_node(tree.m_min_node),
        m_max_node(tree.m_max_node)
    {
        tree.m_root = nullptr;
        tree.m_size = 0;
        tree.m_min_node = nullptr;
        tree.m_max_node = nullptr;
    }

    ~RedBlackTree()
    {
        clear();
    }

    RedBlackTree& operator=(const RedBlackTree& tree)
    {
        clear();
        m_root = do_copy(nullptr, tree.m_root);
        m_size = tree.m_size;
        return *this;
    }

    RedBlackTree& operator=(RedBlackTree&& tree)
    {
        swap(tree);
        return *this;
    }

protected:
    using TreeNode             = TreeNode<Key, Value>;
    using ValueType            = typename TreeNode::ValueType;
    using Iterator             = Iterator<Key, Value>;
    using ConstIterator        = ConstIterator<Key, Value>;
    using ReverseIterator      = ReverseIterator<Key, Value>;
    using ReverseConstIterator = ReverseConstIterator<Key, Value>;

protected:
    Iterator begin()
    { return Iterator(this, m_min_node); }
    Iterator end()
    { return Iterator(this, nullptr); }

    ConstIterator cbegin() const
    { return ConstIterator(this, m_min_node); }
    ConstIterator cend() const
    { return ConstIterator(this, nullptr); }

    ReverseIterator rbegin()
    { return ReverseIterator(this, get_last()); }
    ReverseIterator rend()
    { return ReverseIterator(this, nullptr); }

    ReverseConstIterator rcbegin() const
    { return ReverseIterator(this, get_last()); }
    ReverseConstIterator rcend() const
    { return ReverseIterator(this, nullptr); }

protected:
    bool empty() const
    { return m_size == 0; }
    size_t size() const
    { return m_size; }

protected:
    template <typename ... Args>
    Pair<Iterator, bool> emplace(Args&& ... args)
    {
        auto result = do_emplace(m_root, std::forward<Args>(args)...);
        if (result.second) {
            do_insert_repair(result.first.m_current);
            ++m_size;
        }

        return result;
    }

    template <typename ... Args>
    Pair<Iterator, bool> emplace_hint(ConstIterator hint, Args && ... args)
    {
        TreeNode* node = hint.m_current;
        ValueType value(std::forward<Args>(args)...);

        // Go up the tree while we're in the left  subtree and the key is greather than parent's
        //                   or we're in the right subtree and the key is less     than parent's
        while (node != m_root && (
            (node->parent()->left_child() == node && value.first > node->parent()->value().first) ||
            (node->parent()->right_child() == node && value.first < node->parent()->value().first)))
        {
            node = node->parent();
        }

        auto result = do_emplace(node, std::move(value));
        if (result.second) {
            do_insert_repair(result.first.m_current);
            ++m_size;
        }

        return result;
    }

    Iterator erase(ConstIterator pos)
    {
        TreeNode* node = pos.m_current;
        ++pos;
        do_erase(node);
        return Iterator(this, pos.m_current);
    }

    void swap(RedBlackTree& other) noexcept
    {
        if (&other != this) {
            std::swap(m_root, other.m_root);
            std::swap(m_size, other.m_size);
            std::swap(m_min_node, other.m_min_node);
            std::swap(m_max_node, other.m_max_node);
        }
    }

    size_t count(const Key& key) const
    {
        TreeNode* node = m_root;

        while (node != nullptr) {
            if (key == node->key()) {
                return 1;
            }

            node = (key < node->key()) ? node->left_child()
                                       : node->right_child();
        }

        return 0;
    }

    ConstIterator find(const Key& key) const
    { return ConstIterator(this, do_find(key)); }

    Iterator find(const Key& key)
    { return Iterator(this, do_find(key)); }

    Pair<Iterator, Iterator> equal_range(const Key& key)
    {
        auto result = do_equal_range(key);
        return MakePair(Iterator(this, result.first), Iterator(this, result.second));
    }

    Pair<ConstIterator, ConstIterator> equal_range(const Key& key) const
    {
        auto result = do_equal_range(key);
        return MakePair(ConstIterator(this, result.first), ConstIterator(this, result.second));
    }

    Iterator lower_bound(const Key& key)
    { return Iterator(this, do_lower_bound(key)); }

    ConstIterator lower_bound(const Key& key) const
    { return ConstIterator(this, do_lower_bound(key)); }

    Iterator upper_bound(const Key& key)
    { return Iterator(this, do_upper_bound(key)); }

    ConstIterator upper_bound(const Key& key) const
    { return ConstIterator(this, do_upper_bound(key)); }

    void clear()
    {
        if (m_root != nullptr) {
            do_clear(m_root);
            m_root = nullptr;
        }
        m_size = 0;
        m_min_node = nullptr;
        m_max_node = nullptr;
    }

private:
    template <typename ... Args>
    Pair<Iterator, bool> do_emplace(TreeNode* node, Args && ... args)
    {
        using _In_place_key_extractor = typename std::_In_place_key_extract_map<Key, std::remove_cv_t<std::remove_reference_t<Args>>...>;
        if constexpr (_In_place_key_extractor::_Extractable) {
            const auto& key = _In_place_key_extractor::_Extract(args...);
            return do_emplace_with_key(node, key, std::forward<Args>(args)...);
        }

        ValueType value(std::forward<Args>(args)...);
        const auto& key = value.first;
        return do_emplace_with_key(node, key, std::move(value));
    }

    template <typename ... Args>
    Pair<Iterator, bool> do_emplace_with_key(TreeNode* node, const Key& key, Args && ... args)
    {
        if (m_root == nullptr) {
            m_root = new TreeNode(nullptr, std::forward<Args>(args)...);
            m_min_node = m_root;
            m_max_node = m_root;
            return MakePair(Iterator(this, m_root), true);
        }

        while (true) {
            if (key < node->key()) {
                if (node->left_child() == nullptr) {
                    node->left_child() = new TreeNode(node, std::forward<Args>(args)...);
                    if (key < m_min_node->key()) {
                        m_min_node = node->left_child();
                    }
                    return MakePair(Iterator(this, node->left_child()), true);
                }
                node = node->left_child();
            } else if (key > node->key()) {
                if (node->right_child() == nullptr) {
                    node->right_child() = new TreeNode(node, std::forward<Args>(args)...);
                    if (key > m_max_node->key()) {
                        m_max_node = node->right_child();
                    }
                    return MakePair(Iterator(this, node->right_child()), true);
                }
                node = node->right_child();
            } else {
                break;
            }
        }

        return MakePair(Iterator(this, node), false);
    }

    void do_erase(TreeNode* node)
    {
        if (node == m_min_node) {
            m_min_node = node->right_child() ? node->right_child()
                                             : node->parent();
        }

        if (node == m_max_node) {
            m_max_node = node->left_child() ? node->left_child()
                                            : node->parent();
        }

        node = find_one_non_leaf_child_node(node);

        TreeNode* child_node = node->left_child() ? node->left_child()
                                                  : node->right_child();

        if (node->is_red()) {
            // Case 1: Node is red. Then both its children are leafs
            if (node->parent()->right_child() == node) {
                node->parent()->right_child() = nullptr;
            } else {
                node->parent()->left_child() = nullptr;
            }
        } else if (child_node != nullptr && child_node->is_red()) {
            // Case 2. Node is black and its child is red
            if (node->parent() != nullptr) {
                if (node->parent()->right_child() == node) {
                    node->parent()->right_child() = child_node;
                } else {
                    node->parent()->left_child() = child_node;
                }
            } else {
                m_root = child_node;
            }

            child_node->parent() = node->parent();
            child_node->set_black_color();
        } else {
            // Case 3. Node is black and its both children are black. Because of RB trees properties they are leafs.

            // First delete the node
            TreeNode* parent = node->parent();
            TreeNode* sibling = node->sibling();
            if (parent != nullptr) {
                if (parent->right_child() == node) {
                    parent->right_child() = child_node;
                } else {
                    parent->left_child() = child_node;
                }
            } else {
                m_root = nullptr;
            }

            // Now fix the tree
            do_remove_double_black_repair(child_node, parent, sibling);
        }

        --m_size;
        delete node;
    }

    TreeNode* do_find(const Key& key) const
    {
        TreeNode* node = m_root;
        while (node != nullptr) {
            if (key == node->key()) {
                return node;
            }

            node = (key < node->key()) ? node->left_child()
                                       : node->right_child();
        }

        return nullptr;
    }

    Pair<TreeNode*, TreeNode*> do_equal_range(const Key& key) const
    {
        TreeNode* node = m_root;
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;

        while (node != nullptr) {
            if (key <= node->key()) {
                if (key < node->key()) {
                    second = node;
                }
                first = node;
                node = node->left_child();
            } else {
                node = node->right_child();
            }
        }

        node = (second != nullptr) ? second->left_child()
                                   : m_root;
        while (node != nullptr) {
            if (key < node->key()) {
                second = node;
                node = node->left_child();
            } else {
                node = node->right_child();
            }
        }

        return MakePair(first, second);
    }

    TreeNode* do_lower_bound(const Key& key) const
    {
        TreeNode* node = m_root;
        TreeNode* bound = nullptr;

        while (node != nullptr) {
            if (key <= node->key()) {
                bound = node;
                node = node->left_child();
            } else {
                node = node->right_child();
            }
        }

        return bound;
    }

    TreeNode* do_upper_bound(const Key& key) const
    {
        TreeNode* node = m_root;
        TreeNode* bound = nullptr;

        while (node != nullptr) {
            if (key < node->key()) {
                bound = node;
                node = node->left_child();
            } else {
                node = node->right_child();
            }
        }

        return bound;
    }

    TreeNode* do_copy(TreeNode* parent, TreeNode* source_node)
    {
        TreeNode* node = nullptr;

        if (source_node != nullptr) {
            node = new TreeNode(parent, source_node->value().first, source_node->value().second);
            node->set_color(source_node->is_black());
            node->left_child() = do_copy(node, source_node->left_child());
            node->right_child() = do_copy(node, source_node->right_child());
        }

        return node;
    }

    void do_clear(TreeNode* node)
    {
        if (node->left_child() != nullptr) {
            do_clear(node->left_child());
        }

        if (node->right_child() != nullptr) {
            do_clear(node->right_child());
        }

        delete node;
    }

private:
    void lr_rotate(TreeNode* node)
    {
        rotate_left(node->parent());
        ll_rotate(node->left_child());
    }

    void ll_rotate(TreeNode* node)
    {
        node->parent()->set_black_color();

        TreeNode* grandparent = node->grandparent();
        grandparent->set_red_color();
        rotate_right(grandparent);
    }

    void rl_rotate(TreeNode* node)
    {
        rotate_right(node->parent());
        rr_rotate(node->right_child());
    }

    void rr_rotate(TreeNode* node)
    {
        node->parent()->set_black_color();

        TreeNode* grandparent = node->grandparent();
        grandparent->set_red_color();
        rotate_left(grandparent);
    }

    void rotate_left(TreeNode* node)
    {
        TreeNode* child = node->right_child();

        child->parent() = node->parent();
        if (node->parent() != nullptr) {
            if (node == node->parent()->left_child()) {
                node->parent()->left_child() = child;
            } else {
                node->parent()->right_child() = child;
            }
        } else {
            m_root = child;
        }

        node->right_child() = child->left_child();
        if (node->right_child() != nullptr) {
            node->right_child()->parent() = node;
        }

        child->left_child() = node;
        node->parent() = child;
    }

    void rotate_right(TreeNode* node)
    {
        TreeNode* child = node->left_child();

        child->parent() = node->parent();
        if (node->parent() != nullptr) {
            if (node == node->parent()->left_child()) {
                node->parent()->left_child() = child;
            } else {
                node->parent()->right_child() = child;
            }
        } else {
            m_root = child;
        }

        node->left_child() = child->right_child();
        if (node->left_child() != nullptr) {
            node->left_child()->parent() = node;
        }

        child->right_child() = node;
        node->parent() = child;
    }

    void do_insert_repair(TreeNode* node)
    {
        TreeNode* parent = node->parent();

        // Case 1. Node is root
        if (parent == nullptr) {
            node->set_black_color();
            return;
        }

        // Case 2. Parent is black
        if (parent->is_black()) {
            return;
        }

        TreeNode* uncle = node->uncle();
        TreeNode* grandparent = node->grandparent();

        // Case 3. Parent is red. Uncle is red
        if (uncle != nullptr && uncle->is_red()) {
            parent->set_black_color();
            uncle->set_black_color();
            grandparent->set_red_color();
            do_insert_repair(grandparent);
            return;
        }

        // Case 4. Parent is red. Uncle is black.
        if (parent == grandparent->left_child()) {
            // Left Rotate
            if (node == parent->right_child()) {
                lr_rotate(node);
            } else {
                ll_rotate(node);
            }
        } else {
            // Right Rotate
            if (node == parent->left_child()) {
                rl_rotate(node);
            } else {
                rr_rotate(node);
            }
        }
    }

    TreeNode* find_one_non_leaf_child_node(TreeNode* node)
    {
        if (node->left_child() == nullptr || node->right_child() == nullptr) {
            return node;
        }

        TreeNode* child = find_max(node->left_child());

        // Swap the nodes
        child->right_child() = node->right_child();
        child->right_child()->parent() = child; // right child of node is not null as per condition in the method start
        node->right_child() = nullptr; // right child of max_node_left_subtree is nullptr as it is max element in the subtree

        TreeNode* node_left_child = node->left_child();
        node->left_child() = child->left_child();
        if (node->left_child() != nullptr) {
            node->left_child()->parent() = node;
        }

        TreeNode* child_parent = child->parent();
        if (node->parent() != nullptr) {
            if (node->parent()->left_child() == node) {
                node->parent()->left_child() = child;
            } else {
                node->parent()->right_child() = child;
            }
        } else {
            m_root = child;
        }
        child->parent() = node->parent();

        if (node_left_child == child) {
            child->left_child() = node;
            node->parent() = child;
        } else {
            child->left_child() = node_left_child;
            child->left_child()->parent() = child; // child->left_child() is not nullptr as we went left from node while searching for the child

            node->parent() = child_parent;
            node->parent()->right_child() = node; // We always went right in the left subtree, and it is not the first node
        }

        bool tmp_color = child->is_black();
        child->set_color(node->is_black());
        node->set_color(tmp_color);
        return node;
    }

    void do_remove_double_black_repair(TreeNode* node, TreeNode* parent, TreeNode* sibling)
    {
        // Case 3.1 Node is root, we are done.
        if (parent == nullptr) {
            return;
        }

        // Case 3.2. Sibling is red
        if (sibling->is_red()) {
            parent->set_red_color();
            sibling->set_black_color();
            if (node == parent->left_child()) {
                rotate_left(parent);
                sibling = parent->right_child();
            } else {
                rotate_right(parent);
                sibling = parent->left_child();
            }

            // After rotation node has a new black sibling and a red father
        }

        // Case 3.3. Sibling is black and both sibling children are black
        if ((sibling->left_child() == nullptr || sibling->left_child()->is_black()) &&
            (sibling->right_child() == nullptr || sibling->right_child()->is_black())) {

            if (parent->is_black()) {
                // Case 3.3.1. Parent is black

                // Recolor sibling to red. Now the subtree starting at parent is a valid RB tree, but its black height of every path in it is smaller by one than
                // black height of any other path in the whole tree. So we have to go upper in the tree and fix it again the same way.
                sibling->set_red_color();
                do_remove_double_black_repair(parent, parent->parent(), parent->sibling());
            } else {
                // Case 3.3.2. Parent is red
                parent->set_black_color();
                sibling->set_red_color();
            }

            // In both cases we are done here.
            //
            // In case 3.3.1 the algorithm will go recursively upward the tree, on each step getting a valid RB subtree of the whole tree.
            // It will stop when it either reaches the root or reach a case when imbalance will be fixed by rotation/recoloring
            //
            // In case 3.3.2 recoloring is enough to have the same RB height in the whole tree.
            return;
        }

        // Case 3.4. Node is a left child, sibling is black, its right child is black and left child is red (or symmetrically when node is a right child)
        if (node == parent->left_child() && (sibling->right_child() == nullptr || sibling->right_child()->is_black()) && (sibling->left_child() != nullptr && sibling->left_child()->is_red())) {
            // Recolor and rotate around sibling. Now node has a new black sibling and a configuration which will be handled by case 3.5
            sibling->set_red_color();
            sibling->left_child()->set_black_color();
            rotate_right(sibling);
            sibling = node->sibling();
        } else if (node == parent->right_child() && (sibling->left_child() == nullptr || sibling->left_child()->is_black()) && (sibling->right_child() != nullptr && sibling->right_child()->is_red())) {
            sibling->set_red_color();
            sibling->right_child()->set_black_color();
            rotate_left(sibling);
            sibling = node->sibling();
        }

        // Case 3.5. Here sibling is black and its right child is red (when node is a left  child)
        //                                  or its left  child is red (when node is a right child)

        sibling->set_color(parent->is_black());
        parent->set_black_color();

        if (node == parent->left_child()) {
            sibling->right_child()->set_black_color();
            rotate_left(parent);
        } else {
            sibling->left_child()->set_black_color();
            rotate_right(parent);
        }
        // Now black-height is the same in the whole tree, so we are done
    }

private:
    template <typename K, typename V>
    friend class BaseIterator;

    TreeNode* get_last() const
    { return m_max_node; }

private:
    TreeNode* m_root = nullptr;
    size_t    m_size = 0;

    TreeNode* m_min_node = nullptr;
    TreeNode* m_max_node = nullptr;
};

} /*namespace naive*/
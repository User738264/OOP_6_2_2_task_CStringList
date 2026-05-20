#pragma once

#include <iterator>
#include <string>

class CStringList
{
private:
    struct Node;

public:
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        using pointer = std::string*;
        using reference = std::string&;

        Iterator() = default;

        std::string& operator*() const;
        std::string* operator->() const;

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        explicit Iterator(Node* node);
        Node* m_node = nullptr;

        friend class CStringList;
        friend class ConstIterator;
    };

    class ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::string*;
        using reference = const std::string&;

        ConstIterator() = default;
        ConstIterator(Iterator it);

        reference operator*() const;
        pointer operator->() const;

        ConstIterator& operator++();
        ConstIterator operator++(int);
        ConstIterator& operator--();
        ConstIterator operator--(int);

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        explicit ConstIterator(const Node* node);
        const Node* m_node = nullptr;

        friend class CStringList;
    };

    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    CStringList();
    CStringList(const CStringList& other);
    CStringList(CStringList&& other) noexcept;
    ~CStringList() noexcept;

    CStringList& operator=(const CStringList& other);
    CStringList& operator=(CStringList&& other) noexcept;

    void PushBack(std::string value);
    void PushFront(std::string value);

    void Insert(Iterator pos, std::string value);

    void Erase(Iterator pos);

    void Clear() noexcept;

    size_t GetSize() const noexcept;
    bool IsEmpty() const noexcept;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    ReverseIterator rbegin() noexcept;
    ReverseIterator rend() noexcept;
    ConstReverseIterator rbegin() const noexcept;
    ConstReverseIterator rend() const noexcept;
    ConstReverseIterator crbegin() const noexcept;
    ConstReverseIterator crend() const noexcept;

private:
    struct Node
    {
        std::string value;
        Node* prev = nullptr;
        Node* next = nullptr;

        Node() = default;
        Node(std::string val, Node* p, Node* n)
            : value(std::move(val))
            , prev(p)
            , next(n)
        {
        }
    };

    void InsertBefore(Node* pos, std::string value);

    void DeleteAllNodes() noexcept;

    void Swap(CStringList& other) noexcept;

    Node* m_sentinelHead = nullptr;
    Node* m_sentinelTail = nullptr;
    size_t m_size = 0;
};
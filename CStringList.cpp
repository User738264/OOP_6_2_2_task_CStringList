#include "CStringList.h"

#include <stdexcept>
#include <utility>


CStringList::Iterator::Iterator(Node* node)
    : m_node(node)
{
}

std::string& CStringList::Iterator::operator*() const
{
    return m_node->value;
}

std::string* CStringList::Iterator::operator->() const
{
    return &m_node->value;
}

CStringList::Iterator& CStringList::Iterator::operator++()
{
    m_node = m_node->next;
    return *this;
}

CStringList::Iterator CStringList::Iterator::operator++(int)
{
    auto copy = *this;
    ++(*this);
    return copy;
}

CStringList::Iterator& CStringList::Iterator::operator--()
{
    m_node = m_node->prev;
    return *this;
}

CStringList::Iterator CStringList::Iterator::operator--(int)
{
    auto copy = *this;
    --(*this);
    return copy;
}

bool CStringList::Iterator::operator==(const Iterator& other) const
{
    return m_node == other.m_node;
}

bool CStringList::Iterator::operator!=(const Iterator& other) const
{
    return m_node != other.m_node;
}

CStringList::ConstIterator::ConstIterator(const Node* node)
    : m_node(node)
{
}

CStringList::ConstIterator::ConstIterator(Iterator it)
    : m_node(it.m_node)
{
}

CStringList::ConstIterator::reference CStringList::ConstIterator::operator*() const
{
    return m_node->value;
}

CStringList::ConstIterator::pointer CStringList::ConstIterator::operator->() const
{
    return &m_node->value;
}

CStringList::ConstIterator& CStringList::ConstIterator::operator++()
{
    m_node = m_node->next;
    return *this;
}

CStringList::ConstIterator CStringList::ConstIterator::operator++(int)
{
    auto copy = *this;
    ++(*this);
    return copy;
}

CStringList::ConstIterator& CStringList::ConstIterator::operator--()
{
    m_node = m_node->prev;
    return *this;
}

CStringList::ConstIterator CStringList::ConstIterator::operator--(int)
{
    auto copy = *this;
    --(*this);
    return copy;
}

bool CStringList::ConstIterator::operator==(const ConstIterator& other) const
{
    return m_node == other.m_node;
}

bool CStringList::ConstIterator::operator!=(const ConstIterator& other) const
{
    return m_node != other.m_node;
}

void CStringList::InsertBefore(Node* pos, std::string value)
{
    auto* node = new Node(std::move(value), pos->prev, pos);

    pos->prev->next = node;
    pos->prev = node;
    ++m_size;
}

void CStringList::DeleteAllNodes() noexcept
{
    Node* current = m_sentinelHead->next;
    while (current != m_sentinelTail)
    {
        Node* next = current->next;
        delete current;
        current = next;
    }
    m_sentinelHead->next = m_sentinelTail;
    m_sentinelTail->prev = m_sentinelHead;
    m_size = 0;
}

void CStringList::Swap(CStringList& other) noexcept
{
    std::swap(m_sentinelHead, other.m_sentinelHead);
    std::swap(m_sentinelTail, other.m_sentinelTail);
    std::swap(m_size, other.m_size);
}

CStringList::CStringList()
    : m_sentinelHead(new Node())
    , m_sentinelTail(new Node())
    , m_size(0)
{
    m_sentinelHead->next = m_sentinelTail;
    m_sentinelTail->prev = m_sentinelHead;
}

CStringList::CStringList(const CStringList& other)
    : CStringList()
{
    for (const auto& value : other)
    {
        PushBack(value);
    }
}

CStringList::CStringList(CStringList&& other) noexcept
    : CStringList()
{
    Swap(other);
}

CStringList::~CStringList() noexcept
{
    if (m_sentinelHead == nullptr)
    {
        return;
    }
    DeleteAllNodes();
    delete m_sentinelHead;
    delete m_sentinelTail;
}

CStringList& CStringList::operator=(const CStringList& other)
{
    if (this != &other)
    {
        CStringList temp(other);
        Swap(temp);
    }
    return *this;
}

CStringList& CStringList::operator=(CStringList&& other) noexcept
{
    if (this != &other)
    {
        Swap(other);
    }
    return *this;
}

void CStringList::PushBack(std::string value)
{
    InsertBefore(m_sentinelTail, std::move(value));
}

void CStringList::PushFront(std::string value)
{
    InsertBefore(m_sentinelHead->next, std::move(value));
}

void CStringList::Insert(Iterator pos, std::string value)
{
    InsertBefore(pos.m_node, std::move(value));
}

void CStringList::Erase(Iterator pos)
{
    Node* node = pos.m_node;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    --m_size;
}

void CStringList::Clear() noexcept
{
    DeleteAllNodes();
}

size_t CStringList::GetSize() const noexcept
{
    return m_size;
}

bool CStringList::IsEmpty() const noexcept
{
    return m_size == 0;
}

CStringList::Iterator CStringList::begin() noexcept
{
    return Iterator(m_sentinelHead->next);
}

CStringList::Iterator CStringList::end() noexcept
{
    return Iterator(m_sentinelTail);
}

CStringList::ConstIterator CStringList::begin() const noexcept
{
    return ConstIterator(m_sentinelHead->next);
}

CStringList::ConstIterator CStringList::end() const noexcept
{
    return ConstIterator(m_sentinelTail);
}

CStringList::ConstIterator CStringList::cbegin() const noexcept
{
    return begin();
}

CStringList::ConstIterator CStringList::cend() const noexcept
{
    return end();
}

CStringList::ReverseIterator CStringList::rbegin() noexcept
{
    return ReverseIterator(end());
}

CStringList::ReverseIterator CStringList::rend() noexcept
{
    return ReverseIterator(begin());
}

CStringList::ConstReverseIterator CStringList::rbegin() const noexcept
{
    return ConstReverseIterator(end());
}

CStringList::ConstReverseIterator CStringList::rend() const noexcept
{
    return ConstReverseIterator(begin());
}

CStringList::ConstReverseIterator CStringList::crbegin() const noexcept
{
    return rbegin();
}

CStringList::ConstReverseIterator CStringList::crend() const noexcept
{
    return rend();
}

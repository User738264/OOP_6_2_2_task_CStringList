#include "CStringList.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>


namespace
{

std::vector<std::string> ToVector(const CStringList& list)
{
    return { list.begin(), list.end() };
}

} // namespace

// Construction & destruction

TEST(CStringList_Construction, DefaultConstructor_ListIsEmpty)
{
    const CStringList list;
    EXPECT_TRUE(list.IsEmpty());
    EXPECT_EQ(list.GetSize(), 0u);
    EXPECT_EQ(list.begin(), list.end());
}

TEST(CStringList_Construction, CopyConstructor_ProducesDeepCopy)
{
    CStringList original;
    original.PushBack("alpha");
    original.PushBack("beta");

    const CStringList copy(original);

    EXPECT_EQ(ToVector(copy), ToVector(original));
    EXPECT_EQ(copy.GetSize(), original.GetSize());
}

TEST(CStringList_Construction, CopyConstructor_MutatingCopyDoesNotAffectOriginal)
{
    CStringList original;
    original.PushBack("alpha");

    CStringList copy(original);
    copy.PushBack("beta");

    EXPECT_EQ(original.GetSize(), 1u);
    EXPECT_EQ(copy.GetSize(), 2u);
}

TEST(CStringList_Construction, MoveConstructor_TransfersElements)
{
    CStringList source;
    source.PushBack("one");
    source.PushBack("two");

    const CStringList moved(std::move(source));

    EXPECT_EQ(ToVector(moved), (std::vector<std::string>{ "one", "two" }));
}

TEST(CStringList_Construction, MoveConstructor_SourceBecomesEmpty)
{
    CStringList source;
    source.PushBack("x");

    CStringList moved(std::move(source));

    EXPECT_TRUE(source.IsEmpty());
}

// Assignment

TEST(CStringList_Assignment, CopyAssignment_CopiesElements)
{
    CStringList src;
    src.PushBack("a");
    src.PushBack("b");

    CStringList dst;
    dst.PushBack("old");
    dst = src;

    EXPECT_EQ(ToVector(dst), ToVector(src));
}

TEST(CStringList_Assignment, CopyAssignment_SelfAssignment_IsNoop)
{
    CStringList list;
    list.PushBack("x");

    CStringList& ref = list;
    ref = list;

    EXPECT_EQ(list.GetSize(), 1u);
    EXPECT_EQ(*list.begin(), "x");
}

TEST(CStringList_Assignment, MoveAssignment_TransfersElements)
{
    CStringList src;
    src.PushBack("hello");

    CStringList dst;
    dst = std::move(src);

    EXPECT_EQ(dst.GetSize(), 1u);
    EXPECT_EQ(*dst.begin(), "hello");
}

// PushBack / PushFront

TEST(CStringList_Push, PushBack_SingleElement_ElementAccessible)
{
    CStringList list;
    list.PushBack("first");

    EXPECT_EQ(list.GetSize(), 1u);
    EXPECT_EQ(*list.begin(), "first");
}

TEST(CStringList_Push, PushBack_PreservesOrder)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Push, PushFront_PreservesOrder)
{
    CStringList list;
    list.PushFront("c");
    list.PushFront("b");
    list.PushFront("a");

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Push, PushBack_MoveSemantics_ValueIsTransferred)
{
    CStringList list;
    std::string value = "movable";
    list.PushBack(std::move(value));

    EXPECT_EQ(*list.begin(), "movable");
}

TEST(CStringList_Push, PushFront_MoveSemantics_ValueIsTransferred)
{
    CStringList list;
    std::string value = "front";
    list.PushFront(std::move(value));

    EXPECT_EQ(*list.begin(), "front");
}

// Insert

TEST(CStringList_Insert, Insert_AtBegin_ElementBecomesFirst)
{
    CStringList list;
    list.PushBack("b");
    list.PushBack("c");

    list.Insert(list.begin(), "a");

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Insert, Insert_AtEnd_ElementBecomesLast)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");

    list.Insert(list.end(), "c");

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Insert, Insert_InMiddle_InsertedBeforePos)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("c");

    auto it = list.begin();
    ++it; // points to "c"
    list.Insert(it, "b");

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Insert, Insert_IntoEmptyList_SingleElement)
{
    CStringList list;
    list.Insert(list.end(), "only");

    EXPECT_EQ(list.GetSize(), 1u);
    EXPECT_EQ(*list.begin(), "only");
}

TEST(CStringList_Insert, Insert_Move_ValueIsTransferred)
{
    CStringList list;
    std::string value = "moved";
    list.Insert(list.end(), std::move(value));

    EXPECT_EQ(*list.begin(), "moved");
}

// Erase

TEST(CStringList_Erase, Erase_FirstElement_ListShrinks)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    list.Erase(list.begin());

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "b", "c" }));
}

TEST(CStringList_Erase, Erase_LastElement_ListShrinks)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    auto last = list.end();
    --last;
    list.Erase(last);

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "b" }));
}

TEST(CStringList_Erase, Erase_MiddleElement_NeighboursConnected)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    auto mid = list.begin();
    ++mid;
    list.Erase(mid);

    EXPECT_EQ(ToVector(list), (std::vector<std::string>{ "a", "c" }));
}

TEST(CStringList_Erase, Erase_OnlyElement_ListBecomesEmpty)
{
    CStringList list;
    list.PushBack("sole");

    list.Erase(list.begin());

    EXPECT_TRUE(list.IsEmpty());
}

TEST(CStringList_Erase, Erase_DecrementsSizeByOne)
{
    CStringList list;
    list.PushBack("x");
    list.PushBack("y");

    const size_t sizeBefore = list.GetSize();
    list.Erase(list.begin());

    EXPECT_EQ(list.GetSize(), sizeBefore - 1u);
}

// Clear

TEST(CStringList_Clear, Clear_EmptiesTheList)
{
    CStringList list;
    list.PushBack("x");
    list.PushBack("y");

    list.Clear();

    EXPECT_TRUE(list.IsEmpty());
    EXPECT_EQ(list.GetSize(), 0u);
}

TEST(CStringList_Clear, Clear_OnEmptyList_IsNoop)
{
    CStringList list;
    EXPECT_NO_THROW(list.Clear());
    EXPECT_TRUE(list.IsEmpty());
}

TEST(CStringList_Clear, Clear_ListRemainsUsableAfterwards)
{
    CStringList list;
    list.PushBack("a");
    list.Clear();

    list.PushBack("b");

    EXPECT_EQ(list.GetSize(), 1u);
    EXPECT_EQ(*list.begin(), "b");
}

TEST(CStringList_Clear, Clear_LargeList_NoStackOverflow)
{
    constexpr size_t largeCount = 100'000;
    CStringList list;
    for (size_t i = 0; i < largeCount; ++i)
    {
        list.PushBack("item");
    }

    EXPECT_NO_THROW(list.Clear());
    EXPECT_TRUE(list.IsEmpty());
}

// Destructor

TEST(CStringList_Destructor, Destructor_LargeList_NoStackOverflow)
{
    constexpr size_t largeCount = 100'000;
    {
        CStringList list;
        for (size_t i = 0; i < largeCount; ++i)
        {
            list.PushBack("item");
        }
    }
    SUCCEED();
}

// Iterators — forward

TEST(CStringList_Iterator, ForwardIteration_VisitsAllElements)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    std::vector<std::string> visited;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        visited.push_back(*it);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "a", "b", "c" }));
}

TEST(CStringList_Iterator, PostIncrementReturnsOldPosition)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");

    auto it = list.begin();
    auto old = it++;

    EXPECT_EQ(*old, "a");
    EXPECT_EQ(*it, "b");
}

TEST(CStringList_Iterator, DecrementFromEnd_ReachesLastElement)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");

    auto it = list.end();
    --it;

    EXPECT_EQ(*it, "b");
}

TEST(CStringList_Iterator, Mutation_ThroughIterator_Works)
{
    CStringList list;
    list.PushBack("old");

    *list.begin() = "new";

    EXPECT_EQ(*list.begin(), "new");
}

// Iterators — const

TEST(CStringList_ConstIterator, ConstIteration_VisitsAllElements)
{
    CStringList list;
    list.PushBack("x");
    list.PushBack("y");

    std::vector<std::string> visited;
    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
        visited.push_back(*it);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "x", "y" }));
}

TEST(CStringList_ConstIterator, ImplicitConversionFromIterator_Works)
{
    CStringList list;
    list.PushBack("v");

    CStringList::ConstIterator cit = list.begin();

    EXPECT_EQ(*cit, "v");
}

TEST(CStringList_ConstIterator, ConstList_ProvidesConstIterators)
{
    CStringList list;
    list.PushBack("const");
    const CStringList& ref = list;

    std::vector<std::string> visited;
    for (const auto& value : ref)
    {
        visited.push_back(value);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "const" }));
}

// Iterators — reverse

TEST(CStringList_ReverseIterator, ReverseIteration_VisitsElementsInReverse)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    std::vector<std::string> visited;
    for (auto it = list.rbegin(); it != list.rend(); ++it)
    {
        visited.push_back(*it);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "c", "b", "a" }));
}

TEST(CStringList_ReverseIterator, ConstReverseIteration_Works)
{
    CStringList list;
    list.PushBack("1");
    list.PushBack("2");
    const CStringList& ref = list;

    std::vector<std::string> visited;
    for (auto it = ref.crbegin(); it != ref.crend(); ++it)
    {
        visited.push_back(*it);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "2", "1" }));
}

// STL algorithm compatibility

TEST(CStringList_StlCompat, RangeBasedFor_Works)
{
    CStringList list;
    list.PushBack("hello");
    list.PushBack("world");

    std::vector<std::string> visited;
    for (const auto& value : list)
    {
        visited.push_back(value);
    }

    EXPECT_EQ(visited, (std::vector<std::string>{ "hello", "world" }));
}

TEST(CStringList_StlCompat, StdFind_LocatesExistingElement)
{
    CStringList list;
    list.PushBack("cat");
    list.PushBack("dog");
    list.PushBack("bird");

    const auto it = std::find(list.begin(), list.end(), "dog");

    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, "dog");
}

TEST(CStringList_StlCompat, StdFind_ReturnsEndForMissingElement)
{
    CStringList list;
    list.PushBack("cat");

    const auto it = std::find(list.begin(), list.end(), "fish");

    EXPECT_EQ(it, list.end());
}

TEST(CStringList_StlCompat, StdCopy_FillsDestinationVector)
{
    CStringList list;
    list.PushBack("one");
    list.PushBack("two");

    std::vector<std::string> dest;
    std::copy(list.begin(), list.end(), std::back_inserter(dest));

    EXPECT_EQ(dest, (std::vector<std::string>{ "one", "two" }));
}

TEST(CStringList_StlCompat, StdDistance_ReturnsCorrectLength)
{
    CStringList list;
    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");

    const auto dist = std::distance(list.begin(), list.end());

    EXPECT_EQ(dist, 3);
}

// Exception safety

TEST(CStringList_ExceptionSafety, PushBack_OnFailure_ListUnchanged)
{
    CStringList list;
    list.PushBack("safe");

    try
    {
        list.PushBack(std::string(std::string::npos / 2, 'x'));
    }
    catch (...)
    {
        EXPECT_EQ(list.GetSize(), 1u);
        EXPECT_EQ(*list.begin(), "safe");
    }
}

TEST(CStringList_ExceptionSafety, CopyAssignment_OnFailure_OriginalUnchanged)
{
    CStringList src;
    src.PushBack("original");

    CStringList dst;
    dst.PushBack("keep");

    try
    {
        CStringList bigSrc;
        bigSrc.PushBack(std::string(std::string::npos / 2, 'z'));
        dst = bigSrc;
    }
    catch (...)
    {
        EXPECT_EQ(dst.GetSize(), 1u);
        EXPECT_EQ(*dst.begin(), "keep");
    }
}

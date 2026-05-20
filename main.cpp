#include "CStringList.h"

#include <algorithm>
#include <iostream>

namespace
{

void PrintList(const std::string& label, const CStringList& list)
{
    std::cout << label << " [" << list.GetSize() << "]: ";
    for (const auto& value : list)
    {
        std::cout << '"' << value << "\" ";
    }
    std::cout << "\n";
}

} // namespace

int main()
{
    CStringList list;
    list.PushBack("banana");
    list.PushBack("cherry");
    list.PushFront("apple");
    PrintList("list", list); // apple banana cherry

    std::cout << "Insert / Erase";
    auto it = list.begin();
    ++it; // points to "banana"
    list.Insert(it, "avocado"); // insert before "banana"
    PrintList("after insert", list);// apple avocado banana cherry

    list.Erase(list.begin());       // remove "apple"
    PrintList("after erase ", list);// avocado banana cherry

    CStringList copy = list;
    copy.PushBack("date");
    PrintList("original", list);
    PrintList("copy (+date)", copy);

    CStringList moved = std::move(copy);
    PrintList("moved ", moved);
    std::cout << "copy after move — empty: " << std::boolalpha << copy.IsEmpty() << "\n";

    std::cout << "Reverse iteration";
    std::cout << "reversed:";
    for (auto rit = moved.crbegin(); rit != moved.crend(); ++rit)
    {
        std::cout << '"' << *rit << "\" ";
    }
    std::cout << "\n";

    std::cout << "STL std::find";
    auto found = std::find(moved.begin(), moved.end(), "banana");
    if (found != moved.end())
    {
        std::cout << "found: \"" << *found << "\"\n";
    }

    std::cout << "Clear";
    list.Clear();
    std::cout << "list after Clear — empty: " << list.IsEmpty() << "\n";
    list.PushBack("fresh start");
    PrintList("list after re-push", list);

    return 0;
}

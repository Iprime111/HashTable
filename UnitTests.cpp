#include <LinkedList.hpp>
#include <HashTable.hpp>
#include <cassert>
#include <cstdint>
#include <gtest/gtest.h>

#include "ColorConsole.h"

static const size_t DEFAULT_CAPACITY = 100;

inline uint32_t DefaultHash (int *key) {
    assert (key);

    return (uint32_t) *key;
}

inline HashTableLib::ErrorCode IntPrinter (HashTableLib::Pair <int, int> *pair, FILE *stream) {
    assert (pair);
    assert (stream);
    
    fprintf_color (CONSOLE_BLUE, CONSOLE_BOLD, stream, "%d ", pair->value);

    return HashTableLib::ErrorCode::NO_ERRORS;
}

inline int ListElementComparator (HashTableLib::Pair <int, int> *firstElement, HashTableLib::Pair <int, int> *secondElement) {
    assert (firstElement);
    assert (secondElement);
    
    return firstElement->key - secondElement->key;
}

TEST (InitTest, HashTableTests) {
    HashTableLib::HashTable <int, int, DefaultHash, ListElementComparator> hashTable = {};
    
    EXPECT_EQ (HashTableLib::InitHashTable (&hashTable, DEFAULT_CAPACITY), HashTableLib::ErrorCode::NO_ERRORS);

    for (size_t listIndex = 0; listIndex < hashTable.capacity; listIndex++) {
        EXPECT_EQ (LinkedList::VerifyList_ (&hashTable.table [listIndex]), LinkedList::NO_LIST_ERRORS);
    }
    
    EXPECT_EQ (HashTableLib::DestroyHashTable (&hashTable), HashTableLib::ErrorCode::NO_ERRORS);
}

TEST (FillTest, HashTableTests) {
    HashTableLib::HashTable <int, int, DefaultHash, ListElementComparator> hashTable = {};
    
    EXPECT_EQ (HashTableLib::InitHashTable (&hashTable, DEFAULT_CAPACITY), HashTableLib::ErrorCode::NO_ERRORS);

    int newElement = 0;

    for (size_t elementIndex = 0; elementIndex < hashTable.capacity; elementIndex++) {
        HashTableLib::Pair <int, int> pair = {newElement, newElement};

        EXPECT_EQ (HashTableLib::AddElement (&hashTable, &pair), HashTableLib::ErrorCode::NO_ERRORS);

        int *value = nullptr;
        EXPECT_EQ (HashTableLib::FindElement (&hashTable, &newElement, &value), HashTableLib::ErrorCode::NO_ERRORS);

        EXPECT_NE (value,  nullptr);
        EXPECT_EQ (*value, newElement);

        newElement++;
    }

    int *value = nullptr;

    EXPECT_EQ (HashTableLib::FindElement (&hashTable, &newElement, &value), HashTableLib::ErrorCode::NO_ERRORS);
    EXPECT_EQ (value,nullptr);
    
    EXPECT_EQ (HashTableLib::DestroyHashTable (&hashTable), HashTableLib::ErrorCode::NO_ERRORS);
}

TEST (DumpTest, HashTableTests) {
    HashTableLib::HashTable <int, int, DefaultHash, ListElementComparator> hashTable = {};
    
    EXPECT_EQ (HashTableLib::InitHashTable (&hashTable, DEFAULT_CAPACITY), HashTableLib::ErrorCode::NO_ERRORS);

    int newElement = 0;

    for (size_t elementIndex = 0; elementIndex < hashTable.capacity * 2; elementIndex++) {
        HashTableLib::Pair <int, int> pair = {newElement, newElement};
        EXPECT_EQ (HashTableLib::AddElement (&hashTable, &pair), HashTableLib::ErrorCode::NO_ERRORS);

        newElement += 2;
    }

    HashTableLib::DumpHashTable <int, int, DefaultHash, ListElementComparator, IntPrinter> (&hashTable, stdout);
    
    EXPECT_EQ (HashTableLib::DestroyHashTable (&hashTable), HashTableLib::ErrorCode::NO_ERRORS);

}

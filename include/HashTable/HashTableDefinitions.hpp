#ifndef HASH_TABLE_DEFINITIONS_HPP_
#define HASH_TABLE_DEFINITIONS_HPP_

#include <cstdint>
#include <cstddef>
#include <LinkedList.hpp>

#include "LinkedListDefinitions.hpp"

#define HashTableTemplate \
    template <typename Key, typename Value, HashFunction <Key *> Hash, LinkedList::Comparator <Pair <Key, Value> *> ElementComparator>

namespace HashTableLib {

    const size_t EXPECTED_LOAD_FACTOR = 5;

    enum class ErrorCode {
        NO_ERRORS        = 0,
        ALLOCATION_ERROR = 1 << 0,
        NULL_POINTER     = 1 << 1,
        LIST_ERROR       = 1 << 2,
    };
 
    template <typename Key, typename Value>
    struct Pair {
        Key   key   = {};
        Value value = {};
    };
    
    template <typename KeyPointer>
    using HashFunction = uint32_t (*) (KeyPointer key);

    HashTableTemplate
    struct HashTable {
        size_t capacity = 0;
    
        LinkedList::List <Pair <Key, Value>, ElementComparator> *table = nullptr;
    
        ErrorCode error = ErrorCode::NO_ERRORS;
    };
    
    HashTableTemplate
    ErrorCode AddElement         (HashTable <Key, Value, Hash, ElementComparator> *table, Pair <Key, Value> *newEntry);
    HashTableTemplate
    ErrorCode FindElement        (HashTable <Key, Value, Hash, ElementComparator> *table, Key *elementKey, Value **element);
    HashTableTemplate
    ErrorCode InitHashTable      (HashTable <Key, Value, Hash, ElementComparator> *table, size_t capacity);
    HashTableTemplate
    ErrorCode DestroyHashTable   (HashTable <Key, Value, Hash, ElementComparator> *table);
    HashTableTemplate
    ErrorCode HashTableVerifier  (HashTable <Key, Value, Hash, ElementComparator> *table);
    HashTableTemplate
    inline uint32_t GetListIndex (HashTable <Key, Value, Hash, ElementComparator> *table, Key *elementKey);
}

#undef HashTableTemplate

#endif

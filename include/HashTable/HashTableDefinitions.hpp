#ifndef HASH_TABLE_DEFINITIONS_HPP_
#define HASH_TABLE_DEFINITIONS_HPP_

#include <cstdint>
#include <cstddef>
#include <LinkedList.hpp>

#include "ErrorCode.hpp"
namespace HashTableLib {
    template <typename Key, typename Value>
    struct Pair {
        Key   key   = {};
        Value value = {};
    };
    
    template <typename KeyPointer>
    using HashFunction = uint32_t (*) (KeyPointer key);
    
    template <typename Key, typename Value, HashFunction <Key *> Hash>
    struct HashTable {
        size_t capacity = 0;
    
        LinkedList::List <Pair <Key, Value>> *table = nullptr;
    
        ErrorCode error = ErrorCode::NO_ERRORS;
    };
    
    template <typename Key, typename Value, HashFunction <Key *> Hash>
    ErrorCode AddElement        (HashTable <Key, Value, Hash> *table, Pair <Key, Value> *newEntry);
    
    template <typename Key, typename Value, HashFunction <Key *> Hash>
    ErrorCode FindElement       (HashTable <Key, Value, Hash> *table, Key *elementKey, Value **element);
    
    template <typename Key, typename Value, HashFunction <Key> Hash>
    ErrorCode InitHashTable     (HashTable <Key, Value, Hash> *table, size_t capacity);
    
    template <typename Key, typename Value, HashFunction <Key> Hash>
    ErrorCode DestroyHashTable  (HashTable <Key, Value, Hash> *table);
    
    template <typename Key, typename Value, HashFunction <Key> Hash>
    ErrorCode HashTableVerifier (HashTable <Key, Value, Hash> *table);
}
#endif

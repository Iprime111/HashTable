#ifndef HASH_TABLE_HPP_
#define HASH_TABLE_HPP_

#include <cstdint>
#include <cstddef>
#include <LinkedList.hpp>

#include "ErrorCode.hpp"

struct HashTableEntry {
    size_t length = 0;
    char  *string = NULL;
};

template <typename Key>
using HashFunction = uint32_t (*) (Key *key);

template <typename Key, typename Value, HashFunction <Key> Hash>
struct HashTable {
    size_t capacity = 0;

    LinkedList::List <Value> *table = NULL;

    ErrorCode error = ErrorCode::NO_ERRORS;
};


template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode InitHashTable     (HashTable <Key, Value, Hash> *table, size_t capacity);

template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode DestroyHashTable  (HashTable <Key, Value, Hash> *table);

template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode HashTableVerifier (HashTable <Key, Value, Hash> *table);

#endif

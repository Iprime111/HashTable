#ifndef HASH_TABLE_TEMPLATES_HPP_
#define HASH_TABLE_TEMPLATES_HPP_

#include <cstdint>

#include "HashTable/HashTable.hpp"
#include "ErrorCode.hpp"
#include "HashTable/HashTableConfig.hpp"
#include "LinkedList.hpp"
#include "LinkedListDefinitions.hpp"

#define WriteError(TABLE, ERROR)  (TABLE)->error = (ErrorCode) ((int) (TABLE)->error | (int) ERROR)
#define ReturnError(TABLE, ERROR)   \
    do {                            \
        WriteError (TABLE, ERROR);  \
        return ERROR;               \
    } while (0)

#define CheckError(TABLE, EXPRESSION, ERROR)    \
    do {                                        \
        if (!(EXPRESSION)) {                    \
            ReturnError (TABLE, ERROR);         \
        }                                       \
    } while (0)

#ifdef NDEBUG
    #define ON_DEBUG(...)
#else
    #define ON_DEBUG(...) __VA_ARGS__
#endif

#define VerifyHashTable(TABLE)                                  \
    ON_DEBUG (                                                  \
        do {                                                    \
            ErrorCode tableError_ = HashTableVerifier (TABLE);  \
            if (tableError_ != ErrorCode::NO_ERRORS) {          \
                return tableError_;                             \
            }                                                   \
        } while (0)                                             \
    )


template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode AddElement (HashTable <Key, Value, Hash> *table, Value *newEntry, Key *newKey) {
    VerifyHashTable (table);

    uint32_t elementIndex = Hash (newKey) % table->capacity;
    LinkedList::List <Value> *list = &table->table [elementIndex];

    ssize_t newIndex = 0;
    CheckError (table, LinkedList::InsertAfter (list, list->prev [0], &newIndex, newEntry), ErrorCode::LIST_ERROR); // insert after tail

    return ErrorCode::NO_ERRORS;
}

template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode FindElement (HashTable <Key, Value, Hash> *table, Key *elementKey) {
    VerifyHashTable (table);

    uint32_t elementIndex = Hash (elementKey) % table->capacity;
    LinkedList::List <Value> *list = &table->table [elementIndex];

    //TODO implement

    return ErrorCode::NO_ERRORS;
}

//TODO DumpHashTable ()


template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode HashTableVerifier (HashTable <Key, Value, Hash> *table) {
    if (!table)
        return ErrorCode::NULL_POINTER;

    CheckError (table, table->error == ErrorCode::NO_ERRORS, table->error);

    for (size_t listIndex = 0; listIndex < table->capacity; listIndex++)
        CheckError (table, table->table [listIndex].errors == LinkedList::NO_LIST_ERRORS, ErrorCode::LIST_ERROR);

    return ErrorCode::NO_ERRORS;
}


template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode InitHashTable (HashTable <Key, Value, Hash> *table, size_t capacity) {
    assert (table);

    table->capacity = capacity;
    
    table->table = (LinkedList::List <Value> *) calloc (sizeof (LinkedList::List <Value>), capacity); 

    if (!table->table)
        return ErrorCode::ALLOCATION_ERROR;

    for (size_t tableElement = 0; tableElement < table->capacity; tableElement++) {
        if (LinkedList::InitList (&table->table [tableElement], EXPECTED_LOAD_FACTOR) != LinkedList::NO_LIST_ERRORS) {
            DestroyHashTable (table);
            return ErrorCode::LIST_ERROR;
        }
    }

    return ErrorCode::NO_ERRORS;
}

template <typename Key, typename Value, HashFunction <Key> Hash>
ErrorCode DestroyHashTable (HashTable <Key, Value, Hash> *table) {
    if (!table)
        return ErrorCode::NULL_POINTER;

    for (size_t listIndex = 0; listIndex < table->capacity; listIndex++) {
        LinkedList::DestroyList (&table->table [listIndex]);
    }

    memset (&table->table, 0, sizeof (LinkedList::List <Value>) * table->capacity);
    free (table->table);
    
    table->error = ErrorCode::NULL_POINTER;

    return ErrorCode::NO_ERRORS;
}

#undef WriteError
#undef ReturnError
#undef CheckError
#undef NDEBUG
#undef VerifyHashTable

#endif

#ifndef HASH_TABLE_INTERNAL_HPP_
#define HASH_TABLE_INTERNAL_HPP_

#include <LinkedList.hpp>
#include <cstdint>

#include "HashTableDefinitions.hpp"
#include "LinkedListDefinitions.hpp"

#ifndef TRACY_ENABLE
    #define ZoneScoped
#endif

#define HashTableTemplate \
    template <typename Key, typename Value, HashTableLib::HashFunction <Key *> Hash, LinkedList::Comparator <HashTableLib::Pair <Key, Value> *> ElementComparator>

#define WriteError(TABLE, ERROR)  (TABLE)->error = (HashTableLib::ErrorCode) ((int) (TABLE)->error | (int) HashTableLib::ERROR)
#define ReturnError(TABLE, ERROR)   \
    do {                            \
        WriteError (TABLE, ERROR);  \
        return HashTableLib::ERROR; \
    } while (0)

#define CheckError(TABLE, EXPRESSION, ERROR)    \
    do {                                        \
        if (!(EXPRESSION)) {                    \
            ReturnError (TABLE, ERROR);         \
        }                                       \
    } while (0)

namespace HashTableInternal {
    HashTableTemplate
    inline LinkedList::List <HashTableLib::Pair <Key, Value>, ElementComparator> *GetList (HashTableLib::HashTable <Key, Value, Hash, ElementComparator> *table, Key *elementKey) {
        ZoneScoped;
        uint32_t listIndex = Hash (elementKey) % (uint32_t) table->capacity;

        return &table->table [listIndex];
    }

    HashTableTemplate
    inline HashTableLib::ErrorCode InsertValue (HashTableLib::HashTable <Key, Value, Hash, ElementComparator> *table, 
                                                LinkedList::List <HashTableLib::Pair <Key, Value>, ElementComparator> *list, 
                                                HashTableLib::Pair <Key, Value> *newEntry) {
        ZoneScoped;

        ssize_t newIndex = 0;
        LinkedList::ListErrorCode insertErrorCode = LinkedList::InsertAfter (list, list->prev [0], &newIndex, newEntry);

        CheckError (table, insertErrorCode == LinkedList::NO_LIST_ERRORS, ErrorCode::LIST_ERROR); // insert after tail

        return HashTableLib::ErrorCode::NO_ERRORS;
    }
}

#ifndef TRACY_ENABLE
    #undef ZoneScoped
#endif

#undef HashTableTemplate
#undef WriteError
#undef ReturnError
#undef CheckError

#endif


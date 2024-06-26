#ifndef HASH_TABLE_TEMPLATES_HPP_
#define HASH_TABLE_TEMPLATES_HPP_

#include <LinkedList.hpp>

#ifdef TRACY_ENABLE
    #include <Tracy.hpp>
#endif

#include "HashTableDefinitions.hpp"
#include "HashTableInternal.hpp"
#include "LinkedListDefinitions.hpp"

#ifndef TRACY_ENABLE
    #define ZoneScoped 
#endif

#define HashTableTemplate \
    template <typename Key, typename Value, HashFunction <Key *> Hash, LinkedList::Comparator <Pair <Key, Value> *> ElementComparator>

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

namespace HashTableLib {

    HashTableTemplate
    ErrorCode AddElement (HashTable <Key, Value, Hash, ElementComparator> *table, Pair <Key, Value> *newEntry) {
        ZoneScoped;
        assert (newEntry);
        VerifyHashTable (table);
    
        LinkedList::List <Pair <Key, Value>, ElementComparator> *list = HashTableInternal::GetList (table, &newEntry->key);
    
        return HashTableInternal::InsertValue (table, list, newEntry);
    }

    HashTableTemplate
    ErrorCode AddUniqueElement (HashTable <Key, Value, Hash, ElementComparator> *table, Pair <Key, Value> *newEntry) {
        ZoneScoped;
        assert (newEntry);
        VerifyHashTable (table);

        LinkedList::List <Pair <Key, Value>, ElementComparator> *list = HashTableInternal::GetList (table, &newEntry->key);

        Pair <Key, Value> searchPattern = {.key = newEntry->key};
        ssize_t elementIndex = -1;
    
        LinkedList::FindValue (list, &searchPattern, &elementIndex);

        if (elementIndex <= 0) {
            return HashTableInternal::InsertValue (table, list, newEntry);
        }

        return ErrorCode::NO_ERRORS;
    }
    
    HashTableTemplate
    ErrorCode FindElement (HashTable <Key, Value, Hash, ElementComparator> *table, Key *elementKey, Value **element) {
        ZoneScoped;
        assert (elementKey);
        assert (element);
        VerifyHashTable (table);
    
        LinkedList::List <Pair <Key, Value>, ElementComparator> *list = HashTableInternal::GetList (table, elementKey);
        
        Pair <Key, Value> searchPattern = {.key = *elementKey};
        ssize_t elementIndex = -1;
    
        LinkedList::FindValue (list, &searchPattern, &elementIndex);
    
        if (elementIndex <= 0) {
            (*element) = nullptr;
            return ErrorCode::NO_ERRORS;
        }
    
        (*element) = &(list->data [elementIndex].value);
        return ErrorCode::NO_ERRORS;
    }
    
    HashTableTemplate
    ErrorCode HashTableVerifier (HashTable <Key, Value, Hash, ElementComparator> *table) {
        ZoneScoped;
        if (!table)
            return ErrorCode::NULL_POINTER;
    
        CheckError (table, table->error == ErrorCode::NO_ERRORS, table->error);
    
        for (size_t listIndex = 0; listIndex < table->capacity; listIndex++)
            CheckError (table, table->table [listIndex].errors == LinkedList::NO_LIST_ERRORS, ErrorCode::LIST_ERROR);
    
        return ErrorCode::NO_ERRORS;
    }
    
    
    HashTableTemplate
    ErrorCode InitHashTable (HashTable <Key, Value, Hash, ElementComparator> *table, size_t capacity) {
        ZoneScoped;
        assert (table);
    
        table->capacity = capacity;
        
        table->table = (LinkedList::List <Pair <Key, Value>, ElementComparator> *) 
            calloc (sizeof (LinkedList::List <Pair <Key, Value>, ElementComparator>), capacity); 
    
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
    
    HashTableTemplate
    ErrorCode DestroyHashTable (HashTable <Key, Value, Hash, ElementComparator> *table) {
        ZoneScoped;

        if (!table)
            return ErrorCode::NULL_POINTER;
    
        for (size_t listIndex = 0; listIndex < table->capacity; listIndex++) {
            LinkedList::DestroyList (&table->table [listIndex]);
        }
    
        free (table->table);
        
        table->error = ErrorCode::NULL_POINTER;
    
        return ErrorCode::NO_ERRORS;
    }



}

#ifndef TRACY_ENABLE
    #undef ZoneScoped
#endif

#undef HashTableTemplate
#undef WriteError
#undef ReturnError
#undef CheckError
#undef ON_DEBUG
#undef VerifyHashTable

#endif

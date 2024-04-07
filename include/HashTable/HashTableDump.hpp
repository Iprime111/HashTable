#ifndef HASH_TABLE_DUMP_HPP_
#define HASH_TABLE_DUMP_HPP_

#include <LinkedList.hpp>
#include <cstdio>

#include "ColorConsole.h"
#include "ErrorCode.hpp"
#include "HashTableDefinitions.hpp"
#include "HashTableTemplates.hpp"
#include "LinkedListDefinitions.hpp"

#define DumpFunctionTemplate template <typename Key, typename Value, HashFunction <Key *> Hash,         \
    LinkedList::Comparator <Pair <Key, Value> *> ElementComparator, ValuePrinter <Key, Value> Printer>

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
    template <typename Key, typename Value>
    using ValuePrinter = ErrorCode (*) (Pair <Key, Value> *pair, FILE *stream);
 
    DumpFunctionTemplate
    ErrorCode DumpPairsList (LinkedList::List <Pair <Key, Value>, ElementComparator> *list, FILE *stream) {
        assert (list);
        assert (stream);

        if (list->size <= 1) {
            fprintf_color (CONSOLE_RED, CONSOLE_NORMAL, stream, "\tNo elements\n\n");

            return ErrorCode::NO_ERRORS;
        }

        ssize_t elementIndex = list->next [0];

        fprintf_color (CONSOLE_GREEN, CONSOLE_BOLD, stream, "\tVALUES: ");

        while (elementIndex > 0) {
            Printer (&list->data [elementIndex], stream);

            elementIndex = list->next [elementIndex];
        }

        fprintf (stream, "\n\n");

        return ErrorCode::NO_ERRORS;
    }

    DumpFunctionTemplate
    ErrorCode DumpHashTable (HashTable <Key, Value, Hash, ElementComparator> *table, FILE *stream) {
        assert (stream);
        VerifyHashTable (table);

        for (size_t listIndex = 0; listIndex < table->capacity; listIndex++) {
            fprintf (stream, WHITE_COLOR "LIST INDEX: " BOLD_WHITE_COLOR "%lu\n", listIndex);

            DumpPairsList <Key, Value, Hash, ElementComparator, Printer> (&table->table [listIndex], stream);
        }

        return ErrorCode::NO_ERRORS;
    }


}

#undef DumpFunctionTemplate
#undef ON_DEBUG
#undef VerifyHashTable

#endif

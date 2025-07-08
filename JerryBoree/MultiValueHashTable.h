#ifndef MultiValueHashTable_H
#define MultiValueHashTable_H
#include "Defs.h"

typedef struct MultiValueHashTable_rec* MultiValueHashTable;

/**
 * @brief Creates a multi-value hash table.
 *
 * This function initializes a multi-value hash table for associating keys with multiple values.
 * It allocates memory, configures utility functions for handling keys and values, and sets up
 * the structure for storing associations.
 *
 * @param copyKey Function pointer for creating a deep copy of a key.
 * @param freeKey Function pointer for freeing the memory of a key.
 * @param printKey Function pointer for printing a key.
 * @param copyValue Function pointer for creating a deep copy of a value.
 * @param freeValue Function pointer for freeing the memory of a value.
 * @param printValue Function pointer for printing a value.
 * @param equalKey Function pointer for comparing keys for equality.
 * @param equalValue Function pointer for comparing values for equality.
 * @param transformIntoNumber Function pointer for transforming a key into a numeric hash value.
 * @param size The number of buckets in the hash table.
 *
 * @return
 * - Pointer to the newly created multi-value hash table if successful.
 * - `NULL` if memory allocation fails.
 */
MultiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
    CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
    EqualFunction equalValue,TransformIntoNumberFunction transformIntoNumber,int size);
/**
 * @brief Destroys a multi-value hash table and frees all associated memory.
 *
 * This function deallocates all memory used by the multi-value hash table, including
 * its internal structures and the hash table itself.
 *
 * @param mtv Pointer to the multi-value hash table to be destroyed.
 *
 * @return Void. The function has no return value.
 */
void destroyMultiValueHashTable(MultiValueHashTable mtv);
/**
 * @brief Adds a key-value pair to the multi-value hash table.
 *
 * This function associates a value with a key in the multi-value hash table. If the key
 * does not already exist, it is added along with the value. If the key exists but the
 * value is not yet associated with it, the value is added to the key's set of values.
 *
 * @param mtv Pointer to the multi-value hash table.
 * @param key The key to associate with the value.
 * @param value The value to be added for the specified key.
 *
 * @return
 * - `success` if the key-value pair is successfully added.
 * - `failure` if the table, key, or value is `NULL`, memory allocation fails,
 *   or the key-value pair already exists.
 */
status addToMultiValueHashTable(MultiValueHashTable mtv, Element key, Element value);
/**
 * @brief Retrieves the values associated with a given key in the multi-value hash table.
 *
 * This function looks up a key in the multi-value hash table and returns the collection of
 * values associated with that key.
 *
 * @param mtv Pointer to the multi-value hash table.
 * @param key The key to search for.
 *
 * @return
 * - A collection of values associated with the key if the key exists.
 * - `NULL` if the table or key is `NULL`, or if the key does not exist in the hash table.
 */
Element lookupInMultiValueHashTable(MultiValueHashTable mtv, Element key);
/**
 * @brief Removes a value associated with a key from the multi-value hash table.
 *
 * This function removes a specific value associated with a given key in the multi-value
 * hash table. If the key has no other associated values after the removal, the key is
 * also removed from the hash table.
 *
 * @param mtv Pointer to the multi-value hash table.
 * @param key The key whose associated value is to be removed.
 * @param value The value to be removed for the specified key.
 *
 * @return
 * - `success` if the value is successfully removed.
 * - `failure` if the table, key, or value is `NULL`, the key does not exist, or the value
 *   is not associated with the key.
 */
status removeFromMultiValueHashTable(MultiValueHashTable mtv, Element key, Element value);
/**
 * @brief Displays all keys and their associated values in the multi-value hash table.
 *
 * This function iterates through the multi-value hash table and displays each key along
 * with its associated values.
 *
 * @param mtv Pointer to the multi-value hash table.
 *
 * @return
 * - `success` if all elements are successfully displayed.
 * - `failure` if the table is `NULL`.
 */
status displayMultiValueHashTable(MultiValueHashTable mtv);

#endif
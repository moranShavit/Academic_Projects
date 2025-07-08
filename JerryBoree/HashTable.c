#include"HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s {
    int size;
    linked_list *table;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
};

/**
 * @brief Computes the hash index for a key in the hash table.
 *
 * This macro calculates the bucket index by transforming the key into a number
 * and taking the modulus with the hash table's size.
 *
 * @param table Pointer to the hash table.
 * @param key The key to compute the hash index for.
 *
 * @return The index of the bucket where the key maps.
 */
#define hash_func(table ,key) \
    ((int) (table->transformIntoNumber(key)%table->size))


Element copy_kvp(Element kvp) {
    /**
 * @brief Returns a shallow copy of a key-value pair.
 *
 * @param kvp The key-value pair to copy.
 * @return The same key-value pair.
 */
   return kvp;
}

status print_kvp(Element kvp) {
    /**
 * @brief Prints a key-value pair.
 *
 * @param kvp The key-value pair to print.
 * @return
 * - `success` if the key-value pair is printed successfully.
 * - `failure` if the key-value pair is `NULL`.
 */
    if (kvp == NULL) return failure;
    key_value_pair temp = (key_value_pair) kvp;
    displayKey(temp);
    displayValue(temp);
    return success;
}

status destroy_kvp(Element kvp) {
     /**
     * @brief Destroys a key-value pair.
     *
     * @param kvp The input element representing the key-value pair to destroy.
     * @return
     * - `success` if the key-value pair is successfully destroyed.
     * - `failure` if the input element is `NULL`.
     */
    if (kvp == NULL) return failure;
    key_value_pair temp = (key_value_pair) kvp;
    destroyKeyValuePair(temp);
    return success;
}


Element kvp_get_key(Element kvp) {
    /**
 * @brief Retrieves the key from a key-value pair.
 *
 * @param kvp The input element representing the key-value pair.
 * @return
 * - The key of the key-value pair if the input is valid.
 * - `NULL` if the input element is `NULL`.
 */
    if (kvp == NULL) return NULL;
    key_value_pair temp = (key_value_pair) kvp;
    return get_shallow_key(temp);
}

static linked_list create_kvp_list(hashTable t) {
    /**
 * @brief Creates a linked list for storing key-value pairs in the hash table.
 *
 * @param t The hash table for which the linked list is being created.
 * @return
 * - A pointer to the newly created linked list.
 * - `NULL` if memory allocation fails.
 *
 * @note This is an internal helper function for integrating key-value pairs with the hash table.
 */
    return createLinkedList(copy_kvp, t->equalKey, print_kvp, destroy_kvp, kvp_get_key);
}



hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
    CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
    EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    /**
     * @brief Creates a hash table with specified utility functions and size.
     *
     * This function initializes a hash table, allocates memory for its structure, and
     * sets up buckets to store key-value pairs using the chaining method for collision handling.
     * It also assigns the provided utility functions for handling keys and values.
     *
     * @param copyKey Function pointer for creating a deep copy of a key.
     * @param freeKey Function pointer for freeing the memory of a key.
     * @param printKey Function pointer for printing a key.
     * @param copyValue Function pointer for creating a deep copy of a value.
     * @param freeValue Function pointer for freeing the memory of a value.
     * @param printValue Function pointer for printing a value.
     * @param equalKey Function pointer for comparing two keys for equality.
     * @param transformIntoNumber Function pointer for transforming a key into a numeric hash value.
     * @param hashNumber The number of buckets in the hash table.
     *
     * @return
     * - Pointer to the newly created hash table if memory allocation is successful.
     * - `NULL` if memory allocation fails at any stage.
     */
    hashTable t = (hashTable)malloc(sizeof(struct hashTable_s));
    if (t == NULL) return NULL;
    t->size = hashNumber;
    t->copyKey = copyKey;
    t->freeKey = freeKey;
    t->printKey = printKey;
    t->copyValue = copyValue;
    t->freeValue = freeValue;
    t->printValue = printValue;
    t->equalKey = equalKey;
    t->transformIntoNumber = transformIntoNumber;

    //alocate memory for the table, array of linked lists
    t->table = (linked_list*)malloc(sizeof(linked_list) * t->size);
    if (t->table == NULL) {
        free(t);
        return NULL;
    }
    // initialize linked list in every cell
    for (int i = 0; i < t->size; i++) {
        t->table[i] = create_kvp_list(t);
        if (t->table[i] == NULL) {
            for (int j = 0; j < i; j++) {
                destroyList(t->table[j]);
            }
            free(t->table);
            free(t);
            return NULL;
        }
    }
    return t;
}

status destroyHashTable(hashTable t) {
    /**
     * @brief Destroys a hash table and frees all associated memory.
     *
     * This function deallocates the memory of all key-value pairs stored in the hash table,
     * frees the memory for each bucket, and finally deallocates the hash table structure itself.
     *
     * @param hashTable Pointer to the hash table to be destroyed.
     *
     * @return
     * - `success` if the hash table is successfully destroyed.
     * - `failure` if the hash table is `NULL`.
     */
    if (t == NULL) return failure;
    for (int i = 0; i < t->size; i++) {
        destroyList(t->table[i]);
    }
    free(t->table);
    free(t);
    return success;
}

status addToHashTable(hashTable t, Element key,Element value) {
    /**
     * @brief Adds a key-value pair to the hash table.
     *
     * This function inserts a key-value pair into the hash table. If a pair with the same key
     * already exists in the hash table, the insertion fails. The function calculates the
     * appropriate bucket index using a hash function and handles collisions using the chaining method.
     *
     * @param hashTable Pointer to the hash table.
     * @param key The key of the key-value pair to be added.
     * @param value The value of the key-value pair to be added.
     *
     * @return
     * - `success` if the key-value pair is successfully added.
     * - `failure` if the hash table, key, or value is `NULL`, memory allocation fails,
     *   or a pair with the same key already exists.
     */
    if (t == NULL || key == NULL || value == NULL) return failure;
    key_value_pair kvp = createKeyValuePair(key, value, t->equalKey, t->printKey, t->printValue,
        t->freeKey, t->freeValue, t->copyKey, t->copyValue);
    if (kvp == NULL) return failure;
    int idx = hash_func(t, key);
    if (searchByKey(t->table[idx], key) != NULL) {
        destroyKeyValuePair(kvp);
        return failure;
    }
    if ( appendNode(t->table[idx], kvp)== success) {
        return success;
    }
    destroyKeyValuePair(kvp);
    return failure;
}

Element lookupInHashTable(hashTable t, Element key) {
    /**
     * @brief Retrieves the value associated with a given key in the hash table.
     *
     * This function searches for a key in the hash table and, if found, returns the
     * value associated with that key.
     *
     * @param t Pointer to the hash table.
     * @param key The key to search for.
     *
     * @return
     * - The value associated with the key if the key exists in the hash table.
     * - `NULL` if the hash table or key is `NULL`, or if the key does not exist in the hash table.
     */
    if (key == NULL || t == NULL) return NULL;
    int idx =  hash_func(t, key);
    key_value_pair kvp = (key_value_pair) searchByKey(t->table[idx], key);
    return getValue(kvp);
}

status removeFromHashTable(hashTable t, Element key) {
    /**
     * @brief Removes a key-value pair from the hash table.
     *
     * This function searches for a key in the hash table and removes the corresponding
     * key-value pair if it exists.
     *
     * @param hashTable Pointer to the hash table.
     * @param key The key of the key-value pair to be removed.
     *
     * @return
     * - `success` if the key-value pair is successfully removed.
     * - `failure` if the hash table, key is `NULL`, or the key does not exist in the hash table.
     */
    if (t == NULL || key == NULL ) return failure;
    int idx = hash_func(t, key);
    Element kvp = searchByKey(t->table[idx], key);
    if (kvp == NULL) return failure;
    return deleteNode(t->table[idx], kvp);
}

status displayHashElements(hashTable t) {
    /**
     * @brief Displays all key-value pairs in the hash table.
     *
     * This function iterates through all buckets in the hash table and displays
     * the key-value pairs stored in each bucket.
     *
     * @param hashTable Pointer to the hash table.
     *
     * @return
     * - `success` if all elements are successfully displayed.
     * - `failure` if the hash table is `NULL`.
     */
    if (t == NULL) return failure;
    for (int i=0; i < t->size; i++) {
        displayList(t->table[i]);
    }
    return success;
}




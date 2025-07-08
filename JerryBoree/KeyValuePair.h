#ifndef KEY_VALUE_PAIR_H
#define KEY_VALUE_PAIR_H
#include "Defs.h"

typedef struct key_value_pair_rec* key_value_pair;
/**
 * @brief Creates a key-value pair with specified utility functions.
 *
 * This function allocates memory for a key-value pair structure, copies the provided
 * key and value using their respective copy functions, and initializes the utility
 * functions for operations on the key and value.
 *
 * @param key The key element to be stored in the key-value pair.
 * @param value The value element to be stored in the key-value pair.
 * @param key_comp Function pointer for comparing keys.
 * @param key_print Function pointer for printing the key.
 * @param val_print Function pointer for printing the value.
 * @param key_free Function pointer for freeing the memory of the key.
 * @param val_free Function pointer for freeing the memory of the value.
 * @param key_copy Function pointer for creating a deep copy of the key.
 * @param val_copy Function pointer for creating a deep copy of the value.
 *
 * @return
 * - Pointer to the newly created key-value pair if memory allocation is successful.
 * - `NULL` if memory allocation fails.
 */
key_value_pair createKeyValuePair(Element key, Element value, EqualFunction key_comp, PrintFunction key_print,
    PrintFunction val_print, FreeFunction key_free, FreeFunction val_free, CopyFunction key_copy, CopyFunction val_copy);
/**
 * @brief Frees all memory associated with a key-value pair.
 *
 * This function deallocates the memory of both the key and the value stored
 * in the key-value pair using their respective free functions, and then
 * frees the memory allocated for the key-value pair structure itself.
 *
 * @param kvp Pointer to the key-value pair to be destroyed.
 *
 * @return Void. The function has no return value.
 */
void destroyKeyValuePair(key_value_pair kvp);

/**
 * @brief Retrieves the key stored in a key-value pair.
 *
 * This function returns the key associated with the key-value pair. It's meant to support other ADT'S.
 * If the key-value pair is `NULL`, the function returns `NULL`.
 *
 * @param kvp Pointer to the key-value pair.
 *
 * @return
 * - The key stored in the key-value pair.
 * - `NULL` if the key-value pair is `NULL`.
 */
Element get_shallow_key(key_value_pair kvp);

/**
 * @brief Displays the value stored in a key-value pair.
 *
 * This function prints the value of a key-value pair using the associated
 * `val_print` function.
 *
 * @param kvp Pointer to the key-value pair whose value is to be displayed.
 *
 * @return
 * - `success` if the value is successfully displayed.
 * - `failure` if the key-value pair is `NULL`.
 */
status displayValue(key_value_pair kvp);
/**
 * @brief Displays the key stored in a key-value pair.
 *
 * This function prints the key of a key-value pair using the associated
 * `key_print` function.
 *
 * @param kvp Pointer to the key-value pair whose key is to be displayed.
 *
 * @return
 * - `success` if the key is successfully displayed.
 * - `failure` if the key-value pair is `NULL`.
 */
status displayKey(key_value_pair kvp);
/**
 * @brief Retrieves copy of the value stored in a key-value pair.
 *
 * This function returns the value associated with the key-value pair. If the key-value
 * pair is `NULL`, the function returns `NULL`.
 *
 * @param kvp Pointer to the key-value pair.
 *
 * @return
 * - copy of the value stored in the key-value pair.
 * - `NULL` if the key-value pair is `NULL`.
 */
Element getValue(key_value_pair kvp);
/**
 * @brief Retrieves copy of the key stored in a key-value pair.
 *
 * This function returns the key associated with the key-value pair. If the key-value
 * pair is `NULL`, the function returns `NULL`.
 *
 * @param kvp Pointer to the key-value pair.
 *
 * @return
 * - Copy of the key stored in the key-value pair.
 * - `NULL` if the key-value pair is `NULL`.
 */
Element getKey(key_value_pair kvp);
/**
 * @brief Checks if the keys of two key-value pairs are equal.
 *
 * This function compares the keys of two key-value pairs using their associated
 * `key_comp` function. If either comparison indicates equality, the keys are
 * considered equal.
 *
 * @param kvp_1 Pointer to the first key-value pair.
 * @param kvp_2 Pointer to the second key-value pair.
 *
 * @return
 * - `true` if the keys of the two key-value pairs are equal.
 * - `false` if the keys are not equal or either key-value pair is `NULL`.
 */
bool isEqualKey(key_value_pair kvp_1, key_value_pair kvp_2);

#endif
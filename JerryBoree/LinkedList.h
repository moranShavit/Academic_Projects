#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "Defs.h"

typedef struct linked_list_rec* linked_list;
typedef Element (*getKeyFunction) (Element);

/**
 * @brief Creates a new linked list with specified utility functions.
 *
 * This function initializes a new linked list structure, sets its attributes,
 * and assigns the utility functions for element operations (copy, equality check,
 * printing, freeing, and key extraction).
 *
 * @param copy Function pointer for creating a deep copy of an element.
 * @param equal Function pointer for checking equality between two elements keys.
 * @param print Function pointer for printing an element.
 * @param elem_free Function pointer for freeing the memory of an element.
 * @param getKey Function pointer for extracting a key from an element.
 *
 * @return
 * - Pointer to the newly created linked list if memory allocation is successful.
 * - `NULL` if memory allocation fails.
 */
linked_list createLinkedList(CopyFunction copy, EqualFunction equal, PrintFunction print, FreeFunction elem_free,
     getKeyFunction getKey);
/**
 * @brief Appends a new node with the given element to the end of a linked list.
 *
 * This function creates a new node containing the specified element and adds it
 * to the end of the linked list. If the list is empty, the new node becomes the head.
 *
 * @param list Pointer to the linked list to which the node will be appended.
 * @param elem The element to store in the new node. The element is copied using
 *             the list's `CopyFunction`.
 *
 * @return
 * - `success` if the node is successfully appended to the list.
 * - `failure` if the linked list is `NULL` or memory allocation for the new node fails.
 */
status appendNode(linked_list list, Element elem);
/**
 * @brief Deletes a node containing the specified element from a linked list.
 *
 * This function removes the first node in the linked list whose element matches the
 * given element, based on the list's `EqualFunction`. It updates the pointers of
 * neighboring nodes to maintain the list's structure.
 *
 * @param list Pointer to the linked list from which the node will be deleted.
 * @param elem The element to identify the node to be deleted. The comparison is done
 *             using the list's `EqualFunction` and `getKeyFunction`.
 *
 * @return
 * - `success` if the node is found and successfully deleted.
 * - `failure` if the linked list is `NULL`, or no matching node is found.
 */
status deleteNode(linked_list list, Element elem);
/**
 * @brief Retrieves the data stored at a specified index in a linked list.
 *
 * This function returns the element stored in the node at the given index in the linked list.
 * Indexing starts from 1. If the index is out of bounds or the list is `NULL`, the function
 * returns `NULL`.
 *
 * @param list Pointer to the linked list.
 * @param index The one-based index of the desired node.
 *
 * @return
 * - copy of the element stored at the specified index if the index is valid.
 * - `NULL` if the linked list is `NULL` or the index is out of bounds.
 */
Element getDataByIndex(linked_list list, int index);
/**
 * @brief Retrieves the length of the linked list.
 *
 * This function returns the number of elements currently stored in the linked list.
 * If the list is `NULL`, the function returns 0.
 *
 * @param list Pointer to the linked list.
 *
 * @return
 * - The number of elements in the linked list.
 * - `0` if the linked list is `NULL`.
 */
int getLengthList(linked_list list);
/**
 * @brief Searches for an element in the linked list by a specified key.
 *
 * This function iterates through the linked list to find the first element whose key matches
 * the provided key, based on the list's `EqualFunction` and `getKeyFunction`.
 *
 * @param list Pointer to the linked list.
 * @param key The key to search for, compared using the list's `EqualFunction` and `getKeyFunction`.
 *
 * @return
 * - copy of the element that matches the provided key if found.
 * - `NULL` if the linked list is `NULL` or no matching element is found.
 */
Element searchByKey(linked_list list, Element key);
/**
 * @brief Helper function for iterating through the linked list.
 *
 * This function moves the iterator to the next node in the linked list and retrieves the
 * element stored in that node. It is designed to be used as part of the linked list's iterator
 * functionality.
 *
 * @param list Pointer to the linked list.
 *
 * @return
 * - The element stored in the next node if the list and next node exist.
 * - `NULL` if the linked list is `NULL` or there is no next node.
 */
Element listNext(linked_list list);
/**
 * @brief Helper function for initializing or resetting the iterator to the head of the linked list.
 *
 * This function sets the iterator to point to the head of the linked list and retrieves the
 * element stored in the head node. It is intended to be used as part of the linked list's
 * iterator functionality.
 *
 * @param list Pointer to the linked list.
 *
 * @return
 * - The element stored in the head node if the list is not `NULL`.
 * - `NULL` if the linked list is `NULL`.
 */
Element listHead(linked_list list);
/**
 * @brief Displays all elements in the linked list.
 *
 * This function iterates through the linked list and prints each element using the list's
 * `PrintFunction`. It ensures all elements are displayed in order from the head to the tail.
 *
 * @param list Pointer to the linked list.
 *
 * @return
 * - `success` if the list is successfully displayed.
 * - `failure` if the linked list is `NULL`.
 */
status displayList(linked_list list);
/**
 * @brief Frees all memory associated with the linked list and its elements.
 *
 * This function iterates through the linked list, freeing the memory of each element
 * using the list's `FreeFunction` and deallocating all nodes. Finally, it frees the memory
 * allocated for the linked list structure itself.
 *
 * @param list Pointer to the linked list to be destroyed.
 *
 * @return Void. The function has no return value.
 *
 * @note
 * - If the linked list is `NULL`, the function performs no operation.
 * - This function ensures that no memory leaks occur by deallocating all dynamically
 *   allocated memory associated with the list.
 */
void destroyList(linked_list list);
/**
 * @brief Macro for iterating through all elements in a linked list.
 *
 * This macro simplifies the iteration process over a linked list. It sets the iterator
 * to the head of the list and moves through each element using `listHead` and `listNext`
 * functions until the end of the list is reached.
 *
 * @param e The variable to hold the current element during the iteration.
 * @param l The linked list to iterate over.
 *
 * @note
 * - The macro assumes that the `listHead` and `listNext` functions are implemented correctly
 *   and handle edge cases such as an empty list or reaching the end of the list.
 * - The iteration stops when `listNext` returns `NULL`.
 *
 * @example
 * Element elem;
 * list_forEach(elem, myLinkedList) {
 *     printf("Processing element: ");
 *     printFunction(elem);
 * }
 */
#define list_forEach(e, l) \
    for (e = listHead(l); e != NULL; e = listNext(l))

#endif
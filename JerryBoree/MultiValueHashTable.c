#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "LinkedList.h"


struct MultiValueHashTable_rec {
    hashTable hashTable;
    int size;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    EqualFunction equalValue;
    TransformIntoNumberFunction transformIntoNumber;
};
Element elem_to_elem(Element value) {
    /**
 * @brief Returns the given element as is.
 *
 * This helper function provides a direct mapping of an element to itself. It is used to
 * integrate with ADTs requiring a copy function where no deep copy is necessary.
 *
 * @param value The element to be returned.
 * @return The same element passed as input.
 */
    return value;
}

status destroy_ll_as_elem(Element elem) {
    /**
 * @brief Destroys a linked list when treated as an element.
 *
 * This helper function integrates the linked list ADT with the multi-value hash table
 * by allowing linked lists to be treated as elements. It deallocates all memory used
 * by the linked list.
 *
 * @param elem The element representing the linked list to be destroyed.
 * @return
 * - `success` if the linked list is successfully destroyed.
 * - `failure` if the input element is `NULL`.
 */
    if (elem == NULL) return  failure;
    linked_list ll = (linked_list) elem;
    destroyList(ll);
    return success;
}

status print_ll_as_elem(Element elem) {
    /**
 * @brief Prints a linked list when treated as an element.
 *
 * This helper function integrates the linked list ADT with the multi-value hash table
 * by allowing linked lists to be treated as elements. It displays all elements in the
 * linked list.
 *
 * @param elem The element representing the linked list to be printed.
 * @return
 * - `success` if the linked list is successfully displayed.
 * - `failure` if the input element is `NULL`.
 */
    if (elem == NULL) return  failure;
    linked_list ll = (linked_list) elem;
    displayList(ll);
    return success;
}

linked_list create_inner_ll(MultiValueHashTable mtv) {
    /**
 * @brief Creates a linked list for managing values in the multi-value hash table.
 *
 * @param mtv Pointer to the multi-value hash table.
 * @return
 * - A pointer to the newly created linked list.
 * - `NULL` if memory allocation fails.
 */
    linked_list ll = createLinkedList(mtv->copyValue, mtv->equalValue, mtv->printValue, mtv->freeValue, elem_to_elem );
    return ll;
}

MultiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
    CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
    EqualFunction equalValue,TransformIntoNumberFunction transformIntoNumber,int size) {
    MultiValueHashTable mtv = (MultiValueHashTable) malloc(sizeof(struct MultiValueHashTable_rec));
    if (mtv == NULL) return NULL;
    hashTable hashTable = createHashTable(copyKey, freeKey, printKey, elem_to_elem, destroy_ll_as_elem,
        print_ll_as_elem, equalKey, transformIntoNumber, size);
    if (hashTable == NULL) {
        free(mtv);
        return NULL;
    }
    mtv ->hashTable = hashTable;
    mtv->size = size;
    mtv->copyKey = copyKey;
    mtv->freeKey = freeKey;
    mtv->printKey = printKey;
    mtv->copyValue = copyValue;
    mtv->freeValue = freeValue;
    mtv->printValue = printValue;
    mtv->equalKey = equalKey;
    mtv->equalValue = equalValue;
    mtv->transformIntoNumber = transformIntoNumber;
    return mtv;
}
void destroyMultiValueHashTable(MultiValueHashTable mtv) {
    if (mtv == NULL) return;
    destroyHashTable(mtv->hashTable);
    free(mtv);
}

status addToMultiValueHashTable(MultiValueHashTable mtv, Element key, Element value) {
    if (mtv == NULL || key == NULL || value == NULL) return  failure;
    linked_list ll = (linked_list)lookupInHashTable(mtv->hashTable, key);
    if (ll == NULL) {  //key not exist
        ll = create_inner_ll(mtv);
        if (appendNode(ll, value) == failure) {
            destroyList(ll);
            return failure;
        }
        addToHashTable(mtv->hashTable, key, ll);
        return success;
    }
    Element temp = searchByKey(ll, value);
    if (temp != NULL) { // check if value already map to key
        return failure;
    }
    if (appendNode(ll, value) == failure) return  failure;
    return success;
}

Element lookupInMultiValueHashTable(MultiValueHashTable mtv, Element key) {
    if (mtv == NULL || key == NULL) return NULL;
    return lookupInHashTable(mtv->hashTable, key);
}

status removeFromMultiValueHashTable(MultiValueHashTable mtv, Element key, Element value) {
    if (mtv == NULL || key == NULL || value == NULL) return  failure;
    linked_list ll = (linked_list) lookupInHashTable(mtv->hashTable, key);
    if (ll == NULL) return failure;
    if (deleteNode(ll, value) == failure) return failure;
    if (getLengthList(ll)==0) {
       removeFromHashTable(mtv->hashTable, key);
    }
    return success;
}

status displayMultiValueHashTable(MultiValueHashTable mtv) {
    if (mtv == NULL) return  failure;
    displayHashElements(mtv->hashTable);
    return success;
}
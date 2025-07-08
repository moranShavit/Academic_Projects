#include "LinkedList.h"

typedef struct node_rec{
    /**
 * @brief Structure representing a node in a doubly linked list.
 *
 * This structure defines a single node in a doubly linked list, containing:
 * - `data`: The element stored in the node.
 * - `next`: A pointer to the next node in the list.
 * - `prev`: A pointer to the previous node in the list.
 *
 * The typedef provides two aliases:
 * - `node_rec`: Represents the structure definition.
 * - `node`: Represents a pointer to a `node_rec`.
 */
    Element data;
    struct node_rec* next;
    struct node_rec* prev;
}node_rec, *node;

struct linked_list_rec {
    /**
 * @brief Structure representing a linked list with utility functions.
 *
 * This structure defines a linked list, which includes:
 * - `head`: Pointer to the first node in the list.
 * - `current`: Pointer used for iteration within the list.
 * - `listLength`: The number of nodes in the list.
 * - `CopyFunction`: Function pointer for creating a deep copy of an element.
 * - `FreeFunction`: Function pointer for freeing the memory of an element.
 * - `PrintFunction`: Function pointer for printing an element.
 * - `EqualFunction`: Function pointer for checking equality between two elements.
 * - `getKeyFunction`: Function pointer for extracting a key from an element.
 */
    node head;
    node current;
    int listLength;
    Element(*CopyFunction) (Element);
    status(*FreeFunction) (Element);
    status(*PrintFunction) (Element);
    bool(*EqualFunction) (Element, Element);
    Element (*getKeyFunction) (Element);
};


node createNode(Element elem, node prev,linked_list list) {
    /**
 * @brief Creates a new node for a doubly linked list.
 *
 * This function allocates memory for a new node, copies the provided element
 * into the node using the list's `CopyFunction`, and initializes the node's
 * pointers to its predecessor and successor.
 *
 * @param elem The element to store in the new node.
 *             The element is copied using the list's `CopyFunction`.
 * @param prev Pointer to the previous node in the linked list (can be NULL if no predecessor exists).
 * @param list Pointer to the linked list to which this node will belong.
 *             The list must have a valid `CopyFunction` for creating a deep copy of the element.
 *
 * @return
 * - Pointer to the newly created node if memory allocation is successful.
 * - `NULL` if memory allocation fails.
 */
    node newNode = (node) malloc(sizeof(node_rec));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = list->CopyFunction(elem);
    newNode->next = NULL;
    newNode->prev = prev;
    return newNode;
}

linked_list createLinkedList(CopyFunction copy, EqualFunction equal, PrintFunction print, FreeFunction elem_free,
     getKeyFunction getKey) {
    linked_list newList = (linked_list) malloc(sizeof(struct linked_list_rec));
    if (newList == NULL)
        return NULL;
    newList->head = NULL;
    newList->current = NULL;
    newList->listLength = 0;
    newList->CopyFunction = copy;
    newList->EqualFunction = equal;
    newList -> PrintFunction = print;
    newList -> FreeFunction = elem_free;
    newList -> getKeyFunction = getKey;
    return newList;
}

status appendNode(linked_list list, Element elem) {
    if (list == NULL) {
        return failure;
    }
    if (list -> listLength == 0) {
        node newNode = createNode(elem, NULL, list);
        if (newNode == NULL) return failure;
        list -> head = newNode;
        list ->listLength++;
        return success;
    }
    node cur = list->head;
    for (int i = 0; i < list->listLength-1; i++) {
        cur = cur->next;
    }
    node newNode = createNode(elem, cur, list);
    if (newNode == NULL) return failure;
    cur->next = newNode;
    list -> listLength++;
    return success;
}

status deleteNode(linked_list list, Element elem) {
    if (list == NULL) return failure;
    node cur = list->head;
    for (int i = 0; i < list->listLength; i++) {
        if (list->EqualFunction(list->getKeyFunction(cur->data) ,list->getKeyFunction(elem)) == true) {
            if (i == 0) {
                list->head = cur->next;
                if (cur->next != NULL) {
                    cur->next->prev = NULL;
                }
            }else if (i == list->listLength-1) {
                cur->prev->next = NULL;
            }else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }
            list->FreeFunction(cur->data);
            free(cur);
            list -> listLength--;
            return success;
        }
        cur = cur->next;
    }
    return failure;
}

Element getDataByIndex(linked_list list, int index) {
    if (list == NULL || list->listLength < index) return NULL;
    node cur = list->head;
    for (int i = 0; i < index; i++) {
        if (i == index - 1) return list->CopyFunction(cur->data);
        cur = cur->next;
    }
    return NULL;
}

int getLengthList(linked_list list) {
    if (list == NULL) return 0;
    return list->listLength;
}

Element searchByKey(linked_list list, Element key) {
    if (list == NULL) return NULL;
    node cur = list->head;
    for (int i = 0; i < list->listLength; i++) {
        if (list->EqualFunction(list->getKeyFunction(cur->data), key) == true) {
            return list->CopyFunction(cur->data);
        }
        cur = cur->next;
    }
    return NULL;
}

Element listNext(linked_list list) {
    if (list == NULL) return NULL;
    node cur = list->current->next;
    if (cur == NULL) return NULL;
    list->current = cur;
    return cur->data;
}

Element listHead(linked_list list) {
    if (list == NULL) return NULL;
    list->current = list->head;
    return list->head->data;
}

status displayList(linked_list list) {
    if (list == NULL) return failure;
    node cur = list->head;
    for (int i = 0; i < list->listLength; i++) {
        list->PrintFunction(cur->data);
        cur = cur->next;
    }
    return success;
}

void destroyList(linked_list list) {
    if (list == NULL) return;
    node cur = list->head;
    for (int i = 0; i < list->listLength; i++) {
        list->FreeFunction(cur->data);
        node next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}


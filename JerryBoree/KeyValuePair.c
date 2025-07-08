#include "KeyValuePair.h"

struct key_value_pair_rec {
    Element key;
    Element value;
    EqualFunction key_comp;
    PrintFunction key_print;
    PrintFunction val_print;
    FreeFunction key_free;
    FreeFunction val_free;
    CopyFunction key_copy;
    CopyFunction val_copy;
};

key_value_pair createKeyValuePair(Element key, Element value, EqualFunction key_comp, PrintFunction key_print,
    PrintFunction val_print, FreeFunction key_free, FreeFunction val_free,
    CopyFunction key_copy, CopyFunction val_copy) {
    key_value_pair kv = malloc(sizeof(struct key_value_pair_rec));
    if (kv == NULL) return NULL;
    kv->key = key_copy(key);
    kv->value = val_copy(value);
    kv->key_comp = key_comp;
    kv->key_print = key_print;
    kv->val_print = val_print;
    kv->key_free = key_free;
    kv->val_free = val_free;
    kv->key_copy = key_copy;
    kv->val_copy = val_copy;
    return kv;
}

void destroyKeyValuePair(key_value_pair kvp) {
    if (kvp == NULL) return;
    kvp->key_free(kvp->key);
    kvp->val_free(kvp->value);
    free(kvp);
}

status displayValue(key_value_pair kvp) {
    if (kvp == NULL) return failure;
    kvp->val_print(kvp->value);
    return success;
}

status displayKey(key_value_pair kvp) {
    if (kvp == NULL) return failure;
    kvp->key_print(kvp->key);
    return success;
}

Element getValue(key_value_pair kvp) {
    if (kvp == NULL) return NULL;
    return kvp->val_copy(kvp->value);
}

Element getKey(key_value_pair kvp) {
    if (kvp == NULL) return NULL;
    return kvp->key_copy(kvp->key);
}

Element get_shallow_key(key_value_pair kvp) {
    if (kvp == NULL) return NULL;
    return kvp->key;
}

bool isEqualKey(key_value_pair kvp_1, key_value_pair kvp_2) {
    if (kvp_1 == NULL || kvp_2 == NULL) return false;
    if ((kvp_1->key_comp(kvp_1->key, kvp_2->key) == true) ||
        kvp_2->key_comp(kvp_1->key, kvp_2->key) == true) return true;
    return false;
}
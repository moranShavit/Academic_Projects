//
// Created by moran on 03/12/2024.
//

#include "Jerry.h"

extern bool memoryProb;

Origin *createOrigin(char dim[], Planet* planet);

/**
 * @function createJerry
 * @brief Creates and initializes a new Jerry object with the given ID, happiness level, and origin.
 *
 * This function allocates memory for a new Jerry structure, assigns the provided ID, happiness level,
 * and origin, and initializes the physical characteristics array to NULL.
 *
 * @param id A string representing the Jerry's unique identifier.
 * @param happiness The happiness level of the Jerry.
 * @param origin A pointer to an Origin structure representing the Jerry's origin.
 *
 * @return Jerry* A pointer to the newly created Jerry object, or NULL if memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
Jerry *createJerry(char id[], int happiness, Origin *origin) {
    if (id == NULL || origin == NULL) {
        memoryProb = true;
        return NULL;
    }
    Jerry *j = (Jerry *)malloc(sizeof(Jerry));
    if (j == NULL) {
        memoryProb = true;
        return NULL;
    }
    j->id = (char *)malloc(strlen(id) + 1);
    if (j->id == NULL) {
        delJerry(&j);
        memoryProb = true;
        return NULL;
    }
    strcpy(j->id, id);
    j->happines_level = happiness;
    j->origin = origin;
    j->phys_num = 0;
    j->phys_char = NULL;
    return j;
    }

Jerry *createJerry_with_planet(char id[], int happiness, Planet* planet, char dim[]) {
    Origin* o = createOrigin(dim, planet);
    if (o == NULL) {
        memoryProb = true;
        return NULL;
    }
    Jerry *j = createJerry(id, happiness, o);
    if (j == NULL) {
        memoryProb = true;
        delOrigin(&o);
        return NULL;
    }
    return j;
}

Planet* createPlanet(char name[], float x, float y, float z) {
    if (name == NULL) {
        memoryProb = true;
        return NULL;
    }
    Planet *p = (Planet *)malloc(sizeof(Planet));
    if (p == NULL) {
        memoryProb = true;
        return NULL;
    }
    p->name = (char *)malloc(strlen(name) + 1);
    if (p->name == NULL) {
        delPlanet(&p);
        memoryProb = true;
        return NULL;
    }
    strcpy(p->name, name);
    p->cordinate[0] = x;
    p->cordinate[1] = y;
    p->cordinate[2] = z;
    return p;
}

PhysicalCharacteristics *createPhysChar(char name[], float val) {
    if (name == NULL) {
        memoryProb = true;
        return NULL;
    }
    PhysicalCharacteristics *p = (PhysicalCharacteristics *)malloc(sizeof(PhysicalCharacteristics));
    if (p == NULL) {
        memoryProb = true;
        return NULL;
    }
    p->name = (char *)malloc(strlen(name) + 1);
    if (p->name == NULL) {
        delPhysChar(&p);
        memoryProb = true;
        return NULL;
    }
    strcpy(p->name, name);
    p->val = val;
    return p;
}

/**
 * @function createOrigin
 * @brief Creates and initializes a new Origin object with the given dimension and associated planet.
 *
 * This function allocates memory for a new Origin structure, assigns the provided dimension string
 * and the associated planet pointer, and initializes the dimension by allocating memory for it.
 *
 * @param dim A string representing the dimension of the origin.
 * @param planet A pointer to the Planet structure associated with the origin.
 *
 * @return Origin* A pointer to the newly created Origin object, or NULL if memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
Origin *createOrigin(char dim[], Planet* planet) {
    if (dim == NULL || planet == NULL) {
        memoryProb = true;
        return NULL;
    }
    Origin *o = (Origin *)malloc(sizeof(Origin));
    if (o == NULL) {
        memoryProb = true;
        return NULL;
    }
    o->dim = (char *)malloc(strlen(dim) + 1);
    if (o->dim == NULL) {
        delOrigin(&o);
        memoryProb = true;
        return NULL;
    }
    strcpy(o->dim, dim);
    o->planet = planet;
    return o;
}

bool physExcit(Jerry *j, char name[]) {
    if (j == NULL || name == NULL) {
        memoryProb = true;
        return false;
    }
    for (int i = 0; i < j->phys_num; i++) {
        if (strcmp(j->phys_char[i]->name, name) == 0) {
            return true;
        }
    }
    return false;
}

status addPhys(Jerry *j, PhysicalCharacteristics *p) {
    if (j == NULL || p == NULL) {
        memoryProb = true;
        return failure;
    }
    for (int i = 0; i < j->phys_num; i++) {
        if (strcmp(j->phys_char[i]->name, p->name) == 0) {
            return failure;
        }
    }
    j->phys_num++;
    PhysicalCharacteristics **new_arr = (PhysicalCharacteristics **)realloc(j->phys_char, j->phys_num * sizeof(PhysicalCharacteristics *));
    if (new_arr == NULL && j->phys_num > 0) {
        memoryProb = true;
        return failure;
    }
    j->phys_char = new_arr;
    j->phys_char[j->phys_num-1] = p;
    return success;
}

status delPhysByName(Jerry *j, char physName[]) {
    if (j == NULL || physName == NULL) {
        memoryProb = true;
        return failure;
    }
    if (!physExcit(j, physName)) {
        return failure;
    }
    bool move = false;
    for (int i = 0; i < j->phys_num; i++) {
        if (strcmp(j->phys_char[i]->name, physName) == 0) {
            delPhysChar(&(j->phys_char[i]));
            move = true;

        }
        else if (move == true) {
            j->phys_char[i-1] = j->phys_char[i];
        }
    }
    j->phys_num--;
    if (j->phys_num == 0) {
        free(j->phys_char);
        j->phys_char = NULL;
        return success;
    }
    PhysicalCharacteristics **p = (PhysicalCharacteristics **)realloc(j->phys_char, j->phys_num * sizeof(PhysicalCharacteristics *));
    if (p == NULL && j->phys_num > 0) {
        memoryProb = true;
        return failure;
    }
    j->phys_char = p;
    return success;
}

status printJerry(Jerry *j) {
    if (j == NULL) {
        memoryProb = true;
        return failure;
    }
    printf("Jerry , ID - %s : \n"
           "Happiness level : %d \n"
           "Origin : %s \n"
           "Planet : %s (%.2f,%.2f,%.2f) \n",
           j->id, j->happines_level, j->origin->dim, j->origin->planet->name,
           j->origin->planet->cordinate[0], j->origin->planet->cordinate[1],j->origin->planet->cordinate[2]);
    if (j->phys_num > 0) {
       printf("Jerry's physical Characteristics available : \n");
        printf("\t");
       for (int i = 0; i < j->phys_num; i++) {
           if (i != j->phys_num-1){
               printf("%s : %.2f , ", j->phys_char[i]->name, j->phys_char[i]->val);
           }else {
               printf("%s : %.2f \n", j->phys_char[i]->name, j->phys_char[i]->val);
           }
       }
    }
    return success;
}

status printPlanet(Planet *p) {
    if (p == NULL) {
        memoryProb = true;
        return failure;
    }
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", p->name, p->cordinate[0], p->cordinate[1], p->cordinate[2] );
    return success;
}

status delPlanet(Planet **p) {
    if (p == NULL) {
        return failure;
    }
    free((*p)->name);
    free(*p);
    *p = NULL;
    return success;
}

status delOrigin(Origin **o) {
    if (o == NULL) {
        return failure;
    }
    free((*o)->dim);
    free(*o);
    *o = NULL;
    return success;
}

status delPhysChar(PhysicalCharacteristics **p) {
    if (p == NULL) {
        return failure;
    }
    free((*p)->name);
    free(*p);
    *p = NULL;
    return success;
}

status delJerry(Jerry **j) {
    if (j == NULL) {
        return failure;
    }
    for (int i = 0; i < (*j)->phys_num; i++) {
        delPhysChar(&(*j)->phys_char[i]);
    }
    free((*j)->phys_char);
    delOrigin(&(*j)->origin);
    free((*j)->id);
    free(*j);
    *j = NULL;
    return success;
}










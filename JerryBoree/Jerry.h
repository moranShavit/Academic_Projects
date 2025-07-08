//
// Created by moran on 03/12/2024.
//

#ifndef JERRY_H
#define JERRY_H
#include <stdio.h>

#include "Defs.h"
#include <stdlib.h>
#include <string.h>


/**
 * @typedef Planet
 * @brief Represents a planet with a name and 3D coordinates.
 *
 * Members:
 *   - char *name: A pointer to a string that holds the planet's name.
 *   - float cordinate[3]: An array of three floats representing the planet's
 *                         coordinates in 3D space (x, y, z).
 */
typedef struct {
    char *name;
    float cordinate[3];
} Planet;

/**
 * @typedef Origin
 * @brief Represents an origin point associated with a planet and a dimension.
 *
 * Members:
 *   - Planet *planet: A pointer to a Planet structure associated with the origin.
 *   - char *dim: A pointer to a string that describes the dimension of the origin.
 */
typedef struct {
 Planet *planet;
 char *dim;
} Origin;

/**
 * @typedef PhysicalCharacteristics
 * @brief Represents a physical characteristic with a name and a value.
 *
 * Members:
 *   - char *name: A pointer to a string that holds the name of the characteristic.
 *   - float val: The value of the physical characteristic.
 */
typedef struct {
    char *name;
    float val;
} PhysicalCharacteristics;

/**
 * @typedef Jerry
 * @brief Represents a Jerry with various attributes including ID, happiness level, origin, and physical characteristics.
 *
 * Members:
 *   - char *id: A pointer to a string that holds the Jerry's unique identifier.
 *   - int happines_level: The happiness level of the Jerry.
 *   - Origin *origin: A pointer to the Origin structure representing the Jerry's origin.
 *   - int phys_num: The number of physical characteristics the Jerry possesses.
 *   - PhysicalCharacteristics **phys_char: A pointer to an array of pointers to
 *          PhysicalCharacteristics structures, representing the Jerry's physical traits.
 *
*/
typedef struct {
    char *id;
    int happines_level;
    Origin *origin;
    int phys_num;
    PhysicalCharacteristics **phys_char;
} Jerry;

/**
 * @function createPlanet
 * @brief Creates and initializes a new Planet object with the given name and 3D coordinates.
 *
 * This function allocates memory for a new Planet structure, assigns the provided name and coordinates
 * (x, y, z) to the planet, and initializes the name by allocating memory for it.
 *
 * @param name A string representing the planet's name.
 * @param x The x-coordinate of the planet.
 * @param y The y-coordinate of the planet.
 * @param z The z-coordinate of the planet.
 *
 * @return Planet* A pointer to the newly created Planet object, or NULL if memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
Planet *createPlanet(char name[], float x, float y, float z);
/**
 * @function createPhysChar
 * @brief Creates and initializes a new PhysicalCharacteristics object with the given name and value.
 *
 * This function allocates memory for a new PhysicalCharacteristics structure, assigns the provided name
 * and value to the characteristic, and initializes the name by allocating memory for it.
 *
 * @param name A string representing the name of the physical characteristic.
 * @param val The value of the physical characteristic.
 *
 * @return PhysicalCharacteristics* A pointer to the newly created PhysicalCharacteristics object, or NULL if memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
PhysicalCharacteristics *createPhysChar(char name[], float val);
/**
 * @function physExcit
 * @brief Checks if a specific physical characteristic exists for a given Jerry.
 *
 * This function iterates through the array of physical characteristics of a Jerry and compares
 * each characteristic's name with the provided name. It returns `true` if a match is found,
 * indicating that the physical characteristic exists for the Jerry; otherwise, it returns `false`.
 *
 * @param j A pointer to the Jerry structure.
 * @param name A string representing the name of the physical characteristic to check.
 *
 * @return bool `true` if the physical characteristic exists, `false` otherwise.
 */
bool physExcit(Jerry *j, char name[]);
/**
 * @function addPhys
 * @brief Adds a new physical characteristic to a Jerry if it doesn't already exist.
 *
 * This function checks if the given physical characteristic already exists for the Jerry by comparing
 * its name with the existing physical characteristics. If the characteristic doesn't exist, it is added
 * to the Jerry's list of physical characteristics. The function reallocates memory for the characteristics array
 * and updates the `phys_num` to reflect the new addition.
 *
 * @param j A pointer to the Jerry structure.
 * @param p A pointer to the PhysicalCharacteristics structure to add.
 *
 * @return status Returns `success` if the physical characteristic was added,
 *         or `failure` if the characteristic already exists or if memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
status addPhys(Jerry *j, PhysicalCharacteristics *p);
/**
 * @function delPhysByName
 * @brief Deletes a physical characteristic from a Jerry by its name.
 *
 * This function first checks if the physical characteristic exists using the `physExcit` function. If the characteristic
 * exists, it is removed by shifting the subsequent characteristics to fill the gap. The array of physical characteristics
 * is then reallocated to reflect the updated size.
 *
 * @param j A pointer to the Jerry structure.
 * @param physName A string representing the name of the physical characteristic to remove.
 *
 * @return status Returns `success` if the characteristic was removed,
 *         or `failure` if the characteristic does not exist or memory allocation fails.
 *
 * @note This function may set a global flag `memoryProb` to `true` if memory allocation fails.
 */
status delPhysByName(Jerry *j, char physName[]);
/**
* @function printJerry
* @brief Prints the details of a Jerry object, including its ID, happiness level, origin, planet, and physical characteristics.
*
* This function prints information about the Jerry, such as its ID, happiness level, origin dimension, associated planet,
* and the planet's coordinates. If the Jerry has any physical characteristics, it prints each characteristic's name and value.
*
* @param j A pointer to the Jerry structure to print.
*
* @return status Returns `success` if the information is printed successfully,
*         or `failure` if the Jerry pointer is `NULL`.
*/
status printJerry(Jerry *j);
/**
 * @function printPlanet
 * @brief Prints the details of a Planet object, including its name and coordinates.
 *
 * This function prints the name of the planet and its coordinates in 3-dimensional space (x, y, z).
 *
 * @param p A pointer to the Planet structure to print.
 *
 * @return status Returns `success` if the information is printed successfully,
 *         or `failure` if the Planet pointer is `NULL`.
 */
status printPlanet(Planet *p);
/**
 *@function delPlanet
 * @brief Frees the memory allocated for a planet and its associated resources.
 *
 * This function frees the memory allocated for the planet's name and the planet object itself.
 * It also sets the caller's pointer to `NULL` to prevent a dangling pointer.
 *
 * @param[in,out] p A pointer to the pointer of the `Planet` object to be freed.
 *                   After the function, the caller's `Planet *` pointer will be set to `NULL`.
 *
 * @return status The status of the operation:
 *                 - `success` if the memory was successfully freed and the pointer was set to `NULL`.
 *                 - `failure` if the provided pointer is `NULL` or if any memory freeing operation fails.
 *
 * @note After calling this function, the caller's `Planet *` pointer will be set to `NULL`,
 *       and it should no longer be used.
 */
status delPlanet(Planet **p);
/**
* @brief Frees the memory allocated for an origin and its associated resources.
*
* This function frees the memory allocated for the dimension string and the `Origin` object itself.
* It also sets the caller's pointer to `NULL` to prevent a dangling pointer.
*
* @param o A pointer to the pointer of the `Origin` object to be freed.
*                   After the function, the caller's `Origin *` pointer will be set to `NULL`.
*
* @return status The status of the operation:
*                 - `success` if the memory was successfully freed and the pointer was set to `NULL`.
*                 - `failure` if the provided pointer is `NULL` or if any memory freeing operation fails.
*
* @note After calling this function, the caller's `Origin *` pointer will be set to `NULL`,
*       and it should no longer be used.
*/
status delOrigin(Origin **o);
/**
 *@function delPhysChar
 * @brief Frees the memory allocated for a physical characteristic and its associated resources.
 *
 * This function frees the memory allocated for the name string and the `PhysicalCharacteristics` object itself.
 * It also sets the caller's pointer to `NULL` to prevent a dangling pointer.
 *
 * @param p A pointer to the pointer of the `PhysicalCharacteristics` object to be freed.
 *                   After the function, the caller's `PhysicalCharacteristics *` pointer will be set to `NULL`.
 *
 * @return status The status of the operation:
 *                 - `success` if the memory was successfully freed and the pointer was set to `NULL`.
 *                 - `failure` if the provided pointer is `NULL` or if any memory freeing operation fails.
 *
 * @note After calling this function, the caller's `PhysicalCharacteristics *` pointer will be set to `NULL`,
 *       and it should no longer be used.
 */
status delPhysChar(PhysicalCharacteristics **p);
/**
 *@function delJerry
 * @brief Frees the memory allocated for a Jerry object and its associated resources.
 *
 * This function frees the memory allocated for the `Jerry` object, including its physical characteristics,
 * origin, and other associated resources. It also sets the caller's pointer to `NULL` to prevent a dangling pointer.
 *
 * @param[in,out] j A pointer to the pointer of the `Jerry` object to be freed.
 *                   After the function, the caller's `Jerry *` pointer will be set to `NULL`.
 *
 * @return status The status of the operation:
 *                 - `success` if the memory was successfully freed and the pointer was set to `NULL`.
 *                 - `failure` if the provided pointer is `NULL` or if any memory freeing operation fails.
 *
 * @note After calling this function, the caller's `Jerry *` pointer will be set to `NULL`,
 *       and it should no longer be used.
 */
status delJerry(Jerry **j);
/**
 * @brief Creates a new Jerry with a specified origin planet and dimension.
 *
 * This function creates a Jerry by first initializing its origin with the provided
 * planet and dimension. If any part of the creation process fails, the function
 * handles memory cleanup and sets the global `memoryProb` flag.
 *
 * @param id The ID of the Jerry (string).
 * @param happiness The initial happiness level of the Jerry (integer).
 * @param planet Pointer to the Jerry's origin planet.
 * @param dim The dimension of the Jerry's origin (string).
 *
 * @return
 * - Pointer to the newly created Jerry if successful.
 * - `NULL` if any memory allocation or initialization fails.
 *
 * @note If memory allocation fails at any step, the function cleans up
 *       partially created structures to prevent memory leaks.
 */
Jerry *createJerry_with_planet(char id[], int happiness, Planet* planet, char dim[]);

#endif //JERRY_H

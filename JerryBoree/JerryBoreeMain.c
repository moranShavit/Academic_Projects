#include <stdio.h>
#include "Defs.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include "Jerry.h"
#include <math.h>

bool memoryProb = false;
void preprocess(Planet **planet_array, linked_list jerry_list, char confi[], int planet_num);
Element fake_copy(Element element);
status fake_free(Element element);
bool comp_by_id(Element id_1, Element id_2);
status jerry_elem_print(Element element);
Element jerry_get_id(Element element);
status str_as_elem_print(Element element);
status free_jerry_elem(Element element);
int str_to_num(Element element);
status str_as_elem_free(Element element);
Element str_as_elem_copy(Element element);
bool jerry_as_elem_comp(Element j_1, Element j_2);
void print_main_menu();
void close_program(MultiValueHashTable mtv, linked_list list, hashTable table, Planet** p_arr, int planet_num);
void valid_input_check(char input[], int* out_p_hold);
Planet* find_planet(Planet** p_arr, char* planet_name, int planet_num);
status add_to_system(MultiValueHashTable mtv, linked_list list, hashTable table, Jerry* j);
void adjust_happiness(linked_list jerry_l, int min, int subtraction, int add);
bool isPrime(int number);
int nextPrime(int number);
void option_1(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, Planet** p_arr, int planet_num , char* buffer);
void option_2(MultiValueHashTable phys_t, hashTable id_t, char* buffer);
void option_3(MultiValueHashTable phys_t, hashTable id_t, char* buffer);
void remove_jerry_from_system(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, Jerry* j);
void option_4(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, char* buffer);
Jerry* find_closest_jerry_by_phys(linked_list ll , char* phys_name, float val);
void option_5(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, char* buffer);
void option_6(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l);
void option_7(MultiValueHashTable phys_t, linked_list jerry_l, Planet** p_arr, int planet_num , char* buffer);
void option_8(linked_list jerry_l, char* buffer);

int main(int argc, char **argv) {
    /**
     * @brief Entry point for the Jerry Daycare system.
     *
     * This program initializes and manages a daycare system for Jerries. It reads data from
     * input files, creates and maintains data structures for storing and accessing information
     * about Jerries and planets, and provides an interactive menu for user operations.
     *
     * @param argc Number of command-line arguments.
     * @param argv Array of command-line arguments:
     *             - argv[1]: Number of planets (integer).
     *             - argv[2]: Path to the configuration file.
     *
     * @return Always exits the program after completing all user interactions.
     *
     * @details
     * - Initializes dynamic arrays and data structures (linked list, hash tables, etc.).
     * - Processes input data to populate planets and Jerries.
     * - Provides a menu-driven interface for user operations such as adding, removing,
     *   and displaying Jerries or planets.
     * - Cleans up all allocated memory before exiting.
     *
     * @note
     * - If any memory allocation fails during initialization or execution, the program exits
     *   with an error message.
     */
    int planet_num = atoi(argv[1]);
    Planet **planet_array = (Planet **) malloc(planet_num * sizeof(Planet *));
    if (planet_array == NULL) memoryProb = true;
    linked_list jerry_list = createLinkedList(fake_copy, comp_by_id, jerry_elem_print, free_jerry_elem, jerry_get_id);
    if (jerry_list == NULL) memoryProb = true;
    preprocess(planet_array, jerry_list, argv[2], planet_num);
    if (memoryProb) {
        destroyList(jerry_list);
        if (planet_array != NULL) {
            for (int i = 0; i < planet_num; i++) {
                if (planet_array[i] != NULL) {
                    delPlanet(&(planet_array[i]));
                }
            }
            free(planet_array);
        }
    }
    int next_prime = nextPrime(getLengthList(jerry_list));
    hashTable id_table = createHashTable(fake_copy, fake_free, str_as_elem_print, fake_copy,
        fake_free, jerry_elem_print, comp_by_id,
        str_to_num, next_prime);
    if (id_table == NULL) memoryProb = true;
    MultiValueHashTable phys_table = createMultiValueHashTable(str_as_elem_copy, str_as_elem_free,
        str_as_elem_print,fake_copy, fake_free, jerry_elem_print, comp_by_id,
        jerry_as_elem_comp,str_to_num, next_prime);
    if(phys_table == NULL) memoryProb = true;
    Element elem;
    if (!memoryProb && getLengthList(jerry_list) > 0) {
        list_forEach(elem, jerry_list) {
            Jerry* temp = (Jerry*) elem;
            addToHashTable(id_table,temp->id,temp );
            for (int i=0; i < temp->phys_num; i++) {
                PhysicalCharacteristics* phys = temp->phys_char[i];
                addToMultiValueHashTable(phys_table, phys->name, temp);
            }
        }
    }
    int user_input;
    char buffer[301];
    while (true) {
        if (memoryProb) user_input = 9;
        else {
            print_main_menu();
            scanf("%s", buffer);
            valid_input_check(buffer, &user_input);
        }
        switch (user_input) {
            case 1:
                option_1(phys_table, id_table, jerry_list, planet_array, planet_num, buffer);
                break;
            case 2:
                option_2(phys_table, id_table, buffer);
                break;
            case 3:
                option_3(phys_table, id_table, buffer);
                break;
            case 4:
                option_4(phys_table, id_table, jerry_list, buffer);
                break;
            case 5:
                option_5(phys_table, id_table, jerry_list, buffer);
                break;
            case 6:
                option_6(phys_table, id_table, jerry_list);
                break;
            case 7:
                option_7(phys_table, jerry_list, planet_array, planet_num, buffer);
                break;
            case 8:
                option_8(jerry_list, buffer);
                break;
            case 9:
                close_program(phys_table, jerry_list, id_table, planet_array, planet_num);
                if (memoryProb) {
                    printf("A memory problem has been detected in the program \n");
                    exit(1);
                }
                printf("The daycare is now clean and close ! \n");
                exit(0);
            default:
                printf("Rick this option is not known to the daycare ! \n");
                while (getchar() != '\n');
        }
    }
}

void preprocess(Planet **planet_array, linked_list jerry_list, char confi[], int planet_num) {
    /**
 * @brief Processes a configuration file to initialize planets and Jerries.
 *
 * This function reads data from a configuration file and populates the given arrays and
 * data structures with information about planets and Jerries. It first processes the
 * "Planets" section of the file to create planet objects and stores them in the provided
 * planet array. It then processes the "Jerries" section to create Jerry objects, including
 * their physical characteristics, and adds them to the provided linked list.
 *
 * @param planet_array Pointer to an array for storing planet objects.
 * @param jerry_list Linked list for storing Jerry objects.
 * @param confi The file path to the configuration file.
 * @param planet_num The number of planets to be processed.
 *
 * @note
 * - The file must follow the expected format with sections labeled "Planets" and "Jerries".
 * - Physical characteristics of Jerries must be indented with a tab character in the file.
 * - The function sets a global `memoryProb` flag to `true` if any memory allocation fails.
 *
 * @warning
 * - Ensure the `planet_array` and `jerry_list` are properly allocated and passed as arguments.
 * - If `memoryProb` is set to `true`, the caller should handle memory cleanup appropriately.
 */
    bool planet = false;
    //bool phys = false;
    //char conf[301];
    char line_token[301];
    char *planet_token;
    char *jerry_token;
    char *phys_token;
    int planet_count = 0;
    Jerry *new_jerry;

    FILE *file = fopen(confi, "r");

    // this loop iterate the text file confi line by line
    while (fgets(line_token, sizeof(line_token), file)) {
        if (memoryProb == true) {
            break;
        }
        // check if the current part of the file is the planets information
        line_token[strcspn(line_token, "\n")] = '\0';
        if (strcmp(line_token, "Planets") == 0) {
            planet = true;
        }
        // check if the next part of the file is the jerries part
        else if (strcmp(line_token, "Jerries") == 0) {
            planet = false;
        }

        //this part process the planets and save them in the given array
        else if (planet == true) {
            // break each line that represent a planet to tokens by "," delimiter
            planet_token = strtok(line_token, ",");
            float planet_data[3];
            char  planet_id[strlen(planet_token) +1];
            strcpy(planet_id, planet_token);
            // update the token to the cordinates part
            planet_token = strtok(NULL, ",");

            //save the cordinates
            for (int i = 0; i < 3; i++) {
                planet_data[i] = atof(planet_token);
                planet_token = strtok(NULL, ",");
            }
            Planet *temp_planet = createPlanet(planet_id, planet_data[0], planet_data[1], planet_data[2]);
            if (temp_planet == NULL) {
                memoryProb = true;
                break;
            }
            //insert the current planet to the planets array
            planet_array[planet_count] = temp_planet;
            planet_count++;
        }

        // this part process the jerries and save them in the given ADT'S
        else if (planet == false) {

            // check if new jerry data start from this line
            if (!(line_token[0] == '\t')) {

                //process the jerry data exept the physical charecters
                jerry_token = strtok(line_token, ",");
                char jerry_id[strlen(jerry_token)+1];
                strcpy(jerry_id, jerry_token);
                jerry_token = strtok(NULL, ",");
                char jerry_dim[strlen(jerry_token)+1];
                strcpy(jerry_dim, jerry_token);
                jerry_token = strtok(NULL, ",");
                Planet *jerry_planet = NULL;
                for (int i = 0; i < planet_num; i++) {
                    if (strcmp(planet_array[i]->name,jerry_token) == 0) {
                        jerry_planet = planet_array[i];
                    }
                }
                if (jerry_planet == NULL) {
                    memoryProb = true;
                }
                jerry_token = strtok(NULL, ",");
                int jerry_hap = atoi(jerry_token);
                Jerry *temp_jerry = createJerry_with_planet(jerry_id, jerry_hap, jerry_planet,jerry_dim);
                if (temp_jerry == NULL) {
                    memoryProb = true;
                    break;
                }
                new_jerry = temp_jerry;
                if (appendNode(jerry_list, new_jerry) == failure) memoryProb = true;
            }

            //add the physical characters to the jerry
            else {
                // drop the tab character and break the physical character line to name and value
                phys_token = strtok(line_token+1, ":");
                char pyhs_name[strlen(phys_token)+1];
                strcpy(pyhs_name, phys_token);

                //update the token to the value part
                phys_token = strtok(NULL, ":");
                float phys_val = atof(phys_token);
                PhysicalCharacteristics *temp_phys = createPhysChar(pyhs_name, phys_val);
                if (temp_phys == NULL) {
                    memoryProb = true;
                    break;
                }
                // add the physical character to the current jerry processed
                addPhys(new_jerry, temp_phys);
            }
        }
    }
    fclose(file);
}

Element fake_copy(Element element) {
    return  element;
}

status fake_free(Element element) {
    return success;
}

bool comp_by_id(Element id_1, Element id_2) {
    /**
 * @brief Compares two IDs for equality.
 *
 * This function checks if two IDs, represented as strings, are equal by performing
 * a string comparison.
 *
 * @param id_1 The first ID to compare (as an element).
 * @param id_2 The second ID to compare (as an element).
 *
 * @return
 * - `true` if the IDs are equal.
 * - `false` if the IDs are not equal or if either input is `NULL`.
 */
    if (id_1 == NULL || id_2 == NULL) return false;
    int res = strcmp((char*)id_1, (char*)id_2);
    if (res == 0) return true;
    return false;
}

status jerry_elem_print(Element element) {
    /**
 * @brief Prints a Jerry object.
 *
 * This function prints the details of a Jerry object using the appropriate print function.
 *
 * @param element The Jerry object to be printed (as an element).
 *
 * @return
 * - `success` if the Jerry object is printed successfully.
 * - `failure` if the input element is `NULL` or the print operation fails.
 */
    if (element == NULL) return failure;
    Jerry* temp = (Jerry*) element;
    if (printJerry(temp) == failure) return failure;
    return success;
}

Element jerry_get_id(Element element) {
    /**
 * @brief Retrieves the ID of a Jerry object.
 *
 * This function extracts and returns the ID of a given Jerry object.
 *
 * @param element The Jerry object (as an element).
 *
 * @return
 * - The ID of the Jerry object if the input is valid.
 * - `NULL` if the input element is `NULL`.
 */
    if (element == NULL) return NULL;
    Jerry* temp = (Jerry*) element;
    return temp->id;
}

status str_as_elem_print(Element element) {
    /**
 * @brief Prints a string when treated as an element.
 *
 * This function prints a string, represented as an element, to the standard output.
 *
 * @param element The string to be printed (as an element).
 *
 * @return
 * - `success` if the string is printed successfully.
 * - `failure` if the input element is `NULL`.
 */
    if (element == NULL) return failure;
    char* temp = (char*) element;
    printf("%s\n", temp);
    return success;
}

status free_jerry_elem(Element element) {
    /**
 * @brief Frees a Jerry object when treated as an element.
 *
 * This function deallocates all memory associated with a Jerry object and its attributes.
 *
 * @param element The Jerry object to be freed (as an element).
 *
 * @return
 * - `success` if the Jerry object is successfully freed.
 * - `failure` if the input element is `NULL`.
 */
    if (element == NULL) return failure;
    Jerry* temp = (Jerry*) element;
    delJerry(&temp);
    return success;
}

int str_to_num(Element element) {
    /**
 * @brief Converts a string to a numeric value.
 *
 * This function calculates a numeric value for a given string by summing the ASCII values
 * of its characters. It is typically used as a hash function for strings.
 *
 * @param element The string to be converted (as an element).
 *
 * @return
 * - The numeric representation of the string.
 * - `0` if the input element is `NULL`.
 */
    if (element == NULL) return 0;
    char* str = (char*) element;
    int res = 0;
    while (*str) {
        res = res + *str; // Shift left and add ASCII value
        str++;
    }
    return res;
}

status str_as_elem_free(Element element) {
    /**
 * @brief Frees a string when treated as an element.
 *
 * This function deallocates memory for a string represented as an element.
 *
 * @param element The string to be freed (as an element).
 *
 * @return
 * - `success` if the string is successfully freed.
 * - `failure` if the input element is `NULL`.
 */
    if (element == NULL) return failure;
    free((char*) element);
    return success;
}

bool isPrime(int number) {
    /**
     * @brief Checks if a number is prime.
     *
     * This function determines whether the given integer is a prime number.
     * A prime number is greater than 1 and has no divisors other than 1 and itself.
     *
     * @param number The integer to check for primality.
     * @return
     * - `true` if the number is prime.
     * - `false` otherwise.
     */
    if (number <= 1) {
        return false; // Numbers <= 1 are not prime
    }
    if (number <= 3) {
        return true; // 2 and 3 are prime
    }
    if (number % 2 == 0 || number % 3 == 0) {
        return false; // Eliminate multiples of 2 and 3
    }

    // Check for factors up to the square root of the number
    for (int i = 5; i <= number/2; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

int nextPrime(int number) {
    /**
     * @brief Finds the next prime number greater than the given number.
     *
     * This function computes the smallest prime number that is strictly greater than the input value.
     * If the input number is less than or equal to 1, it returns 2 (the smallest prime number).
     *
     * @param number The integer from which to find the next prime number.
     * @return The next prime number greater than the input.
     */
    if (number <= 1) {
        return 2; // The first prime number
    }

    int candidate = number + 1; // Start checking from the next number

    while (!isPrime(candidate)) {
        candidate++;
    }

    return candidate;
}

Element str_as_elem_copy(Element element) {
    /**
 * @brief Creates a copy of a string when treated as an element.
 *
 * This function allocates memory for a new string and copies the content of the input string into it.
 *
 * @param element The string to be copied (as an element).
 *
 * @return
 * - A pointer to the newly allocated copy of the string.
 * - `NULL` if the input element is `NULL` or memory allocation fails.
 */
    if (element == NULL) return NULL;
    char* temp = (char*) element;
    char* copy = (char*) malloc(sizeof(char) * strlen(temp) + 1);
    strcpy(copy, temp);
    return copy;
}

bool jerry_as_elem_comp(Element j_1, Element j_2) {
    /**
 * @brief Compares two Jerry objects for equality based on their IDs.
 *
 * This function checks if two Jerry objects are equal by comparing their IDs.
 *
 * @param j_1 The first Jerry object (as an element).
 * @param j_2 The second Jerry object (as an element).
 *
 * @return
 * - `true` if the IDs of both Jerry objects are equal.
 * - `false` if the IDs are not equal or if either input is `NULL`.
 */
    if (j_1 == NULL || j_2 == NULL) return false;
    bool res = comp_by_id(((Jerry*) j_1)->id, ((Jerry*) j_2)->id);
    return res;
}

void print_main_menu() {
    printf("Welcome Rick, what are your Jerry's needs today ? \n"
                  "1 : Take this Jerry away from me \n"
                  "2 : I think I remember something about my Jerry \n"
                  "3 : Oh wait. That can't be right \n"
                  "4 : I guess I will take back my Jerry now \n"
                  "5 : I can't find my Jerry. Just give me a similar one \n"
                  "6 : I lost a bet. Give me your saddest Jerry \n"
                  "7 : Show me what you got \n"
                  "8 : Let the Jerries play \n"
                  "9 : I had enough. Close this place \n");
}

void close_program(MultiValueHashTable mtv, linked_list list, hashTable table, Planet** p_arr, int planet_num) {
    /**
 * @brief Frees all allocated resources and closes the program.
 *
 * This function deallocates all memory associated with the program, including the multi-value
 * hash table, linked list, hash table, and planet array.
 *
 * @param mtv Pointer to the multi-value hash table.
 * @param list Pointer to the linked list.
 * @param table Pointer to the hash table.
 * @param p_arr Pointer to the array of planets.
 * @param planet_num The number of planets in the array.
 *
 * @return Void. The function has no return value.
 */
    destroyMultiValueHashTable(mtv);
    destroyHashTable(table);
    destroyList(list);
    if (p_arr != NULL) {
        for (int i = 0; i < planet_num; i++) {
            if (p_arr[i] != NULL) {
                delPlanet(&(p_arr[i]));
            }
        }
        free(p_arr);
    }
}

void valid_input_check(char input[], int* out_p_hold) {
    /**
 * @brief Validates and processes user input for menu options.
 *
 * This function checks if the input string represents a valid menu option (1-9).
 * If valid, it converts the string to an integer and stores the result in the provided pointer.
 * Otherwise, it sets the output to 0.
 *
 * @param input The input string to validate.
 * @param out_p_hold Pointer to an integer where the validated output will be stored.
 *
 * @return Void. The function directly modifies the value pointed to by `out_p_hold`.
 */
    if (strcmp(input, "1") == 0 || strcmp(input, "2") == 0 || strcmp(input, "3") == 0 ||
             strcmp(input, "4") == 0 || strcmp(input, "5") == 0 || strcmp(input, "6") == 0 ||
             strcmp(input, "7") == 0 || strcmp(input, "8") == 0 || strcmp(input, "9") == 0) {
        *out_p_hold = atoi(input);
    }else {
        *out_p_hold = 0;
    }
}

Planet* find_planet(Planet** p_arr, char* planet_name,int planet_num) {
    /**
 * @brief Finds a planet by name in the planet array.
 *
 * This function searches for a planet in the provided array by comparing its name
 * with the given planet name.
 *
 * @param p_arr Pointer to the array of planets.
 * @param planet_name The name of the planet to search for.
 * @param planet_num The number of planets in the array.
 *
 * @return
 * - A pointer to the planet if found.
 * - `NULL` if the planet is not found or the input is invalid.
 */
    for (int i = 0; i < planet_num; i++) {
        if (strcmp(p_arr[i]->name,planet_name) == 0) {
            return p_arr[i];
        }
    }
    return NULL;
}

status add_to_system(MultiValueHashTable mtv, linked_list list, hashTable table, Jerry* j) {
    /**
    * @brief Adds a Jerry to the system, updating all relevant data structures.
    *
    * This function adds a Jerry object to the linked list, a hash table by ID, and a multi-value
    * hash table by its physical characteristics. All data structures will point to the same Jerry object.
    *
    * @param mtv Pointer to the multi-value hash table for physical characteristics.
    * @param list Pointer to the linked list of Jerries.
    * @param table Pointer to the hash table of Jerries indexed by their ID.
    * @param j Pointer to the Jerry object to be added.
    *
    * @return
    * - `success` if the Jerry is successfully added to all structures.
    * - `failure` if any of the additions fail.
    */
    if (j == NULL) return failure;
    if (appendNode(list, j) == failure) return failure;
    if (addToHashTable(table, j->id, j) == failure) return failure;
    for (int i=0; i < j->phys_num; i++) {
        PhysicalCharacteristics* phys = j->phys_char[i];
        if (addToMultiValueHashTable(mtv, phys->name, j) == failure) return failure;
    }
    return success;
}

void remove_jerry_from_system(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, Jerry* j) {
    /**
     * @brief Removes a Jerry from the system, updating all relevant data structures.
     *
     * This function removes a Jerry object from the linked list, hash table by ID, and
     * multi-value hash table by its physical characteristics. All references to the Jerry
     * in the system's data structures are cleared.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table of Jerries indexed by their ID.
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param j Pointer to the Jerry object to be removed.
     *
     * @return Void. The function modifies the data structures directly.
     */
    for (int i=0; i < j->phys_num; i++) {
        removeFromMultiValueHashTable(phys_t, j->phys_char[i]->name, j);
    }
    removeFromHashTable(id_t, j->id);
    deleteNode(jerry_l, j);
}

Jerry* find_closest_jerry_by_phys(linked_list ll , char* phys_name, float val) {
    /**
     * @brief Finds the closest Jerry based on a specified physical characteristic value.
     *
     * This function iterates through a linked list of Jerries to find the Jerry whose
     * physical characteristic value is closest to the specified value for the given
     * physical characteristic name.
     *
     * @param ll Pointer to the linked list of Jerries.
     * @param phys_name The name of the physical characteristic to compare.
     * @param val The target value for the physical characteristic.
     *
     * @return
     * - A pointer to the Jerry with the closest physical characteristic value.
     * - `NULL` if no matching physical characteristic is found in the list.
     */
    Element elem;
    Jerry* min_j = NULL;
    float min_dif = INFINITY;
    list_forEach(elem, ll) {
        Jerry* j = (Jerry*) elem;
        for (int i=0; i < j->phys_num; i++) {
            if (strcmp(phys_name, j->phys_char[i]->name) == 0) {
                if (fabs(val - j->phys_char[i]->val) < min_dif) {
                    min_dif = fabs(val - j->phys_char[i]->val);
                    min_j = j;
                }
                continue;
            }
        }
    }
    return min_j;
}

void adjust_happiness(linked_list jerry_l, int min, int subtraction, int add) {
    /**
     * @brief Adjusts the happiness levels of all Jerries in the linked list.
     *
     * This function modifies the happiness level of each Jerry in the linked list based on
     * a minimum threshold. If a Jerry's happiness level is below the threshold, it is
     * decreased by a specified value. Otherwise, it is increased. The happiness level is
     * capped at 100 and floored at 0.
     *
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param min The minimum happiness threshold.
     * @param subtraction The amount to subtract from happiness if below the threshold.
     * @param add The amount to add to happiness if above the threshold.
     *
     * @return Void. The function modifies the Jerries' happiness levels directly and prints
     *         the updated list at the end.
     */
    Element elem;
    list_forEach(elem, jerry_l) {
        Jerry* j = (Jerry*) elem;
        if (j->happines_level < min) {
            j->happines_level = j->happines_level - subtraction;
        }else {
            j->happines_level = j->happines_level + add;
        }
        if (j->happines_level > 100) j->happines_level = 100;
        if (j->happines_level < 0) j->happines_level = 0;
    }
    printf("The activity is now over ! \n");
    displayList(jerry_l);
}

void option_1(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, Planet** p_arr, int planet_num , char* buffer) {
    /**
     * @brief Adds a new Jerry to the system based on user input.
     *
     * This function prompts the user for details about a new Jerry, including its ID, planet,
     * dimension, and happiness level. It then creates the Jerry object and adds it to the
     * system's data structures, including the linked list, hash table by ID, and the
     * multi-value hash table for physical characteristics.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param p_arr Pointer to the array of planets.
     * @param planet_num The number of planets in the array.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function directly modifies the system's data structures or sets
     *         the `memoryProb` flag to `true` in case of memory allocation issues.
     */
    printf("What is your Jerry's ID ? \n");
    scanf("%s", buffer);
    if (lookupInHashTable(id_t, buffer) != NULL) {
        printf("Rick did you forgot ? you already left him here ! \n");
        return;
    }
    char* id = (char*) malloc(sizeof(char) * (strlen(buffer)+1));
    if (id == NULL) {
        memoryProb = true;
        return;
    }
    strcpy(id, buffer);
    printf("What planet is your Jerry from ? \n");
    scanf("%s", buffer);
    Planet* planet  = find_planet(p_arr, buffer, planet_num);
    if (planet == NULL) {
        printf("%s is not a known planet ! \n", buffer);
        free(id);
        return;
    }
    printf("What is your Jerry's dimension ? \n");
    scanf("%s", buffer);
    char* dimension = (char*) malloc(sizeof(char) * (strlen(buffer)+1));
    if (dimension == NULL) {
        memoryProb = true;
        free(id);
        return;
    }
    strcpy(dimension, buffer);
    printf("How happy is your Jerry now ? \n");
    int hap;
    scanf("%d", &hap);
    Jerry *j = createJerry_with_planet(id, hap, planet, dimension);
    if (j == NULL) {
        memoryProb = true;
        free(id);
        free(dimension);
        return;
    }
    if (add_to_system(phys_t, jerry_l, id_t, j) == failure) {
        memoryProb = true;
        free(id);
        free(dimension);
        return;
    }
    free(id);
    free(dimension);
    printJerry(j);
}

void option_2(MultiValueHashTable phys_t, hashTable id_t, char* buffer) {
    /**
     * @brief Adds a new physical characteristic to an existing Jerry.
     *
     * This function prompts the user to specify a physical characteristic and its value
     * for a specific Jerry by ID. If the characteristic is new, it is added to the Jerry
     * and the system's multi-value hash table.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function modifies the system's data structures directly or sets
     *         the `memoryProb` flag to `true` in case of memory allocation issues.
     */
    printf("What is your Jerry's ID ? \n");
    scanf("%s", buffer);
    Jerry* j = (Jerry*) lookupInHashTable(id_t, buffer);
    if (j == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return;
    }
    printf("What physical characteristic can you add to Jerry - %s ? \n", buffer);
    scanf("%s", buffer);
    if (physExcit(j, buffer) == true) {
        printf("The information about his %s already available to the daycare ! \n", buffer);
        return;
    }
    float val;
    printf("What is the value of his %s ? \n", buffer);
    scanf("%f", &val);
    PhysicalCharacteristics *p = createPhysChar(buffer, val);
    if (p == NULL) {
        memoryProb = true;
        return;
    }
    if (addPhys(j, p) == failure) return;
    if (addToMultiValueHashTable(phys_t, p->name, j) == failure) {
        delPhysChar(&p);
        memoryProb = true;
        return;
    }
    linked_list l = (linked_list) lookupInMultiValueHashTable(phys_t, p->name);
    printf("%s : \n", p->name);
    displayList(l);
}

void option_3(MultiValueHashTable phys_t, hashTable id_t, char* buffer) {
    /**
     * @brief Removes a physical characteristic from an existing Jerry.
     *
     * This function prompts the user to specify a physical characteristic to be removed
     * from a specific Jerry identified by ID. The characteristic is deleted from the Jerry,
     * and the Jerry is removed from the corresponding node in the multi-value hash table.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function directly updates the system's data structures.
     */
    printf("What is your Jerry's ID ? \n");
    scanf("%s", buffer);
    Jerry* j = (Jerry*) lookupInHashTable(id_t, buffer);
    if (j == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n", buffer);
    scanf("%s", buffer);
    if (physExcit(j, buffer) == false) {
        printf("The information about his %s not available to the daycare ! \n", buffer);
        return;
    }
    delPhysByName(j, buffer);
    removeFromMultiValueHashTable(phys_t,buffer, j);
    printJerry(j);
}

void option_4(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, char* buffer) {
    /**
     * @brief Removes a Jerry from the daycare system.
     *
     * This function prompts the user for a Jerry ID and removes the specified Jerry
     * from the linked list, hash table by ID, and the multi-value hash table for physical characteristics.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function directly updates the system's data structures.
     */
    printf("What is your Jerry's ID ? \n");
    scanf("%s", buffer);
    Jerry* j = (Jerry*) lookupInHashTable(id_t, buffer);
    if (j == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return;
    }
    remove_jerry_from_system(phys_t, id_t, jerry_l, j);
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
}

void option_5(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l, char* buffer) {
    /**
     * @brief Finds and removes the closest matching Jerry based on a physical characteristic.
     *
     * This function prompts the user to specify a physical characteristic and its value
     * to search for a Jerry. It identifies the Jerry with the closest matching value
     * for the specified characteristic and removes it from the daycare system.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function directly updates the system's data structures.
     */
    printf("What do you remember about your Jerry ? \n");
    scanf("%s", buffer);
    linked_list ll = (linked_list) lookupInMultiValueHashTable(phys_t,buffer);
    if (ll == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", buffer);
        return;
    }
    float val;
    printf("What do you remember about the value of his %s ? \n", buffer);
    scanf("%f", &val);
    Jerry* j = find_closest_jerry_by_phys(ll, buffer, val);
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(j);
    remove_jerry_from_system(phys_t, id_t, jerry_l, j);
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
}

void option_6(MultiValueHashTable phys_t, hashTable id_t, linked_list jerry_l) {
    /**
     * @brief Finds and removes the least happy Jerry from the daycare system.
     *
     * This function identifies the Jerry with the lowest happiness level in the daycare system
     * and removes it from the linked list, hash table by ID, and the multi-value hash table for
     * physical characteristics.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param id_t Pointer to the hash table for Jerries indexed by ID.
     * @param jerry_l Pointer to the linked list of Jerries.
     *
     * @return Void. The function directly updates the system's data structures.
     *
     * @note If no Jerries are present in the daycare, the function prints a message and exits without action.
     */
    if (getLengthList(jerry_l) == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return;
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    Element elem;
    Jerry* min_j = NULL;
    float min_hap = INFINITY;
    list_forEach(elem, jerry_l) {
        Jerry* j = (Jerry*) elem;
        if (j->happines_level < min_hap) {
            min_hap = j->happines_level;
            min_j = j;
        }
    }
    printJerry(min_j);
    remove_jerry_from_system(phys_t, id_t, jerry_l, min_j);
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
}

void option_7(MultiValueHashTable phys_t, linked_list jerry_l, Planet** p_arr, int planet_num , char* buffer) {
    /**
     * @brief Displays information about Jerries or planets based on user input.
     *
     * This function provides the user with options to display all Jerries, Jerries by a specific
     * physical characteristic, or all known planets in the daycare system. The user selects an
     * option, and the function retrieves and displays the corresponding data.
     *
     * @param phys_t Pointer to the multi-value hash table for physical characteristics.
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param p_arr Pointer to the array of planets.
     * @param planet_num The number of planets in the array.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function displays the requested information directly.
     *
     * @note If no data is available for the selected option, an appropriate message is printed.
     */
    printf("What information do you want to know ? \n"
           "1 : All Jerries \n"
           "2 : All Jerries by physical characteristics \n"
           "3 : All known planets \n");
    scanf("%s", buffer);
    int user_choise;
    if (strcmp(buffer, "1") == 0 || strcmp(buffer, "2") == 0 || strcmp(buffer, "3") == 0) {
        user_choise = atoi(buffer);
    }
    else {
        user_choise = 0;
    }
    switch (user_choise) {
        case 1:
            if (getLengthList(jerry_l) == 0) {
                printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                return;
            }
            displayList(jerry_l);
            return;
        case 2:
            printf("What physical characteristics ? \n");
            scanf("%s", buffer);
            Element elem =  lookupInMultiValueHashTable(phys_t, buffer);
            if (elem == NULL) {
                printf("Rick we can not help you - we do not know any Jerry's %s ! \n", buffer);
                return;
            }
            printf("%s : \n", buffer);
            list_forEach(elem, jerry_l) {
                Jerry* j = (Jerry*) elem;
                if (physExcit(j, buffer) == true) {
                    printJerry(j);
                }
            }
            return;
        case 3:
            for (int i = 0; i < planet_num; i++) {
                printPlanet(p_arr[i]);
            }
            return;
        default:
            printf("Rick this option is not known to the daycare ! \n");
    }
}

void option_8(linked_list jerry_l, char* buffer) {
    /**
     * @brief Initiates an activity for all Jerries in the daycare.
     *
     * This function allows the user to select an activity for all Jerries in the daycare.
     * Based on the activity chosen, the happiness levels of Jerries are adjusted accordingly.
     *
     * @param jerry_l Pointer to the linked list of Jerries.
     * @param buffer A temporary buffer for user input.
     *
     * @return Void. The function adjusts happiness levels or prints a message for invalid input.
     *
     * @note If no Jerries are in the daycare, the function prints an appropriate message and exits.
     */
    if (getLengthList(jerry_l) == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return;
    }
    printf("What activity do you want the Jerries to partake in ? \n"
           "1 : Interact with fake Beth \n"
           "2 : Play golf \n"
           "3 : Adjust the picture settings on the TV \n");
    scanf("%s", buffer);
    int user_choise;
    if (strcmp(buffer, "1") == 0 || strcmp(buffer, "2") == 0 || strcmp(buffer, "3") == 0) {
        user_choise = atoi(buffer);
    }
    else {
        user_choise = 0;
    }
    Element elem;
    switch (user_choise) {
        case 1:
            adjust_happiness(jerry_l, 20, 5, 15);
            return;
        case 2:
            adjust_happiness(jerry_l, 50, 10, 10);
            return;
        case 3:
            adjust_happiness(jerry_l, 0, 0, 20);
            return;
        default:
            printf("Rick this option is not known to the daycare ! \n");
    }
}

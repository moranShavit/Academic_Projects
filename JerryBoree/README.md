# 🧬 Jerry Management System in C

This project simulates a dynamic registry for characters (Jerrys) and their associated characteristics in a multiverse-inspired context. It utilizes custom-built data structures such as hash tables, linked lists, and multi-value hash tables to manage complex object relationships efficiently in C.

---

## 📁 Project Structure

| File                | Description |
|---------------------|-------------|
| `JerryBoreeMain.c`  | Main program interface and simulation logic for managing Jerrys. |
| `Jerry.c/h`         | Defines and implements the Jerry object, including origin, physical traits, and behavior. |
| `Planet` / `Origin` | Nested structs representing a Jerry's universe location and source planet. |
| `LinkedList.c/h`    | Generic doubly linked list implementation with deep-copy and key-based operations. |
| `KeyValuePair.c/h`  | Generic key-value pair abstraction for modular storage. |
| `HashTable.c/h`     | Single-value generic hash table built with chaining and custom hash/equality functions. |
| `MultiValueHashTable.c/h` | Extends `HashTable` to associate multiple values per key using internal linked lists. |
| `makefile`          | Automates build process and dependency resolution. |

---

## 🛠️ Features

- 🔗 **Modular ADT design** using function pointers (copy, compare, print, free).
- 🗃️ **MultiValueHashTable**: Supports associating multiple characteristics with a single Jerry.
- 📋 **Physical Trait Management**: Add/remove/search traits for individual Jerrys.
- 🌍 **Dimension-aware Origins**: Each Jerry is linked to an origin, which is tied to a planet.
- 🚀 **Dynamic Memory Handling**: Proper memory safety using custom destructors and global memory failure flags.
- 🧪 **Built-in Testing & Debugging Hooks**: Error tracing via global flags and output formatting.

---

## ▶️ Compilation & Usage

To compile the program, run:

```bash
make
```

Then run the executable:

```bash
./JerryBoreeMain configoration_file
```

To clean compiled files:

```bash
make clean
```

---

## 🧠 Concepts Demonstrated

- C programming (structs, dynamic allocation, pointers, function pointers)
- Abstract Data Types (ADT): Hash Tables, Linked Lists, Key-Value Pairs
- Memory safety and resource management in C
- Data encapsulation and modular reusable components
- Iterative design for multivalue relationships (One-to-Many mappings)

---

## 👨‍💻 Author

**Moran Shavit**  
A C developer passionate about low-level systems programming and data structure design.

---

## 📄 License

This project is for academic use and demonstration purposes only. For reuse or adaptation, please contact the author.


# APULIB

**APULIB** is a header-only C macro library for generic data structures and functional-style algorithms, including Dynamic Arrays, Doubly Linked Lists, Sorted Sets, and Sorted Maps.

---

## Table of Contents

- [Getting Started](#getting-started)
  - [Feature Flags](#feature-flags)
  - [Configuration](#configuration)
  - [Quick Example](#quick-example)
- [Helper Macros](#helper-macros)
- [Dynamic Array (`APULIB_DA_IMPL`)](#dynamic-array-apulib_da_impl)
- [Doubly Linked List (`APULIB_LL_IMPL`)](#doubly-linked-list-apulib_ll_impl)
- [Sorted Set (`APULIB_SET_IMPL`)](#sorted-set-apulib_set_impl)
- [Sorted Map (`APULIB_MAP_IMPL`)](#sorted-map-apulib_map_impl)

---

## Getting Started

Because APULIB is a single-header macro library, simply drop the header file into your project and `#define` the implementation flags for the data structures you want to use before `#include`-ing it.

> **Compiler Note:** APULIB uses C11 `_Generic` and the `typeof` extension (supported by GCC, Clang, and C23). If you use floating-point containers on Linux/Unix, link the math library with `-lm`.

### Feature Flags

| Macro Flag | Enables |
| :--- | :--- |
| `#define APULIB_DA_IMPL` | Dynamic Array (`da_*` macros) |
| `#define APULIB_LL_IMPL` | Doubly Linked List (`ll_*` macros) |
| `#define APULIB_SET_IMPL` | Sorted Set (`set_*` macros) |
| `#define APULIB_MAP_IMPL` | Sorted Map (`map_*` macros) |

### Configuration

| Macro | Default | Description |
| :--- | :--- | :--- |
| `APULIB_EPSILON_ENV` | `0.0000000005` | Tolerance threshold used by `apulib_epsilon_close` when comparing `float`, `double`, or `long double` values. Define this before including the header to customize precision. |

### Quick Example

```c
#define APULIB_DA_IMPL
#define APULIB_SET_IMPL
#include "apulib.h"

#define is_even(x) ((x) % 2 == 0)
#define times_ten(x) ((x) * 10)

int main(void) {
    da_declare(int, IntArray);
    da_create(IntArray, nums);

    da_append(nums, 5);
    da_append(nums, 2);
    da_append(nums, 8);
    da_append(nums, 1);

    da_stable_sort(nums);
    da_print(nums, "%d"); // [1, 2, 5, 8]

    da_filter_self(nums, is_even);
    da_map_self(nums, times_ten);
    da_print(nums, "%d"); // [20, 80]

    da_clear(nums);
    return 0;
}
```

---

## Helper Macros

Core utility and memory-management macros available across all modules.

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `apulib_malloc(pointer, size)` | `pointer`, `size` | Allocates `size` bytes into `pointer`. Exits the program with code `1` if allocation fails. |
| `apulib_realloc(pointer, size)` | `pointer`, `size` | Reallocates `pointer` to `size` bytes. Exits the program with code `1` if allocation fails. |
| `apulib_swap(a, b)` | `a`, `b` | Swaps the values of variables `a` and `b` in place using `typeof`. |
| `apulib_epsilon_close(a, b)` | `a`, `b` | Evaluates to `1` if `a` and `b` are within `APULIB_EPSILON_ENV` for floating-point types (`float`, `double`, `long double`), or strictly equal (`(a) == (b)`) for all other types. |
| `apulib_true_cond(a)` | `a` | Helper predicate that always returns `(1)`. |
| `apulib_return_1(a, b)` | `a`, `b` | Helper binary function that always returns `(1)`. |
| `apulib_sum_helper(a, b)` | `a`, `b` | Helper binary function returning `((a) + (b))`. |
| `apulib_max_helper(a, b)` | `a`, `b` | Helper binary function returning the larger of `a` and `b`. |
| `apulib_min_helper(a, b)` | `a`, `b` | Helper binary function returning the smaller of `a` and `b`. |

---

## Dynamic Array (`APULIB_DA_IMPL`)

Enable with `#define APULIB_DA_IMPL` before including the header.

### Lifecycle & Mutation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `da_declare(T, typename)` | `T`, `typename` | Creates a dynamic array struct type for elements of type `T` with the name `typename`. |
| `da_create(typename, name)` | `typename`, `name` | Creates and initializes a dynamic array variable named `name` of struct type `typename` with an initial capacity of `1`. |
| `da_append(arr, val)` | `arr`, `val` | Appends `val` to the end of the `arr` dynamic array, automatically growing capacity as needed. |
| `da_insert(arr, val, where)` | `arr`, `val`, `where` | Inserts `val` into the `arr` dynamic array at index position `where`. |
| `da_delete(arr, val)` | `arr`, `val` | Deletes the first occurrence of `val` from the `arr` dynamic array. |
| `da_pop(arr)` | `arr` | Expression that removes and returns the last element of the `arr` dynamic array. |
| `da_shift(arr, out)` | `arr`, `out` | Removes the first element of the `arr` dynamic array and stores it into `out`. |
| `da_copy(arr, out)` | `arr`, `out` | Clears `out` and makes `out` a deep copy of the `arr` dynamic array. |
| `da_clear(arr)` | `arr` | Clears all values of the `arr` dynamic array, resets size/capacity, and frees the underlying buffer. |

### Access, Inspection & Printing

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `da_get(arr, index)` | `arr`, `index` | Expression that evaluates to the value at `index` of the `arr` dynamic array. |
| `da_contains(arr, val, out)` | `arr`, `val`, `out` | Sets `out` to `1` if `val` is inside the `arr` dynamic array, otherwise `0`. |
| `da_find(arr, val, out)` | `arr`, `val`, `out` | Sets `out` to the index of `val` inside the `arr` dynamic array, or `-1` if it is not present. |
| `da_print(arr, form)` | `arr`, `form` | Prints the values of `arr` formatted with the `printf` format string `form` (e.g., `"%d"`). |
| `da_forEach(arr, indexer_name)` | `arr`, `indexer_name` | Expands to a `for` loop header iterating over `arr` using `indexer_name` (`size_t`) as the index variable. |

### Functional Operations & Aggregation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `da_reduce(arr, func, cond, start, accumulator, out_bool)` | `arr`, `func`, `cond`, `start`, `accumulator`, `out_bool` | Reduces `arr` into `accumulator` using binary function/macro `func` for elements where `cond(elem)` is true, initialized to `start`. Sets `out_bool` to `1` if at least one element matched, otherwise `0`. |
| `da_sum(arr, out)` | `arr`, `out` | Adds the values of the `arr` dynamic array into `out`. |
| `da_max(arr, out)` | `arr`, `out` | Finds the maximum value within the `arr` dynamic array and outputs it into `out`. |
| `da_min(arr, out)` | `arr`, `out` | Finds the minimum value within the `arr` dynamic array and outputs it into `out`. |
| `da_sumif(arr, cond, out, out_bool)` | `arr`, `cond`, `out`, `out_bool` | Adds the values of `arr` for elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `da_maxif(arr, cond, out, out_bool)` | `arr`, `cond`, `out`, `out_bool` | Finds the maximum value of `arr` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `da_minif(arr, cond, out, out_bool)` | `arr`, `cond`, `out`, `out_bool` | Finds the minimum value of `arr` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `da_count(arr, cond, out)` | `arr`, `cond`, `out` | Stores the number of elements in `arr` that satisfy `cond(elem)` into `out`. |
| `da_any(arr, cond, out)` | `arr`, `cond`, `out` | Checks if any element of `arr` satisfies `cond(elem)`. Stores `1` in `out` if true, `0` if false. |
| `da_all(arr, cond, out)` | `arr`, `cond`, `out` | Checks if all elements of `arr` satisfy `cond(elem)`. Stores `1` in `out` if true, `0` if false. |
| `da_map(arr, func, out)` | `arr`, `func`, `out` | Populates `out` with the values of `arr`, each transformed by `func(elem)`. |
| `da_filter(arr, cond, out)` | `arr`, `cond`, `out` | Populates `out` with the values of `arr` for which `cond(elem)` is true. |
| `da_map_self(arr, func)` | `arr`, `func` | Mutates the values of `arr` in place, replacing each element with `func(elem)`. |
| `da_filter_self(arr, cond)` | `arr`, `cond` | Filters `arr` in place, keeping only the values for which `cond(elem)` is true. |

### Sorting

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `da_unstable_sort(arr)` | `arr` | Sorts the `arr` dynamic array in ascending order using an in-place **Heap Sort** algorithm ($O(n \log n)$ time, $O(1)$ extra space). |
| `da_stable_sort(arr)` | `arr` | Sorts the `arr` dynamic array in ascending order using a **Merge Sort** algorithm ($O(n \log n)$ time, preserves relative order of equal elements). |
| `da_build_max_heap(arr)` | `arr` | *(Internal helper)* Rearranges `arr` in place into a max-heap. |
| `da_heap_sort_sink(arr, k, n)` | `arr`, `k`, `n` | *(Internal helper)* Sinks element at index `k` down a max-heap of size `n`. |

---

## Doubly Linked List (`APULIB_LL_IMPL`)

Enable with `#define APULIB_LL_IMPL` before including the header.

### Lifecycle & Mutation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `ll_declare(T, typename)` | `T`, `typename` | Declares a doubly linked list node struct of type `T` with the name `typename`. |
| `ll_create(typename, name, start)` | `typename`, `name`, `start` | Allocates and initializes a linked list pointer named `name` of type `typename` with a starting node value of `start`. |
| `ll_append(typename, list, val)` | `typename`, `list`, `val` | Appends `val` to the end of `list` (of node type `typename`). |
| `ll_insert(typename, list, val, where)` | `typename`, `list`, `val`, `where` | Inserts `val` into `list` (of node type `typename`) at index `where`. If the list's size is less than `where`, it appends `val` to the end. |
| `ll_delete(list, val)` | `list`, `val` | Deletes the first node matching `val` from `list`. If `val` is not found, does nothing. |
| `ll_copy(typename, list, out)` | `typename`, `list`, `out` | Clears `out` and deep-copies `list` into `out` (both of node type `typename`). |
| `ll_clear(list)` | `list` | Frees all nodes in `list` and sets `list` to `NULL`. |

### Access, Inspection & Printing

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `ll_size(list, out)` | `list`, `out` | Counts the number of nodes in `list` and stores the result into `out`. |
| `ll_get(list, index, out)` | `list`, `index`, `out` | Gets the value at `index` inside `list` and copies it into `out`. If `index` is out of bounds, does nothing. |
| `ll_contains(list, val, out_bool)` | `list`, `val`, `out_bool` | Sets `out_bool` to `1` if `list` contains `val`, otherwise `0`. |
| `ll_print(list, form)` | `list`, `form` | Prints the elements of `list` using the `printf` format string `form`. |

### Functional Operations, Aggregation & Sorting

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `ll_reduce(list, func, cond, start, accumulator, out_bool)` | `list`, `func`, `cond`, `start`, `accumulator`, `out_bool` | Reduces `list` into `accumulator` using binary function/macro `func` for elements where `cond(elem)` is true, starting with `start`. Sets `out_bool` to `1` if any element matched, otherwise `0`. |
| `ll_sum(list, out)` | `list`, `out` | Adds the values of `list` into `out`. |
| `ll_max(list, out)` | `list`, `out` | Finds the maximum value within `list` and outputs it into `out`. |
| `ll_min(list, out)` | `list`, `out` | Finds the minimum value within `list` and outputs it into `out`. |
| `ll_sumif(list, cond, out, out_bool)` | `list`, `cond`, `out`, `out_bool` | Adds the values of `list` for elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `ll_maxif(list, cond, out, out_bool)` | `list`, `cond`, `out`, `out_bool` | Finds the maximum value of `list` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `ll_minif(list, cond, out, out_bool)` | `list`, `cond`, `out`, `out_bool` | Finds the minimum value of `list` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `ll_count(list, cond, out)` | `list`, `cond`, `out` | Counts the elements of `list` that satisfy `cond(elem)` into `out`. |
| `ll_any(list, cond, out_bool)` | `list`, `cond`, `out_bool` | Sets `out_bool` to `1` if at least one element of `list` satisfies `cond(elem)`, otherwise `0`. |
| `ll_all(list, cond, out_bool)` | `list`, `cond`, `out_bool` | Sets `out_bool` to `1` if all elements of `list` satisfy `cond(elem)`, otherwise `0`. |
| `ll_map(typename, list, func, out)` | `typename`, `list`, `func`, `out` | Applies `func(elem)` to a copy of `list` (of node type `typename`) and stores the new list in `out`. |
| `ll_filter(typename, list, cond, out)` | `typename`, `list`, `cond`, `out` | Copies elements of `list` (of node type `typename`) that satisfy `cond(elem)` into `out`. |
| `ll_map_self(typename, list, func)` | `typename`, `list`, `func` | Mutates all elements of `list` in place using `func(elem)`. |
| `ll_filter_self(typename, list, cond)` | `typename`, `list`, `cond` | Filters `list` in place, freeing nodes where `cond(elem)` is false and keeping those where it is true. |
| `ll_sort(typename, list)` | `typename`, `list` | Sorts `list` (of node type `typename`) in ascending order using **Merge Sort**. |

---

## Sorted Set (`APULIB_SET_IMPL`)

Enable with `#define APULIB_SET_IMPL` before including the header. Elements are stored in sorted order without duplicates, enabling $O(\log n)$ binary search lookups.

### Lifecycle & Mutation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `set_declare(T, typename)` | `T`, `typename` | Declares a sorted set struct of element type `T` with the name `typename`. |
| `set_create(typename, name)` | `typename`, `name` | Creates and initializes a new set named `name` of struct type `typename`. |
| `set_insert(set, val)` | `set`, `val` | Inserts `val` into `set` in sorted position if not already present. |
| `set_delete(set, val)` | `set`, `val` | Deletes `val` from `set` if present. |
| `set_copy(set, out)` | `set`, `out` | Clears `out` and copies all values of `set` into `out`. |
| `set_clear(set)` | `set` | Clears `set` and frees all allocated data. |

### Inspection & Printing

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `set_contains(set, val, out)` | `set`, `val`, `out` | Checks via binary search if `val` is inside `set`, storing `1` (true) or `0` (false) into `out`. |
| `set_print(set, form)` | `set`, `form` | Prints `set` enclosed in `{ ... }` using the `printf` format string `form`. |
| `set_forEach(set, indexer_name)` | `set`, `indexer_name` | Expands to a `for` loop header iterating over `set` using `indexer_name` (`size_t`) as the index variable. |

### Functional Operations & Aggregation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `set_reduce(set, func, cond, start, accumulator, out_bool)` | `set`, `func`, `cond`, `start`, `accumulator`, `out_bool` | Reduces the values of `set` into `accumulator` using `func` for elements where `cond(elem)` is true, starting from `start`. Sets `out_bool` to `1` if any element matched, otherwise `0`. |
| `set_sum(set, out)` | `set`, `out` | Adds the values of `set` into `out`. |
| `set_max(set, out)` | `set`, `out` | Finds the maximum value within `set` and outputs it into `out`. |
| `set_min(set, out)` | `set`, `out` | Finds the minimum value within `set` and outputs it into `out`. |
| `set_sumif(set, cond, out, out_bool)` | `set`, `cond`, `out`, `out_bool` | Adds the values of `set` for elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `set_maxif(set, cond, out, out_bool)` | `set`, `cond`, `out`, `out_bool` | Finds the maximum value of `set` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `set_minif(set, cond, out, out_bool)` | `set`, `cond`, `out`, `out_bool` | Finds the minimum value of `set` among elements where `cond(elem)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `set_count(set, cond, out)` | `set`, `cond`, `out` | Stores the number of elements in `set` that satisfy `cond(elem)` into `out`. |
| `set_any(set, cond, out)` | `set`, `cond`, `out` | Checks if any element of `set` satisfies `cond(elem)`. Stores `1` in `out` if true, `0` if false. |
| `set_all(set, cond, out)` | `set`, `cond`, `out` | Checks if all elements of `set` satisfy `cond(elem)`. Stores `1` in `out` if true, `0` if false. |
| `set_map(set, func, out)` | `set`, `func`, `out` | Populates `out` as a valid sorted set of the values of `set`, each mutated by `func(elem)`. |
| `set_filter(set, cond, out)` | `set`, `cond`, `out` | Populates `out` with the values of `set` for which `cond(elem)` is true. |
| `set_map_self(set, func)` | `set`, `func` | Mutates the values of `set` in place using `func(elem)`. **Warning:** Non-monotonic functions can break the sorted invariant of the set; re-sort with `da_stable_sort` if needed. |
| `set_filter_self(set, cond)` | `set`, `cond` | Filters `set` in place, keeping only the values for which `cond(elem)` is true. |

---

## Sorted Map (`APULIB_MAP_IMPL`)

Enable with `#define APULIB_MAP_IMPL` before including the header. Keys are stored in sorted order for $O(\log n)$ binary search lookups.

### Lifecycle & Mutation

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `map_declare(K, V, typename)` | `K`, `V`, `typename` | Declares a sorted map struct with key type `K`, value type `V`, and struct name `typename`. |
| `map_create(typename, name)` | `typename`, `name` | Creates and initializes a map instance named `name` of struct type `typename`. |
| `map_insert(map, key, val)` | `map`, `key`, `val` | Inserts the `(key, val)` pair into `map` in sorted key order if `key` is not already present. |
| `map_delete(map, key)` | `map`, `key` | Deletes the `(key, value)` pair associated with `key` from `map`. |
| `map_copy(map, out)` | `map`, `out` | Clears `out` and copies all keys and values from `map` into `out`. |
| `map_clear(map)` | `map` | Clears all elements of `map` and frees both key and value buffers. |

### Access, Inspection & Printing

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `map_get(map, key, out)` | `map`, `key`, `out` | Looks up `key` in `map` via binary search and copies its associated value into `out` if present. |
| `map_contains_key(map, key, out)` | `map`, `key`, `out` | Checks if `map` contains `key`, outputting `1` (true) or `0` (false) into `out`. |
| `map_contains_val(map, val, out)` | `map`, `val`, `out` | Checks if `map` contains `val` among its values, outputting `1` (true) or `0` (false) into `out`. |
| `map_print(map, keyform, valform)` | `map`, `keyform`, `valform` | Prints `map` formatted as `{key: val, ...}` using the `printf` format strings `keyform` and `valform`. |

### Functional Operations & Aggregation (on Values)

| Macro | Parameters | Description / How to Use |
| :--- | :--- | :--- |
| `map_reduce(map, func, cond, start, accumulator, out_bool)` | `map`, `func`, `cond`, `start`, `accumulator`, `out_bool` | Reduces the values of `map` into `accumulator` using `func` for values where `cond(val)` is true, starting from `start`. Sets `out_bool` to `1` if any value matched, otherwise `0`. |
| `map_sum(map, out)` | `map`, `out` | Adds the values of `map` into `out`. |
| `map_max(map, out)` | `map`, `out` | Finds the maximum value within `map` and outputs it into `out`. |
| `map_min(map, out)` | `map`, `out` | Finds the minimum value within `map` and outputs it into `out`. |
| `map_sumif(map, cond, out, out_bool)` | `map`, `cond`, `out`, `out_bool` | Adds the values of `map` for elements where `cond(val)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `map_maxif(map, cond, out, out_bool)` | `map`, `cond`, `out`, `out_bool` | Finds the maximum value of `map` among elements where `cond(val)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `map_minif(map, cond, out, out_bool)` | `map`, `cond`, `out`, `out_bool` | Finds the minimum value of `map` among elements where `cond(val)` is true into `out`, setting `out_bool` to `1` if any matched (else `0`). |
| `map_count(map, cond, out)` | `map`, `cond`, `out` | Stores the number of elements in `map` whose values satisfy `cond(val)` into `out`. |
| `map_any(map, cond, out)` | `map`, `cond`, `out` | Checks if any value in `map` satisfies `cond(val)`. Stores `1` in `out` if true, `0` if false. |
| `map_all(map, cond, out)` | `map`, `cond`, `out` | Checks if all values in `map` satisfy `cond(val)`. Stores `1` in `out` if true, `0` if false. |
| `map_filter(map, cond, out)` | `map`, `cond`, `out` | Populates `out` with the `(key, value)` pairs of `map` whose values satisfy `cond(val)`. |
| `map_filter_self(map, cond)` | `map`, `cond` | Filters `map` in place, keeping only the `(key, value)` pairs whose values satisfy `cond(val)`. |

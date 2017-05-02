# flame-linq
.Net style Language Integrated Query library for C++14

This library adds the `linq::array` type which extends from the `std::vector`. I know purests do not like people extending the standard library components. This was just the simplest way to do it without having to essentially "reinvent the wheel" for the low level data management. As a note, there may be some efficiency gain by redoing that low level memory work, but for now it was not necessary.

The array class contains various manipulation methods which may either mutate the current `linq::array`, or create a new array, copying any pertinent values into it. The use of `const` denote which methods do not mutate the current `linq::array`, but rather create a new one as a result.

## Macros
The library contains a set of macros which can be used to create cleaner and more readable LINQ statements. These macros are completely optional and are disabled by default. To enable them, ensure to define the preproc `LINQ_USE_MACROS` before including the `linq.h` header.

> Example
> ```c++
> #define LINQ_USE_MACROS
> #include "linq.h"
> ```

## Examples

### Array filtering
_Standard_
```c++
int nums[10] = {1,2,3,4,5,6,7,8,9,10};
auto odds = linq::from(nums, 10)
    .where([](auto item) { return item % 2; });
```
_Macros_
```c++
int nums[10] = {1,2,3,4,5,6,7,8,9,10};
auto odds = FROM(nums, 10)
            WHERE { return item % 2; }
            END;
```

### Sorting
Lists can be sorted using the `orderby` method or `ORDERBY` macro. If the array contains basic data types, or complex types that have the `>` or `<` comparison operators overloaded, `orderby` can be given the global `linq::ascending|ASCENDING` or `linq::descending|DESCENDING` predicates. Otherwise, a custom predicate must be provided in the form of a lambda or complex object with the `()` operator overloaded.

The callable predicate must be of the form:\
`bool (<type> [left_elem], <type> [right_elem])`

_Standard_
```c++
int nums[10] = {1,2,3,4,5,6,7,8,9,10};
auto odds = linq::from(nums, 10)
    .where([](auto item) { return item % 2; })
    .orderby(linq::descending);
```
_Macros_
```c++
int nums[10] = {1,2,3,4,5,6,7,8,9,10};
auto odds = FROM(nums, 10)
            WHERE { return item % 2; }
            ORDERBY(DESCENDING)
            END;
```

### Joining


#### Paired Join

#### Merged Join



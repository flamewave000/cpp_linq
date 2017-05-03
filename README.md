# flame-linq
.Net style Language Integrated Query library for C++14

This library adds the `linq::array` type which extends from the `std::vector`. I know purests do not like people extending the standard library components. This was just the simplest way to do it without having to essentially "reinvent the wheel" for the low level data management. As a note, there may be some efficiency gain by redoing that low level memory work, but for now it was not necessary.

The array class contains various manipulation methods which may either mutate the current `linq::array`, or create a new array, copying any pertinent values into it. The use of `const` denote which methods do not mutate the current `linq::array`, but rather create a new one as a result.

_Note: Because `linq::array` inherits `std::vector`, it naturally assumes those iterator features and can be subsequently used within the for-each loop:\
`for(auto elem : <linq::array>) { }`_

- [Macros](#macros-top)
- [Examples](#examples-top)
- [Future Features](#future-features-top)

## Macros [top](#flame-linq)
The library contains a set of macros which can be used to create cleaner and more readable LINQ statements. These macros are completely optional and are disabled by default. To enable them, ensure to define the preproc `LINQ_USE_MACROS` before including the `linq.h` header.
> Example
> ```c++
> #define LINQ_USE_MACROS
> #include "linq.h"
> ```

_Note: All macro linq statements must be terminated with the_ `END` _macro._

## Examples [top](#flame-linq)
- [Array Creation](#array-creation-examples)
- [Array Filtering](#array-filtering-examples)
- [Sorting](#sorting-examples)
- [Selecting](#selecting-examples)
- [Joining](#joining-examples)
    - [Pair Join](#pair-join-examples)
    - [Merge Join](#merge-join-examples)

### Array Creation [examples](#examples-top)
`linq::array` can be created by using either a `std::vector` or c-style array. This is done by doing the following:
> _Standard_
> ```c++
> std::vector<int> vec_nums = {1,2,3};
> auto array_nums = linq::from(vec_nums);
> 
> int c_nums[3] = {1,2,3};
> auto array_nums = linq::from(c_nums, 3);
> ```
> _Macros_
> ```c++
> std::vector<int> vec_nums = {1,2,3};
> auto array_nums = FROM (vec_nums);
> 
> int c_nums[3] = {1,2,3};
> auto array_nums = FROM (c_nums, 3);
> ```

### Array Filtering [examples](#examples-top)
A `linq::array` can be filtered using the `where|WHERE` method/macro. This allows a developer to filter the array for only elements that pass a defined condition.
> _Standard_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto odds = linq::from(nums, 10)
>     .where([](int item) { return item % 2; });
> ```
> _Macros_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto odds = FROM (nums, 10)
>             WHERE { return item % 2; }
>             END;
> ```

### Sorting [examples](#examples-top)
Lists can be sorted using the `orderby` method or `ORDERBY` macro. If the array contains basic data types, or complex types that have the `>` or `<` comparison operators overloaded, `orderby` can be given the global `linq::ascending|ASCENDING` or `linq::descending|DESCENDING` predicates. Otherwise, a custom predicate must be provided in the form of a lambda or complex object with the `()` operator overloaded.

The callable predicate must be of the form:\
`bool (<type> [left_elem], <type> [right_elem])`
> _Standard_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto odds = linq::from(nums, 10)
>     .where([](int item) { return item % 2; })
>     .orderby(linq::descending);
> ```
> _Macros_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto odds = FROM (nums, 10)
>             WHERE { return item % 2; }
>             ORDERBY (DESCENDING)
>             END;
> ```

### Selecting [examples](#examples-top)
Selection allows the developer to transform each element into a new type. This type must be the same for all new elements and must be defined before use. This creates a new `linq::array` containing the new elements created within the lambda expression.

> _Standard_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto text_nums = linq::from(nums, 10)
>     .select<std::string>([](int item)->std::string {
>         return int2string(elem);
>     });
> ```
> _Macros_
> ```c++
> int nums[10] = {1,2,3,4,5,6,7,8,9,10};
> auto text_nums =
>     FROM (nums, 10)
>     SELECT (std::string) { return int2string(item); }
>     END;
> ```

### Joining [examples](#examples-top)
The more complex procedure provided by the linq library, joining is performed on two separate lists. This process compares each element in the first list against each element in the second list. Joining has to stages, the first stage is comparision, where elements from both lists are paired based on the provided comparision condition. the second stage is an optional "merge". If the merge stage is not defined, the result will be a `linq::array<linq::core::merge_pair<`_`type_a`_`, `_`type_b`_`>>`.

#### Pair Join [examples](#examples-top)
> _Standard_
> ```c++
> std::vector<int> ints = {1,2,3,4,6};
> std::vector<float> floats = {1.5,2.5,3.5,4.5,6.5};
> linq::array<linq::core::merge_pair<int,float>> pairs =
>     linq::from(ints)
>     .join<float>(floats,
>         [](int left, float right) -> bool {
>             return left == int(right);
>         });
> ```
> _Macros_
> ```c++
> std::vector<int> ints = {1,2,3,4,6};
> std::vector<float> floats = {1.5,2.5,3.5,4.5,6.5};
> linq::array<linq::core::merge_pair<int,float>> pairs =
>     FROM (ints)
>     PAIR_JOIN (float) floats
>     ON { return left == int(right); }
>     END;
> ```

#### Merge Join [examples](#examples-top)
> _Standard_
> ```c++
> struct myPair { int whole, float half; };
> std::vector<int> ints = {1,2,3,4,6};
> std::vector<float> floats = {1.5,2.5,3.5,4.5,6.5};
> linq::array<myPair> pairs = linq::from(ints)
>     .join<float, myPair>(floats,,
>         [](int left, float right) -> myPair {
>             return { left, right };
>         }
>         [](int left, float right) -> bool {
>             return left == int(right);
>         });
> ```
> _Macros_
> ```c++
> struct myPair { int whole, float half; };
> std::vector<int> ints = {1,2,3,4,6};
> std::vector<float> floats = {1.5,2.5,3.5,4.5,6.5};
> linq::array<myPair> pairs =
>     FROM (ints)
>     MERGE_JOIN (float,myPair) floats
>     INTO (myPair) { return { left, right }; }
>     ON { return left == int(right); }
>     END;
> ```

## Future Features [top](#flame-linq)
&#x2714; Complete &#x25a2; Incomplete

### Features Planned for v1.0

&#x2714; _`type`_`first(`_`lambda`_`)`\
&#x2714; _`type`_`first_or_default(`_`lambda`_`,`_`default`_`)`\
&#x25a2; _`type`_`last(`_`lambda`_`)`\
&#x25a2; _`type`_`last_or_default(`_`lambda`_`,`_`default`_`)`\
&#x25a2; `vector to_vector()`\
&#x25a2; Wiki documentation

### Other Possible Features
&#x25a2; _`type`_`min(`_`lambda`_`)`\
&#x25a2; _`type`_`max(`_`lambda`_`)`\
&#x25a2; `bool any()`\
&#x25a2; `bool any(`_`lambda`_`)`\
&#x25a2; `void reverse()`\
&#x25a2; `map<`_`key`_`,`_`type`_`> groupby<`_`key`_`>(`_`lambda`_`)`\
&#x25a2; Simple Website

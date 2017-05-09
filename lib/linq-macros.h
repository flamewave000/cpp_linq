#ifdef LINQ_USE_MACROS
/// <summary>All LINQ queries must be finished with this macro.</summary>
#define END )

/// <summary>Used with <see cref="ORDERBY"/> as a predefined ascending sort.</summary>
#define ASCENDING ::linq::ascending
/// <summary>Used with <see cref="ORDERBY"/> as a predefined descending sort.</summary>
#define DESCENDING ::linq::descending

// ELEM, LEFT, & RIGHT are simple macros to be used where needed for parameter names.
// This allows for easy name changing without the risk of missing a name.
#define __LINQ_ELEM__ item
#define __LINQ_LEFT__ left
#define __LINQ_RIGHT__ right

/// <summary>Performs an item selection which is expected to transform the data in some way.</summary>
/// <param name="type">The new type being created and returned for the new array.</param>
/// <example>SELECT(int) { return str.size(); }</example>
#define SELECT(type) END.select<type>([](auto __LINQ_ELEM__) -> type

/// <summary>Performs a conditional for filtering the list.</summary>
/// <example>WHERE { return item > 10; }</example>
#define WHERE END.where([](auto __LINQ_ELEM__) -> bool

/// <summary>Performs a sort on the list. Can be given the predefined ASCENDING or DESCENDING for comparable types, or any predicate.</summary>
/// <param name="pred">Predicate used for comparing the two items for which should go first.</param>
/// <example>ORDERBY(ASCENDING)</example>
/// <example>ORDERBY(PREDICATE { return left > right; })</example>
#define ORDERBY(pred) END.orderby(pred

/// <summary>Helper for creating a predicate lambda for the ORDERBY macro.</summary>
/// <see cref="ORDERBY"/>
#define PREDICATE [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> bool

/// <summary>Performs a join on the current list and the provided list. This results in an array of <see cref="linq::core::merge_pair"/>.</summary>
/// <param name="type">The type contained within the list to be joined.</param>
/// <example>JOIN(int) nums</example>
#define PAIR_JOIN(type) END.join<type>(

/// <summary>Performs a join on the current list and the provided list. Unlike <see cref="JOIN"/> it requires the <see cref="INTO"/> macro to create a merger of the two paired elements.</summary>
/// <param name="type">The type contained within the list to be joined.</param>
/// <param name="merge_type">The new type to be created for each joined pair</param>
/// <example>MERGE_JOIN(int, myPair) nums INTO { return { left, right }; }</example>
#define MERGE_JOIN(type, merge_type) END.join<type, merge_type>(

/// <summary>
/// (Optional) Performs a conditional check between the left and right during either a <see cref="JOIN"/> or <see cref="MERGE_JOIN"/>.
/// Note: For <see cref="MERGE_JOIN"/>, must come after the <see cref="INTO"/> statement
/// </summary>
/// <example>JOIN(obj) myObjects ON { return left.ref_id == right.id; }</example>
#define ON , [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> bool

/// <summary>Used with <see cref="MERGE_JOIN"/> to define how the paired items are merged into a new type.</summary>
/// <param name="type">The type to be created from the merged items.</param>
/// <example>MERGE_JOIN(int, myPair) nums INTO { return { left, right }; }</example>
#define INTO(type) , [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> type

/// <summary>Simple helper macro for calling <see cref="linq::from"/> and needed for starting a LINQ query when using the defined macros.</summary>
/// <example>FROM(vecOfItems)</example>
/// <example>FROM(cArray, count)</example>
#define FROM(param, ...) ::linq::from(param,##__VA_ARGS__

/// <summary>
/// Searches the array for the first item which satisfies the provided condition.
/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
/// </summary>
/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
/// <example>FROM(nums) FIRST { return item % 2; }</example>
#define FIRST END.first([](auto item) -> bool
/// <summary>
/// Searches the array for the first item which satisfies the provided condition.
/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
/// </summary>
/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
/// <example>FROM(nums) FIRST_OR_DEFAULT(42) { return item % 2; }</example>
#define FIRST_OR_DEFAULT(defaultValue) END.first_or_default(defaultValue, [](auto item) -> bool

/// <summary>
/// Searches the array for the last item which satisfies the provided condition.
/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
/// </summary>
/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
/// <example>FROM(nums) LAST { return item % 2; }</example>
#define LAST END.last([](auto item) -> bool
/// <summary>
/// Searches the array for the last item which satisfies the provided condition.
/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
/// </summary>
/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
/// <example>FROM(nums) LAST_OR_DEFAULT(42) { return item % 2; }</example>
#define LAST_OR_DEFAULT(defaultValue) END.last_or_default(defaultValue, [](auto item) -> bool

/// <summary>
/// Helper macro which converts the <see cref="linq::array"/> into a <see cref="std::vector"/>.
/// </summary>
#define TO_VECTOR END.to_vector(


/*** Custom lambda expression alternatives ***/

/// <summary>Performs an item selection which is expected to transform the data in some way, using a fully defined lambda</summary>
/// <param name="type">The new type being created and returned for the new array.</param>
/// <param name="lambda_item_type">Lambda of the form: _Ret(_Ty item)</param>
/// <example>SELECT_L(int) [](auto item) -> int { return str.size(); }</example>
#define SELECT_L(type) END.select<type>(

/// <summary>Performs a conditional for filtering the list using a fully defined lambda.</summary>
/// <example>WHERE_L [](auto item) -> bool { return item > 10; }</example>
#define WHERE_L END.where(

/// <summary>
/// Performs a conditional check between the left and right during either a <see cref="JOIN"/> or <see cref="MERGE_JOIN"/> using a fully defined lambda..
/// Note: For <see cref="MERGE_JOIN"/>, must come after the <see cref="INTO"/> statement
/// </summary>
/// <example>JOIN(obj) myObjects ON_L [](auto left, auto right) -> bool { return left.ref_id == right.id; }</example>
#define ON_L , 

/// <summary>Used with <see cref="MERGE_JOIN"/> to define how the paired items are merged into a new type using a fully defined lambda.</summary>
/// <example>MERGE_JOIN(int, myPair) nums INTO_L [](auto left, auto right) -> myPair { return { left, right }; }</example>
#define INTO_L , 

/// <summary>
/// Searches the array for the first item which satisfies the provided fully defined lambda condition.
/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
/// </summary>
/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
/// <example>FROM(nums) FIRST [](auto item) -> bool { return item % 2; }</example>
#define FIRST_L END.first(
/// <summary>
/// Searches the array for the first item which satisfies the provided fully defined lambda condition.
/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
/// </summary>
/// <param name="defaultValue">Default value to be returned if no items satisfy the fully defined lambda condition.</param>
/// <example>FROM(nums) FIRST_OR_DEFAULT(42) [](auto item) -> bool { return item % 2; }</example>
#define FIRST_OR_DEFAULT_L(defaultValue) END.first_or_default(defaultValue,

/// <summary>
/// Searches the array for the last item which satisfies the provided fully defined lambda condition.
/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
/// </summary>
/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
/// <example>FROM(nums) LAST [](auto item) -> bool { return item % 2; }</example>
#define LAST_L END.last(
/// <summary>
/// Searches the array for the last item which satisfies the provided fully defined lambda condition.
/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
/// </summary>
/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
/// <example>FROM(nums) LAST_OR_DEFAULT(42) [](auto item) -> bool { return item % 2; }</example>
#define LAST_OR_DEFAULT_L(defaultValue) END.last_or_default(defaultValue,
#endif
// ============================================================================
// types.mak — small, explicit building blocks for MAK programs
// ============================================================================
//
// Naming convention
// -----------------
// MAK does not provide module-qualified names in the syntax used here. Public
// functions therefore use a type-oriented prefix: option_*, result_*, pair_*,
// ordering_*, list_*, vector_*, tree_map_*, and function_*.
//
// Representation and contract notes
// ----------------------------------
// * The tagged values below are ordinary MAK algebraic data types. Their
//   constructor names (Some, None, Ok, Err, Nil, Cons, and so on) are used in
//   both expressions and case patterns.
// * Integer flags returned by predicates use 1 for true and 0 for false.
//   list_filter follows the same convention: only the exact result 1 keeps an
//   element; every other result rejects it.
// * Functions that can fail to produce an ordinary value use Option rather
//   than inventing a value such as 0. This matters when the element type is
//   generic and 0 may not be a valid value of that type.
// * TreeMap is a plain, unbalanced binary-search tree. It is not an AVL tree,
//   red-black tree, or hash table. Its performance depends on the shape of the
//   tree and on the ordering supplied by the caller.
// * Vector is only a small header-shaped value (pointer, length, capacity).
//   This file does not allocate memory, grow a buffer, validate pointers, or
//   provide element access. Those operations require runtime support elsewhere.
//
// ============================================================================
// 1. Optional values
// ============================================================================

// Option a represents either a present value (Some value) or absence (None).
// Use it whenever a lookup or list operation may not have a result.
type Option a {
    Some a;
    None;
};

// Return 1 for Some and 0 for None.
let option_is_some option_value = case option_value of {
    Some _ -> 1;
    None -> 0;
};

// Return 1 for None and 0 for Some.
let option_is_none option_value = case option_value of {
    Some _ -> 0;
    None -> 1;
};

// Return the contained value, or the supplied fallback when the option is
// None. The fallback is an ordinary argument, so it is evaluated before this
// function is called; this is not a lazy/default-thunk operation.
let option_get_or fallback option_value = case option_value of {
    Some value -> value;
    None -> fallback;
};

// Transform a present value while preserving absence.
//   option_map f (Some x) = Some (f x)
//   option_map f None    = None
let option_map transform option_value = case option_value of {
    Some value -> Some (transform value);
    None -> None;
};

// Continue with a computation that itself returns an Option.
// This is useful for chaining operations that can each fail to find a value.
// The callback should return Some ... or None.
let option_and_then continue_with option_value = case option_value of {
    Some value -> continue_with value;
    None -> None;
};


// ============================================================================
// 2. Results: success or failure
// ============================================================================

// Result ok err represents either a successful value (Ok value) or an error
// value (Err error). The error type is chosen by the caller.
type Result ok err {
    Ok ok;
    Err err;
};

// Return 1 for Ok and 0 for Err.
let result_is_ok result_value = case result_value of {
    Ok _ -> 1;
    Err _ -> 0;
};

// Return 1 for Err and 0 for Ok.
let result_is_err result_value = case result_value of {
    Ok _ -> 0;
    Err _ -> 1;
};

// Extract a successful value, or return fallback for an error. This deliberately
// discards the error; use case directly when the error needs to be preserved.
let result_get_or fallback result_value = case result_value of {
    Ok value -> value;
    Err _ -> fallback;
};

// Transform only a successful value. Errors pass through unchanged.
let result_map transform result_value = case result_value of {
    Ok value -> Ok (transform value);
    Err error_value -> Err error_value;
};

// Transform only an error value. Successful values pass through unchanged.
let result_map_error transform_error result_value = case result_value of {
    Ok value -> Ok value;
    Err error_value -> Err (transform_error error_value);
};

// Continue with a computation that itself returns a Result. This propagates an
// existing error without calling continue_with.
let result_and_then continue_with result_value = case result_value of {
    Ok value -> continue_with value;
    Err error_value -> Err error_value;
};


// ============================================================================
// 3. Pairs
// ============================================================================

// Pair a b stores two values, potentially of different types.
type Pair a b {
    Pair a b;
};

// Return the first component of a pair.
let pair_first pair_value = case pair_value of {
    Pair first_value _ -> first_value;
};

// Return the second component of a pair.
let pair_second pair_value = case pair_value of {
    Pair _ second_value -> second_value;
};

// Transform the first component and leave the second component unchanged.
let pair_map_first transform pair_value = case pair_value of {
    Pair first_value second_value -> Pair (transform first_value) second_value;
};

// Transform the second component and leave the first component unchanged.
let pair_map_second transform pair_value = case pair_value of {
    Pair first_value second_value -> Pair first_value (transform second_value);
};


// ============================================================================
// 4. Ordering
// ============================================================================

// The three outcomes of an ordering comparison.
type Ordering {
    Less;
    Equal;
    Greater;
};

// Compare two integer values using the language's < and > operators.
// The comparison expressions are expected to produce 1 when true.
let ordering_compare_i64 left_value right_value = case left_value < right_value of {
    1 -> Less;
    _ -> case left_value > right_value of {
        1 -> Greater;
        _ -> Equal;
    };
};

// Reverse an ordering result: Less becomes Greater, Greater becomes Less, and
// Equal remains Equal.
let ordering_reverse order_value = case order_value of {
    Less -> Greater;
    Equal -> Equal;
    Greater -> Less;
};


// ============================================================================
// 5. Immutable linked lists
// ============================================================================

// List a is a singly linked list. Nil is empty; Cons stores a head value and
// the remainder of the list. A list is built from the end toward the front.
type List a {
    Nil;
    Cons a (List a);
};

// Construct a list node. This does not copy the tail list.
let list_cons element remaining = Cons element remaining;

// Return 1 for Nil and 0 for Cons.
let list_is_empty items = case items of {
    Nil -> 1;
    Cons _ _ -> 0;
};

// Return Some first_element for a non-empty list, or None for Nil.
// Unlike a sentinel-based head function, this is safe for lists of any element
// type and does not confuse a legitimate element with an empty-list marker.
let list_head items = case items of {
    Cons first_element _ -> Some first_element;
    Nil -> None;
};

// Return Some remaining_list for a non-empty list, or None for Nil.
let list_tail items = case items of {
    Cons _ remaining -> Some remaining;
    Nil -> None;
};

// Apply transform to every element, preserving order and list length.
let list_map transform items = case items of {
    Nil -> Nil;
    Cons element remaining ->
        Cons (transform element) (list_map transform remaining);
};

// Fold from the left: each step receives the accumulator first and the next
// list element second. For [x1, x2, ...], the update order is
// f (f (f initial x1) x2) ... .
let list_fold_left combine initial_value items = case items of {
    Nil -> initial_value;
    Cons element remaining ->
        list_fold_left combine (combine initial_value element) remaining;
};

// Keep elements for which predicate returns exactly 1. Any other result,
// including 0, rejects the element. Preserved elements retain their order.
let list_filter predicate items = case items of {
    Nil -> Nil;
    Cons element remaining -> case predicate element of {
        1 -> Cons element (list_filter predicate remaining);
        _ -> list_filter predicate remaining;
    };
};

// Append second_list after first_list. The elements of first_list are rebuilt;
// second_list is returned directly when the end of first_list is reached.
let list_append first_list second_list = case first_list of {
    Nil -> second_list;
    Cons element remaining ->
        Cons element (list_append remaining second_list);
};

// Internal accumulator worker for reversal. The accumulator contains the
// already-reversed prefix; remaining items have not yet been visited.
let list_reverse_accumulator reversed_prefix remaining_items =
    case remaining_items of {
        Nil -> reversed_prefix;
        Cons element rest ->
            list_reverse_accumulator (Cons element reversed_prefix) rest;
    };

// Return a new list in reverse element order.
let list_reverse items = list_reverse_accumulator Nil items;

// Internal length worker. count is the number of elements already visited.
let list_length_accumulator count remaining_items = case remaining_items of {
    Nil -> count;
    Cons _ rest -> list_length_accumulator (count + 1) rest;
};

// Return the number of nodes in the list. The result is an integer count.
let list_length items = list_length_accumulator 0 items;


// ============================================================================
// 6. Vector header
// ============================================================================

// Vector a describes a low-level vector header with three fields:
//   data_pointer : ptr   (represented here as the MAK value supplied by caller)
//   length       : i64   (number of initialized elements)
//   capacity     : i64   (number of element slots available)
//
// This declaration does not allocate or manage the memory behind data_pointer.
// A valid non-empty vector is expected to obey 0 <= length <= capacity. The
// generic element parameter describes the intended element type, but the
// header alone does not encode or verify how individual elements are stored.
type Vector a {
    Vector ptr i64 i64;
};

// Read the logical element count from a vector header.
let vector_length vector_value = case vector_value of {
    Vector _ length_value _ -> length_value;
};

// Read the allocated element capacity from a vector header.
let vector_capacity vector_value = case vector_value of {
    Vector _ _ capacity_value -> capacity_value;
};

// Return the stored data-pointer field without dereferencing it.
let vector_data_pointer vector_value = case vector_value of {
    Vector data_pointer _ _ -> data_pointer;
};

// Empty header convention: zero pointer, zero length, zero capacity.
// This is only a representation value; it does not allocate an empty buffer.
let vector_empty = Vector 0 0 0;

// Return 1 when length is zero and 0 otherwise. This checks the length field;
// it does not validate the pointer or inspect the backing storage.
let vector_is_empty vector_value = case vector_value of {
    Vector _ length_value _ -> case length_value == 0 of {
        1 -> 1;
        _ -> 0;
    };
};


// ============================================================================
// 7. Ordered map backed by an unbalanced binary-search tree
// ============================================================================

// TreeMap key value stores key/value pairs in binary-search-tree order.
// Every key in a node's left subtree must compare Less than the node key, and
// every key in its right subtree must compare Greater. Equal keys are replaced
// by tree_map_insert rather than stored as duplicate nodes.
type TreeMap key value {
    TreeMapEmpty;
    TreeMapNode (TreeMap key value) key value (TreeMap key value);
};

// Construct an empty map.
let tree_map_empty = TreeMapEmpty;

// Insert key/value. compare_keys must return Less, Equal, or Greater when given
// (search_key, node_key), and must define a consistent total ordering. Inserting
// an Equal key replaces that node's key and value while preserving its children.
let tree_map_insert compare_keys key value tree = case tree of {
    TreeMapEmpty -> TreeMapNode TreeMapEmpty key value TreeMapEmpty;
    TreeMapNode left_subtree node_key node_value right_subtree ->
        case compare_keys key node_key of {
            Less -> TreeMapNode
                (tree_map_insert compare_keys key value left_subtree)
                node_key node_value right_subtree;
            Greater -> TreeMapNode left_subtree node_key node_value
                (tree_map_insert compare_keys key value right_subtree);
            Equal -> TreeMapNode left_subtree key value right_subtree;
        };
};

// Look up a key. Returns Some stored_value when found, otherwise None.
let tree_map_lookup compare_keys key tree = case tree of {
    TreeMapEmpty -> None;
    TreeMapNode left_subtree node_key node_value right_subtree ->
        case compare_keys key node_key of {
            Less -> tree_map_lookup compare_keys key left_subtree;
            Greater -> tree_map_lookup compare_keys key right_subtree;
            Equal -> Some node_value;
        };
};

// Return the smallest key/value pair in the tree, or None for an empty tree.
// A Pair is wrapped in Some so this function never needs a fake generic value
// such as Pair 0 0 to represent the absence of a minimum.
let tree_map_min_entry tree = case tree of {
    TreeMapEmpty -> None;
    TreeMapNode left_subtree node_key node_value _ ->
        case left_subtree of {
            TreeMapEmpty -> Some (Pair node_key node_value);
            _ -> tree_map_min_entry left_subtree;
        };
};

// Remove a key if present. When the matching node has two non-empty children,
// its in-order successor (the minimum entry in its right subtree) replaces it.
// This implementation preserves search-tree ordering but does not rebalance.
let tree_map_remove compare_keys key tree = case tree of {
    TreeMapEmpty -> TreeMapEmpty;
    TreeMapNode left_subtree node_key node_value right_subtree ->
        case compare_keys key node_key of {
            Less -> TreeMapNode
                (tree_map_remove compare_keys key left_subtree)
                node_key node_value right_subtree;
            Greater -> TreeMapNode left_subtree node_key node_value
                (tree_map_remove compare_keys key right_subtree);
            Equal -> case left_subtree of {
                TreeMapEmpty -> right_subtree;
                _ -> case right_subtree of {
                    TreeMapEmpty -> left_subtree;
                    _ -> case tree_map_min_entry right_subtree of {
                        Some minimum_pair -> case minimum_pair of {
                            Pair successor_key successor_value -> TreeMapNode
                                left_subtree successor_key successor_value
                                (tree_map_remove compare_keys successor_key right_subtree);
                        };
                        // Unreachable for a well-formed non-empty right subtree.
                        // Retaining the original node is the least destructive
                        // fallback if a future representation change violates
                        // that assumption.
                        None -> TreeMapNode
                            left_subtree node_key node_value right_subtree;
                    };
                };
            };
        };
};


// ============================================================================
// 8. Small higher-order function helpers
// ============================================================================

// Return the supplied value unchanged.
let function_identity value = value;

// Return a function that always returns fixed_value and ignores its argument.
let function_constant fixed_value _ignored_argument = fixed_value;

// Reverse the order of the first two arguments supplied to a two-argument
// function. For example, function_flip subtract 3 10 evaluates as subtract 10 3.
let function_flip two_argument_function first_argument second_argument =
    two_argument_function second_argument first_argument;


// ============================================================================
// End of types.mak
// ============================================================================

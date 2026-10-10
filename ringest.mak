bring "types.mak";

let external printf : i32 = "printf" : (ptr, ...) -> i32 {
    from = "<stdio.h>";
};

// 1. Verify the Option type
let test_option = {
    let opt = Some 42;
    case opt of {
        Some v -> printf "Option has value: %d\n" v;
        None -> printf "Option is empty\n";
    }
};

// 2. Verify the List type with recursion
let print_list xs = case xs of {
    Nil -> printf "List is empty\n";
    Cons x rest -> {
        printf "List item: %d\n" x;
        print_list rest;
        // Note: The compiler will tail-call optimize this into a C for(;;) loop
    }
};

// 3. Construct a List using the Cons constructor
let my_list = Cons 10 (Cons 20 (Cons 30 Nil));

// 4. The final program expression (a block that sequences the actions)
{
    printf "--- Starting Type Verification ---\n";
    test_option;
    print_list my_list;
    printf "--- Verification Complete ---\n";
    0
}
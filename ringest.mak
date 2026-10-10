
let external putchar : i32 -> ! = "putchar" : (i32) -> void {
    from = "<stdio.h>"
};

{
    putchar "MAK"[0];
    putchar "MAK"[1];
    putchar ("MAK" + 2)[0];
    putchar 10
}

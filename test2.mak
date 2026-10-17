let extern printf : (ptr, i64) -> i32 = "printf" : (ptr, ...) -> i32;

{
    printf "01 literal -1: %ld\n" (-1);
    printf "02 0-1: %ld\n" (0-1);
    printf "03 -1-1: %ld\n" (-1-1);
    printf "04 -(1+1): %ld\n" (-(1+1));
    printf "05 --1: %ld\n" (--1);
    printf "06 ---1: %ld\n" (---1);
    printf "07 -(-1): %ld\n" (-(-1));
    printf "08 -5+2: %ld\n" (-5+2);
    printf "09 2+(-5): %ld\n" (2+(-5));
    printf "10 -5-(-2): %ld\n" (-5-(-2));
    printf "11 -2-(-5): %ld\n" (-2-(-5));
    printf "12 (-2)*3: %ld\n" ((-2)*3);
    printf "13 (-2)*(-3): %ld\n" ((-2)*(-3));
    printf "14 -(2*3): %ld\n" (-(2*3));
    printf "15 -1-1-1: %ld\n" (-1-1-1);
    printf "16 -1-(1-2): %ld\n" (-1-(1-2));
    printf "17 -(1-2): %ld\n" (-(1-2));
    printf "18 -1+2*3: %ld\n" (-1+2*3);
    printf "19 (-1+2)*3: %ld\n" (((-1)+2)*3);
    printf "20 -7/2: %ld\n" (-7/2);
    printf "21 -7 remainder 2: %ld\n" (-7%2);
    printf "22 7/(-2): %ld\n" (7/(-2));
    printf "23 7 remainder -2: %ld\n" (7%(-2));
    printf "24 (-7)/(-2): %ld\n" ((-7)/(-2));
    printf "25 -7 remainder -2: %ld\n" ((-7)%(-2));
    printf "26 (0-2)*3: %ld\n" ((0-2)*3);
    printf "27 0-(-1): %ld\n" (0-(-1));
    printf "28 -(0-1): %ld\n" (-(0-1));
    printf "29 (-3)*(-4)-20: %ld\n" (((-3)*(-4))-20);
    printf "30 i64 minimum: %ld\n" ((-9223372036854775807)-1);
    printf "31 max + 1: %ld\n" (9223372036854775807+1);
    printf "32 min - 1: %ld\n" ((-9223372036854775807-1)-1);
    printf "33 min * 2: %ld\n" ((-9223372036854775807-1)*2);
    printf "34 max * 2: %ld\n" (9223372036854775807*2)
}
/* ***********************************
   ***********************************
        BITBOARD CHESS ENGINE IN C
   ***********************************
   ***********************************/

#include <stdio.h>
#include <string.h>

// include important constants and enums
#include "enums/squares.h"
#include "enums/colors.h"
#include "enums/rook_or_bishop.h"
#include "constants.h"
#include "macros.h"

// include important functions and processes
#include "bitboard_functions/mask_attacks.c"
#include "pieces/init_leapers.c"
#include "pieces/slider_attacks.c"
#include "magic_numbers/magic_nums.c"

void init_all() {
    init_leapers_attacks();
}

int main(void)
{
    return 0;
}
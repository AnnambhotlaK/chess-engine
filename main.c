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
#include "bitboards.h"
#include "enums/castling.h"
#include "enums/pieces.h"

// include important functions and processes
#include "bitboard_functions/mask_attacks.c"
#include "pieces/init_leapers.c"
#include "pieces/slider_attacks.c"
#include "magic_numbers/magic_nums.c"

void init_all() {
    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);
}

int main(void)
{
    init_all();

    U64 occupancy = 0ULL;
    set_bit(occupancy, c5);

    print_bitboard(get_bishop_attacks(d4, occupancy));
    return 0;
}
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "ull.h"

// Converting each square in alg. notation to a coordinate
const char* SQUARE_TO_COORDINATES[] = {
    "a8",
    "b8",
    "c8",
    "d8",
    "e8",
    "f8",
    "g8",
    "h8",
    "a7",
    "b7",
    "c7",
    "d7",
    "e7",
    "f7",
    "g7",
    "h7",
    "a6",
    "b6",
    "c6",
    "d6",
    "e6",
    "f6",
    "g6",
    "h6",
    "a5",
    "b5",
    "c5",
    "d5",
    "e5",
    "f5",
    "g5",
    "h5",
    "a4",
    "b4",
    "c4",
    "d4",
    "e4",
    "f4",
    "g4",
    "h4",
    "a3",
    "b3",
    "c3",
    "d3",
    "e3",
    "f3",
    "g3",
    "h3",
    "a2",
    "b2",
    "c2",
    "d2",
    "e2",
    "f2",
    "g2",
    "h2",
    "a1",
    "b1",
    "c1",
    "d1",
    "e1",
    "f1",
    "g1",
    "h1",
};

// Constants for removing off-board moves
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;

// bishop relevant occupancy bit counts for each square
const int bishop_relevant_bits[64] = {
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    7,
    7,
    7,
    7,
    5,
    5,
    5,
    5,
    7,
    9,
    9,
    7,
    5,
    5,
    5,
    5,
    7,
    9,
    9,
    7,
    5,
    5,
    5,
    5,
    7,
    7,
    7,
    7,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
    5,
    5,
    5,
    5,
    5,
    5,
    6,
};

// rook relevant occupancy bit counts for each square
const int rook_relevant_bits[64] = {
    12,
    11,
    11,
    11,
    11,
    11,
    11,
    12,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    11,
    10,
    10,
    10,
    10,
    10,
    10,
    11,
    12,
    11,
    11,
    11,
    11,
    11,
    11,
    12,
};

// rook magic nums
const U64 rook_magic_numbers[64] = {
    0x98060280800000ULL,
    0x200102080410000ULL,
    0x1084003808010000ULL,
    0x240040040880000ULL,
    0x400024020140206ULL,
    0x400a11002040000ULL,
    0x400100140844800ULL,
    0x2080003444810200ULL,
    0x3010200800000ULL,
    0x4006422210000ULL,
    0x944100402080000ULL,
    0x8104100012c0a000ULL,
    0x4800800400088000ULL,
    0x802200040000ULL,
    0x800108024800ULL,
    0x40200092004400ULL,
    0xe40840026010000ULL,
    0x10020c020100002ULL,
    0x2010040014ULL,
    0x10240014088036ULL,
    0x818080040b0000ULL,
    0x805000a010020000ULL,
    0x4250012041800400ULL,
    0x4001200800409200ULL,
    0x100010a08040000ULL,
    0x20601008003ULL,
    0x20000043a0008001ULL,
    0x8000030030082001ULL,
    0x2808083444ULL,
    0x10e701041201800ULL,
    0x4480000c02020000ULL,
    0x124010108000ULL,
    0x600108400410001ULL,
    0x6000043000200002ULL,
    0xa804850c04004002ULL,
    0x14008080000ULL,
    0x4000240004004001ULL,
    0x2004120008101000ULL,
    0x10000c4285003000ULL,
    0xc00000020100d000ULL,
    0x1c0040a021220006ULL,
    0x8000004020028011ULL,
    0x8060c20003ULL,
    0x8020008400440001ULL,
    0xa62040000ULL,
    0x1418000202060010ULL,
    0x2404000401010000ULL,
    0x42100820000ULL,
    0xc000004000800080ULL,
    0x100002000400106ULL,
    0x900002048a401200ULL,
    0x201001000082100ULL,
    0x40000042a010a00ULL,
    0x130000409508200ULL,
    0x800000111000408ULL,
    0x80c000891220040ULL,
    0x280048440122066ULL,
    0x600004010920104ULL,
    0x308001008020500cULL,
    0x820000425001001ULL,
    0x8200002c0201084ULL,
    0x802024004209040cULL,
    0x1060000801100204ULL,
    0x88102100488402ULL,
};

// bishop magic nums
const U64 bishop_magic_numbers[64] = {
    0x2020448008100ULL,
    0x1820843102002050ULL,
    0x8908108112005000ULL,
    0x4042088010220ULL,
    0x4124504000060141ULL,
    0x2001042240440002ULL,
    0x104008884100082ULL,
    0x61208020210d0ULL,
    0x1015060a1092212ULL,
    0x10b4080228004900ULL,
    0x8200040822004025ULL,
    0x2022082044800ULL,
    0xa002411140224800ULL,
    0x120084802500004ULL,
    0x412804606104280ULL,
    0xcab0088088080250ULL,
    0x480081010c202ULL,
    0x108803602244400ULL,
    0x20884a1003820010ULL,
    0x44044824001061ULL,
    0x2200400a000a0ULL,
    0x6000248020802ULL,
    0x181020200900400ULL,
    0x8802208200842404ULL,
    0x120226064040400ULL,
    0x4021004284808ULL,
    0x4001404094050200ULL,
    0x480a040008010820ULL,
    0x2194082044002002ULL,
    0x2008a20001004200ULL,
    0x40908041041004ULL,
    0x881002200540404ULL,
    0x4001082002082101ULL,
    0x8110408880880ULL,
    0x8000404040080200ULL,
    0x200020082180080ULL,
    0x1184440400114100ULL,
    0xc220008020110412ULL,
    0x4088084040090100ULL,
    0x8822104100121080ULL,
    0x100111884008200aULL,
    0x2844040288820200ULL,
    0x90901088003010ULL,
    0x1000a218000400ULL,
    0x1102010420204ULL,
    0x8414a3483000200ULL,
    0x6410849901420400ULL,
    0x201080200901040ULL,
    0x204880808050002ULL,
    0x1001008201210000ULL,
    0x16a6300a890040aULL,
    0x8049000441108600ULL,
    0x2212002060410044ULL,
    0x100086308020020ULL,
    0x484241408020421ULL,
    0x105084028429c085ULL,
    0x4282480801080cULL,
    0x81c098488088240ULL,
    0x1400000090480820ULL,
    0x4444000030208810ULL,
    0x1020142010820200ULL,
    0x2234802004018200ULL,
    0xc2040450820a00ULL,
    0x2101021090020ULL,
};

#endif
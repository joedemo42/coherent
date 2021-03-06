#ifdef vax
#include "INC$LIB:cc1.h"
#else
#include "cc1.h"
#endif
#if !TINY
#define fl(f,l)	, f, l
#else
#define fl(f,l)	/* f, l */
#endif
extern char macros[];
/* /usr/src/c/n1/i8086/tables/prefac.f */
#define	WORD	(FS16|FU16|FSPTR|FSPTB)
#define	UWORD	(FU16|FSPTR|FSPTB)
#define	LONG	(FS32|FU32)
#define	INT	(FS16|FU16|FS32|FU32)
#define	BYTE	(FS8|FU8)
#define	NFLT	(FS8|FU8|FS16|FU16|FSPTR|FSPTB|FLPTR|FLPTB|FS32|FU32)
#define	PEREL	(PEQ|PNE)
#define	PSREL	(PEQ|PNE|PGT|PGE|PLT|PLE)
#define	PUREL	(PEQ|PNE|PUGT|PULE|PUGE|PULT)
#define	PREL	(PEQ|PNE|PGT|PGE|PLT|PLE|PUGT|PUGE|PULT|PULE)
#define	PNEREL	(PGT|PGE|PLT|PLE|PUGT|PUGE|PULT|PULE)
#define	LPTX	(FLPTR|FLPTB)
/* /usr/src/c/n1/i8086/tables/aadd.t */
PAT p1[] = {
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[   0] fl(1,8) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[   0] fl(1,12) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  2,  0, &macros[  12] fl(1,20) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  2,  0, &macros[  24] fl(1,29) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[  40] fl(1,39) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[  53] fl(1,47) }
/* #endif */,
 {  1,  1, ANYR, NONE, NONE, TEMP,  3,  1,  4,  1, &macros[  62] fl(1,55) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  3,  1,  4,  1, &macros[  62] fl(1,59) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[  67] fl(1,67) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  3,  1, &macros[  67] fl(1,71) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[  67] fl(1,75) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  3,  1, &macros[  67] fl(1,79) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  5,  1, &macros[  69] fl(1,87) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  5,  1, &macros[  74] fl(1,96) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[  79] fl(1,106) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[  84] fl(1,114) }
/* #endif */,
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  6,  5, &macros[  89] fl(1,122) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  5,  5, &macros[ 112] fl(1,131) },
 {  6,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[ 120] fl(1,141) },
 {  7,  1, ANYL, NONE, ANYL, TEMP,  1,  1,  7,  1, &macros[ 120] fl(1,145) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[ 125] fl(1,153) },
 {  6,  1,   AX,   AX, NONE,   AX,  1,  3,  7,  1, &macros[ 131] fl(1,162) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[ 137] fl(1,172) },
 {  4,  4, ANYL, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[ 142] fl(1,180) }
/* #endif */,
 {  8,  1, ANYR, NONE, NONE, TEMP,  1,  1,  4,  1, &macros[ 147] fl(1,188) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  4,  1, &macros[ 147] fl(1,192) },
 {  8,  1,   AX, NONE, NONE,   AX,  1,  2,  4,  1, &macros[ 158] fl(1,201) },
 {  8,  1,   AX, NONE, NONE,   AX,  1,  3,  4,  1, &macros[ 171] fl(1,211) },
 {  9,  5, DXAX, NONE, NONE, NONE,  1,  5,  8,  6, &macros[ 182] fl(1,221) },
 {  9,  5, DXAX, NONE, NONE, NONE,  1,  5,  8,  7, &macros[ 198] fl(1,230) },
 {  9,  5, DXAX, NONE, DXAX, NONE,  1,  5,  7,  5, &macros[ 209] fl(1,239) },
 { 10,  5, DXAX, NONE, DXAX, DXAX,  1,  5,  7,  5, &macros[ 211] fl(1,246) },
 { 11,  5, DXAX, NONE, NONE, DXAX,  1,  5,  4,  5, &macros[ 233] fl(1,256) },
 {  3,  1,   AX, NONE, NONE,   AX,  1,  8,  4,  1, &macros[ 255] fl(1,279) },
 {  3,  1, ANYR, NONE, NONE, TEMP,  1,  9,  4,  1, &macros[ 275] fl(1,290) }
/* #ifdef NDPDEF */,
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 12, &macros[ 292] fl(1,307) },
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 12,  7, 12, &macros[ 302] fl(1,315) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/aand.t */
PAT p2[] = {
 {  1,  1, ANYR, NONE, NONE, TEMP,  9,  1, 10,  1, &macros[ 312] fl(2,11) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[ 312] fl(2,15) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  5,  1, &macros[ 315] fl(2,23) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  5,  1, &macros[ 318] fl(2,32) },
 {  6,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[ 321] fl(2,41) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[ 324] fl(2,49) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  3,  7,  1, &macros[ 327] fl(2,58) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  6,  5, &macros[ 330] fl(2,67) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1,  5, 11,  5, &macros[ 338] fl(2,77) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1,  5, 12,  5, &macros[ 357] fl(2,85) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1,  5, 13,  5, &macros[ 365] fl(2,94) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  5,  5, &macros[ 371] fl(2,102) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1,  5,  7,  5, &macros[ 374] fl(2,112) },
 { 12,  6, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[ 386] fl(2,129) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[ 393] fl(2,136) },
 { 12,  6,   AX, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[ 408] fl(2,144) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[ 415] fl(2,151) },
 { 13,  6, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[ 423] fl(2,159) },
 { 13, 13, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[ 428] fl(2,166) },
 { 13,  6,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[ 433] fl(2,174) },
 { 13, 13,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[ 439] fl(2,181) }
};
/* /usr/src/c/n1/i8086/tables/add.t */
PAT p3[] = {
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  2,  0, &macros[ 445] fl(3,11) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1,  2,  0, &macros[ 445] fl(3,15) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  2,  0, &macros[ 448] fl(3,23) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  2,  0, &macros[ 448] fl(3,27) }
/* #endif */,
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1, 15,  0, &macros[ 452] fl(3,34) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1, 15,  0, &macros[ 452] fl(3,38) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4, 15,  0, &macros[ 456] fl(3,47) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4, 15,  0, &macros[ 456] fl(3,51) }
/* #endif */,
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  1, &macros[ 461] fl(3,59) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1,  4,  1, &macros[ 461] fl(3,63) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  4,  1, &macros[ 466] fl(3,71) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  4,  1, &macros[ 466] fl(3,75) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 12,  4, DXAX,   AX, NONE, TEMP,  7,  1, 16,  4, &macros[ 471] fl(3,94) },
 {  2,  4, ANYL, LOTEMP,   AX, TEMP,  7,  1, 16,  4, &macros[ 480] fl(3,101) },
 { 12,  4, DXAX,   AX, NONE, TEMP,  7,  1, 17,  4, &macros[ 495] fl(3,109) },
 {  2,  4, ANYL, LOTEMP,   AX, TEMP,  7,  1, 17,  4, &macros[ 501] fl(3,116) },
 { 12,  4, DXAX,   AX, NONE, TEMP,  7,  1, 18,  4, &macros[ 513] fl(3,124) },
 { 12,  4, DXAX,   AX, NONE, TEMP,  7,  1, 19,  4, &macros[ 523] fl(3,131) },
 {  2,  4, ANYL, LOTEMP, NONE, TEMP,  7,  1, 18,  4, &macros[ 533] fl(3,138) },
 {  2,  4, ANYL, LOTEMP, NONE, TEMP,  7,  1, 19,  4, &macros[ 543] fl(3,146) },
 { 12,  4, DXAX,   AX, NONE, DXAX,  7,  1,  3,  4, &macros[ 552] fl(3,154) },
 {  2,  4, ANYL, LOTEMP, NONE, TEMP,  7,  1,  3,  4, &macros[ 563] fl(3,161) },
 { 12,  4, DXAX,   AX, NONE, DXAX,  7,  1, 20,  4, &macros[ 573] fl(3,169) },
 {  2,  4, ANYL, LOTEMP, NONE, TEMP,  7,  1, 20,  4, &macros[ 573] fl(3,173) }
/* #endif */,
 { 17,  5, DXAX, DXAX, NONE, DXAX,  7,  5,  4,  5, &macros[ 575] fl(3,181) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  4,  5, &macros[ 587] fl(3,190) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  4,  5, &macros[ 587] fl(3,194) },
 { 12,  4, DXAX, DXAX, NONE, DXAX,  7,  5,  4,  4, &macros[ 593] fl(3,200) },
 {  2,  4, ESAX, DXAX, NONE, DXAX,  7,  5,  4,  4, &macros[ 593] fl(3,204) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 18,  4,   AX, NONE, NONE, NONE, 20,  4,  5,  1, &macros[ 595] fl(3,222) },
 { 18,  4,   AX, NONE, NONE, NONE,  5,  1, 20,  4, &macros[ 608] fl(3,231) }
/* #endif */
/* #ifdef NDPDEF */,
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20,  6, &macros[ 620] fl(3,249) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 14, &macros[ 623] fl(3,255) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 11, &macros[ 626] fl(3,261) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 12, &macros[ 629] fl(3,267) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/adiv.t */
PAT p4[] = {
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  6, 20,  1, &macros[ 632] fl(4,10) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  7, 20,  1, &macros[ 643] fl(4,19) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  9, 20,  1, &macros[ 655] fl(4,28) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  2, 20,  1, &macros[ 683] fl(4,40) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  3, 20,  1, &macros[ 701] fl(4,51) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  8, 20,  1, &macros[ 731] fl(4,62) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1, 14,  7,  5, &macros[ 763] fl(4,74) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1, 15,  7,  5, &macros[ 791] fl(4,87) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 14, 20,  5, &macros[ 808] fl(4,100) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 15, 20,  5, &macros[ 816] fl(4,113) }
/* #ifdef NDPDEF */,
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 12, &macros[ 824] fl(4,134) },
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 12,  7, 12, &macros[ 828] fl(4,142) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/aft.t */
PAT p5[] = {
 { 12,  1, ANYR, NONE, NONE, TEMP, 20,  1,  2,  0, &macros[ 832] fl(5,13) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 20,  1,  2,  0, &macros[ 832] fl(5,17) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP, 20,  4,  2,  0, &macros[ 840] fl(5,25) },
 {  4,  4, ANYL, NONE, NONE, TEMP, 20,  4,  2,  0, &macros[ 852] fl(5,33) }
/* #endif */,
 { 12,  1, ANYR, NONE, NONE, TEMP, 20,  1,  5,  1, &macros[ 857] fl(5,41) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 20,  1,  5,  1, &macros[ 857] fl(5,45) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP, 20,  4,  5,  1, &macros[ 866] fl(5,53) },
 { 20,  4, ANYL, NONE, NONE, TEMP, 20,  4,  5,  1, &macros[ 878] fl(5,61) }
/* #endif */,
 { 12,  1,   AX, NONE, NONE,   AX, 20,  2,  2,  0, &macros[ 883] fl(5,69) },
 { 12,  1,   AX, NONE, NONE,   AX, 20,  3,  2,  0, &macros[ 895] fl(5,77) },
 { 12,  5, ANYR, NONE, NONE, TEMP, 20,  5,  2,  0, &macros[ 909] fl(5,85) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  9,  5,  1, &macros[ 934] fl(5,94) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  8,  5,  1, &macros[ 966] fl(5,106) }
};
/* /usr/src/c/n1/i8086/tables/amul.t */
PAT p6[] = {
 { 21,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[ 994] fl(6,11) },
 { 22,  1, DXAX, NONE, NONE,   AX,  1,  6, 20,  1, &macros[1002] fl(6,18) },
 { 22,  1, DXAX, NONE, NONE,   AX,  1, 13, 20,  1, &macros[1010] fl(6,26) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  9, 20,  1, &macros[1018] fl(6,34) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  2, 20,  1, &macros[1031] fl(6,45) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  3, 20,  1, &macros[1041] fl(6,55) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  8, 20,  1, &macros[1057] fl(6,65) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1, 14,  7,  5, &macros[1071] fl(6,76) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1, 15,  7,  5, &macros[1089] fl(6,89) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 14, 20,  5, &macros[1107] fl(6,102) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 15, 20,  5, &macros[1115] fl(6,115) }
/* #ifdef NDPDEF */,
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 12, &macros[1123] fl(6,136) },
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 12,  7, 12, &macros[1127] fl(6,144) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/and.t */
PAT p7[] = {
 { 23,  0, NONE, NONE, NONE, NONE, 20,  1,  5,  1, &macros[1131] fl(7,13) },
 { 23,  0, NONE, NONE, NONE, NONE,  9,  1,  4,  1, &macros[1131] fl(7,17) },
 { 23,  0, NONE, NONE, NONE, NONE, 20, 16,  5,  1, &macros[1137] fl(7,24) },
 { 24,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  1, &macros[1140] fl(7,31) },
 { 25,  0, NONE, NONE, NONE, NONE, 20,  5,  5,  5, &macros[1144] fl(7,38) },
 { 25,  0,   AX, NONE, NONE, NONE, 20,  5,  4,  5, &macros[1152] fl(7,45) },
 { 25,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[1160] fl(7,53) },
 { 26,  0, NONE, NONE, NONE, NONE,  4,  5, 12,  5, &macros[1165] fl(7,60) },
 { 26,  0, NONE, NONE, NONE, NONE,  4,  5,  6,  5, &macros[1171] fl(7,67) },
 { 26,  0, NONE, ANYR, NONE, NONE,  7,  5, 12,  5, &macros[1177] fl(7,74) },
 { 26,  0, NONE, ANYR, NONE, NONE,  7,  5,  6,  5, &macros[1182] fl(7,81) },
 { 27,  0, NONE, NONE, NONE, NONE,  4,  5,  5,  5, &macros[1187] fl(7,88) },
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[1202] fl(7,97) },
 { 28,  0, NONE, NONE, NONE, NONE,  4,  5,  5,  5, &macros[1214] fl(7,106) },
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[1230] fl(7,116) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 13,  5, &macros[1242] fl(7,126) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 12,  5, &macros[1246] fl(7,132) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 11,  5, &macros[1250] fl(7,139) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  6,  5, &macros[1254] fl(7,145) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  4,  5, &macros[1261] fl(7,152) }
};
/* /usr/src/c/n1/i8086/tables/aor.t */
PAT p8[] = {
 {  1,  1, ANYR, NONE, NONE, TEMP,  9,  1, 10,  1, &macros[1268] fl(8,11) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 21,  1, 10,  1, &macros[1268] fl(8,15) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[1268] fl(8,19) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[1268] fl(8,23) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  5,  1, &macros[1271] fl(8,31) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  5,  1, &macros[1274] fl(8,40) },
 {  6,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[1277] fl(8,49) },
 {  7,  1, ANYL, NONE, ANYL, TEMP,  1,  1,  7,  1, &macros[1277] fl(8,53) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[1280] fl(8,61) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  3,  7,  1, &macros[1283] fl(8,70) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  6,  5, &macros[1286] fl(8,79) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1,  5, 12,  5, &macros[1289] fl(8,88) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  5,  5, &macros[1293] fl(8,96) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1,  5,  7,  5, &macros[1300] fl(8,106) },
 { 12,  6, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[1309] fl(8,124) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[1313] fl(8,131) },
 { 12,  6,   AX, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[1317] fl(8,139) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[1321] fl(8,146) },
 { 13,  6, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[1325] fl(8,154) },
 { 13, 13, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[1329] fl(8,161) },
 { 13,  6,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[1333] fl(8,169) },
 { 13, 13,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[1337] fl(8,176) }
};
/* /usr/src/c/n1/i8086/tables/arem.t */
PAT p9[] = {
 { 22,  1, DXAX, NONE, NONE,   DX,  1,  6, 20,  1, &macros[1341] fl(9,11) },
 { 22,  1, DXAX, NONE, NONE,   DX,  1, 13, 20,  1, &macros[1351] fl(9,20) },
 { 12,  1, DXAX, NONE, NONE,   DX,  1,  9, 20,  1, &macros[1363] fl(9,29) },
 { 12,  1, DXAX, NONE, NONE,   AX,  1,  2, 20,  1, &macros[1391] fl(9,41) },
 { 12,  1, DXAX, NONE, NONE,   DX,  1,  3, 20,  1, &macros[1417] fl(9,53) },
 { 12,  1, DXAX, NONE, NONE,   DX,  1,  8, 20,  1, &macros[1447] fl(9,64) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 14, 20,  5, &macros[1478] fl(9,76) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1, 15, 20,  5, &macros[1496] fl(9,89) }
};
/* /usr/src/c/n1/i8086/tables/ashl.t */
PAT p10[] = {
 { 29,  1, ANYR, NONE, NONE, TEMP,  1,  1, 22,  1, &macros[1514] fl(10,12) },
 { 30,  1, ANYL, NONE, NONE, TEMP,  1,  1, 22,  1, &macros[1514] fl(10,16) },
 { 29,  1,   AX, NONE, NONE, TEMP,  1,  2, 22,  1, &macros[1517] fl(10,24) },
 { 29,  1,   AX, NONE, NONE, TEMP,  1,  3, 22,  1, &macros[1520] fl(10,33) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[1523] fl(10,42) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[1523] fl(10,46) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  2,  2,  0, &macros[1529] fl(10,54) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  3,  2,  0, &macros[1535] fl(10,63) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1, 15,  0, &macros[1541] fl(10,72) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1, 15,  0, &macros[1541] fl(10,76) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  2, 15,  0, &macros[1547] fl(10,85) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  3, 15,  0, &macros[1553] fl(10,95) },
 {  1,  1, ANYR, NONE,   CX, TEMP,  1,  1,  5,  1, &macros[1559] fl(10,105) },
 {  2,  1, ANYL, NONE,   CX, TEMP,  1,  1,  5,  1, &macros[1559] fl(10,109) },
 {  1,  1,   AX, NONE,   CX, TEMP,  1,  2,  5,  1, &macros[1569] fl(10,118) },
 {  1,  1,   AX, NONE,   CX, TEMP,  1,  3,  5,  1, &macros[1579] fl(10,128) },
 { 11,  5, DXAX, NONE,   CX, TEMP,  1,  5,  5,  1, &macros[1589] fl(10,138) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  1, 20,  1, &macros[1618] fl(10,152) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  2, 20,  1, &macros[1629] fl(10,160) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  3, 20,  1, &macros[1644] fl(10,169) },
 {  3,  5, DXAX, NONE,   CX, TEMP,  1,  5, 20,  1, &macros[1664] fl(10,178) },
 { 31,  1, ANYR, NONE, NONE, TEMP,  1,  9, 22,  1, &macros[1705] fl(10,193) },
 { 31,  1,   AX, NONE, NONE, TEMP,  1,  8, 22,  1, &macros[1716] fl(10,204) },
 { 12,  1, ANYR, NONE, NONE, TEMP,  1,  9,  2,  0, &macros[1741] fl(10,215) },
 { 12,  1,   AX, NONE, NONE, TEMP,  1,  8,  2,  0, &macros[1743] fl(10,226) },
 { 12,  1, ANYR, NONE, NONE, TEMP,  1,  9, 15,  0, &macros[1745] fl(10,237) },
 { 12,  1,   AX, NONE, NONE, TEMP,  1,  8, 15,  0, &macros[1763] fl(10,249) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  9,  5,  1, &macros[1784] fl(10,261) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  8,  5,  1, &macros[1802] fl(10,273) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  9, 20,  1, &macros[1822] fl(10,285) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  8, 20,  1, &macros[1834] fl(10,297) }
};
/* /usr/src/c/n1/i8086/tables/ashr.t */
PAT p11[] = {
 { 29,  1, ANYR, NONE, NONE, TEMP,  1,  6, 22,  1, &macros[1848] fl(11,12) },
 { 29,  1, ANYR, NONE, NONE, TEMP,  1, 13, 22,  1, &macros[1851] fl(11,20) },
 { 31,  1, ANYR, NONE, NONE, TEMP,  1,  9, 22,  1, &macros[1854] fl(11,28) },
 { 29,  1,   AX, NONE, NONE, TEMP,  1,  2, 22,  1, &macros[1865] fl(11,39) },
 { 29,  1,   AX, NONE, NONE, TEMP,  1,  3, 22,  1, &macros[1868] fl(11,48) },
 { 31,  1,   AX, NONE, NONE, TEMP,  1,  8, 22,  1, &macros[1871] fl(11,57) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  6,  2,  0, &macros[1883] fl(11,68) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1, 13,  2,  0, &macros[1885] fl(11,76) },
 { 12,  1, ANYR, NONE, NONE, TEMP,  1,  9,  2,  0, &macros[1887] fl(11,84) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  2,  2,  0, &macros[1889] fl(11,95) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  3,  2,  0, &macros[1891] fl(11,104) },
 { 12,  1,   AX, NONE, NONE, TEMP,  1,  8,  2,  0, &macros[1893] fl(11,113) },
 { 12,  5, DXAX, NONE, NONE, TEMP,  1, 14,  2,  0, &macros[1895] fl(11,124) },
 { 12,  5, DXAX, NONE, NONE, TEMP,  1, 15,  2,  0, &macros[1905] fl(11,133) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  6, 15,  0, &macros[1908] fl(11,142) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1, 13, 15,  0, &macros[1916] fl(11,151) },
 { 12,  1, ANYR, NONE, NONE, TEMP,  1,  9, 15,  0, &macros[1924] fl(11,160) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  2, 15,  0, &macros[1938] fl(11,172) },
 {  1,  1,   AX, NONE, NONE, TEMP,  1,  3, 15,  0, &macros[1946] fl(11,182) },
 { 12,  1,   AX, NONE, NONE, TEMP,  1,  8, 15,  0, &macros[1954] fl(11,192) },
 {  1,  1, ANYR, NONE,   CX, TEMP,  1,  6,  5,  1, &macros[1971] fl(11,204) },
 {  1,  1, ANYR, NONE,   CX, TEMP,  1, 13,  5,  1, &macros[1979] fl(11,213) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  9,  5,  1, &macros[1987] fl(11,222) },
 {  1,  1,   AX, NONE,   CX, TEMP,  1,  2,  5,  1, &macros[2001] fl(11,234) },
 {  1,  1,   AX, NONE,   CX, TEMP,  1,  3,  5,  1, &macros[2009] fl(11,244) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  8,  5,  1, &macros[2017] fl(11,254) },
 { 11,  5, DXAX, NONE,   CX, TEMP,  1, 14,  5,  1, &macros[2033] fl(11,266) },
 { 11,  5, DXAX, NONE,   CX, DXAX,  1, 15,  5,  1, &macros[2058] fl(11,280) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  6, 20,  1, &macros[2076] fl(11,294) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1, 13, 20,  1, &macros[2084] fl(11,302) },
 { 12,  1, ANYR, NONE,   CX, TEMP,  1,  9, 20,  1, &macros[2092] fl(11,310) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  2, 20,  1, &macros[2104] fl(11,322) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  3, 20,  1, &macros[2112] fl(11,331) },
 { 12,  1,   AX, NONE,   CX, TEMP,  1,  8, 20,  1, &macros[2120] fl(11,340) },
 {  3,  5, DXAX, NONE,   CX, TEMP,  1, 14, 20,  1, &macros[2134] fl(11,352) },
 {  3,  5, DXAX, NONE,   CX, TEMP,  1, 15, 20,  1, &macros[2161] fl(11,367) }
};
/* /usr/src/c/n1/i8086/tables/assign.t */
PAT p12[] = {
 {  9,  1, NONE, NONE, NONE, NONE,  3,  1, 23,  0, &macros[2181] fl(12,18) },
 {  9,  1, NONE, NONE, NONE, NONE,  3,  1, 10,  1, &macros[2185] fl(12,24) },
 {  9,  1, NONE, NONE, NONE, NONE,  1,  1,  3,  1, &macros[2185] fl(12,28) },
 {  9,  1, NONE, NONE, NONE, NONE,  1,  1,  5,  1, &macros[2185] fl(12,32) },
 {  9,  1, NONE, NONE, NONE, NONE,  3,  1, 10,  5, &macros[2189] fl(12,38) },
 {  9,  1, NONE, NONE, NONE, NONE,  9,  1, 24,  1, &macros[2194] fl(12,44) },
 {  9,  1, NONE, NONE, NONE, NONE,  1, 16,  5,  1, &macros[2199] fl(12,50) },
 {  9,  5, NONE, NONE, NONE, NONE,  1,  5,  5,  5, &macros[2203] fl(12,56) },
 { 32,  1, ANYR, NONE, ANYR, TEMP,  1,  2,  7, 16, &macros[2214] fl(12,63) },
 { 33,  1,   AX, NONE,   AX, TEMP,  1,  2,  7, 16, &macros[2225] fl(12,71) },
 { 34,  1, ANYR, NONE, ANYR, TEMP,  1,  3,  7, 16, &macros[2231] fl(12,78) },
 { 35,  1,   AX, NONE, NONE,   AX,  1,  2, 20, 16, &macros[2244] fl(12,87) },
 { 35,  1,   AX, NONE, NONE,   AX,  1,  3, 20, 16, &macros[2256] fl(12,97) },
 { 13,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[2262] fl(12,107) },
 {  7,  1, ANYL, NONE, ANYL, TEMP,  1,  1,  7,  1, &macros[2262] fl(12,111) },
 {  9,  1,   AX, NONE, NONE, NONE,  1,  1, 10,  1, &macros[2265] fl(12,117) },
 { 13,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[2270] fl(12,124) },
 { 13,  1,   AX, NONE,   AX,   AX,  1,  3,  7,  1, &macros[2275] fl(12,131) },
 { 33,  5, ANYR, NONE, ANYR, TEMP,  1,  5,  7,  5, &macros[2282] fl(12,138) },
 { 12,  1,   AX, NONE, DXAX,   AX,  1,  1,  7,  5, &macros[2285] fl(12,145) },
 { 13,  1, DXAX, NONE, DXAX,   AX,  1,  2,  7,  5, &macros[2288] fl(12,151) },
 { 13,  1, DXAX, NONE, DXAX,   AX,  1,  3,  7,  5, &macros[2291] fl(12,158) }
/* #ifndef ONLYSMALL */,
 {  9,  4, NONE, NONE, NONE, NONE,  1,  4, 23,  0, &macros[2294] fl(12,175) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4, 23,  0, &macros[2303] fl(12,182) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4, 16,  4, &macros[2323] fl(12,191) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4, 16,  4, &macros[2335] fl(12,200) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4, 17,  4, &macros[2346] fl(12,208) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4, 17,  4, &macros[2359] fl(12,217) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4, 18,  4, &macros[2370] fl(12,225) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4, 19,  4, &macros[2382] fl(12,234) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4, 18,  4, &macros[2394] fl(12,243) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4, 19,  4, &macros[2405] fl(12,251) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  8,  1, &macros[2416] fl(12,259) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4,  8,  1, &macros[2418] fl(12,268) },
 {  3,  4, ANYR, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[2429] fl(12,277) },
 {  4,  4, ANYL, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[2440] fl(12,286) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  8,  5, &macros[2446] fl(12,294) },
 {  3,  5, ANYR, NONE, NONE, TEMP,  1,  5,  8,  4, &macros[2446] fl(12,298) },
 {  3,  4, ANYR, NONE, ANYR, TEMP,  1,  4,  7,  5, &macros[2448] fl(12,307) },
 {  4,  4, ANYL, NONE, ANYR, TEMP,  1,  4,  7,  5, &macros[2459] fl(12,316) },
 { 33,  4, ANYR, NONE, ANYR, TEMP,  1,  4,  7,  4, &macros[2470] fl(12,324) },
 {  7,  4, ANYL, NONE, ANYL, TEMP,  1,  4,  7,  4, &macros[2470] fl(12,328) },
 { 36,  5, ANYR, NONE, ANYR, TEMP,  1,  5,  7,  4, &macros[2473] fl(12,335) },
 { 37,  1, ESAX, NONE, ESAX,   AX,  1,  1,  7,  4, &macros[2484] fl(12,342) },
 { 37,  1, DXAX, NONE, DXAX,   AX,  1,  2,  7,  4, &macros[2487] fl(12,348) },
 { 37,  1, DXAX, NONE, DXAX,   AX,  1,  3,  7,  4, &macros[2490] fl(12,355) }
/* #endif */,
 { 12,  6, ANYR, NONE, NONE, TEMP,  1,  9,  5,  1, &macros[2493] fl(12,372) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  9,  5,  1, &macros[2500] fl(12,380) },
 { 13,  6, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[2507] fl(12,389) },
 { 13, 13, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[2514] fl(12,397) },
 { 13,  6,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[2521] fl(12,406) },
 { 13, 13,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[2528] fl(12,414) }
/* #ifndef NDPDEF */
/* #endif */
/* #ifdef  NDPDEF */,
 { 38, 12, FPAC, NONE, FPAC, NONE,  1, 11,  7, 12, &macros[2535] fl(12,500) },
 { 38, 12, FPAC, NONE, FPAC, NONE,  1, 11,  7, 11, &macros[2535] fl(12,504) },
 { 33, 12, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 12, &macros[2538] fl(12,510) },
 { 33, 12, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 11, &macros[2538] fl(12,514) },
 { 38, 12, FPAC, NONE, FPAC, NONE,  1, 12,  7, 12, &macros[2541] fl(12,520) },
 { 33, 12, FPAC, NONE, FPAC, FPAC,  1, 12,  7, 12, &macros[2544] fl(12,526) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/asub.t */
PAT p13[] = {
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[2547] fl(13,8) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[2547] fl(13,12) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  2,  0, &macros[2550] fl(13,20) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  2,  0, &macros[2553] fl(13,29) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[2556] fl(13,39) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[2559] fl(13,47) }
/* #endif */,
 {  1,  1, ANYR, NONE, NONE, TEMP,  3,  1,  4,  1, &macros[2562] fl(13,55) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  3,  1,  4,  1, &macros[2562] fl(13,59) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2565] fl(13,67) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  3,  1, &macros[2565] fl(13,71) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2565] fl(13,75) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  3,  1, &macros[2565] fl(13,79) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  5,  1, &macros[2567] fl(13,87) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  5,  1, &macros[2570] fl(13,96) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[2573] fl(13,106) },
 {  4,  4, ANYL, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[2576] fl(13,114) }
/* #endif */,
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  6,  5, &macros[2579] fl(13,122) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  5,  5, &macros[2582] fl(13,131) },
 {  6,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[2590] fl(13,141) },
 {  7,  1, ANYL, NONE, ANYL, TEMP,  1,  1,  7,  1, &macros[2590] fl(13,145) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[2593] fl(13,153) },
 {  6,  1,   AX,   AX, NONE,   AX,  1,  3,  7,  1, &macros[2596] fl(13,162) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[2599] fl(13,172) },
 {  4,  4, ANYL, NONE, ANYR, TEMP,  1,  4,  7,  1, &macros[2602] fl(13,180) }
/* #endif */,
 {  8,  1, ANYR, NONE, NONE, TEMP,  1,  1,  4,  1, &macros[2605] fl(13,188) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  4,  1, &macros[2605] fl(13,192) },
 {  8,  1,   AX, NONE, NONE,   AX,  1,  2,  4,  1, &macros[2611] fl(13,201) },
 {  8,  1,   AX, NONE, NONE,   AX,  1,  3,  4,  1, &macros[2619] fl(13,211) },
 {  9,  5, DXAX, NONE, NONE, NONE,  1,  5,  8,  6, &macros[2630] fl(13,221) },
 {  9,  5, DXAX, NONE, NONE, NONE,  1,  5,  8,  7, &macros[2643] fl(13,230) },
 {  9,  5, DXAX, NONE, DXAX, NONE,  1,  5,  7,  5, &macros[2654] fl(13,239) },
 { 10,  5, DXAX, NONE, DXAX, DXAX,  1,  5,  7,  5, &macros[2656] fl(13,246) },
 { 11,  5, DXAX, NONE, NONE, DXAX,  1,  5,  4,  5, &macros[2678] fl(13,256) },
 {  3,  1,   AX, NONE, NONE,   AX,  1,  8,  4,  1, &macros[2696] fl(13,280) },
 {  3,  1, ANYR, NONE, NONE, TEMP,  1,  9,  4,  1, &macros[2705] fl(13,291) }
/* #ifdef NDPDEF */,
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 11,  7, 12, &macros[2712] fl(13,309) },
 { 12, 10, FPAC, NONE, FPAC, FPAC,  1, 12,  7, 12, &macros[2716] fl(13,317) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/axor.t */
PAT p14[] = {
 {  1,  1, ANYR, NONE, NONE, TEMP,  9,  1, 10,  1, &macros[2720] fl(14,11) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 21,  1, 10,  1, &macros[2720] fl(14,15) },
 {  1,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2720] fl(14,19) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2720] fl(14,23) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  2,  5,  1, &macros[2723] fl(14,31) },
 {  1,  1,   AX, NONE, NONE,   AX,  1,  3,  5,  1, &macros[2726] fl(14,40) },
 {  6,  1, ANYR, NONE, ANYR, TEMP,  1,  1,  7,  1, &macros[2729] fl(14,49) },
 {  7,  1, ANYL, NONE, ANYL, TEMP,  1,  1,  7,  1, &macros[2729] fl(14,53) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  2,  7,  1, &macros[2732] fl(14,61) },
 {  6,  1,   AX, NONE,   AX,   AX,  1,  3,  7,  1, &macros[2735] fl(14,70) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  6,  5, &macros[2738] fl(14,79) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5, 11,  5, &macros[2741] fl(14,88) },
 { 12,  5, DXAX, NONE, NONE, DXAX,  1,  5, 12,  5, &macros[2746] fl(14,98) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5, 13,  5, &macros[2750] fl(14,106) },
 {  5,  5, DXAX, NONE, NONE, DXAX,  1,  5,  5,  5, &macros[2760] fl(14,116) },
 { 12,  5, DXAX, NONE, DXAX, DXAX,  1,  5,  7,  5, &macros[2767] fl(14,126) },
 { 12,  6, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[2776] fl(14,144) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  9, 14,  1, &macros[2780] fl(14,151) },
 { 12,  6,   AX, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[2784] fl(14,159) },
 { 12, 13, ANYR, NONE, NONE, TEMP,  1,  8, 14,  1, &macros[2788] fl(14,166) },
 { 13,  6, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[2792] fl(14,174) },
 { 13, 13, ANYR, NONE, ANYR, TEMP,  1,  9,  7,  1, &macros[2796] fl(14,181) },
 { 13,  6,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[2800] fl(14,189) },
 { 13, 13,   AX, NONE,   AX, TEMP,  1,  8,  7,  1, &macros[2804] fl(14,196) }
};
/* /usr/src/c/n1/i8086/tables/bef.t */
PAT p15[] = {
 { 39,  1, ANYR, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[2808] fl(15,11) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  2,  0, &macros[2808] fl(15,15) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[2811] fl(15,24) },
 { 20,  4, ANYL, NONE, NONE, TEMP,  1,  4,  2,  0, &macros[2814] fl(15,32) }
/* #endif */,
 { 39,  1, ANYR, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2817] fl(15,40) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  1,  1,  5,  1, &macros[2817] fl(15,44) }
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[2820] fl(15,53) },
 { 20,  4, ANYL, NONE, NONE, TEMP,  1,  4,  5,  1, &macros[2823] fl(15,61) }
/* #endif */,
 { 39,  1,   AX, NONE, NONE,   AX,  1,  2,  2,  0, &macros[2826] fl(15,69) },
 { 39,  1,   AX, NONE, NONE,   AX,  1,  3,  2,  0, &macros[2830] fl(15,78) },
 {  5,  5, ANYR, NONE, NONE, TEMP,  1,  5,  2,  0, &macros[2834] fl(15,87) },
 { 12,  1, ANYR, NONE, NONE, TEMP,  1,  9,  5,  1, &macros[2846] fl(15,97) },
 { 12,  1,   AX, NONE, NONE,   AX,  1,  8,  5,  1, &macros[2853] fl(15,107) }
};
/* /usr/src/c/n1/i8086/tables/div.t */
PAT p16[] = {
 { 22,  1, DXAX,   AX, NONE,   AX,  7,  6, 20,  1, &macros[2861] fl(16,9) },
 { 22,  1, DXAX,   AX, NONE,   AX,  7, 13, 20,  1, &macros[2865] fl(16,16) }
/* #ifdef NDPDEF */,
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20,  6, &macros[2873] fl(16,30) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 14, &macros[2876] fl(16,36) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 11, &macros[2879] fl(16,42) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 12, &macros[2882] fl(16,48) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/leaves.t */
PAT p17[] = {
 {  3,  1, ANYR, NONE, NONE, TEMP, 23,  0,  0,  0, &macros[2885] fl(17,29) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 23,  0,  0,  0, &macros[2885] fl(17,33) },
 {  3, 17, ANYR, NONE, NONE, TEMP, 23,  0,  0,  0, &macros[2889] fl(17,39) }
/* #ifndef ONLYSMALL */,
 {  2,  4, ANYL, NONE, NONE, TEMP, 23,  0,  0,  0, &macros[2889] fl(17,44) }
/* #endif */
/* #ifdef NDPDEF */,
 {  3, 12, FPAC, NONE, NONE, FPAC, 23,  0,  0,  0, &macros[2900] fl(17,62) },
 {  3, 12, FPAC, NONE, NONE, FPAC,  2,  0,  0,  0, &macros[2902] fl(17,68) }
/* #endif */,
 {  3,  5, ANYR, NONE, NONE, TEMP, 12,  5,  0,  0, &macros[2904] fl(17,82) },
 {  3,  5, ANYR, NONE, NONE, TEMP,  6,  5,  0,  0, &macros[2912] fl(17,89) },
 {  3,  1, ANYR, NONE, NONE, TEMP, 24,  1,  0,  0, &macros[2919] fl(17,104) },
 {  2,  1, ANYL, NONE, NONE, TEMP, 24,  1,  0,  0, &macros[2919] fl(17,108) },
 {  3, 18, ANYR, NONE, NONE, TEMP,  5, 16,  0,  0, &macros[2924] fl(17,114) },
 {  3,  1, ANYR, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[2924] fl(17,118) },
 {  2,  1, ANYL, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[2924] fl(17,122) },
 {  3,  5, ANYR, NONE, NONE, TEMP,  4,  5,  0,  0, &macros[2928] fl(17,128) },
 {  2,  5, ANYL, NONE, NONE, TEMP,  4,  5,  0,  0, &macros[2928] fl(17,132) }
/* #ifdef NDPDEF */,
 {  3, 12, FPAC, NONE, NONE, FPAC, 20, 12,  0,  0, &macros[2930] fl(17,139) }
/* #endif */
/* #ifndef ONLYSMALL */,
 {  3,  4, ANYR, NONE, NONE, TEMP, 25,  4,  0,  0, &macros[2933] fl(17,156) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 25,  4,  0,  0, &macros[2933] fl(17,160) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 26,  4,  0,  0, &macros[2944] fl(17,168) },
 {  2,  4, ANYL, NONE, NONE, TEMP,  9,  4,  0,  0, &macros[2944] fl(17,172) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 27,  4,  0,  0, &macros[2946] fl(17,179) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 28,  4,  0,  0, &macros[2946] fl(17,183) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 16,  4,  0,  0, &macros[2957] fl(17,191) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 16,  4,  0,  0, &macros[2962] fl(17,198) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 17,  4,  0,  0, &macros[2976] fl(17,206) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 17,  4,  0,  0, &macros[2978] fl(17,213) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 18,  4,  0,  0, &macros[2994] fl(17,221) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 18,  4,  0,  0, &macros[2994] fl(17,225) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 19,  4,  0,  0, &macros[3001] fl(17,233) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 19,  4,  0,  0, &macros[3001] fl(17,237) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 20,  4,  0,  0, &macros[3008] fl(17,245) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 20,  4,  0,  0, &macros[3010] fl(17,252) }
/* #endif */
/* #ifndef NDPDEF */
/* #endif */,
 {  3,  1,   AX, NONE, NONE, TEMP,  9, 16,  0,  0, &macros[3013] fl(17,308) },
 { 37, 19,   AX, NONE, NONE, TEMP,  4,  2,  0,  0, &macros[3018] fl(17,315) },
 { 37, 20,   AX, NONE, NONE, TEMP,  4,  3,  0,  0, &macros[3025] fl(17,322) },
 { 19,  1, ANYR, ANYR, NONE, TEMP,  7, 16,  0,  0, &macros[3032] fl(17,329) },
 {  3,  5, DXAX, NONE, NONE, TEMP,  9, 16,  0,  0, &macros[3033] fl(17,336) },
 { 19,  5, DXAX,   AX, NONE, TEMP,  7, 16,  0,  0, &macros[3039] fl(17,343) },
 {  3,  5, DXAX, NONE, NONE, TEMP, 20,  2,  0,  0, &macros[3041] fl(17,349) },
 {  3,  5, DXAX, NONE, NONE, TEMP, 20,  3,  0,  0, &macros[3049] fl(17,357) },
 {  3,  5, DXAX, NONE, NONE, TEMP,  4,  6,  0,  0, &macros[3063] fl(17,365) },
 {  3,  5, DXAX, NONE, NONE, TEMP,  4, 13,  0,  0, &macros[3065] fl(17,372) }
/* #ifdef NDPDEF */,
 {  3, 10, FPAC, NONE, NONE, FPAC, 20,  6,  0,  0, &macros[3067] fl(17,381) },
 {  3, 10, FPAC, NONE, NONE, FPAC, 20, 14,  0,  0, &macros[3070] fl(17,387) },
 {  3, 10, FPAC, NONE, NONE, FPAC, 20, 11,  0,  0, &macros[3073] fl(17,393) }
/* #endif */
/* #ifndef ONLYSMALL */,
 {  3,  4, DXAX, NONE, NONE, DXAX,  4,  2,  0,  0, &macros[3076] fl(17,409) },
 {  2,  4, ESAX, NONE, NONE, ESAX,  4,  2,  0,  0, &macros[3086] fl(17,417) },
 {  3,  4, DXAX, NONE, NONE, DXAX,  4,  3,  0,  0, &macros[3097] fl(17,426) },
 {  2,  4, ESAX, NONE, NONE, ESAX,  4,  3,  0,  0, &macros[3115] fl(17,434) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[3129] fl(17,443) },
 {  2,  4, ANYL, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[3135] fl(17,450) },
 {  3,  4, ANYR, NONE, NONE, TEMP,  5,  5,  0,  0, &macros[3137] fl(17,459) },
 {  3,  4, ANYR, DXAX, NONE, TEMP,  7,  5,  0,  0, &macros[3144] fl(17,466) },
 {  2,  4, ANYL, DXAX, NONE, TEMP,  7,  5,  0,  0, &macros[3144] fl(17,470) },
 {  3,  4, ANYR, NONE, NONE, TEMP, 20,  5,  0,  0, &macros[3155] fl(17,477) },
 {  2,  4, ANYL, NONE, NONE, TEMP, 20,  5,  0,  0, &macros[3155] fl(17,481) },
 {  3,  5, ANYR, NONE, NONE, TEMP, 25,  4,  0,  0, &macros[3157] fl(17,489) },
 {  3,  5, ANYR, NONE, NONE, TEMP, 16,  4,  0,  0, &macros[3166] fl(17,496) },
 {  3,  5, ANYR, NONE, NONE, TEMP, 17,  4,  0,  0, &macros[3168] fl(17,503) },
 {  3,  5, ANYR, NONE, NONE, TEMP, 19,  4,  0,  0, &macros[3170] fl(17,510) },
 {  3,  5, ANYR, NONE, NONE, TEMP, 20,  4,  0,  0, &macros[3177] fl(17,517) }
/* #endif */,
 {  3,  1, ANYR, NONE, NONE, TEMP,  5, 17,  0,  0, &macros[3179] fl(17,535) },
 { 19,  1, DXAX, DXAX, NONE,   AX,  7, 17,  0,  0, &macros[3182] fl(17,541) },
 {  3,  1, ANYR, NONE, NONE, TEMP, 20, 17,  0,  0, &macros[3183] fl(17,547) },
 {  3,  2,   AX, NONE, NONE, TEMP,  9, 17,  0,  0, &macros[3185] fl(17,554) },
 {  3,  3,   AX, NONE, NONE, TEMP,  9, 17,  0,  0, &macros[3190] fl(17,561) },
 {  3,  2,   AX, NONE, NONE, TEMP,  4, 17,  0,  0, &macros[3198] fl(17,568) },
 {  3,  3,   AX, NONE, NONE, TEMP,  4, 17,  0,  0, &macros[3200] fl(17,575) },
 {  3,  2, DXAX, DXAX, NONE,   AX,  7, 17,  0,  0, &macros[3202] fl(17,582) },
 {  3,  3, DXAX, DXAX, NONE,   AX,  7, 17,  0,  0, &macros[3204] fl(17,588) },
 {  3,  2,   AX, NONE, NONE, TEMP,  9,  1,  0,  0, &macros[3205] fl(17,595) },
 {  3,  3,   AX, NONE, NONE, TEMP,  9,  1,  0,  0, &macros[3207] fl(17,602) },
 {  3,  2,   AX, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[3211] fl(17,609) },
 {  3,  3,   AX, NONE, NONE, TEMP,  4,  1,  0,  0, &macros[3217] fl(17,616) },
 { 19,  2,   AX,   AX, NONE, TEMP,  7,  1,  0,  0, &macros[3223] fl(17,623) },
 { 19,  3,   AX,   AX, NONE, TEMP,  7,  1,  0,  0, &macros[3225] fl(17,629) },
 { 23,  1, NONE, NONE, NONE, NONE,  9,  1,  0,  0, &macros[3227] fl(17,647) },
 { 23,  1,   AX, NONE, NONE, NONE, 20,  1,  0,  0, &macros[3233] fl(17,654) },
 { 23, 18,   AX, NONE, NONE, NONE, 20, 16,  0,  0, &macros[3240] fl(17,661) },
 { 40,  5, NONE, NONE, NONE, NONE,  9,  5,  0,  0, &macros[3243] fl(17,668) },
 { 40,  5,   AX, NONE, NONE, NONE,  4,  5,  0,  0, &macros[3249] fl(17,675) },
 { 25,  5, NONE, NONE, NONE, NONE,  9,  5,  0,  0, &macros[3257] fl(17,683) },
 { 25,  5,   AX, NONE, NONE, NONE, 20,  5,  0,  0, &macros[3265] fl(17,690) },
 { 41,  5, NONE, NONE, NONE, NONE,  9,  5,  0,  0, &macros[3273] fl(17,697) },
 { 41,  5,   AX, NONE, NONE, NONE, 20,  5,  0,  0, &macros[3290] fl(17,708) },
 { 42,  5, NONE, NONE, NONE, NONE,  9,  5,  0,  0, &macros[3307] fl(17,719) },
 { 42,  5,   AX, NONE, NONE, NONE, 20,  5,  0,  0, &macros[3325] fl(17,730) }
/* #ifdef NDPDEF */,
 { 43, 12, NONE, NONE, NONE, NONE,  9, 12,  0,  0, &macros[3341] fl(17,742) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 40,  4, NONE, NONE, NONE, NONE,  9,  4,  0,  0, &macros[3347] fl(17,751) },
 { 40,  4,   AX, NONE, NONE, NONE, 20,  4,  0,  0, &macros[3349] fl(17,758) },
 { 43,  4,   AX, NONE, NONE, NONE, 20,  4,  0,  0, &macros[3358] fl(17,766) }
/* #endif */,
 { 44,  0, NONE, NONE, NONE, NONE,  0,  0,  0,  0, &macros[3362] fl(17,774) },
 { 45,  0, NONE, NONE, NONE, NONE,  0,  0,  0,  0, &macros[3365] fl(17,780) },
 { 23,  1, NONE, ANYR, NONE, NONE,  7,  1,  0,  0, &macros[3366] fl(17,786) }
/* #ifndef ONLYSMALL */,
 { 40,  4, NONE, ANYR, NONE, NONE,  7,  4,  0,  0, &macros[3372] fl(17,794) },
 { 43,  4, NONE, ANYR, NONE, NONE,  7,  4,  0,  0, &macros[3378] fl(17,801) }
/* #endif */
/* #ifndef NDPDEF */
/* #endif */,
 { 46,  1, NONE, NONE, NONE, NONE,  5,  1,  0,  0, &macros[3384] fl(17,851) },
 { 18,  1, ANYR, NONE, NONE, NONE, 23,  0,  0,  0, &macros[3387] fl(17,857) },
 { 18,  1, ANYR, NONE, NONE, NONE,  5,  1,  0,  0, &macros[3393] fl(17,864) },
 { 18,  1, ANYR, NONE, NONE, NONE, 24,  1,  0,  0, &macros[3397] fl(17,871) }
/* #ifndef ONLYSMALL */,
 { 18,  4, ANYR, NONE, NONE, NONE, 25,  4,  0,  0, &macros[3403] fl(17,879) },
 { 46,  4, NONE, NONE, NONE, NONE, 16,  4,  0,  0, &macros[3415] fl(17,887) },
 { 46,  4, NONE, NONE, NONE, NONE, 17,  4,  0,  0, &macros[3420] fl(17,894) },
 { 18,  4,   AX, NONE, NONE, NONE, 23,  0,  0,  0, &macros[3427] fl(17,901) },
 { 18,  4,   AX, NONE, NONE, NONE, 16,  4,  0,  0, &macros[3433] fl(17,909) },
 { 18,  4,   AX, NONE, NONE, NONE, 17,  4,  0,  0, &macros[3440] fl(17,918) },
 { 46,  4, NONE, NONE, NONE, NONE, 18,  4,  0,  0, &macros[3452] fl(17,927) },
 { 46,  4, NONE, NONE, NONE, NONE, 19,  4,  0,  0, &macros[3457] fl(17,934) },
 { 18,  4, ANYR, NONE, NONE, NONE, 18,  4,  0,  0, &macros[3462] fl(17,941) },
 { 18,  4, ANYR, NONE, NONE, NONE, 19,  4,  0,  0, &macros[3470] fl(17,949) }
/* #endif */,
 { 18,  1, NONE, NONE, NONE, NONE, 20,  1,  0,  0, &macros[3475] fl(17,959) },
 { 18, 16, NONE, NONE, NONE, NONE,  9, 18,  0,  0, &macros[3475] fl(17,963) },
 { 46,  5, NONE, NONE, NONE, NONE,  5,  5,  0,  0, &macros[3477] fl(17,969) },
 { 18, 17, ANYR, NONE, NONE, NONE, 23,  0,  0,  0, &macros[3479] fl(17,976) },
 { 18,  5, ANYR, NONE, NONE, NONE,  5,  5,  0,  0, &macros[3489] fl(17,984) },
 { 18, 17, NONE, NONE, NONE, NONE, 20, 17,  0,  0, &macros[3503] fl(17,993) }
/* #ifndef ONLYSMALL */,
 { 18,  4, NONE, NONE, NONE, NONE,  9,  4,  0,  0, &macros[3503] fl(17,998) }
/* #endif */
/* #ifdef NDPDEF */,
 { 18, 12, NONE, NONE, NONE, NONE,  9, 12,  0,  0, &macros[3505] fl(17,1013) }
/* #endif */
/* #ifndef NDPDEF */
/* #endif */,
 {  9, 21, NONE, NONE, NONE, NONE,  4, 21,  0,  0, &macros[3509] fl(17,1042) },
 {  9, 22, NONE, NONE, NONE, NONE,  9, 22,  0,  0, &macros[3510] fl(17,1056) }
/* #ifndef NDPDEF */
/* #endif */
/* #ifdef NDPDEF */,
 {  9, 12, NONE, NONE, NONE, NONE,  9, 12,  0,  0, &macros[3511] fl(17,1070) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/mul.t */
PAT p18[] = {
 { 31,  1, ANYR, NONE, NONE, TEMP, 20,  1,  5,  1, &macros[3513] fl(18,10) },
 { 30,  1, ANYL, NONE, NONE, TEMP, 20,  1,  5,  1, &macros[3513] fl(18,14) },
 { 31,  1, ANYR, NONE, NONE, TEMP,  5,  1, 20,  1, &macros[3518] fl(18,20) },
 { 30,  1, ANYL, NONE, NONE, TEMP,  5,  1, 20,  1, &macros[3518] fl(18,24) },
 { 12,  1, DXAX,   AX, NONE,   AX,  7,  6, 20,  1, &macros[3523] fl(18,30) },
 { 12,  1, DXAX,   AX, NONE,   AX,  7, 13, 20,  1, &macros[3526] fl(18,36) }
/* #ifdef NDPDEF */,
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20,  6, &macros[3529] fl(18,50) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 14, &macros[3532] fl(18,56) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 11, &macros[3535] fl(18,62) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 12, &macros[3538] fl(18,68) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/neg.t */
PAT p19[] = {
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  0,  0, &macros[3541] fl(19,10) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  0,  0, &macros[3544] fl(19,17) }
/* #ifndef NDPDEF */
/* #endif */
/* #ifdef NDPDEF */,
 { 19, 12, FPAC, FPAC, NONE, FPAC,  7, 12,  0,  0, &macros[3557] fl(19,34) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/not.t */
PAT p20[] = {
 { 16,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  0,  0, &macros[3559] fl(20,10) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  0,  0, &macros[3562] fl(20,16) }
};
/* /usr/src/c/n1/i8086/tables/or.t */
PAT p21[] = {
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  1, &macros[3568] fl(21,10) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 12,  5, &macros[3572] fl(21,17) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  6,  5, &macros[3576] fl(21,23) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  4,  5, &macros[3580] fl(21,30) }
};
/* /usr/src/c/n1/i8086/tables/relop.t */
PAT p22[] = {
 { 43,  0, NONE, NONE, NONE, NONE, 20, 16, 22,  1, &macros[3587] fl(22,34) },
 { 43,  0, NONE, NONE, NONE, NONE, 29,  1,  5,  1, &macros[3591] fl(22,41) },
 { 43,  0, NONE, NONE, NONE, NONE, 26,  1,  4,  1, &macros[3591] fl(22,45) },
 { 48,  0, ANYR, ANYR, NONE, NONE,  7, 16,  4, 16, &macros[3595] fl(22,52) },
 { 43,  0,   AX, NONE, NONE, NONE, 20, 16,  4, 16, &macros[3601] fl(22,59) },
 { 48,  0, ANYR, ANYR, NONE, NONE,  7,  1,  4,  1, &macros[3607] fl(22,67) },
 { 28,  1, NONE, NONE, NONE, NONE, 20,  5,  5,  5, &macros[3610] fl(22,81) },
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  5, 12,  5, &macros[3626] fl(22,91) },
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  5,  6,  5, &macros[3638] fl(22,101) },
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[3652] fl(22,111) }
/* #ifndef ONLYSMALL */,
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  4, 20,  4, &macros[3652] fl(22,116) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 28,  0, NONE, ANYR, NONE, NONE,  7,  4,  4,  1, &macros[3659] fl(22,128) },
 { 28,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  4, &macros[3671] fl(22,138) }
/* #endif */,
 { 27,  0, NONE, NONE, NONE, NONE, 20,  5,  5,  5, &macros[3683] fl(22,156) },
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  5, 12,  5, &macros[3697] fl(22,165) },
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  5,  6,  5, &macros[3709] fl(22,174) },
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[3724] fl(22,183) }
/* #ifndef ONLYSMALL */,
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  4, 20,  4, &macros[3724] fl(22,188) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 27,  0, NONE, ANYR, NONE, NONE,  7,  4,  4,  1, &macros[3731] fl(22,199) },
 { 27,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  4, &macros[3744] fl(22,208) }
/* #endif */
/* #ifndef ONLYSMALL */,
 { 49,  0, NONE, NONE, NONE, NONE, 20,  4,  5,  4, &macros[3756] fl(22,226) },
 { 49,  0, NONE, ANYR, NONE, NONE,  7,  4, 20,  4, &macros[3760] fl(22,233) },
 { 49,  0, NONE, NONE, NONE, NONE, 20,  4,  5,  1, &macros[3764] fl(22,240) },
 { 49,  0, NONE, ANYR, NONE, NONE,  7,  4,  4,  1, &macros[3769] fl(22,247) },
 { 49,  0, NONE, NONE, NONE, NONE, 20,  1,  5,  4, &macros[3775] fl(22,254) },
 { 49,  0, NONE, ANYR, NONE, NONE,  7,  1,  4,  4, &macros[3779] fl(22,261) }
/* #endif */,
 { 49,  0, NONE, NONE, NONE, NONE, 20,  5,  5,  5, &macros[3783] fl(22,269) },
 { 49,  0, NONE, ANYR, NONE, NONE,  7,  5, 12,  5, &macros[3800] fl(22,280) },
 { 49,  0, NONE, ANYR, NONE, NONE,  7,  5,  4,  5, &macros[3814] fl(22,291) }
/* #ifdef NDPDEF */,
 { 43,  0, NONE, FPAC, NONE, NONE,  7, 12, 20,  6, &macros[3821] fl(22,309) },
 { 48,  0, NONE, FPAC, NONE, NONE,  7, 12, 20, 14, &macros[3829] fl(22,317) },
 { 48,  0, NONE, FPAC, NONE, NONE,  7, 12, 20, 11, &macros[3832] fl(22,325) },
 { 48,  0, NONE, FPAC, NONE, NONE,  7, 12, 20, 12, &macros[3835] fl(22,333) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/rem.t */
PAT p23[] = {
 { 22,  1, DXAX,   AX, NONE,   DX,  7,  6, 20,  1, &macros[3838] fl(23,10) },
 { 22,  1, DXAX,   AX, NONE,   DX,  7, 13, 20,  1, &macros[3841] fl(23,17) }
};
/* /usr/src/c/n1/i8086/tables/shl.t */
PAT p24[] = {
 { 50,  1, ANYR, ANYR, NONE, TEMP,  7,  1, 22,  1, &macros[3844] fl(24,12) },
 { 51,  1, ANYL, ANYL, NONE, TEMP,  7,  1, 22,  1, &macros[3844] fl(24,16) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  2,  0, &macros[3848] fl(24,23) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1,  2,  0, &macros[3848] fl(24,27) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  1, 15,  0, &macros[3850] fl(24,34) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1, 15,  0, &macros[3850] fl(24,38) },
 { 47,  1, ANYR, ANYR,   CX, TEMP,  7,  1,  5,  1, &macros[3860] fl(24,53) },
 { 15,  1, ANYL, ANYL,   CX, TEMP,  7,  1,  5,  1, &macros[3860] fl(24,57) },
 { 16,  1, ANYR, ANYR,   CX, TEMP,  7,  1, 20,  1, &macros[3870] fl(24,65) },
 { 15,  1, ANYL, ANYL,   CX, TEMP,  7,  1, 20,  1, &macros[3870] fl(24,69) },
 { 16,  5, DXAX, DXAX,   CX, DXAX,  7,  5,  5,  1, &macros[3879] fl(24,84) },
 { 16,  5, DXAX, DXAX,   CX, DXAX,  7,  5, 20,  1, &macros[3897] fl(24,93) }
/* #ifndef ONLYSMALL */,
 { 31,  4, DXAX,   AX, NONE, DXAX,  7,  1, 22,  1, &macros[3918] fl(24,114) },
 { 12,  4, DXAX,   AX,   CX, DXAX,  7,  1,  5,  1, &macros[3922] fl(24,121) },
 { 12,  4, DXAX,   AX,   CX, DXAX,  7,  1, 20,  1, &macros[3934] fl(24,129) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/shr.t */
PAT p25[] = {
 { 50,  1, ANYR, ANYR, NONE, TEMP,  7,  6, 22,  1, &macros[3939] fl(25,15) },
 { 50,  1, ANYR, ANYR, NONE, TEMP,  7, 13, 22,  1, &macros[3943] fl(25,22) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  6,  2,  0, &macros[3947] fl(25,29) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  6, 15,  0, &macros[3950] fl(25,36) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7, 13,  2,  0, &macros[3956] fl(25,44) },
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7, 13, 15,  0, &macros[3959] fl(25,51) },
 { 47,  1,   AX,   AX,   CX, TEMP,  7,  6,  5,  1, &macros[3965] fl(25,66) },
 { 47,  1,   AX,   AX,   CX, TEMP,  7, 13,  5,  1, &macros[3972] fl(25,74) },
 { 16,  1, ANYR, ANYR,   CX, TEMP,  7,  6, 20,  1, &macros[3979] fl(25,82) },
 { 16,  1, ANYR, ANYR,   CX, TEMP,  7, 13, 20,  1, &macros[3986] fl(25,89) },
 { 16,  5, DXAX, DXAX,   CX, DXAX,  7, 14,  5,  1, &macros[3993] fl(25,106) },
 { 16,  5, DXAX, DXAX,   CX, DXAX,  7, 15,  5,  1, &macros[4007] fl(25,115) },
 { 16,  5, ANYR, ANYR,   CX, TEMP,  7, 14, 20,  1, &macros[4015] fl(25,124) },
 { 16,  5, ANYR, ANYR,   CX, TEMP,  7, 15, 20,  1, &macros[4031] fl(25,135) }
};
/* /usr/src/c/n1/i8086/tables/sub.t */
PAT p26[] = {
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  2,  0, &macros[4041] fl(26,12) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1,  2,  0, &macros[4041] fl(26,16) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  2,  0, &macros[4044] fl(26,24) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  2,  0, &macros[4044] fl(26,28) }
/* #endif */,
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1, 15,  0, &macros[4048] fl(26,43) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1, 15,  0, &macros[4048] fl(26,47) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4, 15,  0, &macros[4052] fl(26,56) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4, 15,  0, &macros[4052] fl(26,60) }
/* #endif */,
 { 14,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  1, &macros[4057] fl(26,74) },
 { 15,  1, ANYL, ANYL, NONE, TEMP,  7,  1,  4,  1, &macros[4057] fl(26,78) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  4,  1, &macros[4061] fl(26,86) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  4,  1, &macros[4061] fl(26,90) }
/* #endif */,
 { 17,  5, DXAX, DXAX, NONE, DXAX,  7,  5,  4,  5, &macros[4065] fl(26,97) }
/* #ifndef ONLYSMALL */,
 { 16,  4, ANYR, ANYR, NONE, TEMP,  7,  4,  4,  5, &macros[4074] fl(26,106) },
 { 15,  4, ANYL, ANYL, NONE, TEMP,  7,  4,  4,  5, &macros[4074] fl(26,110) }
/* #endif */
/* #ifndef ONLYSMALL */,
 {  3,  1, ANYR, NONE, NONE, TEMP, 14,  4, 14,  4, &macros[4078] fl(26,125) },
 {  3,  1, DXAX, DXAX, NONE,   AX,  7,  4,  4,  4, &macros[4086] fl(26,132) }
/* #endif */
/* #ifdef NDPDEF */,
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20,  6, &macros[4088] fl(26,146) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 14, &macros[4091] fl(26,152) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 11, &macros[4094] fl(26,158) },
 { 19, 10, FPAC, FPAC, NONE, FPAC,  7, 12, 20, 12, &macros[4097] fl(26,164) }
/* #endif */
};
/* /usr/src/c/n1/i8086/tables/xor.t */
PAT p27[] = {
 { 47,  1, ANYR, ANYR, NONE, TEMP,  7,  1,  4,  1, &macros[4100] fl(27,12) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 12,  5, &macros[4104] fl(27,19) },
 { 16,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 13,  5, &macros[4108] fl(27,25) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  6,  5, &macros[4116] fl(27,32) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5, 11,  5, &macros[4120] fl(27,39) },
 { 17,  5, ANYR, ANYR, NONE, TEMP,  7,  5,  4,  5, &macros[4125] fl(27,47) }
};
PATX patx[] = {
	p3,	35,
	p26,	21,
	p18,	10,
	p16,	6,
	p23,	2,
	p7,	20,
	p21,	4,
	p27,	6,
	p24,	15,
	p25,	14,
	p1,	37,
	p13,	37,
	p6,	13,
	p4,	12,
	p9,	8,
	p2,	21,
	p8,	22,
	p14,	24,
	p10,	31,
	p11,	36,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	p22,	33,
	NULL,	0,
	NULL,	0,
	p19,	3,
	p20,	2,
	NULL,	0,
	NULL,	0,
	NULL,	0,
	p15,	13,
	p15,	13,
	p5,	13,
	p5,	13,
	NULL,	0,
	NULL,	0,
	NULL,	0,
	NULL,	0,
	p17,	121,
	p17,	121,
	NULL,	0,
	NULL,	0,
	p12,	58,
	NULL,	0,
	NULL,	0,
	NULL,	0,
	p17,	121,
	p17,	121,
	NULL,	0
};
PATFLAG	patcache[] = {
	PEFFECT|PRVALUE|PREL,
	PLVALUE,
	PRVALUE,
	PLVALUE|PEFFECT,
	PEFFECT|PRVALUE|PLT|PGE,
	PEFFECT|PRVALUE|PREL|P_SRT,
	PLVALUE|P_SRT,
	PRVALUE|PREL,
	PEFFECT,
	PRVALUE|PLT|PGE|P_SRT,
	PRVALUE|PLT|PGE,
	PEFFECT|PRVALUE,
	PEFFECT|PRVALUE|P_SRT,
	PEFFECT|PRVALUE|PSREL|P_SLT,
	PLVALUE|P_SLT,
	PEFFECT|PRVALUE|P_SLT,
	PEFFECT|PRVALUE|PGE|PLT|P_SLT,
	PFNARG,
	PRVALUE|P_SLT,
	PEFFECT|PLVALUE,
	PEFFECT|PRVALUE|PLVALUE|P80186,
	PEFFECT|PRVALUE|PLVALUE,
	PSREL,
	PEFFECT|PRVALUE|PEREL|P_SLT,
	PLT|PGE,
	PEQ|PNE,
	PNE,
	PEQ,
	PEFFECT|PRVALUE|PREL|P80186,
	PLVALUE|P80186,
	PEFFECT|PRVALUE|P80186,
	PEFFECT|PEREL|P_SRT,
	PRVALUE|P_SRT,
	PEFFECT|PRVALUE|PEREL|P_SRT,
	PEFFECT|PRVALUE|PEREL,
	PRVALUE|PEFFECT|P_SRT,
	PRVALUE|PEFFECT,
	PEFFECT|P_SRT,
	PEFFECT|PRVALUE|PSREL,
	PEREL,
	PLE,
	PGT,
	PREL,
	PUGE,
	PULT,
	PFNARG|P80186,
	PEFFECT|PRVALUE|PREL|P_SLT,
	PREL|P_SLT,
	PNEREL,
	PEFFECT|PRVALUE|PREL|P_SLT|P80186,
	PLVALUE|P_SLT|P80186
};
int patcsize=sizeof(patcache)/sizeof(PATFLAG);
TYPESET	typecache[] = {
	WORD,
	FS8,
	FU8,
	LPTX,
	LONG,
	FS16,
	FU16,
	FFLD8,
	FFLD16,
	FF32|FF64,
	FF32,
	FF64,
	UWORD,
	FS32,
	FU32,
	BYTE,
	LONG|LPTX,
	WORD|BYTE,
	WORD|FS8,
	WORD|FU8,
	FBLK,
	NFLT
};
FLAG	flagcache[] = {
	T_ADR|T_LV,
	T_1|T_MMX,
	T_REG|T_MMX,
	T_ADR|T_IMM,
	T_IMM|T_MMX,
	T_LHC|T_MMX,
	T_TREG,
	T_DIR|T_MMX,
	T_RREG|T_MMX,
	T_EASY|T_MMX,
	T_LHS|T_MMX,
	T_UHC|T_MMX,
	T_UHS|T_MMX,
	T_DIR|T_IMM|T_MMX,
	T_2|T_MMX,
	T_ICN|T_MMX,
	T_LCN|T_MMX,
	T_ACS|T_MMX,
	T_ADS|T_MMX,
	T_ADR,
	T_LREG|T_MMX,
	T_BYTE|T_MMX,
	T_0|T_MMX,
	T_LEA|T_MMX,
	T_LSS|T_MMX,
	T_RREG|T_LREG|T_MMX,
	T_SREG|T_MMX,
	T_LREG|T_SREG|T_MMX,
	T_ADR|T_MMX
};
ival_t	ivalcache[] = {
	0x8,
	0x1,
	0x0
};
lval_t	lvalcache[] = {
	0
};
char	*gidcache[] = {
	"lrdiv",
	"vrdiv",
	"lrmul",
	"vrmul",
	"lrrem",
	"vrrem",
	"tstccp",
	"dp87",
	"cfcc"
};
#if !TINY
char	*namecache[] = {
	"prefac.f",
	"aadd.t",
	"aand.t",
	"add.t",
	"adiv.t",
	"aft.t",
	"amul.t",
	"and.t",
	"aor.t",
	"arem.t",
	"ashl.t",
	"ashr.t",
	"assign.t",
	"asub.t",
	"axor.t",
	"bef.t",
	"div.t",
	"leaves.t",
	"mul.t",
	"neg.t",
	"not.t",
	"or.t",
	"relop.t",
	"rem.t",
	"shl.t",
	"shr.t",
	"sub.t",
	"xor.t"
};
#endif

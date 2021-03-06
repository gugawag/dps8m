typedef uint16_t word9;
typedef uint32_t word18;
typedef uint64_t word36;
typedef uint64_t word36;
typedef unsigned int uint;
typedef __uint128_t word72;

word36 extr36 (uint8_t * bits, uint woffset);
word9 extr9 (uint8_t * bits, uint coffset);
word18 extr18 (uint8_t * bits, uint coffset);
uint8_t getbit (void * bits, int offset);
uint64_t extr (void * bits, int offset, int nbits);
void put36 (word36 val, uint8_t * bits, uint woffset);

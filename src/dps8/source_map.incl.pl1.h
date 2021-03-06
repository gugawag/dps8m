//++ 
//++ /* BEGIN INCLUDE FILE ... source_map.incl.pl1 */
//++ 
//++ dcl        1 source_map    aligned based,
//++    2 version               fixed bin,
//++    2 number                fixed bin,
//++    2 map(n refer(source_map.number))        aligned,
//++      3 pathname    unaligned,
//++        4 offset    bit(18),
//++        4 size      bit(18),
//++      3 uid         bit(36),
//++      3 dtm         fixed bin(71);
//++ 
//++ /* END INCLUDE FILE  ... source_map.incl.pl1 */

typedef struct __attribute__ ((__packed__)) map
  {
    union
      {
        struct __attribute__ ((__packed__))
          {
            uint size : 18;
            uint offset : 18;
          };
        word36 align1;
      };
    word36 uid;
    word36 dtm [2];
  } map;

typedef struct __attribute__ ((__packed__)) source_map
  {
    word36 version;
    word36 number;
    map firstMap;
  } source_map;



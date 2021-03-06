// /* BEGIN INCLUDE FILE area_info.incl.pl1  12/75 */
// 
// dcl  area_info_version_1 fixed bin static init (1) options (constant);
// 
// dcl  area_infop ptr;
// 
// dcl 1 area_info aligned based (area_infop),
//     2 version fixed bin,                             /* version number for this structure is 1 */
//     2 control aligned like area_control,             /* control bits for the area */
//     2 owner char (32) unal,                          /* creator of the area */
//     2 n_components fixed bin,                                /* number of components in the area (returned only) */
//     2 size fixed bin (18),                           /* size of the area in words */
//     2 version_of_area fixed bin,                     /* version of area (returned only) */
//     2 areap ptr,                                     /* pointer to the area (first component on multisegment area) */
//     2 allocated_blocks fixed bin,                    /* number of blocks allocated */
//     2 free_blocks fixed bin,                         /* number of free blocks not in virgin */
//     2 allocated_words fixed bin (30),                        /* number of words allocated in the area */
//     2 free_words fixed bin (30);                     /* number of words free in area not in virgin */
// 
// dcl 1 area_control aligned based,
//     2 extend bit (1) unal,                           /* says area is extensible */
//     2 zero_on_alloc bit (1) unal,                    /* says block gets zerod at allocation time */
//     2 zero_on_free bit (1) unal,                     /* says block gets zerod at free time */
//     2 dont_free bit (1) unal,                        /* debugging aid, turns off free requests */
//     2 no_freeing bit (1) unal,                       /* for allocation method without freeing */
//     2 system bit (1) unal,                           /* says area is managed by system */
//     2 pad bit (30) unal;
// 
// /* END INCLUDE FILE area_info.incl.pl1 */

typedef struct __attribute__ ((__packed__)) area_info
  {
    word36 version;
    word36 owner [8];
    word36 n_components;
    union
      {
        struct __attribute__ ((__packed__))
          {
            uint size : 18;
          };
        word36 align1;
      };
    word36 version_of_area;
    word36 areap [2];
    word36 allocated_blocks;
    word36 free_blocks;
    word36 allocated_words;
    word36 free_words;
  } area_info;


typedef struct __attribute__ ((__packed__)) area_control
  {
    union
      {
        struct __attribute__ ((__packed__))
          {
            uint pad : 30;
            uint system : 1;
            uint no_freeing : 1;
            uint dont_free : 1;
            uint zero_on_free : 1;
            uint zero_on_alloc : 1;
            uint extent : 1;
          };
        word36 align1;
      };
  } area_control;


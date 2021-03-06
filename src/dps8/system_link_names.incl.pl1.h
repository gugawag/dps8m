//++ 
//++ /* BEGIN INCLUDE FILE ... system_link_names.incl.pl1 */
//++ 
//++ 
//++ /****^  HISTORY COMMENTS:
//++   1) change(86-06-24,DGHowe), approve(86-06-24,MCR7396), audit(86-11-12,Zwick),
//++      install(86-11-20,MR12.0-1222):
//++      added the declaration of the heap_header.
//++   2) change(86-10-20,DGHowe), approve(86-10-20,MCR7420), audit(86-11-12,Zwick),
//++      install(86-11-20,MR12.0-1222):
//++      add the seg ptr to the variable node structure.
//++                                                    END HISTORY COMMENTS */
//++ 
//++ 
//++ /* created by M. Weaver 7/28/76 */
//++ /* Modified: 82-11-19 by T. Oke to add LIST_TEMPLATE_INIT. */
//++ /* Modified 02/11/83 by M. Weaver to add have_vla_variables flag */
//++ 
//++ 
//++ dcl 1 variable_table_header aligned based, /* header for name table */
//++      2 hash_table (0:63) ptr unaligned,            /* hash table for variable nodes */
//++      2 total_search_time fixed bin (71),   /* total time to search for variables */
//++      2 total_allocation_time fixed bin (71),       /* total time spent allocating and initializing nodes and variables */
//++      2 number_of_searches fixed bin,               /* number of times names were looked up */
//++      2 number_of_variables fixed bin (35), /* number of variables allocated by the linker, incl deletions */
//++      2 flags unaligned,
//++         3 have_vla_variables bit (1) unaligned,    /* on if some variables are > sys_info$max_seg_size */
//++         3 pad bit (11) unaligned,
//++      2 cur_num_of_variables fixed bin (24) unal,   /* current number of variables allocated */
//++      2 number_of_steps fixed bin,          /* total number of nodes looked at */
//++      2 total_allocated_size fixed bin (35);        /* current amount of storage in user area */
//++ 
//++ 
//++ dcl 1 variable_node aligned based,         /* individual variable information */
//++      2 forward_thread ptr unaligned,               /* thread to next node off same hash bucket */
//++      2 vbl_size fixed bin (24) unsigned unaligned, /* length in words of variable */
//++      2 init_type fixed bin (11) unaligned, /* 0=not init; 3=init template; 4=area 5=list_template*/
//++      2 time_allocated fixed bin (71),              /* time when variable was allocated */
//++      2 vbl_ptr ptr,                                /* pointer to variable's storage */
//++      2 init_ptr ptr,                       /* pointer to original init info in object seg */
//++      2 name_size fixed bin(21) aligned,            /* length of name in characters */
//++      2 name char (nchars refer (variable_node.name_size)), /* name of variable */
//++      2 seg_ptr  pointer;
//++ 
//++ /* variable_node.seg_ptr
//++    Is a pointer to the segment containing the initialization information
//++    for this variable. It is used as a segment base pointer for external
//++    pointer initialization via list_init_.
//++ 
//++    The init_ptr can not be used as a reference to the defining segment
//++    due to the possibility of set_fortran_common being used to initialize
//++    the external variables. sfc will generate an initialization information
//++    structure if multiple intialization sizes are found in the specified 
//++    segments. sfc stores the address of this structure in the init_ptr field.
//++    This is one reason why sfc does not perform external pointer 
//++    initialization.
//++ 
//++    The seg_ptr is set to point at the segment used to define the 
//++    initialization information. term_ sets this field to null on termination
//++    due to the possiblity of executing a different segment which defines
//++    initialization information. In this way the seg_ptr field will either
//++    be valid or null.
//++ */
//++ 
//++ dcl 1 heap_header based,
//++      2 version            char(8),                /* specifies the verison of the header */
//++      2 heap_name_list_ptr pointer,         /* points to the variable_table_header for this heap */
//++      2 previous_heap_ptr  pointer,         /* points to the previous heap or is null */
//++      2 area_ptr          pointer,          /* points to the heap area */
//++      2 execution_level    fixed bin (17);  /* specifies the execution level this header deals with */
//++ 
//++ dcl heap_header_version_1 char(8) static options (constant)
//++                        init ("Heap_v01");
//++    
//++ 
//++ /* END INCLUDE FILE ... system_link_names.incl.pl1 */

typedef struct __attribute__ ((__packed__)) variable_table_header
  {
    word36 hash_table [64] [2]; // hash table for variable nodes
    word36 total_search_time [2]; // total time to search for variables
    word36 total_allocation_time [3]; // total time spent allocating and initializing nodes and variables
    word36 number_of_searches; // number of times names were looked up
    word36 number_of_variables; // number of variables allocated by the linker, incl deletions
    union
      {
        struct __attribute__ ((__packed__))
          {
            uint cur_num_of_variables : 24; // current number of variables allocated
            uint pad : 11;
            uint have_vla_variables : 1; // on if some variables are > sys_info$max_seg_size
          };
        word36 align1;
      };
    word36 number_of_steps; // total number of nodes looked at
    word36 total_allocated_size; // current amount of storage in user area
  } variable_table_header;

typedef struct __attribute__ ((__packed__)) variable_node
  {
    word36 forward_thread [2]; // thread to next node off same hash bucket
    union
      {
        struct __attribute__ ((__packed__))
          {
            int init_type : 12; 
            uint vbl_size : 24; // length in words of variable
          };
        word36 align1;
      };
    word36 vbl_ptr [2]; // pointer to variable's storage
    word36 init_ptr [2]; // pointer to original init info in object seg 
// XXX nchars, chars[], segptr.
  } variable_node;




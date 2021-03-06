//++ 
//++ /* BEGIN INCLUDE FILE linkdcl.incl.pl1 --- last modified 15 Nov 1971 by C Garman */
//++ 
//++ /* Last Modified (Date and Reason):
//++    6/75 by M.Weaver to add virgin_linkage_header declaration
//++    6/75 by S.Webber to comment existing structures better
//++    9/77 by M. Weaver to add run_depth to link
//++    2/83 by M. Weaver to add linkage header flags and change run_depth precision
//++ */
//++ 
//++ /* format: style3 */
//++ dcl        1 link          based aligned,          /* link pair in linkage section */
//++      2 head_ptr    bit (18) unal,          /* rel pointer to beginning of linkage section */
//++      2 ringno      bit (3) unal,
//++      2 mbz         bit (6) unal,
//++      2 run_depth   fixed bin (2) unal,             /* run unit depth, filled when link is snapped */
//++      2 ft2         bit (6) unal,           /* fault tag. 46(8) if not snapped, 43(8) if snapped */
//++      2 exp_ptr     bit (18) unal,          /* pointer (rel to defs) of expression word */
//++      2 mbz2                bit (12) unal,
//++      2 modifier    bit (6) unal;           /* modifier to be left in snapped link */
//++ 
//++ dcl        1 exp_word      based aligned,          /* expression word in link definition */
//++      2 type_ptr    bit (18) unal,          /* pointer (rel to defs) of type pair structure */
//++      2 exp         bit (18) unal;          /* constant expression to be added in when snapping link */
//++ 
//++ dcl        1 type_pair     based aligned,          /* type pair in link definition */
//++      2 type                bit (18) unal,          /* type of link. may be 1,2,3,4,5, or 6 */
//++      2 trap_ptr    bit (18) unal,          /* pointer (rel to defs) to the trap word */
//++      2 seg_ptr     bit (18) unal,          /* pointer to ACC reference name for segment referenced */
//++      2 ext_ptr     bit (18) unal;          /* pointer (rel to defs)  of ACC segdef name */
//++ 
//++ dcl        1 header                based aligned,          /* linkage block header */
//++      2 def_ptr     ptr,                    /* pointer to definition section */
//++      2 symbol_ptr  ptr unal,                       /* pointer to symbol section in object segment */
//++      2 original_linkage_ptr
//++                    ptr unal,                       /* pointer to linkage section in object segment */
//++      2 unused      bit (72),
//++      2 stats,
//++        3 begin_links       bit (18) unal,          /* offset (rel to this section) of first link */
//++        3 block_length      bit (18) unal,          /* number of words in this linkage section */
//++        3 segment_number
//++                    bit (18) unal,          /* text segment number associated with this section */
//++        3 static_length bit (18) unal;              /* number of words of static for this segment */
//++ 
//++ dcl        1 linkage_header_flags
//++                    aligned based,          /* overlay of def_ptr for flags */
//++      2 pad1                bit (28) unaligned,             /* flags are in first word */
//++      2 static_vlas bit (1) unaligned,              /* static section "owns" some LA/VLA segments */
//++      2 perprocess_static
//++                    bit (1) unaligned,              /* 1 copy of static section is used by all tasks/run units */
//++      2 pad2                bit (6) unaligned;
//++ 
//++ dcl        1 virgin_linkage_header
//++                    aligned based,          /* template for linkage header in object segment */
//++      2 pad         bit (30) unaligned,             /* is filled in by linker */
//++      2 defs_in_link        bit (6) unaligned,              /* =o20 if defs in linkage (nonstandard) */
//++      2 def_offset  bit (18) unaligned,             /* offset of definition section */
//++      2 first_ref_relp      bit (18) unaligned,             /* offset of trap-at-first-reference offset array */
//++      2 filled_in_later bit (144),
//++      2 link_begin  bit (18) unaligned,             /* offset of first link */
//++      2 linkage_section_lng
//++                    bit (18) unaligned,             /* length of linkage section */
//++      2 segno_pad   bit (18) unaligned,             /* will be segment number of copied linkage */
//++      2 static_length       bit (18) unaligned;             /* length of static section */
//++ 
//++ 
//++ dcl        1 trap_word     based aligned,          /* trap word in link definition */
//++      2 call_ptr    bit (18) unal,          /* pointer (rel to link) of link to trap procedure */
//++      2 arg_ptr     bit (18) unal;          /* pointer (rel to link) of link to arg info for trap proc */
//++ 
//++ dcl        1 name          based aligned,          /* storage of ASCII names in definitions */
//++      2 nchars      bit (9) unaligned,              /* number of characters in name */
//++      2 char_string char (31) unaligned;    /* 31-character name */
//++ 
//++ /* END INCLUDE FILE linkdcl.incl.pl1 */


typedef struct __attribute__ ((__packed__)) link_header
  {
    union
      {
        struct __attribute__ ((__packed__))
          {
            uint defs_in_link : 6;
            uint pad : 30;
          };
        word36 align1;
      };

    union
      {
        struct __attribute__ ((__packed__))
          {
            uint first_ref_relp : 18;
            uint def_offset : 18;
          };
        word36 align2;
      };

    //word36 filled_in_later [4];
    word36 symbol_ptr [2];
    word36 original_linkage_ptr [2];

    union
      {
        struct __attribute__ ((__packed__))
          {
            uint linkage_section_lng : 18;
            uint link_begin : 18;
          };
        word36 align3;
      };

    union
      {
        struct __attribute__ ((__packed__))
          {
            uint static_length : 18;
            uint segno_pad : 18;
          };
        word36 align4;
      };


  } link_header;

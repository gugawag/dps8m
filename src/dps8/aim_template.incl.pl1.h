//-- /* BEGIN INCLUDE FILE aim_template.incl.pl1 */
//-- 
//-- /* Created 740723 by PG */
//-- /* Modified 06/28/78 by C. D. Tavares to add rcp privilege */
//-- /* Modified 83-05-10 by E. N. Kitltitz to add communications privilege */
//-- 
//-- /* This structure defines the components of both an access
//--    class and an access authorization as interpreted by the
//--    Access Isolation Mechanism. */
//-- 
//-- 
//-- dcl  1 aim_template aligned based,			/* authorization/access class template */
//--        2 categories bit (36),				/* access categories */
//--        2 level fixed bin (17) unaligned,		/* sensitivity level */
//--        2 privileges unaligned,			/* special access privileges (in authorization only) */
//--         (3 ipc,					/* interprocess communication privilege */
//--          3 dir,					/* directory privilege */
//--          3 seg,					/* segment privilege */
//--          3 soos,					/* security out-of-service privilege */
//--          3 ring1,					/* ring 1 access privilege */
//--          3 rcp,					/* RCP resource access privilege */
//--          3 comm) bit (1),				/* communications cross-AIM privilege */
//--          3 pad bit (11);


typedef struct __attribute__ ((__packed__)) aim_template
  {
    word36 categories; /* access categories */
    union
      {
         struct __attribute__ ((__packed__))
          {
            uint pad : 11;
            uint comm : 1; /* communications cross-AIM privilege */
            uint rcp : 1; /* RCP resource access privilege */
            uint ring1 : 1; /* ring 1 access privilege */
            uint soos : 1; /* security out-of-service privilege */
            uint seg : 1; /* segment privilege */
            uint dir : 1; /* directory privilege */
            uint ipc : 1; /* interprocess communication privilege */
            int level : 18;
          };
        word36 align1;
      };
  } aim_template;
//-- 
//-- 
//-- /* END INCLUDE FILE aim_template.incl.pl1 */

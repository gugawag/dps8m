
/**
 * \file dps8_sys.c
 * \project dps8
 * \date 9/17/12
 * \copyright Copyright (c) 2012 Harry Reed. All rights reserved.
*/

#include <stdio.h>

#include "dps8.h"
#include "dps8_iom.h"
#include "dps8_mt.h"
#include "dps8_disk.h"
#include "dps8_utils.h"

// XXX Strictly speaking, memory belongs in the SCU
// We will treat memory as viewed from the CPU and elide the
// SCU configuration that maps memory across multiple SCUs.
// I would guess that multiple SCUs helped relieve memory
// contention across multiple CPUs, but that is a level of
// emulation that will be ignored.

word36 *M = NULL;                                          /*!< memory */


// These are part of the simh interface
char sim_name[] = "dps-8/m";
int32 sim_emax = 4; ///< some EIS can take up to 4-words
static void dps8_init(void);
void (*sim_vm_init) (void) = & dps8_init;    //CustomCmds;


stats_t sys_stats;

static t_stat sys_cable (int32 arg, char * buf);
static t_stat dps_debug_start (int32 arg, char * buf);
static t_stat loadSystemBook (int32 arg, char * buf);
static t_stat lookupSystemBook (int32 arg, char * buf);

CTAB dps8_cmds[] =
{
    {"DPSINIT",  dpsCmd_Init,     0, "dpsinit dps8/m initialize stuff ...\n"},
    {"DPSDUMP",  dpsCmd_Dump,     0, "dpsdump dps8/m dump stuff ...\n"},
    {"SEGMENT",  dpsCmd_Segment,  0, "segment dps8/m segment stuff ...\n"},
    {"SEGMENTS", dpsCmd_Segments, 0, "segments dps8/m segments stuff ...\n"},
    {"CABLE",    sys_cable,       0, "cable String a cable\n" },
    {"DBGSTART", dps_debug_start, 0, "dbgstart Limit debugging to N > Cycle count\n"},
    {"DISPLAYMATRIX", displayTheMatrix, 0, "displaymatrix Display instruction usage counts\n"},
    {"LD_SYSTEM_BOOK", loadSystemBook, 0, "load_system_book: Load a Multics system book for symbolic debugging\n"},
    {"LOOKUP_SYSTEM_BOOK", lookupSystemBook, 0, "lookup_system_book: lookup an address or symbol in the Multics system book\n"},
    {"LSB", lookupSystemBook, 0, "lookup_system_book: lookup an address or symbol in the Multics system book\n"},
    { NULL, NULL, 0, NULL}
};

/*!
 \brief special dps8 VM commands ....
 
 For greater flexibility, SCP provides some optional interfaces that can be used to extend its command input, command processing, and command post-processing capabilities. These interfaces are strictly optional
 and are off by default. Using them requires intimate knowledge of how SCP functions internally and is not recommended to the novice VM writer.
 
 Guess I shouldn't use these then :)
 */

static t_addr parse_addr(DEVICE *dptr, char *cptr, char **optr);
static void fprint_addr(FILE *stream, DEVICE *dptr, t_addr addr);

static void dps8_init(void)    //CustomCmds(void)
{
    // special dps8 initialization stuff that cant be done in reset, etc .....

    // These are part of the simh interface
    sim_vm_parse_addr = parse_addr;
    sim_vm_fprint_addr = fprint_addr;

    sim_vm_cmd = dps8_cmds;

    init_opcodes();
    iom_init ();
    console_init ();
    disk_init ();
    mt_init ();
    //mpc_init ();
    scu_init ();
}

static int getval (char * * save, char * text)
  {
    char * value;
    char * endptr;
    value = strtok_r (NULL, ",", save);
    if (! value)
      {
        //sim_debug (DBG_ERR, & sys_dev, "sys_cable: can't parse %s\n", text);
        sim_printf ("error: sys_cable: can't parse %s\n", text);
        return -1;
      }
    long l = strtol (value, & endptr, 0);
    if (* endptr || l < 0 || l > INT_MAX)
      {
        //sim_debug (DBG_ERR, & sys_dev, "sys_cable: can't parse %s <%s>\n", text, value);
        sim_printf ("error: sys_cable: can't parse %s <%s>\n", text, value);
        return -1;
      }
    return (int) l;
  }

// Connect dev to iom
//
//   cable [TAPE | DISK],<dev_unit_num>,<iom_unit_num>,<chan_num>,<dev_code>
//
//   or iom to scu
//
//   cable IOM <iom_unit_num>,<iom_port_num>,<scu_unit_num>,<scu_port_num>
//
//   or scu to cpu
//
//   cable SCU <scu_unit_num>,<scu_port_num>,<cpu_unit_num>,<cpu_port_num>
//
//   or opcon to iom
//
//   cable OPCON <iom_unit_num>,<chan_num>,0,0
//

static t_stat sys_cable (int32 arg, char * buf)
  {
// XXX Minor bug; this code doesn't check for trailing garbage

    char * copy = strdup (buf);
    t_stat rc = SCPE_ARG;

    // process statement

    // extract name
    char * name_save = NULL;
    char * name;
    name = strtok_r (copy, ",", & name_save);
    if (! name)
      {
        //sim_debug (DBG_ERR, & sys_dev, "sys_cable: can't parse name\n");
        sim_printf ("error: sys_cable: can't parse name\n");
        goto exit;
      }


    int n1 = getval (& name_save, "parameter 1");
    if (n1 < 0)
      goto exit;
    int n2 = getval (& name_save, "parameter 2");
    if (n2 < 0)
      goto exit;
    int n3 = getval (& name_save, "parameter 3");
    if (n3 < 0)
      goto exit;
    int n4 = getval (& name_save, "parameter 4");
    if (n4 < 0)
      goto exit;


    if (strcasecmp (name, "TAPE") == 0)
      {
        rc = cable_mt (n1, n2, n3, n4);
      }
// XXX not yet
//    else if (strcasecmp (name, "DISK") == 0)
//      {
//        rc = cable_disk (n1, n2, n3, n4);
//      }
    else if (strcasecmp (name, "OPCON") == 0)
      {
        rc = cable_opcon (n1, n2);
      }
    else if (strcasecmp (name, "IOM") == 0)
      {
        rc = cable_iom (n1, n2, n3, n4);
      }
    else if (strcasecmp (name, "SCU") == 0)
      {
        rc = cable_scu (n1, n2, n3, n4);
      }
    else
      {
        //sim_debug (DBG_ERR, & sys_dev, "sys_cable: Invalid switch name <%s>\n", name);
        sim_printf ("error: sys_cable: invalid switch name <%s>\n", name);
        goto exit;
      }

exit:
    free (copy);
    return rc;
  }

uint64 sim_deb_start;

static t_stat dps_debug_start (int32 arg, char * buf)
  {
    sim_deb_start = strtoull (buf, NULL, 0);
    sim_printf ("Debug set to start at cycle: %lld\n", sim_deb_start);
    return SCPE_OK;
  }

// LOAD_SYSTEM_BOOK <filename>
#define bookSegmentsMax 1024
#define bookComponentsMax 4096
#define bookSegmentNameLen 33
struct bookSegment
  {
    char * segname;
    int segno;
  } bookSegments [bookSegmentsMax];

static int nBookSegments = 0;

struct bookComponent
  {
    char * compname;
    int bookSegmentNum;
    int txt_start, txt_length, intstat_start, intstat_length, symbol_start, symbol_length;
  } bookComponents [bookComponentsMax];

static int nBookComponents = 0;

static int lookupBookSegment (char * name)
  {
    for (int i = 0; i < nBookSegments; i ++)
      if (strcmp (name, bookSegments [i] . segname) == 0)
        return i;
    return -1;
  }

static int addBookSegment (char * name, int segno)
  {
    int n = lookupBookSegment (name);
    if (n >= 0)
      return n;
    if (nBookSegments >= bookSegmentsMax)
      return -1;
    bookSegments [nBookSegments] . segname = strdup (name);
    bookSegments [nBookSegments] . segno = segno;
    n = nBookSegments;
    nBookSegments ++;
    return n;
  }
 
static int addBookComponent (int segnum, char * name, int txt_start, int txt_length, int intstat_start, int intstat_length, int symbol_start, int symbol_length)
  {
    if (nBookComponents >= bookComponentsMax)
      return -1;
    bookComponents [nBookComponents] . compname = strdup (name);
    bookComponents [nBookComponents] . bookSegmentNum = segnum;
    bookComponents [nBookComponents] . txt_start = txt_start;
    bookComponents [nBookComponents] . txt_length = txt_length;
    bookComponents [nBookComponents] . intstat_start = intstat_start;
    bookComponents [nBookComponents] . intstat_length = intstat_length;
    bookComponents [nBookComponents] . symbol_start = symbol_start;
    bookComponents [nBookComponents] . symbol_length = symbol_length;
    int n = nBookComponents;
    nBookComponents ++;
    return n;
  }
 


// Warning: returns ptr to static buffer
char * lookupSystemBookAddress (word18 segno, word18 offset)
  {
    static char buf [129];
    int i;
    for (i = 0; i < nBookSegments; i ++)
      if (bookSegments [i] . segno == segno)
        break;
    if (i >= nBookSegments)
      return NULL;

    int best = -1;
    int bestoffset = 0;

    for (int j = 0; j < nBookComponents; j ++)
      {
        if (bookComponents [j] . bookSegmentNum != i)
          continue;
        if (bookComponents [j] . txt_start <= offset &&
            bookComponents [j] . txt_start + bookComponents [j] . txt_length > offset)
          {
            sprintf (buf, "%s:%s+0%0o", bookSegments [i] . segname,
              bookComponents [j].compname,
              offset - bookComponents [j] . txt_start);
            return buf;
          }
        if (bookComponents [j] . txt_start <= offset &&
            bookComponents [j] . txt_start > bestoffset)
          {
            best = j;
            bestoffset = bookComponents [j] . txt_start;
          }
      }

    if (best != -1)
      {
        sprintf (buf, "%s:%s+0%0o", bookSegments [i] . segname,
          bookComponents [best].compname,
          offset - bookComponents [best] . txt_start);
        return buf;
      }

   sprintf (buf, "%s:0%0o", bookSegments [i] . segname,
              offset);
   return buf;
 }

// LSB n:n

static t_stat lookupSystemBook (int32 arg, char * buf)
  {
    int segno, offset;
    if (sscanf (buf, "%i:%i", & segno, & offset) != 2)
      return SCPE_ARG;
    char * ans = lookupSystemBookAddress (segno, offset);
    sim_printf ("%s\n", ans ? ans : "not found");
    return SCPE_OK;
  }

static t_stat loadSystemBook (int32 arg, char * buf)
  {
  
#define bufSz 257
    char filebuf [bufSz];
    int current = -1;

    FILE * fp = fopen (buf, "r");
    if (! fp)
      {
        sim_printf ("error opening file %s\n", buf);
        return SCPE_ARG;
      }
    for (;;)
      {
        char * bufp = fgets (filebuf, bufSz, fp);
        if (! bufp)
          break;
        //sim_printf ("<%s\n>", filebuf);
        char name [bookSegmentNameLen];
        int segno, p0, p1, p2;

        // 32 is bookSegmentNameLen - 1
        int cnt = sscanf (filebuf, "%32s %o  (%o, %o, %o)", name, & segno, 
          & p0, & p1, & p2);
        if (filebuf [0] != '\t' && cnt == 5)
          {
            //sim_printf ("A: %s %d\n", name, segno);
            int rc = addBookSegment (name, segno);
            if (rc < 0)
              {
                sim_printf ("error adding segment name\n");
                return SCPE_ARG;
              }
            continue;
          }

        cnt = sscanf (filebuf, "Bindmap for >ldd>h>e>%32s", name);
        if (cnt == 1)
          {
            //sim_printf ("B: %s\n", name);
            //int rc = addBookSegment (name);
            int rc = lookupBookSegment (name);
            if (rc < 0)
              {
                // The collection 3.0 segments do not have segment numbers,
                // and the 1st digit of the 3-tuple is 1, not 0. Ignore
                // them for now.
                current = -1;
                continue;
                //sim_printf ("error adding segment name\n");
                //return SCPE_ARG;
              }
            current = rc;
            continue;
          }

        int txt_start, txt_length, intstat_start, intstat_length, symbol_start, symbol_length;
        cnt = sscanf (filebuf, "%32s %o %o %o %o %o %o", name, & txt_start, & txt_length, & intstat_start, & intstat_length, & symbol_start, & symbol_length);

        if (cnt == 7)
          {
            //sim_printf ("C: %s\n", name);
            if (current >= 0)
              {
                addBookComponent (current, name, txt_start, txt_length, intstat_start, intstat_length, symbol_start, symbol_length);
              }
            continue;
          }

        cnt = sscanf (filebuf, "%32s %o  (%o, %o, %o)", name, & segno, 
          & p0, & p1, & p2);
        if (filebuf [0] == '\t' && cnt == 5)
          {
            //sim_printf ("D: %s %d\n", name, segno);
            int rc = addBookSegment (name, segno);
            if (rc < 0)
              {
                sim_printf ("error adding segment name\n");
                return SCPE_ARG;
              }
            continue;
          }

      }

#if 0
    for (int i = 0; i < nBookSegments; i ++)
      { 
        sim_printf ("  %-32s %6o\n", bookSegments [i] . segname, bookSegments [i] . segno);
        for (int j = 0; j < nBookComponents; j ++)
          {
            if (bookComponents [j] . bookSegmentNum == i)
              {
                printf ("    %-32s %6o %6o %6o %6o %6o %6o\n",
                  bookComponents [j] . compname, 
                  bookComponents [j] . txt_start, 
                  bookComponents [j] . txt_length, 
                  bookComponents [j] . intstat_start, 
                  bookComponents [j] . intstat_length, 
                  bookComponents [j] . symbol_start, 
                  bookComponents [j] . symbol_length);
              }
          }
      }
#endif

    return SCPE_OK;
  }

static struct PRtab {
    char *alias;    ///< pr alias
    int   n;        ///< number alias represents ....
} _prtab[] = {
    {"pr0", 0}, ///< pr0 - 7
    {"pr1", 1},
    {"pr2", 2},
    {"pr3", 3},
    {"pr4", 4},
    {"pr5", 5},
    {"pr6", 6},
    {"pr7", 7},

    {"pr[0]", 0}, ///< pr0 - 7
    {"pr[1]", 1},
    {"pr[2]", 2},
    {"pr[3]", 3},
    {"pr[4]", 4},
    {"pr[5]", 5},
    {"pr[6]", 6},
    {"pr[7]", 7},
    
    // from: ftp://ftp.stratus.com/vos/multics/pg/mvm.html
    {"ap",  0},
    {"ab",  1},
    {"bp",  2},
    {"bb",  3},
    {"lp",  4},
    {"lb",  5},
    {"sp",  6},
    {"sb",  7},
    
    {0,     0}
    
};

static t_addr parse_addr(DEVICE *dptr, char *cptr, char **optr)
{
    // a segment reference?
    if (strchr(cptr, '|'))
    {
        static char addspec[256];
        strcpy(addspec, cptr);
        
        *strchr(addspec, '|') = ' ';
        
        char seg[256], off[256];
        int params = sscanf(addspec, "%s %s", seg, off);
        if (params != 2)
        {
            sim_printf("parse_addr(): illegal number of parameters\n");
            *optr = cptr;   // signal error
            return 0;
        }
        
        // determine if segment is numeric or symbolic...
        char *endp;
        int PRoffset = 0;   // offset from PR[n] register (if any)
        int segno = (int)strtoll(seg, &endp, 8);
        if (endp == seg)
        {
            // not numeric...
            // 1st, see if it's a PR or alias thereof
            struct PRtab *prt = _prtab;
            while (prt->alias)
            {
                if (strcasecmp(seg, prt->alias) == 0)
                {
                    segno = PR[prt->n].SNR;
                    PRoffset = PR[prt->n].WORDNO;
                    
                    break;
                }
                
                prt += 1;
            }
            
            if (!prt->alias)    // not a PR or alias
            {
                segment *s = findSegmentNoCase(seg);
                if (s == NULL)
                {
                    sim_printf("parse_addr(): segment '%s' not found\n", seg);
                    *optr = cptr;   // signal error
                    
                    return 0;
                }
                segno = s->segno;
            }
        }
        
        // determine if offset is numeric or symbolic entry point/segdef...
        int offset = (int)strtoll(off, &endp, 8);
        if (endp == off)
        {
            // not numeric...
            segdef *s = findSegdefNoCase(seg, off);
            if (s == NULL)
            {
                sim_printf("parse_addr(): entrypoint '%s' not found in segment '%s'", off, seg);
                *optr = cptr;   // signal error

                return 0;
            }
            offset = s->value;
        }
        
        // if we get here then seg contains a segment# and offset.
        // So, fetch the actual address given the segment & offset ...
        // ... and return this absolute, 24-bit address
        
        t_addr absAddr = getAddress(segno, offset + PRoffset);
        
        // TODO: only luckily does this work FixMe
        *optr = endp;   //cptr + strlen(cptr);
        
        return absAddr;
    }
    else
    {
        // a PR or alias thereof
        int segno = 0;
        int offset = 0;
        struct PRtab *prt = _prtab;
        while (prt->alias)
        {
            if (strncasecmp(cptr, prt->alias, strlen(prt->alias)) == 0)
            {
                segno = PR[prt->n].SNR;
                offset = PR[prt->n].WORDNO;
                break;
            }
            
            prt += 1;
        }
        if (prt->alias)    // a PR or alias
        {
            t_addr absAddr = getAddress(segno, offset);
            *optr = cptr + strlen(prt->alias);
        
            return absAddr;
        }
    }
    
    // No, determine absolute address given by cptr
    return (t_addr)strtol(cptr, optr, 8);
}

static void fprint_addr(FILE *stream, DEVICE *dptr, t_addr simh_addr)
{
    char temp[256];
    bool bFound = getSegmentAddressString(simh_addr, temp);
    if (bFound)
        fprintf(stream, "%s (%08o)", temp, simh_addr);
    else
        fprintf(stream, "%06o", simh_addr);
}

// This is part of the simh interface
/*! Based on the switch variable, symbolically output to stream ofile the data in array val at the specified addr
 in unit uptr.

 * simh "fprint_sym" – Based on the switch variable, symbolically output to stream ofile the data in array val at the specified addr in unit uptr.
 */

t_stat fprint_sym (FILE *ofile, t_addr addr, t_value *val, UNIT *uptr, int32 sw)
{
// XXX Bug: assumes single cpu
// XXX CAC: This seems rather bogus; deciding the output format based on the
// address of the UNIT? Would it be better to use sim_unit.u3 (or some such 
// as a word width?

    if (!(sw & SWMASK ('M')))
        return SCPE_ARG;
    
    if (uptr == &cpu_unit[0])
    {
        word36 word1 = *val;
        
        // get base syntax
        char *d = disAssemble(word1);
        
        fprintf(ofile, "%s", d);
        
        // decode instruction
        DCDstruct *p = decodeInstruction(word1, NULL);
        
        // MW EIS?
        if (p->info->ndes > 1)
        {
            // Yup, just output word values (for now)
            
            // XXX Need to complete MW EIS support in disAssemble()
            
            for(int n = 0 ; n < p->info->ndes; n += 1)
                fprintf(ofile, " %012llo", val[n + 1]);
          
            freeDCDstruct(p);
            return -p->info->ndes;
        }
        
        freeDCDstruct(p);
        return SCPE_OK;

        //fprintf(ofile, "%012llo", *val);
        //return SCPE_OK;
    }
    return SCPE_ARG;
}

// This is part of the simh interface
/*!  – Based on the switch variable, parse character string cptr for a symbolic value val at the specified addr
 in unit uptr.
 */
t_stat parse_sym (char *cptr, t_addr addr, UNIT *uptr, t_value *val, int32 sswitch)
{
    return SCPE_ARG;
}

// from MM

sysinfo_t sys_opts =
  {
    0, /* clock speed */
    {
// I get too much jitter in cpuCycles when debugging 20184; try turning queing
// off here (changing 0 to -1)
// still get a little jitter, and once a hang in DIS. very strange
      -1, /* iom_times.connect */
      -1,  /* iom_times.chan_activate */
      10, /* boot_time */
    },
    {
// XXX This suddenly started working when I reworked the iom code for multiple units.
// XXX No idea why. However, setting it to zero queues the boot tape read instead of
// XXX performing it immediately. This makes the boot code fail because iom_boot 
// XXX returns before the read is dequeued, causing the CPU to start before the
// XXX tape is read into memory. 
// XXX Need to fix the cpu code to either do actual fault loop on unitialized memory, or force it into the wait for interrupt sate; and respond to the interrupt from the IOM's completion of the read.
//
      -1, /* mt_times.read */
      -1  /* mt_times.xfer */
    },
    0 /* warn_uninit */
  };

static char * encode_timing (int timing)
  {
    static char buf [64];
    if (timing < 0)
      return "Off";
    sprintf (buf, "%d", timing);
    return buf;
  }

static t_stat sys_show_config (FILE * st, UNIT * uptr, int val, void * desc)
  {
    sim_printf ("IOM connect time:         %s\n",
                encode_timing (sys_opts . iom_times . connect));
    sim_printf ("IOM activate time:        %s\n",
                encode_timing (sys_opts . iom_times . chan_activate));
    sim_printf ("IOM boot time:            %s\n",
                encode_timing (sys_opts . iom_times . boot_time));
    sim_printf ("MT Read time:             %s\n",
                encode_timing (sys_opts . mt_times . read));
    sim_printf ("MT Xfer time:             %s\n",
                encode_timing (sys_opts . mt_times . xfer));

    return SCPE_OK;
}

static config_value_list_t cfg_timing_list [] =
  {
    { "disable", -1 },
    { NULL }
  };

static config_list_t sys_config_list [] =
  {
    /*  0 */ { "connect_time", -1, 100000, cfg_timing_list }, // set sim_activate timing
    /*  1 */ { "activate_time", -1, 100000, cfg_timing_list }, // set sim_activate timing
    /*  2 */ { "mt_read_time", -1, 100000, cfg_timing_list }, // set sim_activate timing
    /*  3 */ { "mt_xfer_time", -1, 100000, cfg_timing_list }, // set sim_activate timing
    /*  4 */ { "iom_boot_time", -1, 100000, cfg_timing_list }, // set sim_activate timing
 };

static t_stat sys_set_config (UNIT * uptr, int32 value, char * cptr, void * desc)
  {
    config_state_t cfg_state = { NULL };

    for (;;)
      {
        int64_t v;
        int rc = cfgparse ("sys_set_config", cptr, sys_config_list, & cfg_state, & v);
        switch (rc)
          {
            case -2: // error
              cfgparse_done (& cfg_state);
              return SCPE_ARG;

            case -1: // done
              break;

            case  0: // CONNECT_TIME
              sys_opts . iom_times . connect = v;
              break;

            case  1: // ACTIVATE_TIME
              sys_opts . iom_times . chan_activate = v;
              break;

            case  2: // MT_READ_TIME
              sys_opts . mt_times . read = v;
              break;

            case  3: // MT_XFER_TIME
              sys_opts . mt_times . xfer = v;
              break;

            case  4: // IOM_BOOT_TIME
              sys_opts . iom_times . boot_time = v;
              break;

            default:
              sim_debug (DBG_ERR, & iom_dev, "sys_set_config: Invalid cfgparse rc <%d>\n", rc);
              sim_printf ("error: iom_set_config: invalid cfgparse rc <%d>\n", rc);
              cfgparse_done (& cfg_state);
              return SCPE_ARG;
          } // switch
        if (rc < 0)
          break;
      } // process statements
    cfgparse_done (& cfg_state);
    return SCPE_OK;
  }


static MTAB sys_mod [] =
  {
    {
      MTAB_XTD | MTAB_VDV | MTAB_NMO /* | MTAB_VALR */, /* mask */
      0,            /* match */
      "CONFIG",     /* print string */
      "CONFIG",         /* match string */
      sys_set_config,         /* validation routine */
      sys_show_config, /* display routine */
      NULL          /* value descriptor */
    },
    { 0 }
  };



static t_stat sys_reset (DEVICE *dptr)
  {
    return SCPE_OK;
  }

DEVICE sys_dev = {
    "SYS",       /* name */
    NULL,        /* units */
    NULL,        /* registers */
    sys_mod,     /* modifiers */
    0,           /* #units */
    8,           /* address radix */
    PASIZE,      /* address width */
    1,           /* address increment */
    8,           /* data radix */
    36,          /* data width */
    NULL,        /* examine routine */
    NULL,        /* deposit routine */
    & sys_reset, /* reset routine */
    NULL,        /* boot routine */
    NULL,        /* attach routine */
    NULL,        /* detach routine */
    NULL,        /* context */
    0,           /* flags */
    0,           /* debug control flags */
    0,           /* debug flag names */
    NULL,        /* memory size change */
    NULL         /* logical name */
};


// This is part of the simh interface
DEVICE * sim_devices [] =
  {
    & cpu_dev, // dev[0] is special to simh; it is the 'default device'
    & iom_dev,
    & tape_dev,
    & scu_dev,
    & clk_dev,
    // & mpc_dev,
    & opcon_dev, // Not hooked up yet
//    & disk_dev, // Not hooked up yet
    & sys_dev,

    NULL
};


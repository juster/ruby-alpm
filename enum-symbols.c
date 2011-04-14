#include <stdio.h>
#include <stdlib.h>

#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

/* We cannot count the entries of bitflag style enums at compile time. */
ID * loglevel_symbols, * transflag_symbols, * transconv_symbols;

/* The count for normal integer enums is the last entry when the enum
   is 1-indexed. Add one to the last entry if it is 0-indexed. */
ID pkgreason_symbols       [ PM_PKG_REASON_DEPEND + 1 ];
ID transevt_symbols        [ PM_TRANS_EVT_DISKSPACE_DONE ];
ID transprog_symbols       [ PM_TRANS_PROGRESS_INTEGRITY_START + 1 ];
ID depmod_symbols          [ PM_DEP_MOD_LT ];
ID fileconflicttype_symbols[ PM_FILECONFLICT_FILESYSTEM ];
ID pmerrno_symbols         [ PM_ERR_EXTERNAL_DOWNLOAD ];

/* We may have to do a reverse lookup of symbol IDs to alpm constants. */

typedef struct { ID symid; int alpmval; } sympair;
#define REVREF_COUNT PM_PKG_REASON_DEPEND + 1
static sympair symbol_enum_map [ REVREF_COUNT ];

static ID *
alloc_bitflag_table ( int enum_max )
{
    ID * idarray;
    int count = 0;
    while ( enum_max > 0 ) {
        enum_max >>= 1;
        ++count;
    }

    idarray = calloc( count, sizeof( int ));
    if ( idarray == NULL ) {
        perror( "failed to alloc ruby-alpm enum/symbol tables" );
    }

    /* This memory is never freed ... owell! */

    return idarray;
}

static ID
remember_enum ( ID symid, int alpmval, int idx )
{
    sympair * writeptr = symbol_enum_map + idx;
    writeptr->symid    = symid;
    writeptr->alpmval  = alpmval;

    return symid;
}

int
ralpm_symbol_to_enum ( VALUE symbol )
{
    ID symid;
    int i;

    symid = SYM2ID( symbol );

    for ( i=0 ; i < REVREF_COUNT ; ++i ) {
        if ( symbol_enum_map[i].symid == symid ) {
            return symbol_enum_map[i].alpmval;
        }
    }

    fprintf( stderr, "FATAL ERROR: could not find symbol :%s's alpm value\n",
             rb_id2str( symid ));
    abort();
}

void
define_enum_symbols ()
{
    int idx, backidx;
    backidx = 0;

    loglevel_symbols  = alloc_bitflag_table( PM_LOG_FUNCTION );
    transflag_symbols = alloc_bitflag_table( PM_TRANS_FLAG_NOLOCK );
    transconv_symbols = alloc_bitflag_table( PM_TRANS_CONV_SELECT_PROVIDER );

    /* Use SYM for symbols we don't need to reverse into enums later and
       REMSYM for those that we do. */
#define SYM( TBL, NAME ) TBL[ idx++ ] = rb_intern( NAME )
#define REMSYM( TBL, NAME, VAL ) \
    remember_enum( SYM( TBL, NAME ), VAL, backidx++ )

    /* We must be sure to add symbols in the same order as the enum. */

#define ERR( NAME ) SYM( pmerrno_symbols, NAME )
    idx = 0;
    /* System */
    ERR( "memory" );
    ERR( "system" );
    ERR( "badperms" );
    ERR( "notafile" );
    ERR( "notadir" );
    ERR( "wrongargs" );
    ERR( "diskspace" );
    /* Library Instance */
    ERR( "handlenull" );
    ERR( "handlenotnull" );
    ERR( "handlelock" );
    /* Databases */
    ERR( "dbopen" );
    ERR( "dbcreate" );
    ERR( "dbnull" );
    ERR( "dbnotnull" );
    ERR( "dbnotfound" );
    ERR( "dbversion" );
    ERR( "dbwrite" );
    ERR( "dbremove" );
    /* Servers */
    ERR( "serverbadurl" );
    ERR( "servernone" );
    /* Transactions */
    ERR( "transnotnull" );
    ERR( "transnull" );
    ERR( "transduptarget" );
    ERR( "transnotinitialized" );
    ERR( "transnotprepared" );
    ERR( "transabort" );
    ERR( "transtype" );
    ERR( "transnotlocked" );
    /* Packages */
    ERR( "pkgnotfound" );
    ERR( "pkgignored" );
    ERR( "pkginvalid" );
    ERR( "pkgopen" );
    ERR( "pkgcantremove" );
    ERR( "pkginvalidname" );
    ERR( "pkginvalidarch" );
    ERR( "pkgreponotfound" );
    /* Deltas */
    ERR( "dltinvalid" );
    ERR( "dltpatchfailed" );
    /* Dependencies */
    ERR( "unsatisfieddeps" );
    ERR( "conflictingdeps" );
    ERR( "fileconflicts" );
    /* Misc */
    ERR( "retrieve" );
    ERR( "write" );
    ERR( "invalidregex" );
    /* External library errors */
    ERR( "libarchive" );
    ERR( "libfetch" );
    ERR( "external_download" );
#undef ERR

#define MOD( NAME ) SYM( depmod_symbols, NAME )
    idx = 0;
    MOD( "any" );
    MOD( "eq" );
    MOD( "ge" );
    MOD( "le" );
    MOD( "gt" );
    MOD( "lt" );
#undef MOD

#define REASON( NAME, VAL ) REMSYM( pkgreason_symbols, NAME, VAL )
    idx = 0;
    REASON( "explicit", PM_PKG_REASON_EXPLICIT );
    REASON( "depend",   PM_PKG_REASON_DEPEND   );
#undef REASON

#define LOGLEVEL( NAME ) SYM( loglevel_symbols, NAME )
    idx = 0;
    LOGLEVEL( "error"    );
    LOGLEVEL( "warning"  );
    LOGLEVEL( "debug"    );
    LOGLEVEL( "function" );
#undef LOGLEVEL

#undef SYM
#undef REMSYM

    return;
}

/* Converts a bitflag into a zero-based index. */
int
bitflag_to_tblindex ( int bitflag )
{
    int index = 0;
    while ( 1 ) {
        bitflag >>= 1;
        if ( bitflag == 0 ) { break; }
        ++index;
    }
    return index;
}

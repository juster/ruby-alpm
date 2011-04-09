#include <stdio.h>

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

void
define_enum_symbols ()
{
    int idx;
#define SYM( TBL, NAME ) TBL[ idx++ ] = rb_intern( NAME )
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

    loglevel_symbols  = alloc_bitflag_table( PM_LOG_FUNCTION );
    transflag_symbols = alloc_bitflag_table( PM_TRANS_FLAG_NOLOCK );
    transconv_symbols = alloc_bitflag_table( PM_TRANS_CONV_SELECT_PROVIDER );

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

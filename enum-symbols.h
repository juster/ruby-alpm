#ifndef RUBYALPM_ENUMSYMBOLS_H
#define RUBYALPM_ENUMSYMBOLS_H

extern ID * loglevel_symbols, * transflag_symbols, * transconv_symbols,
    pkgreason_symbols[], transevt_symbols[], transprog_symbols[],
    depmod_symbols[], pmerrno_symbols[], fileconflicttype_symbols[];

/* Define macros so I do not have to remember which ones are zero-based. */

#define PMERRNO_SYM( VAL ) ID2SYM( pmerrno_symbols[ VAL-1 ] )
#define DEPMOD_SYM( VAL )  ID2SYM( depmod_symbols[ VAL-1 ] )

void define_enum_symbols ();
int bitflag_to_tblindex ( int bitflag );

#endif

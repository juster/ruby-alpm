#ifndef RUBYALPM_H
#define RUBYALPM_H

extern VALUE mAlpm, eAlpmError, cDep, cPackage,
    cDatabase, cLocalDB, cSyncDB;

extern VALUE logcb_proc, dlcb_proc, fetchcb_proc, totaldlcb_proc;

#define NEGISERR( FUNCCALL )                                            \
    if ( FUNCCALL == -1 )                                               \
        rb_raise( eAlpmError, "%s", alpm_strerrorlast());               \
    return Qnil

#define ACCESSOR( PREFIX, TYPE, FUNC, MAPPER )                       \
    static VALUE PREFIX##_##FUNC ( VALUE self )                      \
    {                                                                \
        TYPE * internal;                                             \
        Data_Get_Struct( self, TYPE, internal );                     \
        return MAPPER( alpm_##PREFIX##_## FUNC( internal ));         \
    }

#define INT2BOOL(INT) ( (INT) ? Qtrue : Qfalse )
#define GRP2ARY( GRP ) alpmpkglist_to_ary( alpm_grp_get_pkgs( GRP ))
#define GRPNAME( GRP ) alpm_grp_get_name( GRP )

#define PKG2OBJ( PTR ) Data_Wrap_Struct( cPackage, NULL, NULL, PTR )
#define DEP2OBJ( PTR ) Data_Wrap_Struct( cDep, NULL, NULL, PTR )
#define LDB2OBJ( PTR ) Data_Wrap_Struct( cLocalDB, NULL, NULL, PTR )
#define SDB2OBJ( PTR ) Data_Wrap_Struct( cSyncDB, NULL, NULL, PTR )

/* Datatype conversion functions (see datatypes.c) */
alpm_list_t * ary_to_alpmstrlist ( VALUE ary );
VALUE alpmstrlist_to_ary ( alpm_list_t * t );

/* Callbacks. */
void ruby_alpm_cb_log      ( pmloglevel_t, const char *, va_list );
void ruby_alpm_cb_download ( const char *, off_t, off_t );
int  ruby_alpm_cb_fetch    ( const char *, const char *, int );
void ruby_alpm_cb_totaldl  ( off_t );

void Init_options  ( void );
void Init_dep      ( void );
void Init_package  ( void );
void Init_database ( void );

#endif

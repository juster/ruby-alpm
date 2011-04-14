#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

#define ACC( FUNC, MAPPER ) ACCESSOR( db, pmdb_t, FUNC, MAPPER )
#define STRACC( FUNC ) ACC( FUNC, rb_str_new2 )

STRACC( get_name )
STRACC( get_url )

#define INITDBPTR \
    pmdb_t *db;   \
    Data_Get_Struct( self, pmdb_t, db )

VALUE
db_add_url ( VALUE self, VALUE url )
{
    INITDBPTR;
    NEGISERR( alpm_db_setserver( db, StringValueCStr( url )));
}

VALUE
db_unregister ( VALUE self )
{
    INITDBPTR;
    NEGISERR( alpm_db_unregister( db ));
}

VALUE
db_update ( VALUE self, VALUE force )
{
    INITDBPTR;
    NEGISERR( alpm_db_update( !!force, db ));
}

VALUE
db_find ( VALUE self, VALUE pkgname )
{
    pmpkg_t * pkg;
    INITDBPTR;
    pkg = alpm_db_get_pkg( db, StringValueCStr( pkgname ));
    return ( pkg == NULL ? Qnil : PKG2OBJ( pkg ));
}

VALUE
db_pkgs ( VALUE self )
{
    INITDBPTR;
    return alpmpkglist_to_ary( alpm_db_get_pkgcache( db ));
}

VALUE
db_find_group ( VALUE self, VALUE grpname )
{
    INITDBPTR;
    return GRP2ARY( alpm_db_readgrp( db, StringValueCStr( grpname )));
}

VALUE
db_groups ( VALUE self )
{
    alpm_list_t * iter;

    INITDBPTR;
    VALUE grphash = rb_hash_new();
    
    iter = alpm_db_get_grpcache( db );
    while ( iter != NULL ) {
        pmgrp_t * grp = alpm_list_getdata( iter );
        VALUE grpname = rb_str_new2( alpm_grp_get_name( grp ));
        VALUE pkgs    = alpmpkglist_to_ary( alpm_grp_get_pkgs( grp ));
        rb_hash_aset( grphash, grpname, pkgs );
        iter = alpm_list_next( iter );
    }

    return grphash;
}

void Init_database ( void )
{
    cDatabase = rb_define_class_under( mAlpm,     "DB",    rb_cObject );
    cLocalDB  = rb_define_class_under( cDatabase, "Local", cDatabase  );
    cSyncDB   = rb_define_class_under( cDatabase, "Sync",  cDatabase  );

#define CLASSMETH( CLASS, METH, C ) \
    rb_define_method( CLASS, #METH, db_##METH, C )

    /* Methods for both local and sync databases. */
    
#define METH( METH, C ) CLASSMETH( cDatabase, METH, C )
#define METH0( METH ) METH( METH, 0 )
#define METH1( METH ) METH( METH, 1 )
#define METH2( METH ) METH( METH, 2 )
    METH0( name );
    METH1( find );
    METH0( pkgs );
    METH1( find_group );
    METH0( groups     );
    METH0( unregister );
#undef METH

    /* Local database methods. */

#define METH( METH, C ) CLASSMETH( cLocalDB, METH, C )
#undef METH

    /* Sync database methods. */

#define METH( FUNC, C ) rb_define_method( cSyncDB, #FUNC, db_##FUNC, C )
    METH0( update  );
    METH0( url     );
    METH1( add_url );
#undef METH

#undef CLASSMETH
#undef METH0
#undef METH1
#undef METH2

    return;
}

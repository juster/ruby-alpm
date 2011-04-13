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
    cDatabase = rb_define_class_under( mAlpm, "DB", rb_cObject );
    
    rb_define_method( cDatabase, "name", db_get_name, 0 );
#define METH( FUNC, C ) rb_define_method( cDatabase, #FUNC, db_##FUNC, C )
    METH( find,   1 );
    METH( pkgs,   0 );
    METH( find_group, 1 );
    METH( groups,     0 );
#undef METH

    cLocalDB = rb_define_class_under( cDatabase, "Local", cDatabase );
    
    cSyncDB = rb_define_class_under( cDatabase, "Sync", cDatabase );
    rb_define_method( cSyncDB, "update",  db_update,  0 );
    rb_define_method( cSyncDB, "url",     db_get_url, 0 );
    rb_define_method( cSyncDB, "add_url", db_add_url, 1 );

    return;
}

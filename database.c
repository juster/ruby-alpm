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

void Init_database ( void )
{
    cDatabase = rb_define_class_under( mAlpm, "Database", rb_cObject );
    
#define METH( NAME, FUNC ) rb_define_method( cDatabase, NAME, db_##FUNC, 0 )
    METH( "name", get_name );
    METH( "url", get_url );
#undef METH
#define METH( FUNC, C ) rb_define_method( cDatabase, #FUNC, db_##FUNC, C )
    METH( update, 1 );
    METH( find,   1 );
    METH( pkgs,   0 );
#undef METH

    return;
}

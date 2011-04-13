#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

static VALUE
new_timeobj ( time_t pkgtime )
{
    VALUE secs, atsym, timeobj;
    secs    = INT2NUM( pkgtime );
    atsym   = rb_intern( "at" );
    timeobj = rb_funcall( rb_cTime, atsym, 1, secs );
    return timeobj;
}

#define ACC( FUNC, MAPPER ) ACCESSOR( pkg, pmpkg_t, get_ ## FUNC, MAPPER )
#define STRACC( FUNC )      ACC( FUNC, rb_str_new2 )
#define STRLISTACC( FUNC )  ACC( FUNC, alpmstrlist_to_ary )
#define TIMEACC( FUNC )     ACC( FUNC, newtimeobj )
#define BOOLACC( FUNC )     ACC( FUNC, INT2BOOL )
#define INTACC( FUNC )      ACC( FUNC, INT2NUM )

STRACC( filename )
STRACC( name )
STRACC( version )
STRACC( desc )
STRACC( url )

TIMEACC( builddate )
TIMEACC( installdate )

STRACC( packager )
STRACC( md5sum )
STRACC( arch )

INTACC( size  )
INTACC( isize )

STRLISTACC( licenses )
STRLISTACC( groups )

ACC( depends, alpmdeplist_to_ary )
STRLISTACC( optdepends )
STRLISTACC( conflicts )
STRLISTACC( provides )

STRLISTACC( replaces )
STRLISTACC( files )
STRLISTACC( backup )

#undef ACC
#define ACC( FUNC, MAPPER ) ACCESSOR( pkg, pmpkg_t, FUNC, MAPPER )

/* The following accessors don't use the _get prefix. */

BOOLACC( has_scriptlet )
INTACC( download_size )

/* TODO: load, free, checkmd5sum, fetch_pkgurl, vercmp, computer_requiredby
   changelog funcs, unused_deltas */

void
Init_package( void )
{
    cPackage = rb_define_class_under( mAlpm, "Package", rb_cObject );

#define METH( NAME ) rb_define_method( cPackage, #NAME, pkg_get_##NAME, 0 )
    METH( filename );
    METH( name );
    METH( version );
    METH( desc );
    METH( url );
    METH( builddate );
    METH( installdate );
    METH( packager );
    METH( md5sum );
    METH( arch );
    METH( size );
    METH( isize );
    METH( licenses );
    METH( groups );
    METH( depends );
    METH( optdepends );
    METH( conflicts );
    METH( provides );
    METH( replaces );
    METH( files );
    METH( backup );
#undef METH

    /* Removes the get_prefix from C function names. */
#define METH( NAME ) rb_define_method( cPackage, #NAME, pkg_##NAME, 1 )
    METH( has_scriptlet );
    METH( download_size );
#undef METH

    return;
}

#include <alpm.h>
#undef DEPRECATED /* Ruby uses its own DEPRECATED macro. */
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

VALUE mAlpm, eAlpmError;

static VALUE
err_initialize ( int argc, VALUE * argv, VALUE self )
{
    /* pm_errno is an exported variable from libalpm. */
    rb_iv_set( self, "@errno", PMERRNO_SYM( pm_errno ));
    rb_call_super( argc, argv ); /* stores string arguments */

    return self;
}

static VALUE
err_errno ( VALUE self )
{
    return rb_iv_get( self, "@errno" );
}

static VALUE
ralpm_init ()
{
    NEGISERR( alpm_initialize() );
    return Qnil;
}

static VALUE
ralpm_release ()
{
    NEGISERR( alpm_release() );
    return Qnil;
}

static VALUE
ralpm_version ()
{
    return rb_str_new2( alpm_version() );
}

#define FUNC( NAME, ARGCOUNT ) \
    rb_define_module_function( mAlpm, #NAME, ralpm_ ## NAME, ARGCOUNT )
#define FUNC0( NAME ) FUNC( NAME, 0 )

void
Init_alpm ()
{
    alpm_initialize();

    mAlpm = rb_define_module( "Alpm" );
    FUNC0( init );
    FUNC0( release );
    FUNC0( version );

    eAlpmError = rb_define_class( "AlpmError", rb_eRuntimeError );
    rb_define_method( eAlpmError, "initialize", err_initialize, -1 );
    rb_define_method( eAlpmError, "errno",      err_errno,      0  );

    define_enum_symbols();

    Init_options();
}

#undef FUNC
#undef FUNC0

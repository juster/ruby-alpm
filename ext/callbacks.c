#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

void
ruby_alpm_cb_log ( pmloglevel_t level, const char * fmt, va_list args )
{
    VALUE level_symbol = LOGLEVEL_SYM( level );
    VALUE msg          = rb_vsprintf( fmt, args );
    
    if ( ! logcb_proc ) { return; }
    rb_proc_call( logcb_proc, rb_ary_new3( 2, level_symbol, msg ));
    return;
}

void
ruby_alpm_cb_download ( const char * filename, off_t xfered, off_t total )
{
    if ( ! dlcb_proc ) { return; }

    VALUE rbfname  = rb_str_new2( filename );
    VALUE rbxfered = INT2NUM( xfered );
    VALUE rbtotal  = INT2NUM( total );

    rb_proc_call( dlcb_proc, rb_ary_new3( 3, rbfname, rbxfered, rbtotal ));
    return;
}

/* Just a simple wrapper so I can use rb_protect to catch exceptions. */
static VALUE
fetch_proc_call ( VALUE argarray )
{
    return rb_proc_call( fetchcb_proc, argarray );
}

int
ruby_alpm_cb_fetch ( const char * url, const char * localpath, int force )
{
    if ( ! fetchcb_proc ) { return -1; }

    int error_occurred = 0;
    VALUE rburl   = rb_str_new2( url );
    VALUE rbpath  = rb_str_new2( localpath );
    VALUE rbforce = INT2BOOL( force );
    VALUE intret;
    intret = rb_protect( fetch_proc_call,
                         rb_ary_new3( rburl, rbpath, rbforce ),
                         &error_occurred );
    if ( error_occurred ) { return -1; }

    /* The callback should return 0 for success or 1 for file exists. */
    return NUM2INT( intret );
}

void
ruby_alpm_cb_totaldl ( off_t total )
{
    if ( ! totaldlcb_proc ) { return; }

    VALUE rbtotal = INT2NUM( total );
    rb_proc_call( totaldlcb_proc, rbtotal );
    return;
}

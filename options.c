#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"

#define DEF_OPTGET( NAME, MAPPER )                      \
    static VALUE opt_get_ ## NAME ( VALUE self ) {             \
        return MAPPER( alpm_option_get_ ## NAME ());    \
    }
#define DEF_OPTSET( NAME, MAPPER )                           \
    static VALUE opt_set_ ## NAME ( VALUE self, VALUE newval ) {    \
        alpm_option_set_ ## NAME ( MAPPER( newval ));        \
    }

#define DEF_OPT( NAME, GETMAP, SETMAP ) \
    DEF_OPTGET( NAME, GETMAP )          \
    DEF_OPTSET( NAME, SETMAP )

#define string_or_nil( STR ) ( ((STR) == NULL) ? Qnil : rb_str_new2( STR ))
#define DEF_STROPT( NAME ) \
    DEF_OPT( NAME, string_or_nil, StringValueCStr )
#define DEF_STRLISTOPT( NAME ) \
    DEF_OPT( NAME, alpmstrlist_to_ary, ary_to_alpmstrlist )
#define DEF_BOOLOPT( NAME ) \
    DEF_OPT( NAME, /*empty*/, INT2BOOL )

DEF_STROPT( root )
DEF_STROPT( dbpath )
DEF_STRLISTOPT( cachedirs )
DEF_STROPT( logfile )
DEF_OPTGET( lockfile, string_or_nil ) /* there is no mutator for lockfile */
DEF_STRLISTOPT( noupgrades )
DEF_BOOLOPT( usesyslog )
DEF_STRLISTOPT( noextracts )
DEF_STRLISTOPT( ignorepkgs )
DEF_STRLISTOPT( ignoregrps )
DEF_STROPT( arch )
DEF_BOOLOPT( usedelta )
DEF_BOOLOPT( checkspace )

#undef DEF_OPTGET
#undef DEF_OPTSET
#undef DEF_OPT
#undef DEF_STROPT
#undef DEF_STRLISTOPT
#undef DEF_BOOLOPT
#undef string_or_nil

/* Callbacks. The use of blocks was copied from Kernel.at_exit(). */

#define DEF_CBOPT( NAME, FUNC, VAR )                            \
    static VALUE opt_set_##NAME ( void )                        \
    {                                                           \
        if ( ! rb_block_given_p() ) {                           \
            if ( VAR ) {                                        \
                rb_gc_unregister_address( &VAR );               \
            }                                                   \
            alpm_option_set##NAME( NULL );                      \
            VAR = Qfalse;                                       \
        }                                                       \
        else {                                                  \
            VAR = rb_block_proc();                              \
            rb_gc_register_address( &VAR );                     \
            alpm_option_set_##NAME( FUNC );                     \
        }                                                       \
        return Qnil;                                            \
    }

DEF_CBOPT( logcb,   ruby_alpm_cb_log,      logcb_proc   )
DEF_CBOPT( dlcb,    ruby_alpm_cb_download, dlcb_proc    )
DEF_CBOPT( fetchcb, ruby_alpm_cb_fetch,    fetchcb_proc )

void
Init_options ()
{
#define OPTFUNC( NAME )                                               \
    rb_define_module_function( mAlpm, #NAME, opt_get_##NAME, 0 );     \
    rb_define_module_function( mAlpm, #NAME"=", opt_set_##NAME, 1 )

    OPTFUNC( root );
    OPTFUNC( dbpath );
    OPTFUNC( cachedirs );
    OPTFUNC( usesyslog );
    OPTFUNC( noupgrades );
    OPTFUNC( usesyslog );
    OPTFUNC( noextracts );
    OPTFUNC( ignorepkgs );
    OPTFUNC( ignoregrps );
    OPTFUNC( arch );
    OPTFUNC( usedelta );
    OPTFUNC( checkspace );
#undef OPTFUNC

#define CBFUNC( NAME ) \
    rb_define_module_function( mAlpm, #NAME, opt_set_##NAME, 0 )

    CBFUNC( logcb   );
    CBFUNC( dlcb    );
    CBFUNC( fetchcb );
#undef CBFUNC
}

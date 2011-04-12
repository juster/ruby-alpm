#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

void ruby_alpm_cb_log ( pmloglevel_t level, const char * fmt, va_list args )
{
    VALUE level_symbol = LOGLEVEL_SYM( level );
    VALUE msg          = rb_vsprintf( fmt, args );
    
    if ( ! logcb_proc ) { return; }
    rb_proc_call( logcb_proc, rb_ary_new3( 2, level_symbol, msg ));
    return;
}

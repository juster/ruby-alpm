#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

#define ACC( FUNC, MAPPER ) ACCESSOR( dep, pmdepend_t, FUNC, MAPPER )

ACC( get_mod, DEPMOD_SYM )
ACC( get_name, rb_str_new2 )
ACC( get_version, rb_str_new2 )
ACC( compute_string, rb_str_new2 )

#undef ACC

void Init_dep ()
{
    cDep = rb_define_class_under( mAlpm, "Dep", rb_cObject );
    
#define METH( NAME, FUNC ) rb_define_method( cDep, NAME, dep_ ## FUNC, 0 )
    METH( "mod", get_mod );
    METH( "name", get_name );
    METH( "version", get_version );
    METH( "to_s", compute_string );
#undef METH

    return;
}

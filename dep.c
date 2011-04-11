#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"
#include "enum-symbols.h"

#define ACC( FUNC, MAPPER )                             \
    static VALUE dep_ ## FUNC ( VALUE self )            \
    {                                                   \
        pmdepend_t * dep;                               \
        Data_Get_Struct( self, pmdepend_t, dep );       \
        return MAPPER( alpm_dep_ ## FUNC( dep ));       \
    }

ACC( get_mod, DEPMOD_SYM )
ACC( get_name, rb_str_new2 )
ACC( get_version, rb_str_new2 )
ACC( compute_string, rb_str_new2 )

#undef ACC

void Init_dep ()
{
    cDep = rb_define_class_under( mAlpm, "Dep", rb_cObject );
    
#define METH( NAME, FUNC ) rb_define_method( cDep, NAME, dep_ ## FUNC, 1 )
    METH( "mod", get_mod );
    METH( "name", get_name );
    METH( "version", get_version );
    METH( "to_s", compute_string );
#undef METH

    return;
}

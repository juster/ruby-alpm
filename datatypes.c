#include <string.h>
#include <alpm.h>
#undef DEPRECATED
#include "ruby.h"

#include "ruby-alpm.h"

#define DEF_LIST2ARY( FUNCNAME, MAPPER, TYPE )                          \
    VALUE                                                               \
    FUNCNAME ( alpm_list_t * list )                                     \
    {                                                                   \
        VALUE ary;                                                      \
                                                                        \
        ary = rb_ary_new();                                             \
        while ( list != NULL ) {                                        \
            rb_ary_push( ary, MAPPER( TYPE alpm_list_getdata( list ))); \
            list = alpm_list_next( list );                              \
        }                                                               \
                                                                        \
        return ary;                                                     \
    }

DEF_LIST2ARY( alpmstrlist_to_ary, rb_str_new2, (char *)       )
DEF_LIST2ARY( alpmdeplist_to_ary, DEP2OBJ,     (pmdepend_t *) )
DEF_LIST2ARY( alpmpkglist_to_ary, PKG2OBJ,     (pmpkg_t *)    )

#undef DEF_LIST2ARY

#define DEF_ARY2LIST( FUNCNAME, MAPPER )                    \
    alpm_list_t * FUNCNAME ( VALUE ary )                    \
    {                                                       \
        alpm_list_t * list;                                 \
        VALUE elem;                                         \
        long len, i;                                        \
                                                            \
        len  = RARRAY_LEN( ary );                           \
        list = NULL;                                        \
                                                            \
        for ( i = 0 ; i < len ; ++i ) {                     \
            elem = rb_ary_entry( ary, i );                  \
            list = alpm_list_add( list, (void *)MAPPER( elem ));    \
        }                                                   \
                                                            \
        return list;                                        \
    }

static char *
dup_rbstring ( VALUE str )
{
    return strdup( StringValueCStr( str ));
}

DEF_ARY2LIST( ary_to_alpmstrlist, dup_rbstring )

#undef DEF_ARY2LIST

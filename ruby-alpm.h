#ifndef RUBYALPM_H
#define RUBYALPM_H

extern VALUE mAlpm, eAlpmError, cDep, cPackage;

#define NEGISERR( FUNCCALL )                                            \
    do { if ( FUNCCALL == -1 )                                          \
            rb_raise( eAlpmError, "%s", alpm_strerrorlast()); } while (0)

#define ACCESSOR( PREFIX, TYPE, FUNC, MAPPER )                       \
    static VALUE PREFIX##_##FUNC ( VALUE self )                      \
    {                                                                \
        TYPE * internal;                                             \
        Data_Get_Struct( self, TYPE, internal );                     \
        return MAPPER( alpm_##PREFIX##_## FUNC( internal ));         \
    }

#define INT2BOOL(INT) ( (INT) ? Qtrue : Qfalse )

/* Datatype conversion functions (see datatypes.c) */
alpm_list_t * ary_to_alpmstrlist ( VALUE ary );
VALUE alpmstrlist_to_ary ( alpm_list_t * t );

/* Adds option functions to Alpm module */
void Init_options ( void );
void Init_dep     ( void );
void Init_package ( void );

#endif

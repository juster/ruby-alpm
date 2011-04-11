#ifndef RUBYALPM_H
#define RUBYALPM_H

extern VALUE mAlpm, cDep;
extern VALUE eAlpmError;

#define NEGISERR( FUNCCALL )                                            \
    do { if ( FUNCCALL == -1 )                                          \
            rb_raise( eAlpmError, "%s", alpm_strerrorlast()); } while (0)

/* Datatype conversion functions (see datatypes.c) */
alpm_list_t * ary_to_alpmstrlist ( VALUE ary );
VALUE alpmstrlist_to_ary ( alpm_list_t * t );

/* Adds option functions to Alpm module */
void Init_options ( void );
void Init_dep ( void );

#endif

#ifndef RUBYALPM_H
#define RUBYALPM_H

extern VALUE mAlpm;
extern VALUE eAlpmError;

#define NEGISERR( FUNCCALL )                                            \
    do { if ( FUNCCALL == -1 )                                          \
            rb_raise( eAlpmError, "%s", alpm_strerrorlast()); } while (0)

void Init_alpmerror ( void );

#endif

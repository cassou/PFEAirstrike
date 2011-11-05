#ifndef FIXNUM_H
#define FIXNUM_H

#define FIXNUM_DECIMALS 15
#define FIXNUM_UNIT (1 << FIXNUM_DECIMALS)

#define FIX2INT(x) ((x) >> FIXNUM_DECIMALS)
#define NUM2FIX(x) ((int)((x)*FIXNUM_UNIT))

#endif

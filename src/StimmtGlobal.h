#ifndef STIMMTGLOBAL_H
#define STIMMTGLOBAL_H

#include <QtGlobal>

#ifndef STIMMT_EXPORT
#  ifdef STIMMT_STATIC
#    define STIMMT_EXPORT
#  else
#    ifdef STIMMT_LIBRARY
#      define STIMMT_EXPORT Q_DECL_EXPORT
#    else
#      define STIMMT_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // STIMMTGLOBAL_H

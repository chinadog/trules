#ifndef TRULES_GLOBAL_H
#define TRULES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRULES_LIBRARY)
#  define TRULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TRULES_GLOBAL_H

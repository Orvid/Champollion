#ifndef DECOMPILER_GLOBAL_HPP
#define DECOMPILER_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(DECOMPILER_LIBRARY)
#  define DECOMPILER_API Q_DECL_EXPORT
#else
#  define DECOMPILER_API Q_DECL_IMPORT
#endif

#define UNUSED(x) ((void)(x))
#endif // DECOMPILER_GLOBAL_HPP

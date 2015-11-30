#ifndef PEX_GLOBAL_HPP
#define PEX_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(PEX_LIBRARY)
#  define PEX_API Q_DECL_EXPORT
#else
#  define PEX_API Q_DECL_IMPORT
#endif

#define UNUSED(x) ((void)(x))

#endif // ASSEMBLY_GLOBAL_HPP

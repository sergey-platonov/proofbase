#ifndef PROOFCORE_GLOBAL_H
#define PROOFCORE_GLOBAL_H

#ifdef PROOF_CORE_LIB
#  define PROOF_CORE_EXPORT Q_DECL_EXPORT
#else
#  define PROOF_CORE_EXPORT Q_DECL_IMPORT
#endif

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(proofLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreLog)

#endif // PROOFCORE_GLOBAL_H

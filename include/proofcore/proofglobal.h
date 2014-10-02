#ifndef PROOFGLOBAL_H
#define PROOFGLOBAL_H

#include "proofcore/proofcore_global.h"
#include <QString>

namespace Proof {
PROOF_CORE_EXPORT QString proofVersion();
PROOF_CORE_EXPORT int proofVersionMajor();
PROOF_CORE_EXPORT int proofVersionYear();
PROOF_CORE_EXPORT int proofVersionMonth();
PROOF_CORE_EXPORT int proofVersionDay();
}

#endif // PROOFGLOBAL_H
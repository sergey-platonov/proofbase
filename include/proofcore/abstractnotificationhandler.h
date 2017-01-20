#ifndef PROOF_ABSTRACTNOTIFICATIONHANDLER_H
#define PROOF_ABSTRACTNOTIFICATIONHANDLER_H

#include "proofcore/proofobject.h"
#include "proofcore/proofcore_global.h"

#include <QString>

namespace Proof {
class AbstractNotificationHandlerPrivate;
class PROOF_CORE_EXPORT AbstractNotificationHandler : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbstractNotificationHandler)
public:
    QString appId() const;

    virtual void notify(const QString &message) = 0;
    //All subclasses should have static QString id() method which should return unique name for this handler

protected:
    AbstractNotificationHandler(AbstractNotificationHandlerPrivate &dd, const QString &appId);
};
} // namespace Proof

#endif // PROOF_ABSTRACTNOTIFICATIONHANDLER_H
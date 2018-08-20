TEMPLATE = lib
TARGET = ProofCore
QT -= gui
include(../proofboot/proof.pri)
CONFIG += proofseed
DESTDIR = $$BUILDPATH/lib

DEFINES += PROOF_CORE_LIB
msvc {
    CONFIG(debug, debug|release):LIBS += -lzlibd
    else:LIBS += -lzlib
} else {
    LIBS += -lz
}

HEADERS += \
    include/proofcore/proofcore_global.h \
    include/proofcore/settings.h \
    include/proofcore/settingsgroup.h \
    include/proofcore/helpers/humanizer.h \
    include/proofcore/taskchain.h \
    include/proofcore/unbounded_taskchain.h \
    include/proofcore/proofobject.h \
    include/private/proofcore/proofobject_p.h \
    include/proofcore/proofglobal.h \
    include/proofcore/objectscache.h \
    include/proofcore/expirator.h \
    include/proofcore/logs.h \
    include/proofcore/coreapplication.h \
    include/private/proofcore/coreapplication_p.h \
    include/proofcore/updatemanager.h \
    include/proofcore/proofobjectprivatepointer.h \
    include/private/proofcore/errormessagesregistry_p.h \
    include/proofcore/abstractnotificationhandler.h \
    include/private/proofcore/abstractnotificationhandler_p.h \
    include/proofcore/memorystoragenotificationhandler.h \
    include/proofcore/errornotifier.h \
    include/proofcore/helpers/versionhelper.h \
    include/private/proofcore/abstractbarcodeconfigurator_p.h \
    include/proofcore/basic_package.h

SOURCES += \
    src/proofcore/settings.cpp \
    src/proofcore/settingsgroup.cpp \
    src/proofcore/helpers/humanizer.cpp \
    src/proofcore/taskchain.cpp \
    src/proofcore/unbounded_taskchain.cpp \
    src/proofcore/proofobject.cpp \
    src/proofcore/proofcore_init.cpp \
    src/proofcore/proofglobal.cpp \
    src/proofcore/expirator.cpp \
    src/proofcore/logs.cpp \
    src/proofcore/coreapplication.cpp \
    src/proofcore/updatemanager.cpp \
    src/proofcore/proofobjectprivatepointer.cpp \
    src/proofcore/errormessagesregistry.cpp \
    src/proofcore/abstractnotificationhandler.cpp \
    src/proofcore/memorystoragenotificationhandler.cpp \
    src/proofcore/errornotifier.cpp

include(../proofboot/proof_translation.pri)

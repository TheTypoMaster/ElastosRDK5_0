
CAR_FLAGS += -n -u
LUBE_FLAGS += -n -u

C_DEFINES += -DHAVE_ANDROID_PTHREAD_SETNAME_NP
C_DEFINES += -DHAVE_GETTID
C_DEFINES += -DHAVE_SELINUX
C_DEFINES += -DHAVE_ANDROID_OS
C_DEFINES += -DHAVE_PRCTL
C_DEFINES += -DHAVE_SYS_UIO_H
C_DEFINES += -DHAVE_OFF64_T
C_DEFINES += -fpermissive
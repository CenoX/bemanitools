avsdlls += jbhook2

deplibs_jbhook2    := \
    avs \
    glhelper \

ldflags_jbhook2   := \
    -lws2_32 \
    -liphlpapi \
    -lopengl32 \

libs_jbhook2       := \
    acioemu \
    eamio \
    jbio \
    jbhook-util \
    jbhook-util-p3io \
    p3ioemu \
    p3io \
    p4ioemu \
    hook \
    hooklib \
    cconfig \
    security \
    util \

src_jbhook2        := \
    config-io.c \
    dllmain.c \
    options.c

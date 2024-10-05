#ifndef JBHOOK3_CONFIG_IO_H
#define JBHOOK3_CONFIG_IO_H

#include <windows.h>

#include "cconfig/cconfig.h"

struct jbhook3_config_io {
    bool windowed;
    bool disable_p4ioemu;
    bool disable_cardemu;
    bool disable_adapteremu;
    bool show_cursor;
    bool use_external_ip;
    bool use_override_ip;
    char override_ip[16];
};

void jbhook3_config_io_init(struct cconfig *config);

void jbhook3_config_io_get(
    struct jbhook3_config_io *config_io, struct cconfig *config);

#endif
#ifndef HOOKLIB_ADAPTER_H
#define HOOKLIB_ADAPTER_H

/**
 * Hooks GetAdaptersInfo to attempt only returning 1 adapter.
 *
 */
void adapter_hook_init(void);

/**
 * Uses the provided address to try and match a network adapter.
 * This adapter is then returned as the only adapter.
 *
 * @param network ip to match
 */
void adapter_hook_override(const char *adapter_address);

/**
 * Set the address to WAN IP.
 * If override adapter address is already applied,
 * this setting may be ignored.
 */
void adapter_hook_wan_override();

/**
 * Use only specific adapter
 */
void adapter_hook_use_specific_adapter(const char *adapter_uuid);
#endif

#include "hook/table.h"
#include <iphlpapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wincrypt.h> /* Required by mingw for some reason */
#include <windows.h>
#include <winsock2.h>

#include "hooklib/adapter.h"

#include "util/codepage.h"
#include "util/defs.h"
#include "util/log.h"

#ifndef BIT_CLEAR
#define BIT_CLEAR(p, n) ((p) &= ~((1) << (n)))
#endif

static DWORD WINAPI
my_GetAdaptersInfo(PIP_ADAPTER_INFO adapter_info, PULONG out_buf_len);

static DWORD(WINAPI *real_GetAdaptersInfo)(
    PIP_ADAPTER_INFO adapter_info, PULONG out_buf_len);

static IP_ADDRESS_STRING override_address;
static bool use_address_override;
static IP_ADDRESS_STRING override_wan_address;
static bool use_wan_address_override;
static char specific_adapter_uuid[MAX_ADAPTER_NAME_LENGTH + 4];
static bool use_specific_adapter_uuid;

static const struct hook_symbol adapter_hook_syms[] = {
    {.name = "GetAdaptersInfo",
     .patch = my_GetAdaptersInfo,
     .link = (void *) &real_GetAdaptersInfo},
};
static int countBits(u_long n)
{
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
static DWORD WINAPI
my_GetAdaptersInfo(PIP_ADAPTER_INFO adapter_info, PULONG out_buf_len)
{
    DWORD ret;
    PMIB_IPFORWARDTABLE ip_fwd_table;
    DWORD table_size;
    DWORD best_adapter;
    PIP_ADAPTER_INFO info, wan_info, specific_adapter_info, dhcp_info;

    ret = real_GetAdaptersInfo(adapter_info, out_buf_len);

    if (ret != 0) {
        return ret;
    }
    dhcp_info = adapter_info;
    while (dhcp_info) {
        if (!dhcp_info->DhcpEnabled) {
            char gateway_addr[sizeof(IP_ADDRESS_STRING)] = {
                0,
            };
            IN_ADDR addr;
            addr.S_un.S_addr =
                inet_addr(dhcp_info->IpAddressList.IpAddress.String);
            if (!(addr.S_un.S_addr == INADDR_NONE ||
                  addr.S_un.S_addr == INADDR_ANY)) {
                u_long mask = inet_addr(dhcp_info->IpAddressList.IpMask.String);
                const int mask_range = (32 - countBits(mask));
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                addr.S_un.S_addr >>= mask_range;
                addr.S_un.S_addr <<= mask_range;
#else
                addr.S_un.S_addr <<= mask_range;
                addr.S_un.S_addr >>= mask_range;
#endif
                addr.S_un.S_un_b.s_b4 = 1;
                sprintf(
                    gateway_addr,
                    "%u.%u.%u.%u",
                    addr.S_un.S_un_b.s_b1,
                    addr.S_un.S_un_b.s_b2,
                    addr.S_un.S_un_b.s_b3,
                    addr.S_un.S_un_b.s_b4);
                log_info(
                    "gateway update : %s[%s], %s",
                    wan_info->AdapterName,
                    wan_info->Description,
                    gateway_addr);
                memset(
                    dhcp_info->DhcpServer.IpAddress.String,
                    0,
                    sizeof(IP_ADDRESS_STRING));
                strncpy(
                    dhcp_info->DhcpServer.IpAddress.String,
                    gateway_addr,
                    sizeof(IP_ADDRESS_STRING));
                memset(
                    dhcp_info->GatewayList.IpAddress.String,
                    0,
                    sizeof(IP_ADDRESS_STRING));
                strncpy(
                    dhcp_info->GatewayList.IpAddress.String,
                    gateway_addr,
                    sizeof(IP_ADDRESS_STRING));
                dhcp_info->DhcpEnabled = 1;
            }
        }
        dhcp_info = dhcp_info->Next;
    }
    wan_info = adapter_info;
    if (use_wan_address_override) {
        while (wan_info) {
            if (!memcmp(
                    wan_info->IpAddressList.IpAddress.String,
                    override_wan_address.String,
                    sizeof(IP_ADDRESS_STRING))) {
                log_info(
                    "%s: using [wan override] adapter: %s, %s, %s, %s",
                    __FUNCTION__,
                    wan_info->AdapterName,
                    wan_info->Description,
                    wan_info->IpAddressList.IpAddress.String,
                    wan_info->IpAddressList.IpMask.String);

                // copy only this adapter over
                memcpy(adapter_info, wan_info, sizeof(*info));
                adapter_info->Next = 0;
                return ret;
            } else {
                memcpy(
                    wan_info->IpAddressList.IpAddress.String,
                    override_wan_address.String,
                    sizeof(IP_ADDRESS_STRING));
                log_info(
                    "%s: update [wan override] adapter: %s, %s, %s, %s",
                    __FUNCTION__,
                    wan_info->AdapterName,
                    wan_info->Description,
                    wan_info->IpAddressList.IpAddress.String,
                    wan_info->IpAddressList.IpMask.String);
            }
            wan_info = wan_info->Next;
        }
    }
    info = adapter_info;
    if (use_address_override) {
        while (info) {
            // this is well defined to be at most sizeof(IP_ADDRESS_STRING)
            // and NULL filled if shorter (hence the memset in
            // adapter_hook_override)
            if (!memcmp(
                    info->IpAddressList.IpAddress.String,
                    override_address.String,
                    sizeof(IP_ADDRESS_STRING))) {
                log_info(
                    "%s: using [override] adapter: %s, %s, %s, %s",
                    __FUNCTION__,
                    info->AdapterName,
                    info->Description,
                    info->IpAddressList.IpAddress.String,
                    info->IpAddressList.IpMask.String);

                // copy only this adapter over
                memcpy(adapter_info, info, sizeof(*info));
                adapter_info->Next = 0;
                return ret;
            }
            info = info->Next;
        }
    }

    ip_fwd_table = (MIB_IPFORWARDTABLE *) malloc(sizeof(MIB_IPFORWARDTABLE));
    table_size = 0;

    if (GetIpForwardTable(ip_fwd_table, &table_size, 1) ==
        ERROR_INSUFFICIENT_BUFFER) {
        free(ip_fwd_table);
        ip_fwd_table = (MIB_IPFORWARDTABLE *) malloc(table_size);
    }

    if (GetIpForwardTable(ip_fwd_table, &table_size, 1) != NO_ERROR ||
        ip_fwd_table->dwNumEntries == 0) {
        return ret;
    }

    best_adapter = ip_fwd_table->table[0].dwForwardIfIndex;
    free(ip_fwd_table);

    specific_adapter_info = adapter_info;
    if (use_specific_adapter_uuid) {
        while (specific_adapter_info) {
            if (strstr(
                    specific_adapter_info->AdapterName,
                    specific_adapter_uuid)) {
                log_info(
                    "%s: using [specific] adapter: %s, %s, %s, %s, %s, %s",
                    __FUNCTION__,
                    specific_adapter_info->AdapterName,
                    specific_adapter_info->Description,
                    specific_adapter_info->IpAddressList.IpAddress.String,
                    specific_adapter_info->IpAddressList.IpMask.String,
                    dhcp_info->DhcpServer.IpAddress.String,
                    dhcp_info->GatewayList.IpAddress.String);

                // copy only this adapter over
                memcpy(adapter_info, specific_adapter_info, sizeof(*info));
                adapter_info->Next = 0;
                return ret;
            }
            specific_adapter_info = specific_adapter_info->Next;
        }
    }
    info = adapter_info;
    while (info) {
        if (info->Index == best_adapter) {
            log_info(
                "%s: using [best] adapter: %s, %s, %s, %s",
                __FUNCTION__,
                info->AdapterName,
                info->Description,
                info->IpAddressList.IpAddress.String,
                info->IpAddressList.IpMask.String);

            // copy only this adapter over
            memcpy(adapter_info, info, sizeof(*info));
            adapter_info->Next = 0;
            return ret;
        }
        info = info->Next;
    }

    return ret;
}

void adapter_hook_init(void)
{
    hook_table_apply(
        NULL, "iphlpapi.dll", adapter_hook_syms, lengthof(adapter_hook_syms));
}

void adapter_hook_use_specific_adapter(const char *adapter_uuid)
{
    use_specific_adapter_uuid = false;
    log_info("Start specific adapter override");

    if (adapter_uuid == NULL || *adapter_uuid == '\0') {
        // empty, do nothing
        return;
    }

    memset(specific_adapter_uuid, 0, sizeof(specific_adapter_uuid));
    memcpy(
        specific_adapter_uuid, adapter_uuid, strlen(specific_adapter_uuid) + 1);

    use_specific_adapter_uuid = true;
}

void adapter_hook_override(const char *adapter_address)
{
    // starts off false anyways due to static
    // but in case it gets called multiple times, set it anyways
    use_address_override = false;
    log_info("Start adapter override");

    if (adapter_address == NULL || *adapter_address == '\0') {
        // empty, do nothing
        log_warning("%s: empty adapter address", __FUNCTION__);
        return;
    }

    if (strlen(adapter_address) > sizeof(IP_ADDRESS_STRING)) {
        log_warning(
            "%s: %s is not an ipv4 address", __FUNCTION__, adapter_address);
        return;
    }

    memset(override_address.String, 0, sizeof(IP_ADDRESS_STRING));
    memcpy(override_address.String, adapter_address, sizeof(IP_ADDRESS_STRING));
    log_info("%s: override address set to %s", __FUNCTION__, adapter_address);

    use_address_override = true;
}

#define HTML_BODY_SIZE 512
void adapter_hook_wan_override()
{
    use_wan_address_override = false;
    log_info("Start Wan override");
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[] =
        "GET /raw HTTP/1.1\r\nHost: myexternalip.com\r\nConnection: "
        "close\r\n\r\n";
    char buffer[HTML_BODY_SIZE];
    char *body_start = NULL;
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        log_warning("WSAStartup failed. Error Code : %d", WSAGetLastError());
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        log_warning("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server = gethostbyname("myexternalip.com");
    if (server == NULL) {
        log_warning("No such host\n");
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    result =
        connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        log_warning("Connection failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    result = send(sockfd, request, strlen(request), 0);
    if (result == SOCKET_ERROR) {
        log_warning("Send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    int received;
    int header_ended = 0;
    memset(buffer, 0, HTML_BODY_SIZE);

    while ((received = recv(sockfd, buffer, HTML_BODY_SIZE - 1, 0)) > 0) {
        buffer[received] = '\0';
        if (!header_ended) {
            body_start = strstr(buffer, "\r\n\r\n");
            if (body_start) {
                body_start += 4;
                header_ended = 1;
                log_info("my ip : %s", body_start);
                break;
            }
        }
        memset(buffer, 0, HTML_BODY_SIZE);
    }

    closesocket(sockfd);
    WSACleanup();

    if (strlen(body_start) > 16) {
        log_warning("your ip is ipv6 %lu", strlen(body_start));
        return;
    }

    log_info("parse ip -> %s", body_start);
    memset(override_wan_address.String, 0, sizeof(IP_ADDRESS_STRING));
    memcpy(override_wan_address.String, body_start, sizeof(IP_ADDRESS_STRING));
    use_wan_address_override = true;
}

#undef HTML_BODY_SIZE
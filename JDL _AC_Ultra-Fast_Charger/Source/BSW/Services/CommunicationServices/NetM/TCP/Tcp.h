#ifndef _TCP_H_
#define _TCP_H_
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "Tcp_Cfg.h"


typedef enum {
    TCP_STATE_DISCONNECTED = 0,
    TCP_STATE_CONNECTING,
    TCP_STATE_CONNECTED
} tcp_state_t;

typedef struct
{
    uint8_t conn_id;        // Connection ID (usually 0-5)
    char remote_ip[16];     // Remote IP address (e.g., "192.168.1.100")
    uint16_t remote_port;   // Remote port
    tcp_state_t state;      // Connection state
} at_tcp_connection_t;

typedef struct
{
    at_tcp_connection_t *tcp_conn; // TCP connection
    SemaphoreHandle_t mutex;       // Mutex for thread safety
} tcp_manager_t;

typedef struct
{
    uint8_t socket_id;
    char ip[16];
    uint16_t port;
}tcp_paramater;

extern tcp_paramater Cloud_Tcp_Parameter[TCP_ID_MAXIMUM];

extern void tcp_init(void);
extern void tcp_destroy(tcp_id_enum manager_id);
extern uint8_t tcp_connect(tcp_id_enum manager_id, YeeCom_WorkingMode_E type);
extern void tcp_disconnect(tcp_id_enum manager_id);
extern void tcp_set_conn_state(tcp_id_enum manager_id, tcp_state_t state);
extern uint8_t tcp_send_data(tcp_id_enum manager_id, const uint8_t *data, size_t length);
#endif

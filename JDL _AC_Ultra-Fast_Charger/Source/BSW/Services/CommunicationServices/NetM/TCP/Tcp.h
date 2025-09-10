#ifndef _TCP_H_
#define _TCP_H_
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


typedef enum {
    TCP_STATE_DISCONNECTED = 0,
    TCP_STATE_CONNECTING,
    TCP_STATE_CONNECTED,
    TCP_STATE_SENDING,
    TCP_STATE_RECEIVING,
    TCP_STATE_ERROR
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

extern tcp_manager_t* tcp_create(void);
extern void tcp_destroy(tcp_manager_t *manager);
extern uint8_t tcp_connect(tcp_manager_t *manager);
extern void tcp_disconnect(tcp_manager_t *manager);
extern uint8_t tcp_send_data(tcp_manager_t *manager, const uint8_t *data, size_t length);
#endif

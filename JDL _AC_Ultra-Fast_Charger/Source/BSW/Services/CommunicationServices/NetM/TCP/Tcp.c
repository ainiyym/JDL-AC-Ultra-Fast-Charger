#include "Tcp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static uint8_t tcp_connect_count = 0U; // Connection count
static tcp_manager_t* tcp_manager[TCP_ID_MAXIMUM] = {NULL};

tcp_paramater Cloud_Tcp_Parameter[TCP_ID_MAXIMUM] =
{
    {
        .socket_id = TCP_ID_PROTOCOL_GAGA,
        .ip = CLOUD_PROTOCOL_GAGA_IP,
        .port = CLOUD_PROTOCOL_GAGA_PORT
    },
    {
        .socket_id = TCP_ID_PROTOCOL_SG,
        .ip = CLOUD_PROTOCOL_SG_IP,
        .port = CLOUD_PROTOCOL_SG_PORT
    }
};

/**
 * @brief Create a TCP connection manager
 * @return Returns a tcp_manager_t handler on success, NULL on failure
 */
static tcp_manager_t* tcp_create(void)
{
    tcp_manager_t *manager = pvPortMalloc(sizeof(tcp_manager_t));
    if (manager == NULL)
    {
        Tcp_Err("Error: Failed to create TCP manager\r\n");
        return NULL;
    }

    memset(manager, 0, sizeof(tcp_manager_t));

    manager->mutex = xSemaphoreCreateMutex();
    if (manager->mutex == NULL)
    {
        Tcp_Err("Error: Failed to create TCP manager mutex\r\n");
        vPortFree(manager);
        return NULL;
    }

    if (tcp_connect_count < TCP_ID_MAXIMUM)
    {
        tcp_connect_count++;
    }
    else
    {
        vSemaphoreDelete(manager->mutex);  // clean the mutex
        vPortFree(manager);
        Tcp_Err("Error: Maximum number of TCP connections reached\r\n");
        return NULL;
    }

    // create connections
    manager->tcp_conn = pvPortMalloc(sizeof(at_tcp_connection_t));
    if (manager->tcp_conn == NULL)
    {
        vSemaphoreDelete(manager->mutex);  // clean the mutex
        vPortFree(manager);
        Tcp_Err("Fault: heap is not enough to create connections. Fault!!!\r\n");
        return NULL;
    }
    memset(manager->tcp_conn, 0, sizeof(at_tcp_connection_t));
    manager->tcp_conn->state = TCP_STATE_DISCONNECTED;

    return manager;
}

// Destroy TCP manager
void tcp_destroy(tcp_id_enum manager_id)
{
    tcp_manager_t *manager = tcp_manager[manager_id];
    tcp_manager[manager_id] = NULL;

    if (manager == NULL)
        return;

    // Close connections
    tcp_disconnect(manager_id);

    // Delete mutex
    if (manager->mutex != NULL)
    {
        vSemaphoreDelete(manager->mutex);
    }

    // Free memory
    if (manager->tcp_conn != NULL)
    {
        vPortFree(manager->tcp_conn);
    }
    vPortFree(manager);
    tcp_connect_count--;
    Tcp_Trace("TCP manager destroyed\r\n");
}

// Create TCP connection
uint8_t tcp_connect(tcp_id_enum manager_id)
{
    tcp_manager_t *manager = tcp_manager[manager_id];
    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        // Set connection parameters
        at_tcp_connection_t *conn = manager->tcp_conn;
        strncpy(conn->remote_ip, manager->tcp_conn->remote_ip, sizeof(conn->remote_ip) - 1);
        conn->remote_port = manager->tcp_conn->remote_port;
        conn->state = TCP_STATE_CONNECTING;

        // Send AT connection command
        Tcp_At_Cmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_WORKING_MODE, NULL, conn->conn_id, YEECOM_WORKING_MQTT_ONENET, conn->remote_ip, conn->remote_port);
        // Query connection status
        Tcp_At_Cmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_WORKING_MODE, NULL, conn->conn_id);

        xSemaphoreGive(manager->mutex); 
        return 0;
    }
    else
    {
        Tcp_Err("Error: %s Failed to take TCP mutex\r\n", __func__);
        return 2;
    }
}

void tcp_disconnect(tcp_id_enum manager_id)
{
    tcp_manager_t *manager = tcp_manager[manager_id];
    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        at_tcp_connection_t *conn = manager->tcp_conn;
        conn->state = TCP_STATE_DISCONNECTED;

        xSemaphoreGive(manager->mutex);
    }
}

void tcp_set_conn_state(tcp_id_enum manager_id, tcp_state_t state)
{
    tcp_manager_t *manager = tcp_manager[manager_id];
    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        at_tcp_connection_t *conn = manager->tcp_conn;
        conn->state = state;

        xSemaphoreGive(manager->mutex);
    }
}

uint8_t tcp_send_data(tcp_id_enum manager_id, const uint8_t *data, size_t length)
{
    uint8_t ret = 0;
    tcp_manager_t *manager = tcp_manager[manager_id];

    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        at_tcp_connection_t *conn = manager->tcp_conn;

        if (conn->state != TCP_STATE_CONNECTED)
        {
            xSemaphoreGive(manager->mutex);
            return 1;
        }

        // Build the send command
        ret = Tcp_Data_Passthrough(conn->conn_id, data, length);

        xSemaphoreGive(manager->mutex);

        if (ret != 0)
        {
            Tcp_Err("Error: Failed to send data over TCP connection %d\r\n", conn->conn_id);
            return 2;
        }
        return 0;

    }
    else
    {
        Tcp_Err("Error: %s Failed to take TCP mutex\r\n", __func__);
        return 3;
    }
}

void tcp_init(void)
{
    for (int i = 0; i < TCP_ID_MAXIMUM; i++)
    {
        tcp_manager[i] = tcp_create();
        if (NULL != tcp_manager[i])
        {
            tcp_manager[i]->tcp_conn->conn_id = Cloud_Tcp_Parameter[i].socket_id;
            memcpy(tcp_manager[i]->tcp_conn->remote_ip, Cloud_Tcp_Parameter[i].ip, sizeof(tcp_manager[i]->tcp_conn->remote_ip));
            tcp_manager[i]->tcp_conn->remote_port = Cloud_Tcp_Parameter[i].port;
        }
        else
        {
            Tcp_Err("Error: Failed to create TCP manager for socket %d\r\n", Cloud_Tcp_Parameter[i].socket_id);
        }
    }
}

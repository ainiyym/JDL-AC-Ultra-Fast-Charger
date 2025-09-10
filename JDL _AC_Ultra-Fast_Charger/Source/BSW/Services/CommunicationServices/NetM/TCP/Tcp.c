#include "Tcp.h"
#include "Tcp_Cfg.h"

#define MAX_TCP_CONNECTIONS 3

static uint8_t tcp_connect_count = 0U; // Connection count

/**
 * @brief Create a TCP connection manager
 * @return Returns a tcp_manager_t handler on success, NULL on failure
 */
tcp_manager_t* tcp_create(void)
{
    tcp_manager_t *manager = pvPortMalloc(sizeof(tcp_manager_t));
    if (manager == NULL)
    {
        Tcp_Err("Error: Failed to create TCP manager\r\n");
        return NULL;
    }
    manager->mutex = xSemaphoreCreateMutex();
    if (manager->mutex == NULL)
    {
        Tcp_Err("Error: Failed to create TCP manager mutex\r\n");
        vPortFree(manager);
        return NULL;
    }

    memset(manager, 0, sizeof(tcp_manager_t));

    if (tcp_connect_count < MAX_TCP_CONNECTIONS)
    {
        tcp_connect_count++;
    }
    else
    {
        Tcp_Err("Error: Maximum number of TCP connections reached\r\n");
        return NULL;
    }

    // create connections
    manager->tcp_conn = pvPortMalloc(sizeof(at_tcp_connection_t));
    if (manager->tcp_conn == NULL)
    {
        Tcp_Err("Fault: heap is not enough to create connections. Fault!!!\r\n");
        return NULL;
    }
    manager->tcp_conn->state = TCP_STATE_DISCONNECTED;

    return manager;
}

// Destroy TCP manager
void tcp_destroy(tcp_manager_t *manager)
{
    if (manager == NULL)
        return;

    // Close connections
    tcp_disconnect(manager);

    // Delete mutex
    if (manager->mutex != NULL)
    {
        vSemaphoreDelete(manager->mutex);
    }

    // Free memory
    vPortFree(manager);
    tcp_connect_count--;
    Tcp_Trace("TCP manager destroyed\r\n");
}

// Create TCP connection
uint8_t tcp_connect(tcp_manager_t *manager)
{
    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        // Set connection parameters
        at_tcp_connection_t *conn = manager->tcp_conn;
        strncpy(conn->remote_ip, manager->tcp_conn->remote_ip, sizeof(conn->remote_ip) - 1);
        conn->remote_port = manager->tcp_conn->remote_port;
        conn->state = TCP_STATE_CONNECTING;

        // Send AT connection command
        Tcp_At_Cmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_WORKING_MODE, NULL, conn->conn_id, YEECOM_WORKING_TCP, conn->remote_ip, conn->remote_port);

        xSemaphoreGive(manager->mutex); 
        return 0;
    }
    else
    {
        Tcp_Err("Error: %s Failed to take TCP mutex\r\n", __func__);
        return 2;
    }
}

void tcp_disconnect(tcp_manager_t *manager)
{
    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        at_tcp_connection_t *conn = manager->tcp_conn;
        conn->state = TCP_STATE_DISCONNECTED;

        xSemaphoreGive(manager->mutex);
    }
}

uint8_t tcp_send_data(tcp_manager_t *manager, const uint8_t *data, size_t length)
{
    uint8_t ret = 0;

    if (xSemaphoreTake(manager->mutex, pdMS_TO_TICKS(500)) == pdTRUE)
    {
        at_tcp_connection_t *conn = manager->tcp_conn;

        if (conn->state != TCP_STATE_CONNECTED)
        {
            xSemaphoreGive(manager->mutex);
            return 1;
        }

        // 构建发送命令
        ret = Tcp_Data_Passthrough(conn->conn_id, data, length);

        xSemaphoreGive(manager->mutex);

        return ret;
    }
    else
    {
        Tcp_Err("Error: %s Failed to take TCP mutex\r\n", __func__);
        return 2;
    }
}

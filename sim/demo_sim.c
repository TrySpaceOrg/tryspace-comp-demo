#include "demo_sim.h"
#include "simulith_uart.h"

// Global state pointer for callback access
static demo_sim_state_t* g_state = NULL;

// UART port struct for Simulith
static uart_port_t g_uart_port = {0};

static void send_housekeeping(demo_sim_state_t* state)
{
    if (!state) return;
    uint8_t response[8];
    response[0] = DEMO_DEVICE_HDR_0;
    response[1] = DEMO_DEVICE_HDR_1;
    response[2] = (state->hk.DeviceCounter >> 8) & 0xFF;
    response[3] = state->hk.DeviceCounter & 0xFF;
    response[4] = (state->hk.DeviceConfig >> 8) & 0xFF;
    response[5] = state->hk.DeviceConfig & 0xFF;
    response[6] = DEMO_DEVICE_TRAILER_0;
    response[7] = DEMO_DEVICE_TRAILER_1;
    simulith_uart_send(&g_uart_port, response, sizeof(response));
}

static void send_demo_data(demo_sim_state_t* state)
{
    if (!state) return;
    uint8_t response[10];
    response[0] = DEMO_DEVICE_HDR_0;
    response[1] = DEMO_DEVICE_HDR_1;
    response[2] = (state->data.Chan1 >> 8) & 0xFF;
    response[3] = state->data.Chan1 & 0xFF;
    response[4] = (state->data.Chan2 >> 8) & 0xFF;
    response[5] = state->data.Chan2 & 0xFF;
    response[6] = (state->data.Chan3 >> 8) & 0xFF;
    response[7] = state->data.Chan3 & 0xFF;
    response[8] = DEMO_DEVICE_TRAILER_0;
    response[9] = DEMO_DEVICE_TRAILER_1;
    simulith_uart_send(&g_uart_port, response, sizeof(response));
}

static void handle_command(demo_sim_state_t* state, const uint8_t* data, size_t length)
{
    if (!state || !data || length <= DEMO_DEVICE_CMD_SIZE) 
    {  // Check for minimum command size
        printf("Invalid command parameters: state=%p, data=%p, length=%zu\n", 
               (void*)state, (const void*)data, length);
        return;
    }
    
    // Parse command fields from bytes (big-endian/network order)
    if (length < 9) 
    {
        printf("Command too short to parse fields.\n");
        return;
    }

    uint16_t header  = ((uint16_t) data[0] << 8) | data[1];
    uint16_t cmd_id  = ((uint16_t) data[2] << 8) | data[3];
    uint16_t payload = ((uint16_t) data[4] << 8) | data[5];
    uint16_t trailer = ((uint16_t) data[6] << 8) | data[7];

    // Validate header
    if (header != DEMO_DEVICE_HDR) 
    {
        printf("Invalid command header (0x%04X)\n", header);
        return;
    }

    // Validate trailer
    if (trailer != DEMO_DEVICE_TRAILER) 
    {
        printf("Invalid command trailer (0x%04X)\n", trailer);
        return;
    }

    // Echo command back
    printf("handle_command: Echo command back to UART: ID=%d, Payload=0x%08X\n", cmd_id, payload);
    simulith_uart_send(&g_uart_port, data, length);

    // Process command
    switch (cmd_id) 
    {
        case DEMO_DEVICE_NOOP_CMD:
            printf("Processing NOOP command\n");
            // Just echo the command back, which was already done
            break;

        case DEMO_DEVICE_REQ_HK_CMD:
            printf("Processing GET_HK command\n");
            send_housekeeping(state);
            break;

        case DEMO_DEVICE_REQ_DATA_CMD:
            printf("Processing GET_DATA command\n");
            send_demo_data(state);
            break;

        case DEMO_DEVICE_CFG_CMD:
            printf("Processing SET_CONFIG command with payload 0x%08X\n", payload);
            state->hk.DeviceConfig = payload;
            break;

        default:
            printf("Unknown command ID: %d\n", cmd_id);
            break;
    }

    // Increment command counter
    state->hk.DeviceCounter++;
}

static void on_tick(uint64_t tick_time_ns)
{
    int bytes;
    uint8_t data[256];

    if (!g_state) return;
    
    // Convert nanoseconds to seconds
    double current_time = tick_time_ns / 1e9;
    
    // Update demo data at the specified rate
    if (current_time - g_state->last_update_time >= (1.0 / DEMO_SIM_UPDATE_RATE_HZ)) 
    {
        // Update simulated data based on command counter
        g_state->data.Chan1 = (uint16_t)(g_state->hk.DeviceCounter * 1);
        g_state->data.Chan2 = (uint16_t)(g_state->hk.DeviceCounter * 2);
        g_state->data.Chan3 = (uint16_t)(g_state->hk.DeviceCounter * 3);
        g_state->last_update_time = current_time;
    }

    // Process UART
    bytes = simulith_uart_available(&g_uart_port);
    if (bytes > 0)
    {
        // Read UART
        bytes = simulith_uart_receive(&g_uart_port, data, sizeof(data));

        printf("Received %d bytes from UART\n", bytes);
        for(int i = 0; i < bytes; i++) 
        {
            printf("%02X ", data[i]);
        }
        printf("\n");

        // Process the command
        handle_command(g_state, data, bytes);
    }
}

int demo_sim_init(demo_sim_state_t* state)
{
    if (!state) return DEMO_SIM_ERROR;

    // Initialize state
    memset(state, 0, sizeof(demo_sim_state_t));

    // Set global state pointer
    g_state = state;

    // Wait a second for the Simulith server to start up
    sleep(1);

    // Initialize Simulith client
    if (simulith_client_init(CLIENT_PUB_ADDR, CLIENT_REP_ADDR, "tryspace-comp-demo-sim", INTERVAL_NS) != 0) 
    {
        printf("Failed to initialize Simulith client\n");
        return DEMO_SIM_ERROR;
    }

    // Handshake with Simulith server
    if (simulith_client_handshake() != 0) 
    {
        printf("Failed to handshake with Simulith server\n");
        simulith_client_shutdown();
        return DEMO_SIM_ERROR;
    }

    // Initialize UART port struct for Simulith (server/bind)
    memset(&g_uart_port, 0, sizeof(g_uart_port));
    snprintf(g_uart_port.name, sizeof(g_uart_port.name), "demo_sim_uart");
    snprintf(g_uart_port.address, sizeof(g_uart_port.address), "tcp://*:%d", SIMULITH_UART_BASE_PORT + DEMO_CFG_HANDLE);
    g_uart_port.is_server = 1; // Always server/bind for the simulator

    int uart_result = simulith_uart_init(&g_uart_port);
    if (uart_result < 0) 
    {
        printf("Failed to initialize Simulith UART server\n");
        simulith_client_shutdown();
        return DEMO_SIM_ERROR;
    }

    // Initialize time provider
    state->time_handle = simulith_time_init();
    if (!state->time_handle) 
    {
        printf("Failed to initialize time provider\n");
        simulith_uart_close(&g_uart_port);
        simulith_client_shutdown();
        return DEMO_SIM_ERROR;
    }

    // Initialize default values
    state->hk.DeviceCounter = 0;
    state->hk.DeviceConfig = 0;
    state->data.Chan1 = 0;
    state->data.Chan2 = 0;
    state->data.Chan3 = 0;
    state->last_update_time = simulith_time_get(state->time_handle);

    printf("Sample simulator initialized successfully as UART server on %s\n", g_uart_port.address);
    printf("Waiting for commands...\n");
    return DEMO_SIM_SUCCESS;
}

void demo_sim_cleanup(demo_sim_state_t* state)
{
    if (!state) return;

    g_state = NULL;  // Clear global state pointer
    simulith_uart_close(&g_uart_port);

    if (state->time_handle) 
    {
        simulith_time_cleanup(state->time_handle);
        state->time_handle = NULL;
    }
    simulith_client_shutdown();
}

int main(int argc, char* argv[])
{
    demo_sim_state_t state;
    
    if (demo_sim_init(&state) != DEMO_SIM_SUCCESS) 
    {
        printf("Failed to initialize demo simulator\n");
        return 1;
    }
    
    printf("Sample simulator running. Press Ctrl+C to exit.\n");
    
    // Run the client loop with our tick callback
    simulith_client_run_loop(on_tick);
    
    demo_sim_cleanup(&state);
    return 0;
} 
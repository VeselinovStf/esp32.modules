/* HTTP Restful API Server

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_chip_info.h"
#include "esp_random.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "cJSON.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"

#include "gbaby_esp.h"
#include "shift_reley_module.h"
#include "gbaby_dht.h"

// #define LP 4  // SH/LD PIN
// #define CP 2  // CLK PIN
// #define DP 15 // QH PIN

#define POWER_EXCHAUST_FAN_PIN 27
#define PUMP_1_PIN 33
#define PUMP_2_PIN 12
#define EAR_PUMP_PIN 13
#define LIGHT_PIN 25
#define EXCHAUST_FAN_PIN 15
#define PUMP_3_PIN 4
#define PUMP_4_PIN 22
#define PUMP_5_PIN 17

#define DHT_PLANTS_PIN 21
#define DHT_ELECTRONICS_PIN 32
#define DHT_TOTAL_SENSORS = 2

static const dht_sensor_type_t dht_electronics_sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_electronics_sensor_gpio = DHT_ELECTRONICS_PIN;

static const dht_sensor_type_t dht_plant_sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_plants_sensor_gpio = DHT_PLANTS_PIN;

#define WATER_LEVEL_POWER_PIN 2
#define WATER_LEVEL_RESERVOUR_TANK 34
#define WATER_LEVEL_PLANTS_TANK 35

static const char *REST_TAG = "esp-rest";
#define REST_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                 \
    {                                                                                  \
        if (!(a))                                                                      \
        {                                                                              \
            ESP_LOGE(REST_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                             \
        }                                                                              \
    } while (0)

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context
{
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION(filepath, ".html"))
    {
        type = "text/html";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".js"))
    {
        type = "application/javascript";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".css"))
    {
        type = "text/css";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".png"))
    {
        type = "image/png";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".ico"))
    {
        type = "image/x-icon";
    }
    else if (CHECK_FILE_EXTENSION(filepath, ".svg"))
    {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];

    rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    strlcpy(filepath, rest_context->base_path, sizeof(filepath));
    if (req->uri[strlen(req->uri) - 1] == '/')
    {
        strlcat(filepath, "/index.html", sizeof(filepath));
    }
    else
    {
        strlcat(filepath, req->uri, sizeof(filepath));
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1)
    {
        ESP_LOGE(REST_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = rest_context->scratch;
    ssize_t read_bytes;
    do
    {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1)
        {
            ESP_LOGE(REST_TAG, "Failed to read file : %s", filepath);
        }
        else if (read_bytes > 0)
        {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
            {
                close(fd);
                ESP_LOGE(REST_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGI(REST_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* Simple handler for light brightness control */
// static esp_err_t water_level_get_handler(httpd_req_t *req)
// {

//      digitalWrite(WATER_LEVEL_POWER_PIN, HIGH);

//     digitalWrite(WATER_LEVEL_POWER_PIN, LOW); // turn the sensor OFF

//     httpd_resp_set_type(req, "application/json");
//     cJSON *root = cJSON_CreateObject();
//     cJSON_AddNumberToObject(root, "plants", water_level_plants);
//     cJSON_AddNumberToObject(root, "reservour", water_level_resorvour);

//     const char *sys_info = cJSON_Print(root);
//     httpd_resp_sendstr(req, sys_info);
//     free((void *)sys_info);
//     cJSON_Delete(root);

//     return ESP_OK;

// }

// Define a structure to hold pin numbers
typedef struct
{
    gpio_num_t pin;
} RelayConfig;

// Define an array of structures with GPIO pin numbers
RelayConfig relayPins[] = {
    {PUMP_1_PIN},             // Example pin, replace with your actual GPIO pin numbers
    {PUMP_2_PIN},             // Add more pins as needed
    {POWER_EXCHAUST_FAN_PIN}, // Add more pins as needed
    {EAR_PUMP_PIN},           // Add more pins as needed
    {LIGHT_PIN},              // Add more pins as needed
    {EXCHAUST_FAN_PIN},       // Add more pins as needed
    {PUMP_3_PIN},             // Add more pins as needed
    {PUMP_4_PIN},             // Add more pins as needed
    {PUMP_5_PIN},             // Add more pins as needed
    // ...
};

void setRelayState(int index, uint8_t bitValue)
{
    // Ensure that the index is within the valid range
    if (index < sizeof(relayPins) / sizeof(relayPins[0]))
    {
        ESP_LOGI(REST_TAG, "PIN: %d :VALUE: %d", relayPins[index].pin, bitValue);
        // Set the GPIO pin based on the bit value
        gpio_set_level(relayPins[index].pin, bitValue);
    }
}

/* Simple handler for light brightness control */
static esp_err_t reley_post_handler(httpd_req_t *req)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *)(req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE)
    {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len)
    {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0)
        {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);

    ESP_LOGI(REST_TAG, "Reley Control: value = %s", cJSON_GetObjectItem(root, "value")->valuestring);

    // uint8_t movingBytes = strtol(cJSON_GetObjectItem(root, "value")->valuestring, NULL, 2);

    // // Loop through each bit of movingBytes
    // for (int i = 0; i < 8; i++)
    char *valueString = cJSON_GetObjectItem(root, "value")->valuestring;
    uint8_t movingBytes = strtol(valueString, NULL, 2);

    size_t movingBytesLength = strlen(valueString);

    size_t totalRelayPins = sizeof(relayPins) / sizeof(relayPins[0]);

    // Loop through each bit of movingBytes and check if i is within the range of totalRelayPins
    for (size_t i = 0; i < movingBytesLength && i < totalRelayPins; i++)
    {
        // Extract the i-th bit value using bitwise AND
        uint8_t bitValue = (movingBytes >> i) & 0x01;

        // Set the relay state based on the bit value and index
        setRelayState(i, bitValue);

        // You may introduce a delay here if needed
        // vTaskDelay(pdMS_TO_TICKS(100)); // Example: 100 ms delay
    }

    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}
/* Simple handler for getting system handler */
static esp_err_t system_info_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    cJSON_AddStringToObject(root, "version", IDF_VER);
    cJSON_AddNumberToObject(root, "cores", chip_info.cores);
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

/* Simple handler for getting temperature data */
static esp_err_t get_dht_data_handler(httpd_req_t *req)
{

    int16_t temperature = 0;
    int16_t humidity = 0;

    if (dht_read_data(dht_electronics_sensor_type, dht_electronics_sensor_gpio, &humidity, &temperature) != ESP_OK)
    {
        printf("ELECTRONICS:DHT: Could not read data from sensor\n");
    }

    int16_t p_temperature = 0;
    int16_t p_humidity = 0;

    if (dht_read_data(dht_plant_sensor_type, dht_plants_sensor_gpio, &p_humidity, &p_temperature) != ESP_OK)
    {
        printf("PLANT:DHT: Could not read data from sensor\n");
    }

    vTaskDelay(200);

    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "dht_el_t", temperature / 10);
    cJSON_AddNumberToObject(root, "dht_el_h", humidity / 10);
    cJSON_AddNumberToObject(root, "dht_pl_t", p_temperature / 10);
    cJSON_AddNumberToObject(root, "dht_pl_h", p_humidity / 10);

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}

esp_err_t start_rest_server(const char *base_path)
{
    REST_CHECK(base_path, "wrong base path", err);
    rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));
    REST_CHECK(rest_context, "No memory for rest context", err);
    strlcpy(rest_context->base_path, base_path, sizeof(rest_context->base_path));

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(REST_TAG, "Starting HTTP Server");
    REST_CHECK(httpd_start(&server, &config) == ESP_OK, "Start server failed", err_start);

    /* URI handler for fetching system info */
    httpd_uri_t system_info_get_uri = {
        .uri = "/api/v1/system/info",
        .method = HTTP_GET,
        .handler = system_info_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &system_info_get_uri);

    /* URI handler for fetching temperature data */
    httpd_uri_t dht_data_get_uri = {
        .uri = "/api/v1/dht",
        .method = HTTP_GET,
        .handler = get_dht_data_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &dht_data_get_uri);

    /* URI handler for light brightness control */
    // httpd_uri_t water_level_get_uri = {
    //     .uri = "/api/v1/water-level",
    //     .method = HTTP_GET,
    //     .handler = water_level_get_handler,
    //     .user_ctx = rest_context};
    // httpd_register_uri_handler(server, &water_level_get_uri);

    /* URI handler for light brightness control */
    httpd_uri_t reley_post_uri = {
        .uri = "/api/v1/reley",
        .method = HTTP_POST,
        .handler = reley_post_handler,
        .user_ctx = rest_context};

    httpd_register_uri_handler(server, &reley_post_uri);

    /* URI handler for getting web server files */
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = rest_context};
    httpd_register_uri_handler(server, &common_get_uri);

    // /* URI handler for light brightness control */
    // httpd_uri_t releyNine_post_uri = {
    //     .uri = "/api/v1/r9",
    //     .method = HTTP_POST,
    //     .handler = releyNine_post_post_handler,
    //     .user_ctx = rest_context};
    // httpd_register_uri_handler(server, &releyNine_post_uri);

    // /* URI handler for light brightness control */
    // httpd_uri_t releyOne_post_uri = {
    //     .uri = "/api/v1/r1",
    //     .method = HTTP_POST,
    //     .handler = releyOne_post_post_handler,
    //     .user_ctx = rest_context};
    // httpd_register_uri_handler(server, &releyOne_post_uri);

    // /* URI handler for light brightness control */
    // httpd_uri_t releyTwo_post_uri = {
    //     .uri = "/api/v1/r2",
    //     .method = HTTP_POST,
    //     .handler = releyTwo_post_post_handler,
    //     .user_ctx = rest_context};
    // httpd_register_uri_handler(server, &releyTwo_post_uri);

    return ESP_OK;
err_start:
    free(rest_context);
err:
    return ESP_FAIL;
}

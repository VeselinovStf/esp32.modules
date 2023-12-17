#include <stdio.h>
/*
    This example opens Sqlite3 databases from SD Card and
    retrieves data from them.
    Before running please copy following files to SD Card:
    examples/sqlite3_sdmmc/data/mdr512.db
    examples/sqlite3_sdmmc/data/census2000names.db
    Connections:
     * SD Card | ESP32
     *  DAT2       -
     *  DAT3       SS (D5)
     *  CMD        MOSI (D23)
     *  VSS        GND
     *  VDD        3.3V
     *  CLK        SCK (D18)
     *  DAT0       MISO (D19)
     *  DAT1       -
*/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <FS.h>
#include <esp_log.h>

static const char* TAG = "[SQL_DB]";

const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   ESP_LOGI(TAG, "%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       ESP_LOGI(TAG,"%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   return 0;
}

int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       ESP_LOGE(TAG,"Can't open database: %s", sqlite3_errmsg(*db));
       return rc;
   } else {
       ESP_LOGI(TAG,"Opened database successfully");
   }
   return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   ESP_LOGI(TAG, "SQL: %s",sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       ESP_LOGE(TAG,"SQL error: %s", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       ESP_LOGI(TAG,"Operation done successfully");
   }
   ESP_LOGI(TAG,"Time taken: %d",micros()-start);
   
   return rc;
}


void app_main(void)
{
sqlite3 *db1;
   sqlite3 *db2;
   char *zErrMsg = 0;
   int rc;

   SPI.begin();
   SD.begin();

   sqlite3_initialize();

   // Open database 1
   if (openDb("/sd/census2000names.db", &db1))
       return;
   if (openDb("/sd/mdr512.db", &db2))
       return;

   rc = db_exec(db1, "Select * from surnames where name = 'MICHELLE'");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   rc = db_exec(db2, "Select * from domain_rank where domain between 'google.com' and 'google.com.z'");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   rc = db_exec(db1, "Select * from surnames where name = 'SPRINGER'");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   rc = db_exec(db2, "Select * from domain_rank where domain = 'zoho.com'");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

   sqlite3_close(db1);
   sqlite3_close(db2);
}
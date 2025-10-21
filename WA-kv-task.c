#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_KEY_LEN 50
#define MAX_VALUE_LEN 100
#define MAX_ITEMS 100

// Structure for key-value pair
typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} KeyValue;

// In-memory key-value store
KeyValue store[MAX_ITEMS];
int store_size = 0;

// Function to get current timestamp as string
char* current_timestamp() {
    static char buffer[30];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

// Function to log operation to WAL file
void log_operation(const char *operation, const char *key, const char *value) {
    FILE *log_file = fopen("wal.log", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    fprintf(log_file, "[%s] %s: Key=%s, Value=%s\n",
            current_timestamp(), operation, key, value ? value : "NULL");
    fclose(log_file);
}

// Insert or Update a key-value pair
void insert_or_update(const char *key, const char *value) {
    // Check if key exists (for update)
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            log_operation("UPDATE", key, value);
            strcpy(store[i].value, value);
            printf("Updated key '%s' with value '%s'\n", key, value);
            return;
        }
    }

    // Insert new key
    if (store_size < MAX_ITEMS) {
        log_operation("INSERT", key, value);
        strcpy(store[store_size].key, key);
        strcpy(store[store_size].value, value);
        store_size++;
        printf("Inserted key '%s' with value '%s'\n", key, value);
    } else {
        printf("Store full! Cannot insert more data.\n");
    }
}

// Delete a key-value pair
void delete_key(const char *key) {
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            log_operation("DELETE", key, NULL);

            // Shift remaining items
            for (int j = i; j < store_size - 1; j++) {
                store[j] = store[j + 1];
            }
            store_size--;
            printf("Deleted key '%s'\n", key);
            return;
        }
    }
    printf("Key '%s' not found.\n", key);
}

// Display all key-value pairs
void display_store() {
    printf("\nCurrent In-Memory Store:\n");
    for (int i = 0; i < store_size; i++) {
        printf("%s => %s\n", store[i].key, store[i].value);
    }
}

int main() {
    int choice;
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];

    while (1) {
        printf("\n==== WAL Key-Value Store ====\n");
        printf("1. Insert/Update\n");
        printf("2. Delete\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%s", key);
                printf("Enter value: ");
                scanf("%s", value);
                insert_or_update(key, value);
                break;
            case 2:
                printf("Enter key to delete: ");
                scanf("%s", key);
                delete_key(key);
                break;
            case 3:
                display_store();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}

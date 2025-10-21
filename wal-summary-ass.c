#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 100
#define MAX_KEY_LEN 50
#define MAX_LINE_LEN 200

// Function to check if a key is already in the list
int key_exists(char keys[][MAX_KEY_LEN], int count, const char *key) {
    for (int i = 0; i < count; i++) {
        if (strcmp(keys[i], key) == 0)
            return 1; // key already exists
    }
    return 0;
}

int main() {
    FILE *log_file = fopen("wal.log", "r");
    if (log_file == NULL) {
        perror("Error opening wal.log");
        return 1;
    }

    char line[MAX_LINE_LEN];
    char keys[MAX_KEYS][MAX_KEY_LEN];
    int key_count = 0;

    int insert_count = 0, update_count = 0, delete_count = 0;

    // Read the WAL file line by line
    while (fgets(line, sizeof(line), log_file)) {
        // Example line format:
        // [2025-10-21 11:02:15] INSERT: Key=name, Value=suchitra

        if (strstr(line, "INSERT:"))
            insert_count++;
        else if (strstr(line, "UPDATE:"))
            update_count++;
        else if (strstr(line, "DELETE:"))
            delete_count++;

        // Extract the key from the line
        char *key_start = strstr(line, "Key=");
        if (key_start) {
            key_start += 4; // move pointer after "Key="
            char *comma = strchr(key_start, ',');
            if (comma) *comma = '\0'; // replace ',' with '\0' to isolate the key

            if (!key_exists(keys, key_count, key_start)) {
                strcpy(keys[key_count++], key_start);
            }
        }
    }

    fclose(log_file);

    // Print summary
    printf("=== WAL Log Summary ===\n");
    printf("Insertions: %d\n", insert_count);
    printf("Updates   : %d\n", update_count);
    printf("Deletions : %d\n", delete_count);
    printf("\nUnique Keys Affected (%d):\n", key_count);
    for (int i = 0; i < key_count; i++) {
        printf(" - %s\n", keys[i]);
    }

    return 0;
}

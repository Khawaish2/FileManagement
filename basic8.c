#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#define MAX_FILENAME 256


// Function to validate numeric input
int get_valid_choice() {
    char input[MAX_FILENAME];
    int valid = 0;
    int choice = 0;

    while (!valid) {
        printf("Enter your choice: ");
        fgets(input, MAX_FILENAME, stdin);

        // Check if the input is numeric
        valid = 1; // Assume valid until proven otherwise
        for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
            if (!isdigit(input[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            choice = atoi(input); // Convert string to integer
            if (choice >= 1 && choice <= 8) {
                return choice;
            } else {
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                valid = 0;
            }
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }

    return choice; // This won't be reached but is required to silence warnings
}

// Function prototypes
void create_file(const char *filename , const char *directory);
void delete_file(const char *filename , const char *directory );
void search_file(const char *directory, const char *filename);
void list_files(const char *directory);
void encrypt_file(const char *filename);
void decrypt_file(const char *filename);
void create_directory(const char *directory);
int main() {
    int choice;
    char filename[MAX_FILENAME];
    char directory[MAX_FILENAME];

    do {
        printf("\nFile Management System:\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Search File\n");
        printf("4. List Files\n");
        printf("5. Create Directory\n");
        printf("6. Encrypt File\n");
        printf("7. Decrypt File\n");
        printf("8. Exit\n");
        
        
         choice = get_valid_choice();
         
        switch (choice) {
           case 1:
    printf("Enter the directory to create the file in: ");
    fgets(directory, MAX_FILENAME, stdin);
    directory[strcspn(directory, "\n")] = '\0'; // Remove newline

    printf("Enter the filename to create: ");
    fgets(filename, MAX_FILENAME, stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove newline

    create_file(directory, filename);
  break;
            case 2:
                
               printf("Enter the directory of the file to delete: ");
    fgets(directory, MAX_FILENAME, stdin);
    directory[strcspn(directory, "\n")] = '\0'; // Remove newline

    printf("Enter the filename to delete: ");
    fgets(filename, MAX_FILENAME, stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove newline

    delete_file(directory, filename);
    break;
            case 3:
                printf("Enter the directory to search in: ");
                fgets(directory, MAX_FILENAME, stdin);
                directory[strcspn(directory, "\n")] = '\0';
                printf("Enter the filename to search: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                search_file(directory, filename);
                break;
            case 4:
                printf("Enter the directory to list files: ");
                fgets(directory, MAX_FILENAME, stdin);
                directory[strcspn(directory, "\n")] = '\0';
                list_files(directory);
                break;
           case 5:
                printf("Enter the full path of the directory to create: ");
                fgets(directory, MAX_FILENAME, stdin);
                directory[strcspn(directory, "\n")] = '\0';
                create_directory(directory);
                break;               
            case 6:
                printf("Enter the filename to encrypt: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                encrypt_file(filename);
                break;
            case 7:
                printf("Enter the filename to decrypt: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                decrypt_file(filename);
                break;
            case 8:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

// Function to create a file


void create_file(const char *directory, const char *filename) {
    // Check if the directory exists
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    closedir(dir);

    // Construct full file path
    char full_path[MAX_FILENAME];
    sprintf(full_path, "%s/%s", directory, filename);

    // Create the file
    FILE *file = fopen(full_path, "w");
    if (file == NULL) {
        perror("Error creating file");
        return;
    }

    printf("File '%s' created successfully in directory '%s'.\n", filename, directory);
    fclose(file);
}

// Function to delete a file
void delete_file(const char *directory, const char *filename) {
    // Check if the directory exists
    struct stat st = {0};
    if (stat(directory, &st) == -1 || !S_ISDIR(st.st_mode)) {
        printf("Error: Directory '%s' does not exist or is not valid.\n", directory);
        return;
    }

    // Construct the full file path
    char full_path[MAX_FILENAME];
    snprintf(full_path, sizeof(full_path), "%s/%s", directory, filename);

    // Check if the file exists
    if (stat(full_path, &st) == -1 || !S_ISREG(st.st_mode)) {
        printf("Error: File '%s' does not exist in directory '%s'.\n", filename, directory);
        return;
    }

    // Attempt to delete the file
    if (remove(full_path) == 0) {
        printf("File '%s' successfully deleted from directory '%s'.\n", filename, directory);
    } else {
        perror("Error deleting file");
    }
}

// Function to search for a file in a directory
void search_file(const char *directory, const char *filename) {
    DIR *dir = opendir(directory);
    struct dirent *entry;

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    int found = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, filename) == 0) {
            found = 1;
            break;
        }
    }

    closedir(dir);

    if (found) {
        printf("File '%s' found in directory '%s'.\n", filename, directory);
    } else {
        printf("File '%s' not found in directory '%s'.\n", filename, directory);
    }
}

// Function to list files in a directory
void list_files(const char *directory) {
    DIR *dir = opendir(directory);
    struct dirent *entry;

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    printf("Files in directory '%s':\n", directory);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') { // Skip hidden files
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}
// Function to create a new directory
void create_directory(const char *directory) {
    if (mkdir(directory, 0777) == 0) {
        printf("Directory '%s' created successfully.\n", directory);
    } else {
        perror("Error creating directory");
    }
}

// Function to encrypt a file (simple XOR encryption)
void encrypt_file(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int key = 42; // Simple key for XOR encryption
    char ch;
    long pos;

    printf("Encrypting file '%s'...\n", filename);

    while ((pos = ftell(file)) != EOF && fread(&ch, 1, 1, file) == 1) {
        ch ^= key; // XOR operation
        fseek(file, pos, SEEK_SET);
        fwrite(&ch, 1, 1, file);
    }

    fclose(file);
    printf("File '%s' encrypted successfully.\n", filename);
}

// Function to decrypt a file (simple XOR decryption)
void decrypt_file(const char *filename) {
    encrypt_file(filename); // Decrypt is the same as encrypt for XOR
    printf("File '%s' decrypted successfully.\n", filename);
}


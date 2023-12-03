#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000
#define MAX_FIELD_LENGTH 100

int login(const char *userFilename, const char *username, const char *password) {
    FILE *file = fopen(userFilename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    char *token;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Remove the newline character, if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char lineCopy[MAX_LINE_LENGTH];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, ",");

        if (strcmp(token, username) == 0) {
            token = strtok(NULL, ",");
            
            // Remove the newline character from the password token, if present
            len = strlen(token);
            if (len > 0 && token[len - 1] == '\n') {
                token[len - 1] = '\0';
            }

            if (strcmp(token, password) == 0) {
                fclose(file);
                printf("Login successful\n");
                return 1; 
            }
        }
    }

    fclose(file);
    printf("Login unsuccessful\n");
    return 0; 
}


int main() {
    const char *productFilename = "sample_products.csv"; 
    const char *userFilename = "sample_users.csv"; 
    char loginUsername[MAX_FIELD_LENGTH];
    char loginPassword[MAX_FIELD_LENGTH];

    printf("enter username : ");
    scanf("%s",loginUsername);
    printf("enter password : ");
    scanf("%s",loginPassword);
    login(userFilename,loginUsername,loginPassword);
    printf("user : %s\n",loginUsername);
    printf("pass : %s",loginPassword);

    return 0;
}







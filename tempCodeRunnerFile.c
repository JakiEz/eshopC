#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define USER_FILE "users.csv"
#define PRODUCT_FILE "products.csv"

#define ADMIN 1
#define EMPLOYEE 2

void registerUser();
int loginUser();
void addItem();
void purchaseItem();
void viewProducts();
void deleteProduct();

int loggedInUserRole = 0;

int main() {
    int choice;
    int loggedIn = 0;

    while (1) {
        if (!loggedIn) {
            printf("1. Register\n");
            printf("2. Login\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    if (loginUser()) {
                        printf("Login successful!\n");
                        loggedIn = 1;
                    } else {
                        printf("Login failed. Invalid username or password.\n");
                    }
                    break;
                case 3:
                    printf("Exiting program. Goodbye!\n");
                    exit(0);
                default:
                    printf("Invalid choice. Please enter a valid option.\n");
            }
        } else {
            if (loggedInUserRole == ADMIN) {
                printf("1. Add Item\n");
                printf("2. Delete Item\n");
                printf("3. View Products\n");
                printf("4. Logout\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        addItem();
                        break;
                    case 3:
                        viewProducts();
                        break;
                    case 4:
                        loggedIn = 0;
                        printf("Logged out successfully!\n");
                        break;
                    case 2:
                        deleteProduct();
                        break;
                    default:
                        printf("Invalid choice. Please enter a valid option.\n");
                }
            } else if (loggedInUserRole == EMPLOYEE) {
                printf("1. Purchase Item\n");
                printf("2. Logout\n");
                printf("3. View Products\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        purchaseItem();
                        break;
                    case 2:
                        loggedIn = 0;
                        printf("Logged out successfully!\n");
                        break;
                    case 3:
                        viewProducts();
                        break;
                    default:
                        printf("Invalid choice. Please enter a valid option.\n");
                }
            }
        }
    }

    return 0;
}

void registerUser() {
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    int role;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    
    printf("Choose role (1 for Admin, 2 for Employee): ");
    scanf("%d", &role);

    FILE *file = fopen(USER_FILE, "a");
    if (file == NULL) {
        printf("Error opening user file.\n");
        exit(1);
    }

    fprintf(file, "%s,%s,%d\n", username, password, role);
    fclose(file);

    printf("Registration successful!\n");
}

int loginUser() {
    char username[MAX_SIZE];
    char password[MAX_SIZE];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("Error opening user file.\n");
        exit(1);
    }

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_SIZE];
        char storedPassword[MAX_SIZE];
        int storedRole;
        sscanf(line, "%[^,],%[^,],%d", storedUsername, storedPassword, &storedRole);

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            loggedInUserRole = storedRole;
            fclose(file);
            return 1; // Login successful
        }
    }

    fclose(file);
    return 0; // Login failed
}

void addItem() {
    char name[MAX_SIZE];
    float price;
    int quantity;

    printf("Enter product name: ");
    scanf("%s", name);
    printf("Enter price: ");
    scanf("%f", &price);
    printf("Enter quantity: ");
    scanf("%d", &quantity);

    FILE *file = fopen(PRODUCT_FILE, "a");
    if (file == NULL) {
        printf("Error opening product file.\n");
        exit(1);
    }

    fprintf(file, "%s,%.2f,%d\n", name, price, quantity);
    fclose(file);

    printf("Item added successfully!\n");
}

void purchaseItem() {
    float totalCost = 0.0;
    char name[MAX_SIZE];
    int quantity;
    float money;
    float change = 0.0;
    int finishTransaction = 0;

    while (!finishTransaction) {
        printf("Enter product name (or type 'finish' to complete the purchase): ");
        scanf("%s", name);

        if (strcmp(name, "finish") == 0) {
            finishTransaction = 1;
        } else {
            FILE *file = fopen(PRODUCT_FILE, "r");
            if (file == NULL) {
                printf("Error opening product file.\n");
                exit(1);
            }

            char line[MAX_SIZE];
            int found = 0;
            float totalPrice = 0.0;

            while (fgets(line, sizeof(line), file)) {
                char storedName[MAX_SIZE];
                float price;
                int storedQuantity;
                sscanf(line, "%[^,],%f,%d", storedName, &price, &storedQuantity);

                if (strcmp(name, storedName) == 0) {
                    found = 1;
                    printf("Enter quantity to purchase for %s: ", name);
                    scanf("%d", &quantity);

                    if (storedQuantity >= quantity) {
                        totalPrice = quantity * price;
                        totalCost += totalPrice;
                        printf("Added %d %s to the purchase. Current total cost: %.2f\n", quantity, name, totalCost);

                        // Update the quantity in the product file
                        FILE *tempFile = fopen("temp.csv", "w");
                        rewind(file);
                        while (fgets(line, sizeof(line), file)) {
                            sscanf(line, "%[^,],%f,%d", storedName, &price, &storedQuantity);
                            if (strcmp(name, storedName) == 0) {
                                storedQuantity -= quantity;
                            }
                            fprintf(tempFile, "%s,%.2f,%d\n", storedName, price, storedQuantity);
                        }
                        fclose(file);
                        fclose(tempFile);
                        remove(PRODUCT_FILE);
                        rename("temp.csv", PRODUCT_FILE);
                    } else {
                        printf("Not enough stock. Please choose a lower quantity.\n");
                    }
                }
            }

            fclose(file);

            if (!found) {
                printf("Product not found. Please enter a valid product name.\n");
            }
        }
    }

    printf("Total cost for the entire purchase: %.2f\n", totalCost);

    printf("Enter money: ");
    scanf("%f", &money);

    if (money >= totalCost) {
        change = money - totalCost;
        printf("Change: %.2f\n", change);
        printf("Purchase successful!\n");
    } else {
        printf("Not enough money. Purchase failed.\n");
    }
}


void viewProducts() {
    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        printf("Error opening product file.\n");
        exit(1);
    }

    printf("\nProduct List:\n");
    printf("=======================================\n");
    printf("Name\t\tPrice\t\tQuantity\n");
    printf("=======================================\n");

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_SIZE];
        float price;
        int quantity;
        sscanf(line, "%[^,],%f,%d", name, &price, &quantity);
        printf("%-15s%-15.2f%-15d\n", name, price, quantity);
    }

    printf("=======================================\n");
    
    fclose(file);
}
void deleteProduct() {
    char name[MAX_SIZE];

    printf("Enter the name of the product to delete: ");
    scanf("%s", name);

    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        printf("Error opening product file.\n");
        exit(1);
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        fclose(file);
        printf("Error opening temporary file.\n");
        exit(1);
    }

    char line[MAX_SIZE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char storedName[MAX_SIZE];
        float price;
        int quantity;
        sscanf(line, "%[^,],%f,%d", storedName, &price, &quantity);

        if (strcmp(name, storedName) == 0) {
            found = 1;
            printf("Product found and deleted successfully!\n");
        } else {
            fprintf(tempFile, "%s,%.2f,%d\n", storedName, price, quantity);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(PRODUCT_FILE);
    rename("temp.csv", PRODUCT_FILE);

    if (!found) {
        printf("Product not found. Deletion failed.\n");
    }
}


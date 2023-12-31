#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100
#define USER_FILE "users.csv"
#define PRODUCT_FILE "products.csv"
#define REVENUE_FILE "revenue.csv"

#define ADMIN 1
#define EMPLOYEE 2

void registerUser();
int loginUser();
void addItem();
void purchaseItem();
void viewProducts();
void updateProduct();
void deleteProduct();
float checkTotalRevenue();
void updateRevenueFile(float amount);

int loggedInUserRole = 0;
float totalRevenue = 0.0;

int main() {
    int choice;
    int loggedIn = 0;

    while (1) {
        if (!loggedIn) {
            printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
            printf("%*s*      SNEAKER SHOP     *\n", (80 + 28) / 2, "");
            printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");

            printf("\033[1;31m1. Register\n");
            printf("2. Login\n");
            printf("3. Exit\n");
            printf("Enter your choice: \033[0m");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    registerUser();
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    if (loginUser()) {
                        printf("\033[1;32mLogin successful!\n\033[0m");
                        loggedIn = 1;
                        system("pause");
                        system("cls");
                    } else {
                        printf("\033[1;31mLogin failed. Invalid username or password.\n\033[0m");
                    }
                    break;
                case 3:
                    printf("\033[1;31mExiting program. Goodbye!\n\033[0m");
                    exit(0);
                default:
                    printf("\033[1;31mInvalid choice. Please enter a valid option.\n\033[0m");
            }
        } else {
            if (loggedInUserRole == ADMIN) {
                printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
                printf("%*s*         ADMIN         *\n", (80 + 28) / 2, "");
                printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");
                printf("\033[1;31m1. Add Item\n");
                printf("2. Delete Item\n");
                printf("3. View Products and Update Product\n");
                printf("4. Logout\n");
                printf("5. Check Total Revenue\n");
                printf("Enter your choice: \033[0m");
                scanf("%d", &choice);
                system("cls");

                switch (choice) {
                    case 1:
                        viewProducts();
                        addItem();
                        break;
                    case 3:
                        viewProducts();
                        updateProduct();
                        system("pause");
                        system("cls");
                        break;
                    case 4:
                        loggedIn = 0;
                        printf("\033[1;32mLogged out successfully!\n\033[0m");
                        break;
                    case 2:
                        viewProducts();
                        deleteProduct();
                        break;
                    case 5:
                        printf("\033[1;32mTotal Revenue: $%.2f\n\033[0m", checkTotalRevenue());
                        // updateRevenueFile(totalRevenue);
                        
                        system("pause");
                        system("cls");
                        break;
                    default:
                        printf("\033[1;31mInvalid choice. Please enter a valid option.\n\033[0m");
                }
            } else if (loggedInUserRole == EMPLOYEE) {
                printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
                printf("%*s*        EMPLOYEE       *\n", (80 + 28) / 2, "");
                printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");
                printf("\033[1;31m1. Purchase Item\n");
                printf("2. Logout\n");
                printf("3. View Products\n");
                printf("Enter your choice: \033[0m");
                scanf("%d", &choice);
                system("cls");

                switch (choice) {
                    case 1:
                        viewProducts();
                        purchaseItem();
                        break;
                    case 2:
                        loggedIn = 0;
                        printf("Logged out successfully!\n");
                        break;
                    case 3:
                        viewProducts();
                        system("pause");
                        system("cls");
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

    // Display RIGISTER 
    printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
    printf("%*s*        REGISTER       *\n", (80 + 28) / 2, "");
    printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");

    printf("\033[1;33mEnter username: \033[0m");
    scanf("%s", username);
    printf("\033[1;33mEnter password: \033[0m");
    scanf("%s", password);

    printf("\033[1;33mChoose role (1 for Admin, 2 for Employee): \033[0m");
    scanf("%d", &role);

    FILE *file = fopen(USER_FILE, "a");
    if (file == NULL) {
        printf("\033[1;31mError opening user file.\n\033[0m");
        exit(1);
    }

    fprintf(file, "%s,%s,%d\n", username, password, role);
    fclose(file);

    printf("\033[1;32mRegistration successful!\n\033[0m");
}

int loginUser() {
    char username[MAX_SIZE];
    char password[MAX_SIZE];

     // Display LOGIN 
    printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
    printf("%*s*          LOGIN        *\n", (80 + 28) / 2, "");
    printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");

    printf("\033[1;33mEnter username: \033[0m");
    scanf("%s", username);
    printf("\033[1;33mEnter password: \033[0m");
    scanf("%s", password);

    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("\033[1;31mError opening user file.\n\033[0m");
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

    printf("\033[1;33mEnter product name: \033[0m");
    scanf("%s", name);
    printf("\033[1;33mEnter price: \033[0m");
    scanf("%f", &price);
    printf("\033[1;33mEnter quantity: \033[0m");
    scanf("%d", &quantity);

    FILE *file = fopen(PRODUCT_FILE, "a");
    if (file == NULL) {
        printf("\033[1;31mError opening product file.\n\033[0m");
        exit(1);
    }
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char datetime[MAX_SIZE];
    strftime(datetime, MAX_SIZE, "%m/%d/%y %H:%M:%S", tm_info);

    fprintf(file, "%s,%.2f,%d,%s\n", name, price, quantity, datetime);

    fclose(file);

    printf("\033[1;32mProduct '%s' added successfully at %s.\n\033[0m", name, datetime);
    // updateRevenueFile(price * quantity);
    system("pause");
    system("cls");

}

void purchaseItem() {
    float totalCost = 0.0;
    char name[MAX_SIZE];
    int quantity;
    float money;
    float change = 0.0;
    int finishTransaction = 0;

    while (!finishTransaction) {
        printf("\033[1;33mEnter product name (or type 'finish' to complete the purchase): \033[0m");
        scanf("%s", name);

        if (strcmp(name, "finish") == 0) {
            finishTransaction = 1;
        } else {
            FILE *file = fopen(PRODUCT_FILE, "r");
            if (file == NULL) {
                printf("\033[1;31mError opening product file.\n\033[0m");
                exit(1);
            }

            char line[MAX_SIZE];
            int found = 0;
            float totalPrice = 0.0;

            while (fgets(line, sizeof(line), file)) {
                char storedName[MAX_SIZE];
                float price;
                int storedQuantity;
                char datetime[MAX_SIZE];
                sscanf(line, "%[^,],%f,%d,%[^\n]", storedName, &price, &storedQuantity, datetime);

                if (strcmp(name, storedName) == 0) {
                    found = 1;
                    printf("\033[1;33mEnter quantity to purchase for %s: \033[0m", name);
                    scanf("%d", &quantity);

                    if (storedQuantity >= quantity) {
                        totalPrice = quantity * price;
                        totalCost += totalPrice;
                        printf("\033[1;32mAdded %d %s to the purchase. Current total cost: %.2f\n\033[0m", quantity, name, totalCost);

                        // Update the quantity in the product file
                        FILE *tempFile = fopen("temp.csv", "w");
                        rewind(file);
                        while (fgets(line, sizeof(line), file)) {
                            sscanf(line, "%[^,],%f,%d,%[^\n]", storedName, &price, &storedQuantity, datetime);
                            if (strcmp(name, storedName) == 0) {
                                storedQuantity -= quantity;
                                // Preserve the timestamp while updating the quantity
                                fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, price, storedQuantity, datetime);
                            } else {
                                // If not the target peoduct, write the line as is
                                fprintf(tempFile, "%s", line);
                            }
                        }
                        fclose(file);
                        fclose(tempFile);
                        remove(PRODUCT_FILE);
                        rename("temp.csv", PRODUCT_FILE);
                        updateRevenueFile(totalPrice);
                    } else {
                        printf("\033[1;31mNot enough stock. Please choose a lower quantity.\n\033[0m");
                    }
                }
            }

            fclose(file);

            if (!found) {
                printf("\033[1;31mProduct not found. Please enter a valid product name.\n\033[0m");
                system("cls");
            }
        }
    }

    printf("\033[1;32mTotal cost for the entire purchase: %.2f\n\033[0m", totalCost);

    printf("\033[1;33mEnter money: \033[0m");
    scanf("%f", &money);

    if (money >= totalCost) {
        change = money - totalCost;
        printf("\033[1;32mChange: %.2f\n", change);
        printf("Purchase successful!\n");
        system("pause");
        system("cls");
    } else {
        printf("\033[1;31mNot enough money. Purchase failed.\n\033[0m");
        system("cls");
    }
}

void viewProducts() {
    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        printf("Error opening product file.\n");
        exit(1);
    }
    // Display the shop product
    printf("\033[1;33m%*s*************************\n", (80 + 28) / 2, "");
    printf("%*s*        PRODUCTS       *\n", (80 + 28) / 2, "");
    printf("%*s*************************\033[0m\n", (80 + 28) / 2, "");

    
    printf("\033[1;35m==================================================================================================================================\n");
    printf("\x1b[36m    Name\t\t                           Price\t\t         Quantity\t\t        Stock-date\n");
    printf("\033[1;35m==================================================================================================================================\n");

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_SIZE];
        float price;
        int quantity;
        char datetime[MAX_SIZE];
        sscanf(line, "%[^,],%f,%d,%[^\n]", name, &price, &quantity, datetime);
        printf("\x1b[36m%-50s%-33.2f%-25d%-25s\n", name, price, quantity, datetime);
    }

    printf("\033[1;35m==================================================================================================================================\n");
    
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
        char datetime[MAX_SIZE];  // Add a variable to store the timestamp
        sscanf(line, "%[^,],%f,%d,%[^\n]", storedName, &price, &quantity, datetime);

        if (strcmp(name, storedName) == 0) {
            found = 1;
            printf("Product found and deleted successfully!\n");
            system("pause");
            system("cls");

        } else {
            fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, price, quantity, datetime);
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
void updateRevenueFile(float amount) {
    FILE *file = fopen(REVENUE_FILE, "a");
    if (file == NULL) {
        printf("\033[1;31mError opening revenue file.\n\033[0m");
        exit(1);
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char datetime[MAX_SIZE];
    strftime(datetime, MAX_SIZE, "%m/%d/%y %H:%M:%S", tm_info);

    fprintf(file, "%.2f,%s\n", amount, datetime);
    fclose(file);

}

float checkTotalRevenue() {
    FILE *file = fopen(REVENUE_FILE, "r");
    if (file == NULL) {
        printf("\033[1;31mError opening revenue file.\n\033[0m");
        exit(1);
    }

    float total = 0.0;
    char line[MAX_SIZE];

    while (fgets(line, sizeof(line), file)) {
        float amount;
        sscanf(line, "%f,", &amount);
        total += amount;
    }

    fclose(file);

    
    totalRevenue = total;

    return total;
}
void updateProduct() {
    char name[MAX_SIZE];
    int choice;
    float newPrice;
    int newQuantity;

    printf("\033[1;33mEnter product name to update: \033[0m");
    scanf(" %99[^\n]", name);

    FILE *file = fopen(PRODUCT_FILE, "r");
    if (file == NULL) {
        printf("\033[1;31mError opening product file.\n\033[0m");
        exit(1);
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        fclose(file);
        printf("\033[1;31mError opening temporary file.\n\033[0m");
        exit(1);
    }

    char line[MAX_SIZE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char storedName[MAX_SIZE];
        float price;
        int quantity;
        char datetime[MAX_SIZE];
        sscanf(line, " %99[^,],%f,%d,%19[^\n]", storedName, &price, &quantity, datetime);

        if (strcmp(name, storedName) == 0) {
            found = 1;

            printf("\033[1;33mSelect attribute to update or cancel:\n");
            printf("1. Price\n");
            printf("2. Quantity\n");
            printf("3. Cancel\n");
            printf("Enter your choice: \033[0m");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("\033[1;33mEnter new price: \033[0m");
                    scanf("%f", &newPrice);
                    fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, newPrice, quantity, datetime);
                    printf("\033[1;32mPrice updated successfully!\n\033[0m");
                    break;
                case 2:
                    printf("\033[1;33mEnter new quantity: \033[0m");
                    scanf("%d", &newQuantity);
                    fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, price, newQuantity, datetime);
                    printf("\033[1;32mQuantity updated successfully!\n\033[0m");
                    break;
                case 3:
                    fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, price, quantity, datetime);
                    break;
                default:
                    printf("\033[1;31mInvalid choice. No updates made.\n\033[0m");
            }
        } else {
            fprintf(tempFile, "%s,%.2f,%d,%s\n", storedName, price, quantity, datetime);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(PRODUCT_FILE);
    rename("temp.csv", PRODUCT_FILE);

    if (!found) {
        printf("\033[1;31mProduct not found. Update failed.\n\033[0m");
    }
}




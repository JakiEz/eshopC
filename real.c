#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_FIELD_LENGTH 100

struct Product {
    char name[MAX_FIELD_LENGTH];
    float price;
    int quantity;
};

void deleteProduct(const char *filename, const char *productName, int quantity) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    long int currentPosition = 0;  

    char line[MAX_LINE_LENGTH];
    char *token;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char lineCopy[MAX_LINE_LENGTH];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, ",");
        
        if (strcmp(token, productName) == 0) {
            token = strtok(NULL, ",");
            int currentQuantity = atoi(token);

            if (currentQuantity > quantity) {
                currentQuantity -= quantity;
    
                // Move the file pointer to the beginning of the line
                fseek(file, currentPosition, SEEK_SET);
                fprintf(file, "%s,%d\n", productName, currentQuantity);
            } else {
                // If the quantity is less than or equal to the requested quantity, do not write this line back
                continue;
            }
        }

        currentPosition = ftell(file);  // Update the current position in the file
    }

    fclose(file);

    printf("Product '%s' deleted successfully.\n\n", productName);
}


void addProduct(const char *filename, const char *productName, const char *price, const char *quantity) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s,%s,%s\n", productName, price, quantity);

    fclose(file);

    printf("Product '%s' added successfully.\n", productName);
}

void registerUser(const char *userFilename) {
    char username[MAX_FIELD_LENGTH];
    char password[MAX_FIELD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen(userFilename, "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s,%s\n", username, password);

    fclose(file);

    printf("User '%s' registered successfully.\n", username);
}

int login(const char *userFilename, const char *username, const char *password) {
    FILE *file = fopen(userFilename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    char *token;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        char lineCopy[MAX_LINE_LENGTH];
        strcpy(lineCopy, line);

        token = strtok(lineCopy, ",");

        if (strcmp(token, username) == 0) {
            token = strtok(NULL, ",");
            
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

int readProductsFromCSV(const char *filename, struct Product *products, int maxProducts) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numProducts = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && numProducts < maxProducts) {
        char *token = strtok(line, ",");
        strcpy(products[numProducts].name, token);

        token = strtok(NULL, ",");
        products[numProducts].price = atof(token);

        token = strtok(NULL, ",");
        products[numProducts].quantity = atoi(token);

        numProducts++;
    }

    fclose(file);

    return numProducts;
}

void browseProducts(struct Product *products, int numProducts) {
    float totalCost = 0.0;

    while (1) {
        printf("Products available for purchase:\n\n");
        for (int i = 0; i < numProducts; i++) {
            printf("%d. %s (Quantity: %d, Price: $%.2f)\n\n", i + 1, products[i].name, products[i].quantity, products[i].price);
        }

        printf("Choose a product to add to your cart, or enter 0 to finish and calculate the total: ");

        int choice;
        scanf("%d", &choice);

        if (choice < 0 || choice > numProducts) {
            printf("Invalid choice. Please enter a valid option.\n");
            continue;
        }

        if (choice == 0) {
            break; // Exit the loop if the user is done purchasing
        }

        printf("Enter the quantity to purchase: ");
        int quantity;
        scanf("%d", &quantity);

        if (quantity > products[choice - 1].quantity) {
            printf("Insufficient quantity. Please enter a smaller quaget only ntity.\n");
            continue;
        }

        deleteProduct("sample_product.csv", products[choice - 1].name, quantity);
        totalCost += quantity * products[choice - 1].price;

        printf("Added %d units of %s to your cart. Current total: $%.2f\n", quantity, products[choice - 1].name, totalCost);
    }

    printf("Total cost of your purchases: $%.2f\n\n", totalCost);

    while (1) {
        printf("Enter the amount of money: ");
        float userAmount;
        scanf("%f", &userAmount);

        if (userAmount >= totalCost) {
            float change = userAmount - totalCost;
            printf("Thank you for your purchase! Your change: $%.2f\n", change);
            break;
        } else {
            printf("Insufficient funds. Please enter a higher amount.\n");
        }
    }
}

int main() {
    const char *productFilename = "sample_product.csv"; 
    const char *userFilename = "sample_users.csv"; 
    char loginUsername[MAX_FIELD_LENGTH];
    char loginPassword[MAX_FIELD_LENGTH];

    int choice;
    int logins = 0;
    int state = 1;    

    while (state) {
        printf("Choose action:\n 1. Login\n 2. Register\n 0. Exit\n");
        scanf("%d", &choice);

        if(choice == 1)
        {
            printf("Enter Username: ");
            scanf("%s", loginUsername);
            
            printf("Enter Password: ");
            scanf("%s", loginPassword);

            if(login(userFilename,loginUsername,loginPassword))
            {
                logins = 1;
                break;
            }
            else{
                printf("Failed to log in\n");
            }
        }

        if(choice == 2)
        {
            registerUser(userFilename);
        }
        if(choice == 0){
            break;
        }
    }

    while (logins) {
        printf("Choose action:\n 1. Delete product\n 2. Add product\n 3. Purchase product\n 0. Exit\n");
        scanf("%d", &choice);

        if (choice == 0) {
            break; // Exit the loop
        } else if (choice == 1) {
            char productNameToDelete[MAX_FIELD_LENGTH];
            printf("Enter product name to delete: ");
            scanf("%s", productNameToDelete);

            deleteProduct(productFilename, productNameToDelete, 1);
        } else if (choice == 2) {
            char newProductName[MAX_FIELD_LENGTH];
            char newProductPrice[MAX_FIELD_LENGTH];
            char newProductQuantity[MAX_FIELD_LENGTH];

            printf("Enter product name: ");
            scanf("%s", newProductName);

            printf("Enter product price: ");
            scanf("%s", newProductPrice);

            printf("Enter product quantity: ");
            scanf("%s", newProductQuantity);

            addProduct(productFilename, newProductName, newProductPrice, newProductQuantity);
        } else if (choice == 3) {
            struct Product products[100];
            int numProducts = readProductsFromCSV(productFilename, products, 100);
            browseProducts(products, numProducts);
        } else {
            printf("Invalid choice. Please choose 1, 2, 3, or 0.\n");
        }
    }

    return 0;
}
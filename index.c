#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_USERS 100
#define MAX_PRODUCTS 10  // Maximum number of products

// Structure to represent a user
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

// Structure to represent a product
struct Product {
    char name[50];
    int quantity;
    int reorder_threshold;  // Minimum quantity for reordering
    float price;  // Price of the product
};


// Function to validate login credentials
int validateLogin(struct User *users, int numUsers, char *username, char *password) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Login successful
        }
    }
    return 0; // Login failed
}

// Function to add a new user
void signup(struct User *users, int *numUsers) {
    if (*numUsers < MAX_USERS) {
        printf("Enter a new username: ");
        fgets(users[*numUsers].username, sizeof(users[*numUsers].username), stdin);
        users[*numUsers].username[strcspn(users[*numUsers].username, "\n")] = '\0'; // Remove newline character

        printf("Enter a new password: ");
        fgets(users[*numUsers].password, sizeof(users[*numUsers].password), stdin);
        users[*numUsers].password[strcspn(users[*numUsers].password, "\n")] = '\0'; // Remove newline character

        (*numUsers)++;
        printf("Registration successful. You can now login.\n");
    } else {
        printf("Maximum number of users reached. Cannot register.\n");
    }
}

// Function to browse and purchase shoes
void browseProducts(struct Product *products, int numProducts) {
    float totalCost = 0.0;

    while (1) {
        printf("Products available for purchase:\n");
        for (int i = 0; i < numProducts; i++) {
            printf("%d. %s (Quantity: %d, Price: $%.2f)\n", i + 1, products[i].name, products[i].quantity, products[i].price);
        }

        printf("Choose a product to add to your cart, or enter 0 to finish and calculate the total: ");

        int choice;
        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf

        if (choice < 0 || choice > numProducts) {
            printf("Invalid choice. Please enter a valid option.\n");
            continue;
        }

        if (choice == 0) {
            break; // ออกจาก loop การเลือกสินค้า
        }

        // คำนวณราคารวมของสินค้าทั้งหมด
        totalCost += products[choice - 1].price;

        printf("Added %s to your cart. Current total: $%.2f\n", products[choice - 1].name, totalCost);
    }

    // ตรวจสอบรายการที่ซื้อและดำเนินการต่อ
    printf("Total cost of your purchases: $%.2f\n", totalCost);

    while (1) {
        printf("Enter the amount of money: ");
        float userAmount;
        scanf("%f", &userAmount);
        getchar(); // Consume newline character left by scanf

        if (userAmount >= totalCost) {
            // คำนวณเงินทอนและแสดงข้อความขอบคุณ
            float change = userAmount - totalCost;
            printf("Thank you for your purchase! Your change: $%.2f\n", change);
            break; // ออกจาก loop การเลือกสินค้า
        } else {
            printf("Insufficient funds. Please enter a higher amount.\n");

            printf("The entered amount is not sufficient. Do you want to enter a new amount? (1 for Yes, 0 for No): ");
            int enterNewAmount;
            scanf("%d", &enterNewAmount);
            getchar(); // Consume newline character left by scanf

            if (enterNewAmount == 0) {
                break; // ออกจาก loop การเลือกสินค้า
            }
        }
    }
}

// Function to check and reorder products
void checkAndReorder(struct Product *products, int numProducts) {
    for (int i = 0; i < numProducts; i++) {
        if (products[i].quantity <= products[i].reorder_threshold) {
            // Reorder the product
            printf("Reordering %s. Current quantity: %d\n", products[i].name, products[i].quantity);
            products[i].quantity += 10;  // Reorder by adding 10 to the quantity
        }
    }
}

// Function to save product data to a binary file
void saveProductsToFile(struct Product *products, int numProducts) {
    FILE *file = fopen("products.dat", "wb");
    if (file != NULL) {
        fwrite(products, sizeof(struct Product), numProducts, file);
        fclose(file);
    } else {
        printf("Error opening file for writing.\n");
    }
}

// Function to load product data from a binary file
void loadProductsFromFile(struct Product *products, int *numProducts) {
    FILE *file = fopen("products.dat", "rb");
    if (file != NULL) {
        *numProducts = fread(products, sizeof(struct Product), MAX_PRODUCTS, file);
        fclose(file);
    } else {
        printf("Error opening file for reading. Starting with empty product list.\n");
    }
}

int main() {
    
    FILE *the_file = fopen("sample.csv", "r");
    if (the_file == NULL)
    {
        perror("File not found");
        exit(1);
    }

    char line[200];

    while(fgets(line,sizeof(line), the_file))
    {
        char *token;
        token = strtok(line,",");

        while(token != NULL)
        {
            printf("%-12s", token);
            token = strtok(NULL,",");
        }

        printf("\n");

    }

    

    return 0;
}

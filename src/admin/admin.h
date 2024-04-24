#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./product.h"


int validate_choice(int min, int max);
int validate_positive_number();

struct Product;
void displayProductsFromFile(const char *filename);
void addProduct(struct Product product);
void writeProductsToFile(const char *filename, struct Product products[], int numProducts);
void removeProduct(int id);
void updateProduct(int id, char *newName, float newPrice, int newQuantity) ;
void displayProducts();
void displayProduct(struct Product product);
void sortProductsByID();
void loadProductsFromFile(const char *filename);
void reloadProductsData();


// ADMIN MENU
void adminMenu(int choice) {
    do {
        printf("MENU - Product Management: \n");
        printf("\t1. Add product \n");
        printf("\t2. Remove product \n");
        printf("\t3. Update product \n");
        printf("\t4. Display list of product \n");
        printf("\t0. Exit\n");
        choice = validate_choice(0, 4);


        switch (choice) {
            case 1: // add product
            {
                int id, quantity;
                float price;
                char name[100];

                printf("Enter product name: ");
                scanf("%s", name);

                printf("Enter product price: ");
                price = validate_positive_number();

                printf("Enter product quantity: ");
                quantity = validate_positive_number();

                struct Product newProduct = createProduct(id, name, price, quantity);
                addProduct(newProduct);

                sortProductsByID();

                // Save products to file
                writeProductsToFile(FILE_PATH, products, numProducts);

                break;
            }


            case 2: // remove product
            {
                int id;
                printf("Enter ID of the product to remove: ");
                scanf("%d", &id);
                removeProduct(id);
                sortProductsByID();
                // Save products to file after removal
                writeProductsToFile(FILE_PATH, products, numProducts);
                break;
            }

            case 3: // update product
           {
               int id, quantity;
               float price;
               char name[100];

               printf("Enter product ID to update: ");
               scanf("%d", &id);

               printf("Enter new product name: ");
               scanf("%s", name);

               printf("Enter new product price: ");
               price = validate_positive_number();

               printf("Enter new product quantity: ");
               quantity = validate_positive_number();

               updateProduct(id, name, price, quantity);

               sortProductsByID();
               // Save products to file after update
               writeProductsToFile(FILE_PATH, products, numProducts);

               break;
           }
            case 4: // display list of product
            {
                reloadProductsData();
                displayProducts();

                break;
            }
            case 0:
                printf("Successfully exited the product management program.\n");
                // Reload products data from file before exiting
                reloadProductsData();
                break;
            default:
                printf("Invalid selection! Please try again.\n");
        }
    } while(choice != 0);
}


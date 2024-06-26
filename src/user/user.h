#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_CART 100
#define MAX_PRODUCTS 100
#define CART_ID_LENGTH 10
#define MAX_PRODUCTS_IN_CART 100


struct Product;
struct Cart;
void displayProduct(struct Product product);
void displayProductsFromFile(const char *filename); 
void initCart( struct Cart *cart);
void displayCart(struct Cart cart);
void addProductToCart(struct Cart *cart, struct Product *products, int numProducts, struct Product newProduct);
void removeProductFromCart(struct Cart *cart, int id);
void updateProductQuantity(struct Cart *cart, int id, int newQuantity);
char *generate_cart_id();
void readDataFromFile(struct Product *products, int *numProducts);
void saveCartToFile(struct Cart *cart);
void loadCartFromFile(struct Cart *cart);
void loadProductsFromFile(const char *filename);
void cancel_bill_by_idOrder(int idOrder);
void display_all_bill_information(int n);
void display_bill_by_idOrder(int idOrder);
int validate_choice(int min, int max);
int validate_positive_number();


//CART       
struct Cart {
    char cart_id[CART_ID_LENGTH + 1];
    struct Product products[MAX_CART];
    int count;
};

struct Cart cart;
int numCarts = 0;

// Initialize cart
void initCart(struct Cart *cart) {
    cart->count = 0;
}

// Generate cart ID
char *generate_cart_id() {
   
    char *cart_id = (char *)malloc((CART_ID_LENGTH + 1) * sizeof(char));
  
    srand(time(NULL));
    
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    for (int i = 0; i < CART_ID_LENGTH; i++) {
        int index = rand() % (sizeof(charset) - 1);
        cart_id[i] = charset[index];
    }
    
    cart_id[CART_ID_LENGTH] = '\0';
    
    return cart_id;
}

//Read data from file product.txt
void readDataFromFile(struct Product products[], int *numProducts) {
    FILE *file = fopen("./src/data/product.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    // Read data from the file
    while (*numProducts < MAX_CART &&
           fscanf(file, "%d,%49[^,],%f,%d\n",
                  &products[*numProducts].id,
                  products[*numProducts].name,
                  &products[*numProducts].price,
                  &products[*numProducts].quantity) == 4) {
        (*numProducts)++;
    }

    if (*numProducts == MAX_CART) {
        printf("Warning: Maximum number of products reached. Some products might not have been loaded.\n");
    }

    fclose(file);
}



void saveCartToFile(struct Cart *cart) {
    FILE *file = fopen("./src/data/cart.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    static int cartCounter = 0;
    int cartID = ++cartCounter;
    
    // Ghi cart ID vào file
    fprintf(file, "Cart ID: %d\n", cartID);

    for (int i = 0; i < cart->count; i++) {
        fprintf(file, "Product ID: %d, Quantity: %d, Name: %s, Price: %.0f\n",
                cart->products[i].id, cart->products[i].quantity,
                cart->products[i].name, cart->products[i].price);
      }
    fclose(file);
 }

// Load cart data from file
void loadCartFromFile(struct Cart *cart) {
    FILE *file = fopen("./src/data/cart.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    // read id from file
    fscanf(file, "Cart Id: %s\n", cart->cart_id);
    
    // Read product data from file and populate cart
    while (fscanf(file, "Product ID: %d, Quantity: %d, Name: %49[^,], Price: %f\n",
                  &cart->products[cart->count].id,
                  &cart->products[cart->count].quantity,
                  cart->products[cart->count].name,
                  &cart->products[cart->count].price) == 4) {
        cart->count++;
        if (cart->count >= MAX_PRODUCTS_IN_CART) {
            printf("Maximum number of products in cart reached.\n");
            break;
        }
    }

    fclose(file);
}

// display cart
void displayCart(struct Cart cart) {
  if (cart.count == 0) {
    printf("Cart is empty!\n");
    return;
  }

  printf("List of products in the shopping cart:\n");
  for (int i = 0; i < cart.count; i++) {
    displayProduct(cart.products[i]);
  }
}

// Add product to the cart
void addProductToCart(struct Cart *cart,struct Product *products, int numProducts,struct Product newProduct) {
// Find the product with the entered ID
    int productIndex = -1;
    for (int i = 0; i < numProducts; i++) {
        if (products[i].id == newProduct.id) {
            productIndex = i;
            break;
        }
    }

    if (productIndex == -1) {
        printf("Product with ID %d not found.\n", newProduct.id);
        return;
    }

    if (newProduct.quantity <= 0) {
        printf("Product is out of stock.\n");
        return;
    }
// Add the product to the cart
    for (int i = 0; i < cart->count; i++) {
        if (cart->products[i].id == newProduct.id) {
            cart->products[i].quantity+=newProduct.quantity;
            printf("Product added to cart successfully!\n");
            products[productIndex].quantity-=newProduct.quantity;
            saveCartToFile(cart);
            return;
        }
    }
// If the product is not already in the cart, add it
    if (cart->count < MAX_PRODUCTS_IN_CART) {
        cart->products[cart->count]= newProduct;
        cart->products[cart->count].quantity = newProduct.quantity;
        cart->count++;
        printf("Product added to cart successfully!\n");
        products[productIndex].quantity-= newProduct.quantity;
        saveCartToFile(cart);
    } else {
        printf("Cart is full. Cannot add more products.\n");
    }
}
// Function to reset the screen
void resetScreen() {
    system("cls || clear"); 
}
// Function to handle exiting the task menu
void exitTaskMenu() {
    saveCartToFile(&cart);
    resetScreen();
}
// Remove product from cart
void removeProductFromCart(struct Cart *cart, int id) {
    int foundIndex = -1;
    for (int i = 0; i < cart->count; i++) {
        if (cart->products[i].id == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("The product does not exist in the shopping cart!\n");
        return;
    }

    // Move the following products to the previous position
    for (int i = foundIndex; i < cart->count - 1; i++) {
        cart->products[i] = cart->products[i + 1];
    }

    cart->count--;
    printf("Product successfully removed from cart!\n");

    // Update the cart.txt file
    FILE *file = fopen("./src/data/cart.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < cart->count; i++) {
        fprintf(file, "%d %s %.2f\n", cart->products[i].id, cart->products[i].name, cart->products[i].price);
    }

    fclose(file);
}
//update quantity
int get_max_quantity_from_product_txt(int id) {
    FILE *file = fopen("./src/data/product.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }

    int productId, maxQuantity;
    while (fscanf(file, "%d %d", &productId, &maxQuantity) != EOF) {
        if (productId == id) {
            fclose(file);
            return maxQuantity;
        }
    }

    fclose(file);
    return -1; 
}
void updateProductQuantity(struct Cart *cart, int id, int newQuantity) {
    int foundIndex = -1;

    for (int i = 0; i < cart->count; i++) {
        if (cart->products[i].id == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("The product does not exist in the shopping cart!\n");
        return;
    }

    int maxQuantity = get_max_quantity_from_product_txt(id); 
    if (newQuantity <= 0 || newQuantity > maxQuantity) {
        printf("Invalid product quantity. Please choose a valid positive quantity within the available stock.\n");
        return;
    }

    cart->products[foundIndex].quantity = newQuantity;
    printf("Product quantity updated successfully!\n");
}


//ORDER
struct Order;
int validatePhoneNumber(const char *phoneNumber);
int validateDate(char *date);
void read_order_from_file(const char *filename, struct Order *orders);
void write_order_to_file(const char *filename, struct Order *order);
void remove_newline(char *str);
void enter_bill_information(struct Order *x, int billID);
void create_bill(struct Order *numbers, int n);
void cancel_bill_by_idOrder(int idOrder);
void update_bill_by_idOrder(struct Order *count, int n, int id);
void display_all_bill_information(int n);
void display_bill_by_idOrder(int idOrder);


struct Order {
    int idOrder;
    char customerName[50];
    char address[100];
    char phoneNumber[15];
    char purchaseDate[11];
    struct Product products[MAX_CART];
    int productCount;
    float totalPrice;
    char paymentMethod[20];
    char paymentStatus[20];
    float productPrice;
};

int validatePhoneNumber(const char *phoneNumber) {
    int len = strlen(phoneNumber);
    if (len != 10) return 0; 
    for (int i = 0; i < len; i++) {
        if (!isdigit(phoneNumber[i])) return 0;
    }
    return 1;
}

int validateDate(char *date) {
    int len = strlen(date);
    return len == 10 && date[2] == '/' && date[5] == '/';
}

void read_order_from_file(const char *filename, struct Order *orders) {
    FILE *file = fopen("./src/data/bill/BillNumber%d.txt", "r");
    if (file == NULL) {
        printf("Can't open file '%s'.\n", filename);
        return;
    }

    int n = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%s,%s",
                  &orders[n].idOrder,
                  orders[n].purchaseDate,
                  orders[n].customerName,
                  orders[n].address,
                  orders[n].phoneNumber) != EOF) {
        printf("Bill ID: %d\n", orders[n].idOrder);
        printf("Purchase Date: %s\n", orders[n].purchaseDate);
        printf("Customer's Name: %s\n", orders[n].customerName);
        printf("Customer's Address: %s\n", orders[n].address);
        printf("Customer's Phone Number: %s\n", orders[n].phoneNumber);
        n++;
    }
    fclose(file);
}


void writeOrderToFile(const char *filename, struct Order *order, int n) {
    char fileName[100];
    sprintf(fileName, "src/data/bill/BillNumber%d.txt", n);
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d,%s,%s,%s,%s\n", order->idOrder,
            order->purchaseDate,
            order->customerName,
            order->address,
            order->phoneNumber);

    for (int i = 0; i < order->productCount; i++) {
        fprintf(file, "%d,%s,%.2f,%d\n", order->products[i].id,
                order->products[i].name,
                order->products[i].price,
                order->products[i].quantity);
    }

    fclose(file);
}

void removeNewline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0'; // Replace '\n' with '\0'
    }
}

void enter_Bill_Information(struct Order *x, int n, int billID) {
    x->idOrder = billID; 
    int check = 1;
    printf("Enter purchase date (DD/MM/YYYY): ");
    fgets(x->purchaseDate, sizeof(x->purchaseDate), stdin);
    fflush(stdin);
    while (check == 1) {
        if (!validateDate(x->purchaseDate)) {
            printf("Invalid date format. Please use DD/MM/YYYY.\n");
            printf("Rewrite purchase date.\n");
            fgets(x->purchaseDate, sizeof(x->purchaseDate), stdin);
            fflush(stdin);
        } else {
            check += 1;
        }
    }
    printf("Enter customer's name: ");
    fgets(x->customerName, sizeof(x->customerName), stdin);
    removeNewline(x->customerName);
    fflush(stdin);
    printf("Enter customer's address: ");
    fgets(x->address, sizeof(x->address), stdin);
    removeNewline(x->address);
    fflush(stdin);
    
    
    do {
        printf("Enter customer's phone number (10 digits): ");
        fgets(x->phoneNumber, sizeof(x->phoneNumber), stdin);
        removeNewline(x->phoneNumber);
        fflush(stdin);
    } while (!validatePhoneNumber(x->phoneNumber)); 
}

void create_bill(struct Order *numbers, int n) {
  
    int billID = 1;
    while (1) {
        char fileName[100];
        sprintf(fileName, "src/data/bill/BillNumber%d.txt", billID);
        FILE *file = fopen(fileName, "r");
        if (file == NULL) {
           
            fclose(file);
            break;
        } else {
           
            fclose(file);
            billID++;
        }
    }
    char fileName[100];
    sprintf(fileName, "src/data/bill/BillNumber%d.txt", billID);
    printf("Your ID bill is: %d\n", billID);
    enter_Bill_Information(&numbers[billID], n, billID); 
    writeOrderToFile(fileName, &numbers[billID], billID);
}

void cancel_bill_by_idOrder(int idOrder) {
    char fileName[100];
    sprintf(fileName, "./src/data/bill/BillNumber%d.txt", idOrder);
    if (remove(fileName) == 0) {
        printf("BillNumber%d deleted successfully.\n", idOrder);
    } else {
        printf("Error deleting BillNumber%d: Bill not found.\n", idOrder);
    }
}

void update_bill_by_idOrder(struct Order *count, int n, int id) {
    char fileName[20];
    snprintf(fileName, sizeof(fileName), "./src/data/bill/BillNumber%d.txt", id);
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: Bill '%s' not found.\n", fileName);
        return;
    }

    struct Order order;
    fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%d\n", &order.idOrder,
           order.purchaseDate,
           order.customerName,
           order.address,
           order.phoneNumber,
           &order.productCount);

    printf("\nCurrent Bill Information:\n");
    printf("Bill ID: %d\n", order.idOrder);
    printf("Purchase Date: %s\n", order.purchaseDate);
    printf("Customer's Name: %s\n", order.customerName);
    printf("Customer's Address: %s\n", order.address);
    printf("Customer's Phone Number: %s\n", order.phoneNumber);
    
    fclose(file);

    
    printf("\nEnter new information:\n");
    enter_Bill_Information(&count[id], n, id);

    
    writeOrderToFile(fileName, &count[id], id);
}

void display_all_bill_information(int n) {
    for (int i = 1; i < MAX_PRODUCTS; i++) {
        char fileName[100];
        sprintf(fileName, "./src/data/bill/BillNumber%d.txt", i);
        FILE *file = fopen(fileName, "r");
        if (file != NULL) {
            struct Order order;
            fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%d\n", &order.idOrder,
                   order.purchaseDate,
                   order.customerName,
                   order.address,
                   order.phoneNumber,
                   &order.productCount);

            printf("\n----- Bill %d -----\n", i);
            printf("Bill ID: %d\n", order.idOrder);
            printf("Purchase Date: %s\n", order.purchaseDate);
            printf("Customer's Name: %s\n", order.customerName);
            printf("Customer's Address: %s\n", order.address);
            printf("Customer's Phone Number: %s\n", order.phoneNumber);
            fclose(file);
        }
    }
}

void display_bill_by_idOrder(int idOrder) {
    char fileName[100];
    sprintf(fileName, "./src/data/bill/BillNumber%d.txt", idOrder);
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: Bill '%s' not found.\n", fileName);
        return;
    }

    struct Order order;
    fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%d\n", &order.idOrder,
           order.purchaseDate,
           order.customerName,
           order.address,
           order.phoneNumber,
           &order.productCount);

    printf("\n----- Bill %d -----\n", idOrder);
    printf("Bill ID: %d\n", order.idOrder);
    printf("Purchase Date: %s\n", order.purchaseDate);
    printf("Customer's Name: %s\n", order.customerName);
    printf("Customer's Address: %s\n", order.address);
    printf("Customer's Phone Number: %s\n", order.phoneNumber);
    fclose(file);
}



// MENU
void userMenu() {
    int choice;
    do {
        printf("MENU: \n");
        printf("\t1. Shopping Cart Management\n");
        printf("\t2. Order Management\n");
        printf("\t0. Exit\n");
        choice = validate_choice(0, 2);

        switch (choice) {
            case 1: {
                initCart(&cart);

                // Take product data from the file
                takeProductsFromFile(products, &numProducts);
    
                // Load cart data from file
                loadCartFromFile(&cart);

                int cartChoice;
                do {
                    printf("\nMenu - Shopping Cart Management:\n");
                    printf("1. Display all products in cart\n");
                    printf("2. Add new product to cart\n");
                    printf("3. Remove product from cart\n");
                    printf("4. Update product quantity in cart\n");
                    printf("5. Display all available products\n");
                    printf("0. Exit\n");
                    cartChoice = validate_choice(0, 5);

                    switch (cartChoice) {
                        case 1:
                            displayCart(cart);
                            break;
                        case 2: {
                            // Add product to cart
                            struct Product newProduct;
                            do {
                            printf("Enter the product ID: ");
                            scanf("%d", &newProduct.id);

                            if (newProduct.id <= 0) {
                            printf("Error!!! Invalid product ID. Please enter a valid product ID.\n");
                            }
                            } while (newProduct.id <= 0);
                            printf("Enter the quantity of the product: ");
                            newProduct.quantity = validate_positive_number();

                            for (int i = 0; i < numProducts; i++) {
                              if(newProduct.id == products[i].id) {
                                if (newProduct.quantity > products[i].quantity) {
                                  printf("Error!!! Over quantity");
                                  break;
                                }
                                strcpy(newProduct.name, products[i].name);
                                newProduct.price = products[i].price;
                                addProductToCart(&cart,products, numProducts,newProduct);
                                break;
                              }
                            }
                            break;
                        }
                        case 3: {
                            int productId;
                            printf("Enter the product ID to delete: ");
                            scanf("%d", &productId);
                            removeProductFromCart(&cart, productId);
                            break;
                        }
                        case 4: {
                        int productId, newQuantity,maxQuantity;
                        do {
                        printf("Enter the product ID to update quantity: ");
                        scanf("%d", &productId);
        
                        int maxQuantity = get_max_quantity_from_product_txt(productId);
        
                        if (maxQuantity == -1) {
                        printf("Product ID not found. Please try again.\n");
                        continue; 
                        }
        
                        printf("Enter the new quantity: ");
                        newQuantity = validate_positive_number();
        
                        updateProductQuantity(&cart, productId, newQuantity);
        
                        } while (maxQuantity == -1 || newQuantity == -1); 
                        break;
                        }
                        case 5:
                            displayAvailableProducts(products, numProducts);
                            break;
                        case 0:
                            printf("Successfully exited the shopping cart management program.\n");
                            break;
                        default:
                            printf("Invalid selection! Please try again.\n");
                    }
                      } while (cartChoice != 0); 

                       // Generate and display the cart ID
                       char *cart_id = generate_cart_id();
                       printf("Cart ID: %s\n", cart_id);
                       free(cart_id); 
                       break;
                      }

            case 2: {
                int orderChoice;
                int n = 1;
                struct Order NumberOrder[100];
                int ID;
                do {
                  printf("\nMenu - Order Management:\n");
                  printf("\t1. Create order\n");
                  printf("\t2. Cancel order\n");
                  printf("\t3. Update order\n");
                  printf("\t4. Display information of order\n");
                  printf("\t0. Exit\n");
                  orderChoice = validate_choice(0, 4);

                  switch (orderChoice) {
                     case 1: 
                        if (n < 100) {
                          create_bill(NumberOrder, n);
                          n++;
                        } else {
                          printf("Maximum number of orders reached.\n");
                        }
                     break;
                     case 2:
                         if (n > 0) {
                             printf("Enter file ID of Bill to cancel: ");
                             scanf("%d", &ID);
                             getchar();
                             cancel_bill_by_idOrder(ID);
                         } else {
                             printf("No orders to cancel.\n");
                         }
                        break;
                        case 3:
                            printf("Enter file ID of Bill to update: ");
                            scanf("%d", &ID);
                            getchar();
                            update_bill_by_idOrder(NumberOrder, n, ID);
                         break;
                         case 4:
                            display_all_bill_information(n);
                         break;
                        case 5:
                            printf("Enter the file ID of the bill to display: ");
                            scanf("%d", &ID);
                            getchar();
                            display_bill_by_idOrder(ID);
                        break;
            
                        default:
                        printf("Invalid choice.\n");
                        break;
                      }
                } while (orderChoice != 0);
                break;
              }

            case 0:{
              printf("Successfully exited.\n");
              break;
            }

            default:
                printf("Invalid selection! Please try again.\n");
        }
    } while (choice != 0);
}

#endif 

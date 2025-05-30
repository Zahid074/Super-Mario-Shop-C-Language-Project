#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUCTS 100
#define MAX_CART_ITEMS 100
#define MAX_QUEUE 100
#define MAX_STACK 100
char password[20] = "admin123";


typedef struct
{
    char name[50];
    char category[50];
    float price;
    int quantity;
} Product;

typedef struct
{
    Product product;
    int quantity;
} CartItem;

typedef struct
{
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
} ShoppingCart;

typedef struct
{
    int id;
    char name[50];
    int arrivalTime;
    int startTime;
    int waitTime;
    int serviceTime;
} Customer;

typedef struct
{
    Customer queue[MAX_QUEUE];
    int front, rear;
} CustomerQueue;

typedef struct
{
    Customer stack[MAX_STACK];
    int top;
} CallHistory;


Product products[MAX_PRODUCTS] =
{
    {"Rice", "Groceries", 50.0, 100},
    {"Dal", "Groceries", 60.0, 100},
    {"Puffed_Rice", "Groceries", 30.0, 100},
    {"Noodles", "Groceries", 20.0, 100},
    {"Onion", "Groceries", 40.0, 100},
    {"Garlic", "Groceries", 70.0, 100},
    {"Ginger", "Groceries", 80.0, 100},
    {"Cinnamon", "Groceries", 90.0, 100},
    {"Beef", "Butcheries", 300.0, 50},
    {"Mutton", "Butcheries", 600.0, 50},
    {"Chicken", "Butcheries", 200.0, 50},
    {"Pen", "Stationary", 10.0, 200},
    {"Notebook", "Stationary", 50.0, 100},
    {"A4_Paper", "Stationary", 2.0, 500},
    {"Stapler", "Stationary", 100.0, 50},
    {"Pencil", "Stationary", 10.0, 300},
    {"Sharpener", "Stationary", 10.0, 200},
    {"Eraser", "Stationary", 10.0, 300},
    {"Soap", "Cosmetics", 30.0, 150},
    {"Shampoo", "Cosmetics", 150.0, 100},
    {"Conditioner", "Cosmetics", 180.0, 80},
    {"Perfume", "Cosmetics", 500.0, 50}
};

int productCount = sizeof(products) / sizeof(products[0]);
CustomerQueue customerQueue = {.front = 0, .rear = 0};
CallHistory callHistory = {.top = -1};
int customerID = 1;


void customerMenu();
void adminMenu();
void addProduct();
void deleteProduct();
void updateProduct();
void changePassword();
void viewCategories();
void addToCart(ShoppingCart *cart);
void viewCart(ShoppingCart *cart);
void checkout(ShoppingCart *cart);
void enqueueCustomer(char name[50]);
void serveNextCustomer();
void showStatistics();
int getCurrentTime();


int main()
{
    //initializeProducts();
    int choice;
    while (1)
    {
        printf("\n***** Super Mario Shop *****\n");
        printf("1. Customer Menu\n");
        printf("2. Admin Menu\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            customerMenu();
            break;
        case 2:
            adminMenu();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}


// -------------------- Customer Menu --------------------
void customerMenu()
{
    static int isShopping = 0;
    if (isShopping)
    {
        printf("\nAnother customer is already shopping. Please wait.\n");
        return;
    }
    isShopping = 1;
    ShoppingCart cart = {.itemCount = 0};
    char name[50];
    printf("\nEnter your name: ");
    gets(name);  // Consider replacing with safer fgets()
    int arrivalTime = getCurrentTime();

    int choice;
    while (1)
    {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Categories\n2. Add to Cart\n3. View Cart\n4. Checkout\n5. Cancel\nEnter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            selectCategoryAndShowProducts();
            break; // ✅ FIXED LINE
        case 2:
            addToCart(&cart);
            break;
        case 3:
            viewCart(&cart);
            break;
        case 4:
            viewCart(&cart);
            checkout(&cart);
            enqueueCustomer(name);
            isShopping = 0;
            return;
        case 5:
            isShopping = 0;
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}


// -------------------- Admin Menu --------------------
void adminMenu()
{
    char pass[20];
    printf("\nEnter Admin Password: ");
    gets(pass);
    if (strcmp(pass, password) != 0)
    {
        printf("Wrong password!\n");
        return;
    }
    int choice;
    while (1)
    {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Product\n2. Delete Product\n3. Update Product\n4. Serve Next Customer\n5. Show Statistics\n6. Change Password\n7. Logout\nEnter choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            deleteProduct();
            break;
        case 3:
            updateProduct();
            break;
        case 4:
            serveNextCustomer();
            break;
        case 5:
            showStatistics();
            break;
        case 6:
            changePassword();
            break;
        case 7:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

// -------------------- Product Management --------------------
void addProduct()
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf("Product list is full!\n");
        return;
    }
    printf("\nEnter product name: ");
    gets(products[productCount].name);
    printf("Enter category: ");
    gets(products[productCount].category);
    printf("Enter price: ");
    scanf("%f", &products[productCount].price);
    printf("Enter quantity: ");
    scanf("%d", &products[productCount].quantity);
    getchar();
    productCount++;
    printf("Product added successfully!\n");
}

void changePassword()
{
    char newPassword[20];
    char confirmPassword[20];
    printf("\nWarning! Spaces cannot be used as passwords, anything else can be used.\n");
    printf("Enter new password: ");
    scanf("%s", newPassword);
    printf("Confirm new password: ");
    scanf("%s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) == 0)
    {
        strcpy(password, newPassword);  // ✅ Use modifiable global array
        printf("Password changed successfully!\n");
    }
    else
    {
        printf("Passwords do not match. Please try again.\n");
    }
}


void deleteProduct()
{
    char name[50];
    printf("Enter product name to delete: ");
    gets(name);
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].name, name) == 0)
        {
            products[i] = products[--productCount];
            printf("Product deleted successfully!\n");
            return;
        }
    }
    printf("Product not found!\n");
}

void updateProduct()
{
    char name[50];
    printf("Enter product name to update: ");
    gets(name);
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].name, name) == 0)
        {
            printf("Enter new price: ");
            scanf("%f", &products[i].price);
            printf("Enter new quantity: ");
            scanf("%d", &products[i].quantity);
            getchar();
            printf("Product updated successfully!\n");
            return;
        }
    }
    printf("Product not found!\n");
}

// -------------------- Customer Actions --------------------
void listCategories()
{
    char categories[MAX_PRODUCTS][50];
    int categoryCount = 0;
    printf("\n--- Categories ---\n");
    for (int i = 0; i < productCount; i++)
    {
        int found = 0;
        for (int j = 0; j < categoryCount; j++)
        {
            if (strcmp(products[i].category, categories[j]) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(categories[categoryCount], products[i].category);
            printf("%d. %s\n", categoryCount + 1, categories[categoryCount]);
            categoryCount++;
        }
    }
}

void showProductsByCategory(const char* category)
{
    printf("\nProducts in %s category:\n", category);
    -
        printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("Name\t\t\t\tPrice (BDT)\t\t\tQuantity\t\tStock Avaiable\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].category, category) == 0)
        {
            printf("%-8s\t\t\t%10.2f\t\t\t 1 \t\t\t%10d\n", products[i].name, products[i].price, products[i].quantity);
        }
    }
    printf("-------------------------------------------------------------------------------------------------------------------\n");
}

void viewCategories()
{
    listCategories();
}


void selectCategoryAndShowProducts()
{
    listCategories();
    int choice;
    printf("Enter the number of the category to view products: ");
    scanf("%d", &choice);

    char categories[MAX_PRODUCTS][50];
    int categoryCount = 0;
    for (int i = 0; i < productCount; i++)
    {
        int found = 0;
        for (int j = 0; j < categoryCount; j++)
        {
            if (strcmp(categories[j], products[i].category) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(categories[categoryCount], products[i].category);
            categoryCount++;
        }
    }

    if (choice >= 1 && choice <= categoryCount)
    {
        showProductsByCategory(categories[choice - 1]);
    }
    else
    {
        printf("404 ERROR! Not Found. Please try again later.\n");
    }
}

void addToCart(ShoppingCart *cart)
{
    char name[50];
    int qty;
    printf("Enter product name: ");
    gets(name);
    printf("Enter quantity: ");
    scanf("%d", &qty);
    getchar();
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].name, name) == 0 && products[i].quantity >= qty)
        {
            cart->items[cart->itemCount].product = products[i];
            cart->items[cart->itemCount].quantity = qty;
            cart->itemCount++;
            products[i].quantity -= qty;
            printf("Added to cart!\n");
            return;
        }
    }
    printf("Product not available or insufficient stock.\n");
}

void viewCart(ShoppingCart *cart)
{
    printf("\n--- Your Cart ---\n");
    for (int i = 0; i < cart->itemCount; i++)
    {
        printf("%s - $%.2f x %d = $%.2f\n", cart->items[i].product.name, cart->items[i].product.price,
               cart->items[i].quantity, cart->items[i].product.price * cart->items[i].quantity);
    }
}

void checkout(ShoppingCart *cart)
{
    float total = 0;
    printf("\n--- Invoice ---\n");
    for (int i = 0; i < cart->itemCount; i++)
    {
        float cost = cart->items[i].product.price * cart->items[i].quantity;
        total += cost;
        printf("%s x %d = $%.2f\n", cart->items[i].product.name, cart->items[i].quantity, cost);
    }
    printf("Total: $%.2f\n", total);
}

// -------------------- Queue & Stack --------------------
void enqueueCustomer(char name[50])
{
    Customer c;
    c.id = customerID++;
    strcpy(c.name, name);
    c.arrivalTime = getCurrentTime();
    customerQueue.queue[customerQueue.rear++] = c;
    printf("\nThank you %s! You've been added to the service queue.\n", name);
}

void serveNextCustomer()
{
    if (customerQueue.front == customerQueue.rear)
    {
        printf("\nNo customers in queue.\n");
        return;
    }
    Customer *c = &customerQueue.queue[customerQueue.front++];
    c->startTime = getCurrentTime();
    c->waitTime = c->startTime - c->arrivalTime;
    c->serviceTime = rand() % 5 + 1;
    callHistory.stack[++callHistory.top] = *c;
    printf("\nServing Customer: %s (ID %d)\nWait Time: %d seconds\nService Time: %d seconds\n",
           c->name, c->id, c->waitTime, c->serviceTime);
}

void showStatistics()
{
    if (callHistory.top == -1)
    {
        printf("\nNo customer served yet.\n");
        return;
    }
    int totalWait = 0, totalService = 0;
    printf("\n--- Service History ---\n");
    for (int i = 0; i <= callHistory.top; i++)
    {
        Customer c = callHistory.stack[i];
        totalWait += c.waitTime;
        totalService += c.serviceTime;
        printf("ID %d - %s | Wait: %ds | Service: %ds\n", c.id, c.name, c.waitTime, c.serviceTime);
    }
    printf("\nTotal Customers: %d\n", callHistory.top + 1);
    printf("Average Wait Time: %.2f seconds\n", totalWait / (float)(callHistory.top + 1));
    printf("Average Service Time: %.2f seconds\n", totalService / (float)(callHistory.top + 1));
}

int getCurrentTime()
{
    return (int)time(NULL);
}

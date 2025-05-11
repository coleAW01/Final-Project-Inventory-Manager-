/*
     Project: Final Project
     Project Name: Inventory Manager
     Programmer: Cole Nator
     Date: May 9, 2025
     Description: This program manages an inventory of products, allowing the user to 
     add items, apply discounts, sell items, restock low inventory, and save inventory 
     to a file. It supports two product types: Electronics and Food.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <memory>
#include <ctime>
#include <limits>
#include <climits>
#include <cfloat>
#include <iomanip> // For safe timestamp formatting

using namespace std;

/*
 * Abstract base class representing a generic product.
 * Provides common attributes and methods for all product types.
 */
class Product {
protected:
    string productName;
    double price;
    int stockQuantity;

public:
    // Constructor to initialize product details
    Product(string name, double pr, int stock) : productName(name), price(pr), stockQuantity(stock) {}

    // Display basic product information
    virtual void displayProduct() const {
        cout << "Product Name: " << productName
            << ", Price: $" << price
            << ", Stock Quantity: " << stockQuantity << endl;
    }

    // Apply a percentage discount to the product price
    virtual void applyDiscount(double percentage) {
        price -= price * (percentage / 100);
    }

    // Pure virtual function to identify the product type (must be overridden by derived classes)
    virtual string getProductType() const = 0;

    // Modify stock by a given quantity (can be positive or negative)
    void updateStock(int quantity) {
        stockQuantity += quantity;
    }

    // Attempt to sell a product if sufficient stock is available
    bool sellProduct(int quantity) {
        if (stockQuantity >= quantity) {
            stockQuantity -= quantity;
            return true;
        }
        return false;
    }

    // Accessor methods for product information
    int getStockQuantity() const { return stockQuantity; }
    string getProductName() const { return productName; }
};

// Electronics product with additional warranty info
class Electronics : public Product {
private:
    int warrantyPeriod; // Warranty period in months

public:
    // Constructor to initialize electronics product
    Electronics(string name, double pr, int stock, int warranty)
        : Product(name, pr, stock), warrantyPeriod(warranty) {}

    // Display product details along with warranty information
    void displayProduct() const override {
        Product::displayProduct();
        cout << "Warranty Period: " << warrantyPeriod << " months" << endl;
    }

    // Return the product type as a string (override of the base class method)
    string getProductType() const override {
        return "Electronics";
    }
};

// Food product with expiration date info
class Food : public Product {
private:
    string expirationDate; // Expiration date of the food product

public:
    // Constructor to initialize food product
    Food(string name, double pr, int stock, string expiration)
        : Product(name, pr, stock), expirationDate(expiration) {}

    // Display product details along with expiration information
    void displayProduct() const override {
        Product::displayProduct();
        cout << "Expiration Date: " << expirationDate << endl;
    }

    // Return the product type as a string (override of the base class method)
    string getProductType() const override {
        return "Food";
    }
};

// Manages inventory of products
class InventoryManager {
private:
    map<string, shared_ptr<Product>> inventory; // Map of product names to product pointers
    const int restockAmount = 10; // Amount to restock low inventory items

public:
    // Add a product to the inventory (if it doesn't already exist)
    void addProduct(shared_ptr<Product> product) {
        string name = product->getProductName();
        if (inventory.find(name) != inventory.end()) {
            cout << "Product with name '" << name << "' already exists. Skipping...\n";
            return;
        }
        inventory[name] = product;
    }

    // Display all products in the inventory
    void displayInventory() const {
        if (inventory.empty()) {
            cout << "Inventory is empty.\n";
            return;
        }
        for (const auto& pair : inventory) {
            pair.second->displayProduct();
        }
    }

    // Sell a product by name and log the transaction
    bool sellProduct(const string& productName, int quantity) {
        auto it = inventory.find(productName);
        if (it != inventory.end()) {
            bool success = it->second->sellProduct(quantity);
            if (success) {
                logTransaction("Sale", productName, quantity); // Log successful sale
            }
            return success;
        }
        return false; // Return false if product is not found or insufficient stock
    }

    // Apply a discount to a product by name and log the transaction
    void applyDiscount(const string& productName, double percentage) {
        auto it = inventory.find(productName);
        if (it != inventory.end()) {
            it->second->applyDiscount(percentage);
            logTransaction("Discount", productName, percentage); // Log discount application
        }
        else {
            cout << "Product not found.\n";
        }
    }

    // Save the inventory state to a file (used for persistent storage)
    void saveInventoryToFile() const {
        ofstream outFile("inventory.txt");
        for (const auto& pair : inventory) {
            outFile << pair.first << " | "
                << pair.second->getProductType() << " | "
                << pair.second->getStockQuantity() << " | " << endl;
        }
        outFile.close();
    }

    // Placeholder function for future inventory loading (not yet implemented)
    void loadInventoryFromFile() {
        ifstream inFile("inventory.txt");
        string line;
        while (getline(inFile, line)) {
            // Parse and recreate objects (to be implemented in the future)
        }
    }

    // Check the inventory for low stock items and restock them if necessary
    void checkAndRestock(int threshold) {
        for (const auto& pair : inventory) {
            if (pair.second->getStockQuantity() < threshold) {
                restockProduct(pair.second);
            }
        }
    }

    // Restock a product by a set amount and log the transaction
    void restockProduct(shared_ptr<Product> product) {
        product->updateStock(restockAmount);
        logTransaction("Restock", product->getProductName(), restockAmount);
        cout << "Restocked " << product->getProductName() << " by " << restockAmount << " units.\n";
    }

    // Log transaction involving quantity change (integer-based) with timestamp
    void logTransaction(const string& type, const string& productName, int quantity) const {
        ofstream logFile("transaction_log.txt", ios::app);
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now);  // Thread-safe localtime_s function
        logFile << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")  // Format timestamp
            << " " << type << " - " << productName << " | Quantity: " << quantity << endl;
        logFile.close();
    }

    // Log transaction involving discount percentage with timestamp
    void logTransaction(const string& type, const string& productName, double percentage) const {
        ofstream logFile("transaction_log.txt", ios::app);
        std::time_t now = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &now);  // Thread-safe localtime_s function
        logFile << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")  // Format timestamp
            << " " << type << " - " << productName << " | Discount: " << percentage << "%" << endl;
        logFile.close();
    }
};

// Helper function to get and validate integer input from the user
int getValidatedInt(const string& prompt, int minValue = INT_MIN) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minValue) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
        }
        else {
            cin.ignore();
            return value;
        }
    }
}

// Helper function to get and validate double input from the user
double getValidatedDouble(const string& prompt, double minValue = -DBL_MAX) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minValue) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
        }
        else {
            cin.ignore();
            return value;
        }
    }
}

// Main entry point and user interface loop
int main() {
    InventoryManager manager;  // Inventory manager instance
    string input;

    cout << "=== Inventory Management System ===\n";

    // Product entry loop
    while (true) {
        cout << "\nAdd a new product? (yes/no): ";
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "no") break;

        string name, type;
        double price;
        int stock;

        cout << "Enter product name: ";
        getline(cin, name);

        price = getValidatedDouble("Enter product price: $", 0.0);
        stock = getValidatedInt("Enter stock quantity: ", 0);

        cout << "Enter product type (Electronics or Food): ";
        getline(cin, type);
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type == "electronics") {
            int warranty = getValidatedInt("Enter warranty period (months): ", 0);
            shared_ptr<Product> product = make_shared<Electronics>(name, price, stock, warranty);
            manager.addProduct(product);
        }
        else if (type == "food") {
            string expiration;
            cout << "Enter expiration date (YYYY-MM-DD): ";
            getline(cin, expiration);
            shared_ptr<Product> product = make_shared<Food>(name, price, stock, expiration);
            manager.addProduct(product);
        }
        else {
            cout << "Invalid product type. Skipping...\n";
        }
    }

    // Main menu loop
    while (true) {
        cout << "\n=== Menu ===\n"
            << "1. Display Inventory\n"
            << "2. Sell Product\n"
            << "3. Apply Discount\n"
            << "4. Restock Low Inventory\n"
            << "5. Save Inventory to File\n"
            << "6. Exit\n"
            << "Choose an option: ";

        int option = getValidatedInt("", 1);

        if (option == 1) {
            manager.displayInventory();
        }
        else if (option == 2) {
            string name;
            int quantity;
            cout << "Enter product name to sell: ";
            getline(cin, name);
            quantity = getValidatedInt("Enter quantity to sell: ", 1);

            if (manager.sellProduct(name, quantity)) {
                cout << "Sale successful!\n";
            }
            else {
                cout << "Sale failed. Product not found or insufficient stock.\n";
            }
        }
        else if (option == 3) {
            string name;
            double percent;
            cout << "Enter product name for discount: ";
            getline(cin, name);
            percent = getValidatedDouble("Enter discount percentage: ", 0.0);

            manager.applyDiscount(name, percent);
            cout << "Discount applied.\n";
        }
        else if (option == 4) {
            int threshold = getValidatedInt("Enter stock threshold for restocking: ", 0);
            manager.checkAndRestock(threshold);
        }
        else if (option == 5) {
            manager.saveInventoryToFile();
            cout << "Inventory saved.\n";
        }
        else if (option == 6) {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
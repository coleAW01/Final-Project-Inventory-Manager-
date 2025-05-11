# Inventory Manager (C++ Final Project)

### 👨‍💻 Developed by: ColeAW01 
### 📅 Date: May 9, 2025

## 📌 Project Description

**Inventory Manager** is a C++ console-based application designed to manage a simple inventory system for two types of products: **Electronics** and **Food**. The system allows users to:

- Add new products
- Apply discounts
- Sell items
- Restock low inventory
- Save inventory state to a file
- Log transactions with timestamps

---

## 🧰 Features

- **Object-Oriented Design**: Uses inheritance and polymorphism with an abstract base class `Product` and derived classes `Electronics` and `Food`.
- **Inventory Management**: Add, sell, restock, and apply discounts to products.
- **Data Persistence**: Save current inventory to a file (`inventory.txt`).
- **Logging System**: Logs sales, restocking, and discount actions to `transaction_log.txt` with timestamps.
- **Input Validation**: Safeguards against invalid user input (e.g., negative values).
- **Modular Design**: Cleanly separated logic and helper functions for clarity and reusability.

---

## 🏗️ Class Structure

- `Product` (Abstract base class)
  - `Electronics` (with warranty info)
  - `Food` (with expiration date)
- `InventoryManager` (handles operations and storage)
- Utility functions for input validation

---

## 🖥️ How to Run

1. **Compile the program** (using g++ or your favorite C++ IDE):

   ```bash
   g++ -std=c++17 -o inventory_manager main.cpp

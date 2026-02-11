# 🏧 ATM Banking System (C++)

A console-based **ATM Banking System** built using **C++**.  
This project simulates real ATM operations such as login, deposit, withdrawal, and balance checking.

> 📂 All client data is loaded from and saved to a text file named `Clients.txt`.

---

## 📌 Features

- 🔐 **Secure Login**
  - Account Number
  - Pin Code authentication

- 💵 **Quick Withdraw**
  - Predefined withdrawal amounts

- 💰 **Normal Withdraw**
  - Custom withdrawal amount
  - Must be a multiple of 5
  - Balance validation

- ➕ **Deposit**
  - Positive amount validation
  - Confirmation before processing

- 📊 **Check Balance**
  - Displays current account balance
  - Balance shown in green color

- 🎨 **Console UI Styling**
  - Centered layout
  - Colored text using `windows.h`

- 💾 **Persistent Data Storage**
  - File handling using `fstream`
  - Client data stored in `Clients.txt`

---

## 🧱 Technologies Used

- **C++**
- **Structs & Enums**
- **STL (vector, string)**
- **File Handling (fstream)**
- **Windows Console API (`windows.h`)**



---

## 🗄 Data Storage Format

All client information is stored in:
Clients.txt


## Each client record follows this format:
AccountNumber#//#PinCode#//#Name#//#Phone#//#AccountBalance


### Example:
12345#//#1234#//#John Doe#//#0799999999#//#1500

---

## 🔄 System Workflow

1. User logs in
2. Main Menu is displayed
3. User selects operation:
   - Quick Withdraw
   - Normal Withdraw
   - Deposit
   - Check Balance
4. Transaction is validated and saved to file
5. Updated balance is displayed

---

## 🎯 Learning Outcomes

This project demonstrates:

- File-based data persistence
- Console UI design
- Input validation techniques
- Function modularization
- Basic banking logic implementation

---

⭐ If you like this project, consider giving it a star!



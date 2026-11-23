#  Smart Campus Lost and Found Matching System

##  Project Overview

The Smart Campus Lost and Found Matching System is a campus-oriented application designed to simplify the process of reporting, searching, and identifying lost and found items. It provides a structured platform where users can report lost belongings, register found items, search for items, and identify potential matches.

The system aims to improve the traditional lost and found process, which often relies on notice boards, WhatsApp groups, or manually asking around, by providing a more organized and accessible solution.



##  Problem Statement

Students and staff frequently lose personal belongings within the campus, such as ID cards, wallets, books, keys, and other essential items. Finding these belongings can be difficult because information about lost and found items is often scattered across different communication channels.

The absence of a centralized system makes it challenging to connect people who have lost items with those who have found them. This project addresses the problem by providing a unified platform for reporting, searching, managing, and matching lost and found items.



##  Objectives

- To develop a centralized system for managing lost and found items within the campus.
- To provide an efficient platform for reporting lost and found belongings.
- To enable users to search for items based on their names and categories.
- To identify potential matches between lost and found items using matching logic.
- To implement efficient data storage and retrieval using a hash table with separate chaining for collision handling.
- To maintain data persistence through file handling, ensuring that item records remain available across application sessions.
- To provide a simple and user-friendly interface for interacting with the system.



##  Features

- **Report Lost Items:** Register items that have been lost on campus.
- **Report Found Items:** Register items found within the campus.
- **Item Search:** Search for items using keywords related to their names or categories.
- **Potential Match Detection:** Identify possible matches between lost and found item records.
- **View All Items:** Display all registered lost and found items.
- **Image Support:** Attach images to item reports for easier identification.
- **Data Persistence:** Store and retrieve records using binary file handling.
- **Hash-Based Storage:** Organize item records using a hash table with collision handling.



##  Technology Stack

| Technology | Purpose |
|---|---|
| C | Data structure implementation and data management |
| C++ | Application logic and object-oriented implementation |
| HTML | Structure of the web interface |
| CSS | Styling and layout of the web interface |
| JavaScript | Frontend functionality and data management |
| Windows API | File selection and image handling in the desktop application |
| File Handling | Persistent storage and retrieval of item records |



##  Data Structures & Concepts Used

- Hash Table
- Hash Function
- Separate Chaining
- Singly Linked Lists
- Structures
- Dynamic Memory Allocation
- File Handling



##  OOP Concepts Used

- Class
- Object
- Encapsulation
- Constructor
- Destructor
- Abstraction



##  System Workflow

1. The application initializes the hash table.
2. Previously stored item records are loaded from the database file.
3. Users can report lost items or found items by entering the required details.
4. Item records are inserted into the hash table using the hash function.
5. Separate chaining handles collisions between records mapped to the same bucket.
6. Users can search for items using names or categories.
7. The matching system checks for potential matches between lost and found records.
8. All registered records can be displayed.
9. When the application exits, the records are saved to the binary database file.



##  Matching Mechanism

The system uses a rule-based matching mechanism to identify potential matches between lost and found records.

When a new item is reported, the system compares it with existing records and checks:

- Whether the item statuses are different (Lost and Found).
- Whether the item names have a matching substring.
- Whether the item categories have a matching substring.

If these conditions indicate a possible match, the system displays the matching item details to help users identify potentially related records.

The matching mechanism is based on string comparisons and predefined conditions rather than machine learning.



##  Data Persistence

The system uses binary file handling to maintain records across application sessions.

- Records are stored in `campus_items.dat`.
- `fwrite()` is used to write item records to the file.
- `fread()` is used to retrieve saved item records.
- The hash table is reconstructed when the application starts by loading the saved records.

This allows the system to retain previously registered items even after the application is closed.



##  Frontend

The project also includes a web-based interface developed using HTML, CSS, and JavaScript.


### Frontend Features
- Interactive home page.
- Lost and found item reporting form.
- Item search functionality.
- Dynamic item cards.
- Lost and found status indicators.
- Image upload and preview.
- Statistics displaying total, lost, and found items.
- Responsive layout for different screen sizes.

The frontend currently stores its records using browser Local Storage.

The web frontend and the C/C++ desktop application are separate implementations and are not currently connected to a shared database.



##  Project Structure

```text
Smart-Campus-Lost-and-Found/
│
├── ds.h                  # Data structure declarations
├── ds.c                  # Hash table implementation
├── main.cpp               # C++ application logic
│
├── index.html              # Web interface structure
├── style.css               # Web interface styling
├── script.js               # Frontend functionality
├── geu.jpg                # Background image
│
├── images/                 # Uploaded item images
├── campus_items.dat        # Binary database file (generated)
│
└── README.md                # Project documentation

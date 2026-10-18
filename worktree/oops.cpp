#include <iostream>
#include <cstring>
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include <fstream>
#include <ctime>
#include <shellapi.h>

using namespace std;

extern "C" {
    typedef struct {
        char name[50];
        char category[30];
        char date[20];
        char status[10];
        char contact[50];
        char imagePath[200];
    } Item;

    typedef struct Node {
        Item data;
        struct Node* next;
    } Node;

    void initList();
    void insertNode(Item newItem);
    Node* getHead();
    void freeList();
    void saveDatabaseC(const char* filename);
    void loadDatabaseC(const char* filename);
}

bool selectAndCopyImage(char* destinationPath, int destinationSize) {
    char sourcePath[MAX_PATH] = "";

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = sourcePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter =
        "Image Files\0*.jpg;*.jpeg;*.png;*.bmp;*.gif\0"
        "All Files\0*.*\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameA(&ofn)) {
        cout << "File picker cancelled or failed. Error: "
             << CommDlgExtendedError() << endl;
        return false;
    }

    // Find the directory where the program is running
    char currentDir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, currentDir) == 0) {
        cout << "Cannot get current directory." << endl;
        return false;
    }

    cout << "Program working directory: " << currentDir << endl;

    // Build the images folder path
    char folderPath[MAX_PATH];
    sprintf(folderPath, "%s\\images", currentDir);

    // Create the folder; allow it to already exist
    if (_mkdir(folderPath) != 0 && errno != EEXIST) {
        cout << "Cannot create folder: " << folderPath << endl;
        perror("mkdir");
        return false;
    }

    // Get extension
    const char* extension = strrchr(sourcePath, '.');
    if (extension == NULL) {
        cout << "Invalid image filename." << endl;
        return false;
    }

    // Generate a filename
    char fileName[100];
    sprintf(fileName, "item_%lld%s", (long long)time(NULL), extension);

    // Build full destination path
    char fullDestination[MAX_PATH];
    sprintf(fullDestination, "%s\\%s", folderPath, fileName);

    // Copy the image
    ifstream source(sourcePath, ios::binary);
    if (!source) {
        cout << "Cannot read selected image: " << sourcePath << endl;
        return false;
    }

    ofstream destination(fullDestination, ios::binary);
    if (!destination) {
        cout << "Cannot create image file: " << fullDestination << endl;
        return false;
    }

    destination << source.rdbuf();

    source.close();
    destination.close();

    // Verify destination exists
    if (GetFileAttributesA(fullDestination) == INVALID_FILE_ATTRIBUTES) {
        cout << "Copy failed. File not found: " << fullDestination << endl;
        return false;
    }

    // Store path in the Item
    strncpy(destinationPath, fullDestination, destinationSize - 1);
    destinationPath[destinationSize - 1] = '\0';

    cout << "\nImage copied successfully!" << endl;
    cout << "Saved location: " << fullDestination << endl;

    return true;
}

void viewImage(const char* imagePath) {
    if (strcmp(imagePath, "No image") == 0) {
        cout << "No image available for this item." << endl;
        return;
    }

    HINSTANCE result = ShellExecuteA(
        NULL,
        "open",
        imagePath,
        NULL,
        NULL,
        SW_SHOWNORMAL
    );

    if ((INT_PTR)result <= 32) {
        cout << "Unable to open the image." << endl;
    }
}

class LostAndFoundSystem {
private:
    const char* dbFile = "campus_items.dat";

    void checkMatch(Item newlyReported) {
        Node* temp = getHead();
        bool matchFound = false;
        
        cout << "\n--- Smart Matching System ---" << endl;
        while (temp != nullptr) {
            if (strcmp(temp->data.status, newlyReported.status) != 0) {
                if (strstr(temp->data.name, newlyReported.name) != nullptr || 
                    strstr(temp->data.category, newlyReported.category) != nullptr) {
                    
                    cout << "Potential Match Found!" << endl;
                    cout << "Item: " << temp->data.name << " (" << temp->data.category << ")" << endl;
                    cout << "Reported as " << temp->data.status << " on: " << temp->data.date << endl;
                    cout << "Contact: " << temp->data.contact << endl;
                    matchFound = true;
                }
            }
            temp = temp->next;
        }
        if (!matchFound) cout << "No immediate matches found in the system." << endl;
        cout << "-----------------------------\n" << endl;
    }

public:
    LostAndFoundSystem() {
        initList(); 
        loadDatabaseC(dbFile); 
    }

    ~LostAndFoundSystem() {
        saveDatabaseC(dbFile); 
        freeList(); 
    }

    void reportItem(const char* status) {
        Item newItem;
        strcpy(newItem.status, status);
        
        cin.ignore();
        cout << "Enter Item Name (e.g., Earbuds, ID Card): ";
        cin.getline(newItem.name, 50);
        
        cout << "Enter Category (Electronics, IDs, Books, etc.): ";
        cin.getline(newItem.category, 30);
        
        cout << "Enter Date (DD/MM/YYYY): ";
        cin.getline(newItem.date, 20);
        
        cout << "Enter Your Contact Info (Phone/Email): ";
        cin.getline(newItem.contact, 50);

        cout << "\nSelect an image of the item..." << endl;

        if (!selectAndCopyImage(newItem.imagePath, 200)) {
            strcpy(newItem.imagePath, "No image");
        }

        insertNode(newItem);
        cout << "\nItem successfully registered as " << status << "!" << endl;

        cout << "Image: " << newItem.imagePath << endl;
        
        checkMatch(newItem);
    }

    void searchItem() {
        char searchKeyword[50];
        cin.ignore();
        cout << "Enter keyword to search (Name or Category): ";
        cin.getline(searchKeyword, 50);

        Node* temp = getHead();
        bool found = false;
        
        cout << "\n--- Search Results ---" << endl;
        while (temp != nullptr) {
            if (strstr(temp->data.name, searchKeyword) != nullptr || 
                strstr(temp->data.category, searchKeyword) != nullptr) {
                
                cout << "[" << temp->data.status << "] " 
                     << temp->data.name << " | Category: " << temp->data.category 
                     << " | Date: " << temp->data.date 
                     << " | Contact: " << temp->data.contact << endl;
                cout << "Image: " << temp->data.imagePath << endl;
                char viewChoice;

                cout << "View image? (Y/N): ";
                cin >> viewChoice;

                if (viewChoice == 'Y' || viewChoice == 'y') {
                    viewImage(temp->data.imagePath);
                }

                cout << endl;     
                found = true;
            }
            temp = temp->next;
        }
        if (!found) cout << "No items match your search." << endl;
        cout << "----------------------\n" << endl;
    }

    void displayAllItems() {
        Node* temp = getHead();
        if (temp == nullptr) {
            cout << "\nThe database is currently empty.\n" << endl;
            return;
        }

        cout << "\n--- All Registered Items ---" << endl;
        while (temp != nullptr) {
            cout << "[" << temp->data.status << "] " 
                 << temp->data.name << " (" << temp->data.category << ") - " 
                 << temp->data.date << endl;
            cout << "Image: " << temp->data.imagePath << endl;     
            temp = temp->next;
        }
        cout << "----------------------------\n" << endl;
    }
};

int main() {
    LostAndFoundSystem campusSystem;
    int choice;

    do {
        cout << "========================================" << endl;
        cout << " Smart Campus Lost & Found System" << endl;
        cout << "========================================" << endl;
        cout << "1. Report a Lost Item" << endl;
        cout << "2. Report a Found Item" << endl;
        cout << "3. Search for an Item" << endl;
        cout << "4. View All Items" << endl;
        cout << "5. Exit System" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: campusSystem.reportItem("Lost"); break;
            case 2: campusSystem.reportItem("Found"); break;
            case 3: campusSystem.searchItem(); break;
            case 4: campusSystem.displayAllItems(); break;
            case 5: cout << "Saving data and exiting..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 5);

    return 0;
}
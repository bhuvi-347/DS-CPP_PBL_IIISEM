#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include "item.h"
using namespace std;

namespace
{
string trim(const string& s)
{
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

string lower(string s)
{
    for (size_t i = 0; i < s.size(); ++i)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

void copyField(char* dst, size_t size, const string& src)
{
    strncpy(dst, src.c_str(), size - 1);
    dst[size - 1] = '\0';
}

bool parseInt(const string& s, int& out)
{
    if (s.empty()) return false;
    char* end = NULL;
    long v = strtol(s.c_str(), &end, 10);
    if (*end != '\0' || v < 0 || v > 1000000) return false;
    out = (int)v;
    return true;
}

bool validDate(const string& s)
{
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    for (size_t i = 0; i < s.size(); ++i)
        if (i != 2 && i != 5 && !isdigit((unsigned char)s[i])) return false;

    int d = atoi(s.substr(0, 2).c_str());
    int m = atoi(s.substr(3, 2).c_str());
    int y = atoi(s.substr(6, 4).c_str());
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;

    static const int dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = dim[m - 1];
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)) maxDay = 29;
    return d <= maxDay;
}

bool readLine(const string& prompt, string& out)
{
    cout << prompt;
    if (!getline(cin, out)) return false;
    out = trim(out);
    return true;
}

bool readRequired(const string& prompt, size_t size, string& out)
{
    while (true)
    {
        if (!readLine(prompt, out)) return false;
        if (out.empty())
        {
            cout << "  This field cannot be empty.\n";
        }
        else if (out.size() > size - 1)
        {
            cout << "  Too long (maximum " << (size - 1) << " characters).\n";
        }
        else
        {
            return true;
        }
    }
}

vector<string> words(const string& s)
{
    vector<string> result;
    string cur;
    for (size_t i = 0; i <= s.size(); ++i)
    {
        if (i < s.size() && isalnum((unsigned char)s[i]))
        {
            cur += (char)tolower((unsigned char)s[i]);
        } else
        {
            if (cur.size() >= 3) result.push_back(cur);
            cur.clear();
        }
    }
    return result;
}

bool namesMatch(const string& a, const string& b)
{
    string x = lower(a), y = lower(b);
    if (x.empty() || y.empty()) return false;
    if (x.find(y) != string::npos || y.find(x) != string::npos) return true;

    vector<string> wa = words(a), wb = words(b);
    for (size_t i = 0; i < wa.size(); ++i)
        for (size_t j = 0; j < wb.size(); ++j)
            if (wa[i] == wb[j]) return true;
    return false;
}

bool isOpen(const Item& it)
{
    return strcmp(it.status, "Lost") == 0 || strcmp(it.status, "Found") == 0;
}
}

class LostAndFoundSystem
{
private:
    const string dbFile;

    void persist()
    {
        if (!saveDatabaseC(dbFile.c_str()))
            cout << "  Warning: could not save to " << dbFile
                 << ". Your latest change is NOT stored on disk.\n";
    }

    void checkMatch(const Item& n)
    {
        cout << "\n--- Smart Matching System ---\n";
        bool found = false;

        for (Node* t = getHead(); t != NULL; t = t->next)
        {
            const Item& o = t->data;
            if (!isOpen(o)) continue;
            if (strcmp(o.status, n.status) == 0) continue;
            if (!namesMatch(o.name, n.name)) continue;

            bool sameCategory = lower(o.category) == lower(n.category);
            cout << (sameCategory ? "Strong match" : "Possible match") << " (" 
            << (sameCategory ? "same name and category" : "similar name") << ")\n";
            cout << "  Item: " << o.name << " (" << o.category << ")\n";
            cout << "  Reported as " << o.status << " on: " << o.date << "\n";
            cout << "  Contact: " << o.contact << "\n";
            found = true;
        }
        if (!found) cout << "No matches found in the system yet.\n";
        cout << "-----------------------------\n\n";
    }

public:
    LostAndFoundSystem() : dbFile("campus_items.dat")
    {
        initList();
        int n = loadDatabaseC(dbFile.c_str());
        if (n > 0) cout << "Loaded " << n << " saved item(s).\n\n";
    }

    ~LostAndFoundSystem()
    {
        saveDatabaseC(dbFile.c_str());
        freeList();
        clearStack();
    }

    void reportItem(const char* status)
    {
        Item it;
        memset(&it, 0, sizeof it);
        copyField(it.status, sizeof it.status, status);

        string v;
        if (!readRequired("Enter Item Name (e.g., Earbuds, ID Card): ", sizeof it.name, v)) return;
        copyField(it.name, sizeof it.name, v);

        if (!readRequired("Enter Category (Electronics, IDs, Books, etc.): ", sizeof it.category, v)) return;
        copyField(it.category, sizeof it.category, v);

        while (true)
        {
            if (!readLine("Enter Date (DD/MM/YYYY): ", v)) return;
            if (validDate(v)) break;
            cout << "  Invalid date. Use DD/MM/YYYY, e.g. 05/10/2026.\n";
        }
        copyField(it.date, sizeof it.date, v);

        if (!readRequired("Enter Your Contact Info (Phone/Email): ", sizeof it.contact, v)) return;
        copyField(it.contact, sizeof it.contact, v);

        if (!insertNode(it))
        {
            cout << "Error: out of memory, item not saved.\n";
            return;
        }
        pushAction(ACTION_ADD, &it);
        persist();
        cout << "\nItem successfully registered as " << status << "!\n";

        checkMatch(it);
    }

    void searchItem()
    {
        string kw;
        if (!readRequired("Enter keyword to search (Name or Category): ", 50, kw)) return;
        string k = lower(kw);

        bool found = false;
        cout << "\n--- Search Results ---\n";
        for (Node* t = getHead(); t != NULL; t = t->next)
        {
            const Item& o = t->data;
            if (lower(o.name).find(k) != string::npos || lower(o.category).find(k) != string::npos)
            {
                cout << "[" << o.status << "] " << o.name
                     << " | Category: " << o.category
                     << " | Date: " << o.date
                     << " | Contact: " << o.contact << "\n";
                found = true;
            }
        }
        if (!found) cout << "No items match your search.\n";
        cout << "----------------------\n\n";
    }

    void displayAllItems()
    {
        Node* t = getHead();
        if (t == NULL)
        {
            cout << "\nThe database is currently empty.\n\n";
            return;
        }

        cout << "\n--- All Registered Items ---\n";
        int n = 0;
        for (; t != NULL; t = t->next)
        {
            cout << ++n << ". [" << t->data.status << "] " << t->data.name << " (" << t->data.category << ") - " 
            << t->data.date << "\n";
        }
        cout << "Total: " << n << " item(s)\n";
        cout << "----------------------------\n\n";
    }

    void markRecovered()
    {
        vector<Node*> open;
        for (Node* t = getHead(); t != NULL; t = t->next)
            if (isOpen(t->data)) open.push_back(t);

        if (open.empty())
        {
            cout << "\nThere are no open Lost/Found items to mark.\n\n";
            return;
        }

        cout << "\n--- Open Items ---\n";
        for (size_t i = 0; i < open.size(); ++i)
        {
            const Item& o = open[i]->data;
            cout << (i + 1) << ". [" << o.status << "] " << o.name << " (" << o.category << ") - " << o.date 
            << " - Contact: " << o.contact << "\n";
        }

        string line;
        int pick = 0;
        while (true)
        {
            if (!readLine("Enter number to mark as Recovered (0 to cancel): ", line)) return;
            if (parseInt(line, pick) && pick <= (int)open.size()) break;
            cout << "  Please enter a number from 0 to " << open.size() << ".\n";
        }
        if (pick == 0)
        {
            cout << "Cancelled.\n\n";
            return;
        }

        Node* node = open[pick - 1];
        Item before = node->data;
        pushAction(ACTION_RECOVER, &before);
        copyField(node->data.status, sizeof node->data.status, "Recovered");
        persist();
        cout << "\"" << node->data.name << "\" is now marked as Recovered.\n\n";
    }

    void undoLast()
    {
        Action a;
        if (!popAction(&a))
        {
            cout << "\nNothing to undo.\n\n";
            return;
        }

        if (a.type == ACTION_ADD)
        {
            if (removeNode(&a.item))
                cout << "\nUndone: removed \"" << a.item.name << "\" ("
                     << a.item.status << ").\n\n";
            else
                cout << "\nCould not undo: that item was not found.\n\n";
        } else
        {
            Item now = a.item;
            copyField(now.status, sizeof now.status, "Recovered");
            Node* n = findNode(&now);
            if (n != NULL)
            {
                copyField(n->data.status, sizeof n->data.status, a.item.status);
                cout << "\nUndone: \"" << a.item.name << "\" is back to "
                     << a.item.status << ".\n\n";
            } else
            {
                cout << "\nCould not undo: that item was not found.\n\n";
            }
        }
        persist();
    }
};

int main()
{
    LostAndFoundSystem campusSystem;
    string line;

    while (true)
    {
        cout << "========================================\n"
             << " Smart Campus Lost & Found System\n"
             << "========================================\n"
             << "1. Report a Lost Item\n"
             << "2. Report a Found Item\n"
             << "3. Search for an Item\n"
             << "4. View All Items\n"
             << "5. Mark an Item as Recovered\n"
             << "6. Undo Last Action\n"
             << "7. Exit System\n";

        if (!readLine("Enter your choice: ", line)) break;

        int choice = 0;
        if (!parseInt(line, choice))
        {
            cout << "Invalid input. Please enter a number from 1 to 7.\n\n";
            continue;
        }

        if (choice == 7)
        {
            cout << "Saving data and exiting...\n";
            break;
        }

        switch (choice)
        {
            case 1: campusSystem.reportItem("Lost"); break;
            case 2: campusSystem.reportItem("Found"); break;
            case 3: campusSystem.searchItem(); break;
            case 4: campusSystem.displayAllItems(); break;
            case 5: campusSystem.markRecovered(); break;
            case 6: campusSystem.undoLast(); break;
            default: cout << "Invalid choice. Please enter a number from 1 to 7.\n\n";
        }
    }
    return 0;
}

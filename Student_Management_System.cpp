/*
 * File: 543391_MuhammadQasim_Lab09.cpp
 * Student Name: Muhammad Qasim
 * Registration Number: 543391
 * Section:  BSAI-2A
 *
 * DESIGN CHOICES & TIME COMPLEXITY ANALYSIS (Task 1 & Task 3):
 * 1. Student Records Storage: Binary Search Tree (BST)
 *    - Justification: Unique roll numbers naturally partition into left and right subtrees.
 *      Eliminates fixed-capacity constraints of arrays and avoids linear O(n) search scans of linked lists.
 *    - Average-case Add & Search Complexity: O(log n)
 *    - Worst-case Add & Search Complexity: O(n) (occurs if keys are inserted in skewed order)
 * 2. Additional Feature: Linear Array Queue (HelpDeskQueue)
 *    - Justification: Follows strict FIFO (First-In, First-Out) order for student help desk assistance.
 *    - Reset Logic: Resets indices to 0 when all waiting students are served.
 *    - Enqueue / Dequeue Complexity: O(1)
 */

#include <iostream>
#include <string>

using namespace std;

// ====================================================
// TASK 1: Data Structure Implementations from Scratch
// ====================================================

struct BSTNode {
    int rollNo;
    string name;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int r, string n) {
        rollNo = r;
        name = n;
        left = nullptr;
        right = nullptr;
    }
};

class StudentBST {
private:
    BSTNode* root;

    BSTNode* insertNode(BSTNode* current, BSTNode* newNode) {
        if (current == nullptr) {
            return newNode;
        }

        if (newNode->rollNo < current->rollNo) {
            current->left = insertNode(current->left, newNode);
        } else if (newNode->rollNo > current->rollNo) {
            current->right = insertNode(current->right, newNode);
        }

        return current;
    }

    void inOrderTraversal(BSTNode* node) const {
        if (node == nullptr) return;
        inOrderTraversal(node->left);
        cout << "Roll No: " << node->rollNo << " | Name: " << node->name << "\n";
        inOrderTraversal(node->right);
    }

    BSTNode* searchNode(BSTNode* node, int rollNo) const {
        if (node == nullptr || node->rollNo == rollNo) {
            return node;
        }
        if (rollNo < node->rollNo) {
            return searchNode(node->left, rollNo);
        }
        return searchNode(node->right, rollNo);
    }

    void destroyTree(BSTNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    StudentBST() {
        root = nullptr;
    }

    bool addStudent(int rollNo, const string& name) {
        // Reject duplicate roll numbers
        if (search(rollNo) != nullptr) {
            cout << "Error: Student with Roll No " << rollNo << " already exists! Duplicate rejected.\n";
            return false;
        }

        BSTNode* newNode = new BSTNode(rollNo, name);
        root = insertNode(root, newNode);
        cout << "Success: Student \"" << name << "\" (Roll No: " << rollNo << ") registered.\n";
        return true;
    }

    BSTNode* search(int rollNo) const {
        return searchNode(root, rollNo);
    }

    void displayAll() const {
        if (root == nullptr) {
            cout << "Notice: Student record database is currently empty.\n";
            return;
        }
        cout << "\n---------------- Registered Students (Sorted by Roll No) ----------------\n";
        inOrderTraversal(root);
        cout << "-------------------------------------------------------------------------\n";
    }

    void clear() {
        destroyTree(root);
        root = nullptr;
    }

    ~StudentBST() {
        clear();
    }
};

class HelpDeskQueue {
private:
    static const int CAPACITY = 10;
    int arr[CAPACITY];
    int front;
    int rear;

public:
    HelpDeskQueue() {
        front = 0;
        rear = -1;
    }

    bool isEmpty() const {
        return (front > rear);
    }

    bool isFull() const {
        return (rear == CAPACITY - 1);
    }

    bool enqueue(int rollNo) {
        if (isFull()) {
            cout << "Error: Queue is full (Capacity: " << CAPACITY << "). Cannot add more requests.\n";
            return false;
        }
        rear++;
        arr[rear] = rollNo;
        cout << "Success: Help request registered for Roll No: " << rollNo << ".\n";
        return true;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Notice: No pending help requests in the queue.\n";
            return -1;
        }

        int served = arr[front];
        front++;

        // Reset logic when all served
        if (front > rear) {
            front = 0;
            rear = -1;
        }

        return served;
    }

    void displayQueue() const {
        if (isEmpty()) {
            cout << "Help Desk Queue is empty.\n";
            return;
        }
        cout << "\n--- Pending Help Desk Requests ---\n";
        int position = 1;
        for (int i = front; i <= rear; i++) {
            cout << position++ << ". Roll No: " << arr[i] << "\n";
        }
        cout << "----------------------------------\n";
    }
};

// ====================================================
// TASK 2: Menu Application
// ====================================================

void printMenu() {
    cout << "\n=========================================\n";
    cout << "       STUDENT HELP DESK SYSTEM (BST)    \n";
    cout << "=========================================\n";
    cout << "1. Add Student Record\n";
    cout << "2. Search Student by Roll Number\n";
    cout << "3. Display All Students\n";
    cout << "4. Submit Help Desk Request\n";
    cout << "5. Serve Next Help Desk Request\n";
    cout << "6. View Pending Help Requests\n";
    cout << "7. Exit Application\n";
    cout << "=========================================\n";
    cout << "Enter your choice (1-7): ";
}

int main() {
    StudentBST directory;
    HelpDeskQueue deskQueue;
    int choice;


    do {
        printMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input format. Exiting...\n";
            break;
        }

        switch (choice) {
            case 1: {
                int rollNo;
                string name;
                cout << "Enter Roll Number: ";
                cin >> rollNo;
                cin.ignore();
                cout << "Enter Student Name: ";
                getline(cin, name);
                directory.addStudent(rollNo, name);
                break;
            }
            case 2: {
                int rollNo;
                cout << "Enter Roll Number to search: ";
                cin >> rollNo;
                BSTNode* res = directory.search(rollNo);
                if (res != nullptr) {
                    cout << "Found: Roll No: " << res->rollNo << " | Name: " << res->name << "\n";
                } else {
                    cout << "Search Failed: Student with Roll No " << rollNo << " not found in database.\n";
                }
                break;
            }
            case 3: {
                directory.displayAll();
                break;
            }
            case 4: {
                // Task 3: Unregistered student validation
                int rollNo;
                cout << "Enter Roll Number seeking help: ";
                cin >> rollNo;

                BSTNode* verifiedStudent = directory.search(rollNo);
                if (verifiedStudent == nullptr) {
                    cout << "Request Rejected: Roll No " << rollNo << " is NOT registered in the system.\n";
                } else {
                    cout << "Student verified: " << verifiedStudent->name << "\n";
                    deskQueue.enqueue(rollNo);
                }
                break;
            }
            case 5: {
                int served = deskQueue.dequeue();
                if (served != -1) {
                    BSTNode* s = directory.search(served);
                    string name = (s != nullptr) ? s->name : "Unknown";
                    cout << "Now Serving: Roll No " << served << " | " << name << " at the Help Desk.\n";
                }
                break;
            }
            case 6: {
                deskQueue.displayQueue();
                break;
            }
            case 7: {
                cout << "Releasing all dynamically allocated BST nodes...\n";
                directory.clear();
                cout << "Clean shutdown completed.\n";
                break;
            }
            default:
                cout << "Invalid selection! Please enter an option from 1 to 7.\n";
                break;
        }
    } while (choice != 7);

    return 0;
}
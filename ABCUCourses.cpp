#include <algorithm>
#include <iostream>
#include <cstring>

#include "CSVparser.hpp"

using namespace std;

struct Course {
    string courseId;
    string courseName;
    double prerequisiteNum;
    string prerequisiteOne;
    string prerequisiteTwo;
    Course() {
        prerequisiteOne = "none";
        prerequisiteTwo = "none";
    }
};


// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);
    int size = 0;

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);

};

BinarySearchTree::BinarySearchTree() {

    // set the root
    root = nullptr;

}

BinarySearchTree::~BinarySearchTree() {
}

void BinarySearchTree::InOrder() {

    // call inOrder fuction and pass root 
    this->inOrder(root);
}


void BinarySearchTree::Insert(Course course) {

    // if root equarl to null ptr
    if (root == nullptr) {

        // root is equal to new node course
        root = new Node(course);
    }

    else {
        
        // add Node root and course
        this->addNode(root, course);
    }




}  


// Remove a course
void BinarySearchTree::Remove(string courseId) {

    // remove node root courseId
    this->removeNode(root, courseId);
}


// Search for a course
Course BinarySearchTree::Search(string courseId) {

    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching course found
    while (current != nullptr) {
        // if match found, return current bid
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }

        // if bid is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }

        // else larger so traverse right
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}  

// Add course
void BinarySearchTree::addNode(Node* node, Course course) {

    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {

        // if no left node
        if (node->left == nullptr) {

            // this node becomes left
            node->left = new Node(course);
        }

        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }

    // add to right subtree
    else {
        // if no right node
        if (node->right == nullptr) {

            // this node becomes right
            node->right = new Node(course);
        }

        // else recurse down the right node
        else {
            this->addNode(node->right, course);
        }

    }
}

// Print courses in order
void BinarySearchTree::inOrder(Node* node) {

    //if node is not equal to null ptr
    if (node != nullptr) {
        
        //InOrder left
        inOrder(node->left);

        //output course ID, and prerequisites
        cout << node->course.courseId << ", "
            << node->course.courseName << endl;

        //InOder right
        inOrder(node->right);

    }



}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    
    // if this node is null, then return 
    if (node == nullptr) {
        return node;
    }


    // recurse down the left subtree
    if (courseId.compare(node->course.courseId) < 0) {
        node->left = removeNode(node->left, courseId);
    }
    else if (courseId.compare(node->course.courseId) > 0) {
        node->right = removeNode(node->right, courseId);
    }

    else {
        // no childern / leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }

        // one child to left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }

        // one child to right
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }

        // two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseId);
        }

    }

    return node;
} 

// display course
void displayCourse(Course course) {
    
    // empty course
    if (course.courseId == "") {
        cout << "invalid course selection" << endl;
    }

    // no prerequisites
    else if (course.prerequisiteOne == "") {
        cout << course.courseId << ", " << course.courseName << endl;
        cout << "Prerequisites: none" << endl;
    }
    
    // one prerequisite
    else if (course.prerequisiteTwo == "") {
        cout << course.courseId << ", " << course.courseName << endl;
        cout << "Prerequisites: " << course.prerequisiteOne << endl;
    }

    // two prerequisites
    else {
        cout << course.courseId << ", " << course.courseName << endl;
        cout << "Prerequisites: " << course.prerequisiteOne << ", " << course.prerequisiteTwo << endl;

    }
    return;
}

void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << " ..." << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a course using data from current row (i)
            Course course;
            course.courseId = file[i][0];
            course.courseName = file[i][1];
            course.prerequisiteOne = file[i][2];
            course.prerequisiteTwo = file[i][3];

            //cout << course.courseId << ": " << course.courseName << " | " << course.prerequisiteOne << " | " << course.prerequisiteTwo << endl;

            // add this course to the end
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}


int main() {
    string courseSearch;
    string csvPath = "CourseData.csv";
    string errorString;

    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    
    cout << "Welcome to the course planner." << endl;

    while (choice != 4) {
        try {
            cout << endl;
            cout << "  1. Load Data Structure. " << endl;
            cout << "  2. Print Course List. " << endl;
            cout << "  3. Print Course. " << endl;
            cout << "  4. Exit" << endl;
            cout << endl << "What would you like to do? ";
            cin >> choice;

            // error catch
            if (choice > 10 || cin.fail()) {
                cin.clear();
                cin.ignore();
                throw runtime_error("Invalid choice");

            }

            switch (choice) {

                // load data structures
            case 1:

                cout << endl;

                loadBids(csvPath, bst);
                cout << "Data structures loaded successfully. " << endl;

                break;

                // print sample schedule
            case 2:

                cout << endl << "Here is a sample schedule: " << endl << endl;

                bst->InOrder();
                cout << endl;

                break;

                // print specific course information
            case 3:
                cout << "What course do you want to know about? " << endl;
                cin >> courseSearch;

                // convert search key to capital letter
                for (int i = 0; i < courseSearch.size(); ++i) {
                    courseSearch.at(i) = toupper(courseSearch.at(i));
                }

                cout << endl;

                displayCourse(bst->Search(courseSearch));

                break;

                // exit 
            case 4:
                break;

                // invalid option
            default:
                cout << choice << " is not a valid option. " << endl;
                break;

            }
        }
        catch (runtime_error& excpt) {
            cin.clear();
            cin.ignore();
            cout << excpt.what() << endl;


        }
    }

    cout << "Thank you for using the course planner! " << endl;

    return 0;
}
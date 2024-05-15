#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Subscriber {
    std::string phoneNumber;
    int callDuration;
};

struct TreeNode {
    Subscriber data;
    TreeNode* left;
    TreeNode* right;
};

class TelephoneDirectory {
private:
    TreeNode* root;

    void addNode(TreeNode*& node, const std::string& phoneNumber, int callDuration) {
        if (!node) {
            node = new TreeNode;
            node->data.phoneNumber = phoneNumber;
            node->data.callDuration = callDuration;
            node->left = nullptr;
            node->right = nullptr;
        }
        else {
            if (phoneNumber < node->data.phoneNumber) {
                addNode(node->left, phoneNumber, callDuration);
            }
            else {
                addNode(node->right, phoneNumber, callDuration);
            }
        }
    }

    void deleteNode(TreeNode*& node, const std::string& phoneNumber) {
        if (!node)
            return;

        if (phoneNumber < node->data.phoneNumber) {
            deleteNode(node->left, phoneNumber);
        }
        else if (phoneNumber > node->data.phoneNumber) {
            deleteNode(node->right, phoneNumber);
        }
        else {
            if (!node->left && !node->right) {
                delete node;
                node = nullptr;
            }
            else if (!node->left) {
                TreeNode* temp = node;
                node = node->right;
                delete temp;
            }
            else if (!node->right) {
                TreeNode* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                TreeNode* minNode = node->right;
                while (minNode->left) {
                    minNode = minNode->left;
                }
                node->data = minNode->data;
                deleteNode(node->right, minNode->data.phoneNumber);
            }
        }
    }

    void clear(TreeNode*& node) {
        if (!node)
            return;

        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }

    void printInOrder(TreeNode* node) {
        if (!node)
            return;

        printInOrder(node->left);
        std::cout << "Phone number: " << node->data.phoneNumber << ", Call duration: " << node->data.callDuration << " minutes\n";
        printInOrder(node->right);
    }

    void balance(std::vector<Subscriber>& subscribers, int low, int high) {
        if (low > high)
            return;

        int mid = (low + high) / 2;
        addSubscriber(subscribers[mid].phoneNumber, subscribers[mid].callDuration);
        balance(subscribers, low, mid - 1);
        balance(subscribers, mid + 1, high);
    }

    void storeInOrder(TreeNode* node, std::vector<Subscriber>& subscribers) {
        if (!node)
            return;

        storeInOrder(node->left, subscribers);
        subscribers.push_back(node->data);
        storeInOrder(node->right, subscribers);
    }

    TreeNode* search(TreeNode* node, const std::string& phoneNumber) {
        if (!node)
            return nullptr;

        if (phoneNumber == node->data.phoneNumber)
            return node;
        else if (phoneNumber < node->data.phoneNumber)
            return search(node->left, phoneNumber);
        else
            return search(node->right, phoneNumber);
    }

public:
    TelephoneDirectory() : root(nullptr) {}

    void addSubscriber(const std::string& phoneNumber, int callDuration) {
        addNode(root, phoneNumber, callDuration);
    }

    void removeSubscriber(const std::string& phoneNumber) {
        deleteNode(root, phoneNumber);
    }

    void printDirectory() {
        printInOrder(root);
    }

    void sortDirectory() {
        std::vector<Subscriber> subscribers;
        storeInOrder(root, subscribers);
        clear(root);
        balance(subscribers, 0, subscribers.size() - 1);
    }

    void printPaymentMessage(const std::string& phoneNumber) {
        TreeNode* subscriberNode = search(root, phoneNumber);
        if (subscriberNode) {
            std::cout << "Payment message for phone number " << phoneNumber << ": Please pay " << subscriberNode->data.callDuration * 0.1 << " USD\n";
        }
        else {
            std::cout << "Phone number " << phoneNumber << " not found in the directory.\n";
        }
    }

    void clear() {
        clear(root);
    }

    ~TelephoneDirectory() {
        clear();
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        std::vector<Subscriber> subscribers;
        storeInOrder(root, subscribers);
        for (const auto& subscriber : subscribers) {
            file << subscriber.phoneNumber << " " << subscriber.callDuration << "\n";
        }

        file.close();
        std::cout << "Data saved to file: " << filename << std::endl;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        clear();

        std::string phoneNumber;
        int callDuration;
        while (file >> phoneNumber >> callDuration) {
            addSubscriber(phoneNumber, callDuration);
        }

        file.close();
        std::cout << "Data loaded from file: " << filename << std::endl;
    }
};

int main() {
    TelephoneDirectory directory;

    int choice;
    bool running = true;
    std::string filename;

    while (running) {
        std::cout << "\nTelephone Directory Menu:\n";
        std::cout << "1. Add Subscriber\n";
        std::cout << "2. Remove Subscriber\n";
        std::cout << "3. Print Directory\n";
        std::cout << "4. Sort Directory\n";
        std::cout << "5. Print Payment Message\n";
        std::cout << "6. Save Directory to File\n";
        std::cout << "7. Load Directory from File\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string phoneNumber;
            int callDuration;
            std::cout << "Enter phone number: ";
            std::cin >> phoneNumber;
            std::cout << "Enter call duration (in minutes): ";
            std::cin >> callDuration;
            directory.addSubscriber(phoneNumber, callDuration);
            break;
        }
        case 2: {
            std::string phoneNumber;
            std::cout << "Enter phone number to remove: ";
            std::cin >> phoneNumber;
            directory.removeSubscriber(phoneNumber);
            break;
        }
        case 3:
            std::cout << "Telephone Directory:\n";
            directory.printDirectory();
            break;
        case 4:
            std::cout << "Sorting directory...\n";
            directory.sortDirectory();
            std::cout << "Directory sorted.\n";
            break;
        case 5: {
            std::string phoneNumber;
            std::cout << "Enter phone number: ";
            std::cin >> phoneNumber;
            directory.printPaymentMessage(phoneNumber);
            break;
        }
        case 6:
            std::cout << "Enter filename to save: ";
            std::cin >> filename;
            directory.saveToFile(filename);
            break;
        case 7:
            std::cout << "Enter filename to load: ";
            std::cin >> filename;
            directory.loadFromFile(filename);
            break;
        case 8:
            running = false;
            break;
        default:
            std::cout << "Invalid choice! Please enter a number between 1 and 8.\n";
        }
    }

    return 0;
}

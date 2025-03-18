#include <iostream>
#include "RTree.h"

void showMenu() {
    std::cout << "R-tree Operations Menu:\n";
    std::cout << "1. Insert MBR\n";
    std::cout << "2. Search MBR\n";
    std::cout << "3. Find Area of MBRs\n";
    std::cout << "4. Display Tree\n"; // New option
    std::cout << "5. Exit\n";        // Adjusted numbering
    std::cout << "Enter your choice: ";
}

int main() {
    RTree rtree(4);
    int choice;
    int x_min, y_min, x_max, y_max;

    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:  // Insert MBR
            std::cout << "Enter MBR coordinates (x_min, y_min, x_max, y_max): ";
            std::cin >> x_min >> y_min >> x_max >> y_max;
            if (x_min > x_max || y_min > y_max) {
                std::cout << "Invalid MBR: x_min must be <= x_max and y_min must be <= y_max\n";
            }
            else {
                rtree.insert(MBR(x_min, y_min, x_max, y_max));
                std::cout << "MBR inserted successfully.\n";
            }
            break;

        case 2:  // Search MBR
            std::cout << "Enter search MBR coordinates (x_min, y_min, x_max, y_max): ";
            std::cin >> x_min >> y_min >> x_max >> y_max;
            if (x_min > x_max || y_min > y_max) {
                std::cout << "Invalid search MBR: x_min must be <= x_max and y_min must be <= y_max\n";
            }
            else {
                rtree.search(MBR(x_min, y_min, x_max, y_max));
            }
            break;

        case 3:  // Find Area of MBRs
            rtree.find_area();
            break;

        case 4:  // Display Tree
            rtree.display_tree();
            break;

        case 5:  // Exit
            std::cout << "Exiting program.\n";
            return 0;

        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
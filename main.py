def show_menu():
    print("R-tree Operations Menu:")
    print("1. Insert MBR")
    print("2. Search MBR")
    print("3. Find Area of MBRs")
    print("4. Display Tree")
    print("5. Exit")
    print("Enter your choice: ", end="")

def main():
    from rtree import RTree, MBR  # Import RTree and MBR from rtree module
    rtree = RTree(4)
    
    while True:
        show_menu()
        choice = int(input())
        
        if choice == 1:  # Insert MBR
            print("Enter MBR coordinates (x_min, y_min, x_max, y_max): ", end="")
            x_min, y_min, x_max, y_max = map(int, input().split())
            if x_min > x_max or y_min > y_max:
                print("Invalid MBR: x_min must be <= x_max and y_min must be <= y_max")
            else:
                rtree.insert(MBR(x_min, y_min, x_max, y_max))
                print("MBR inserted successfully.")
        
        elif choice == 2:  # Search MBR
            print("Enter search MBR coordinates (x_min, y_min, x_max, y_max): ", end="")
            x_min, y_min, x_max, y_max = map(int, input().split())
            if x_min > x_max or y_min > y_max:
                print("Invalid search MBR: x_min must be <= x_max and y_min must be <= y_max")
            else:
                rtree.search(MBR(x_min, y_min, x_max, y_max))
        
        elif choice == 3:  # Find Area of MBRs
            rtree.find_area()
        
        elif choice == 4:  # Display Tree
            rtree.display_tree()
        
        elif choice == 5:  # Exit
            print("Exiting program.")
            return
        
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()
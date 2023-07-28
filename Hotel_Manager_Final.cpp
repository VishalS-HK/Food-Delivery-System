#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

namespace MenuManagement {

    struct MenuItem
    {
        int serialNumber;
        std::string foodItem;
        double price;
    };

    class Menu
    {
    private:
        std::vector<MenuItem> menuItems;
        std::string filename;

    public:
        Menu(const std::string& filename) : filename(filename) {
            readMenuFromCSV();
        }

        void readMenuFromCSV()
        {
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Error: Unable to open file " << filename << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string serialNoStr, foodItem, priceStr;
                if (!(std::getline(iss, serialNoStr, ',') &&
                      std::getline(iss, foodItem, ',') &&
                      std::getline(iss, priceStr, ',')))
                {
                    std::cerr << "Error: Invalid line in file " << filename << std::endl;
                    continue;
                }
                try
                {
                    int serialNo = std::stoi(serialNoStr);
                    double price = std::stod(priceStr);
                    menuItems.push_back({ serialNo, foodItem, price });
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }

            file.close();
        }

        void writeMenuToCSV() const
        {
            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Error: Unable to open file " << filename << std::endl;
                return;
            }

            for (const auto& item : menuItems)
            {
                file << item.serialNumber << "," << item.foodItem << "," << item.price << "\n";
            }

            file.close();
        }

        void printMenu() const
        {
            if (menuItems.empty())
            {
                std::cout << "Menu is empty.\n";
            }
            else
            {
                std::cout << "Menu:\n";
                std::cout << std::left << std::setw(15) << "Serial Number" << std::setw(25) << "Food Item"
                          << "Price" << std::endl;
                for (const auto& item : menuItems)
                {
                    std::cout << std::left << std::setw(15) << item.serialNumber << std::setw(25) << item.foodItem << "Rs " << std::fixed << std::setprecision(2) << item.price << "\n";
                }
            }
        }

        void updateMenuItem()
        {
            int serialNo;
            std::cout << "Enter the serial number of the item you want to update: ";
            std::cin >> serialNo;
            while (std::cin.fail() || serialNo <= 0 || serialNo > static_cast<int>(menuItems.size()))
            {
                std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin >> serialNo;
            }
            
            int n,m=3;
            std::cout << "Which do you wish to update: " << std::endl;
            std::cout << "1. Food item name\n";
            std::cout << "2. Price\n";
            std::cout << "3. Both\n";
            std::cin.ignore();
            std::cin >> n;
            while (std::cin.fail() || n <= 0 || n > static_cast<int>(m))
            {
                std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin >> n;
            }

            
            bool found=false;
            for (auto& item : menuItems)
            {
                if (item.serialNumber == serialNo)
                {
                    found=true;
                    if (n == 1)
                    {
                        std::cout << "Enter the new name for the item: ";
                        std::cin.ignore();
                        std::getline(std::cin, item.foodItem);
                    }
                    else if (n == 2)
                    {
                        std::cout << "Enter the new price for the item: ";
                        std::cin >> item.price;
                        while (std::cin.fail() || item.price <= 0)
                        {
                            std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                            std::cin.clear();

                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            std::cin >> item.price;
                        }
                    }
                    else if (n == 3)
                    {
                        std::cout << "Enter the new name for the item: ";
                        std::cin.ignore();
                        std::getline(std::cin, item.foodItem);
                        std::cout << "Enter the new price for the item: ";
                        std::cin >> item.price;
                        while (std::cin.fail() || item.price <= 0)
                        {
                            std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                            std::cin.clear();

                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            std::cin >> item.price;
                        }
                    }

                    std::cout << "Item updated successfully.\n";
                    writeMenuToCSV();
                    return;
                }
            }
            if(!found)
            {
                std::cout << "Invalid serial number. Item with serial number " << serialNo << " not found.\n";

            }
            // std::cout << "Item with serial number " << serialNo << " not found.\n";
        }

        void deleteMenuItem()
        {
            int serialNo;
            std::cout << "Enter the serial number of the item you want to delete: ";
            std::cin >> serialNo;
             while (std::cin.fail() || serialNo <= 0 || serialNo > static_cast<int>(menuItems.size()))
            {
                std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin >> serialNo;
            }

            auto it = std::remove_if(menuItems.begin(), menuItems.end(), [serialNo](const MenuItem& item)
                { return item.serialNumber == serialNo; });

            if (it != menuItems.end())
            {
                menuItems.erase(it, menuItems.end());
                std::cout << "Item deleted successfully.\n";
                writeMenuToCSV();
            }
            else
            {
                std::cout << "Item with serial number " << serialNo << " not found.\n";
            }
        }

        void addNewItem()
        {
            MenuItem newItem;
            std::cout << "Enter the serial number for the new item: ";
            std::cin >> newItem.serialNumber;
            while (std::cin.fail() || newItem.serialNumber <= 0)
            {
                std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin >> newItem.serialNumber;
            }
            std::cout << "Enter the name for the new item: ";
            std::cin.ignore();
            std::getline(std::cin, newItem.foodItem);
            std::cout << "Enter the price for the new item: ";
            std::cin >> newItem.price;
            while (std::cin.fail() || newItem.price <= 0)
            {
                std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
                std::cin.clear();

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin >> newItem.price;
            }
            menuItems.push_back(newItem);
            writeMenuToCSV();
            std::cout << "New item added successfully.\n";
        }
    };

} // namespace MenuManagement

int main()
{
    std::string menuFilename = "menu.csv";
    MenuManagement::Menu menu(menuFilename);

    int choice;
    do
    {
        std::cout << "\nMenu Management System\n";
        std::cout << "1. View menu\n";
        std::cout << "2. Add new item\n";
        std::cout << "3. Update item\n";
        std::cout << "4. Delete item\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        int size = 5;
        while (std::cin.fail() || choice <= 0 || choice > static_cast<int>(size))
        {
            std::cout << "Invalid input. Please enter a valid option within the range of the Options: ";
            std::cin.clear();

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cin >> choice;
        }
        
        switch (choice)
        {
        case 1:
            menu.printMenu();
            break;
        case 2:
            menu.addNewItem();
            break;
        case 3:
            menu.updateMenuItem();
            break;
        case 4:
            menu.deleteMenuItem();
            break;
        case 5:
            std::cout << "Thank you!!\n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}



#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Character class representing the player
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    std::vector<std::string> inventory;
    std::string equippedWeapon;
    std::string equippedArmor;
public:
    Character(std::string name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) {
        equippedWeapon = "None";
        equippedArmor = "None";
    }

    // Getter methods
    std::string getName() const {
        return name;
    }

    int getHealth() const {
        return health;
    }

    int getAttack() const {
        return attack;
    }

    int getDefense() const {
        return defense;
    }

    std::string getEquippedWeapon() const {
        return equippedWeapon;
    }

    std::string getEquippedArmor() const {
        return equippedArmor;
    }

    // Attack an enemy
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.getName() << " for " << damage << " damage!\n";
        } else {
            std::cout << name << "'s attack has no effect on " << enemy.getName() << "!\n";
        }
    }

    // Equip an item
    void equipItem(const std::string& item) {
        if (item == "Health Potion") {
            health += 20;
            std::cout << name << " drank a health potion and restored 20 HP!\n";
            removeItemFromInventory(item);
        } else if (item == "Legendary Sword" || item == "Dragon Scale Armor") {
            if (std::find(inventory.begin(), inventory.end(), item) != inventory.end()) {
                if (item == "Legendary Sword") {
                    equippedWeapon = item;
                } else {
                    equippedArmor = item;
                }
                std::cout << name << " equipped " << item << "!\n";
            } else {
                std::cout << name << " doesn't own " << item << "!\n";
            }
        } else {
            std::cout << "Cannot equip " << item << "!\n";
        }
    }

    // Buy an item
    void buyItem(const std::string& item) {
        inventory.push_back(item);
        std::cout << name << " bought " << item << "!\n";
    }

    // Sell an item
    void sellItem(const std::string& item) {
        removeItemFromInventory(item);
        std::cout << name << " sold " << item << "!\n";
    }

    // Display inventory
    void displayInventory() const {
        std::cout << "Inventory:\n";
        for (const std::string& item : inventory) {
            std::cout << "- " << item << "\n";
        }
    }

private:
    // Remove an item from inventory
    void removeItemFromInventory(const std::string& item) {
        auto iter = std::find(inventory.begin(), inventory.end(), item);
        if (iter != inventory.end()) {
            inventory.erase(iter);
        }
    }
};

// Enemy class
class Enemy {
private:
    std::string name;
    std::string asciiArt;
    int health;
    int attack;
    int defense;
public:
    Enemy(std::string name, std::string asciiArt, int health, int attack, int defense)
        : name(name), asciiArt(asciiArt), health(health), attack(attack), defense(defense) {}

    // Getter methods
    std::string getName() const {
        return name;
    }

    std::string getAsciiArt() const {
        return asciiArt;
    }

    int getHealth() const {
        return health;
    }

    int getAttack() const {
        return attack;
    }

    int getDefense() const {
        return defense;
    }

    // Attack the player
    void attackPlayer(Character& player) {
        int damage = attack - player.getDefense();
        if (damage > 0) {
            player.attackEnemy(*this);
            if (health > 0) {
                std::cout << name << " counterattacks " << player.getName() << " for " << damage << " damage!\n";
            } else {
                std::cout << name << " has been defeated!\n";
            }
        } else {
            std::cout << name << "'s attack has no effect on " << player.getName() << "!\n";
        }
    }
};

// Store class
class Store {
private:
    std::vector<std::string> items;
public:
    Store() {
        items.push_back("Health Potion");
        items.push_back("Legendary Sword");
        items.push_back("Dragon Scale Armor");
    }

    // Display available items
    void displayItems() const {
        std::cout << "Available items:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". " << items[i] << "\n";
        }
    }

    // Buy an item from the store
    std::string buyItem(int index) const {
        if (index > 0 && index <= static_cast<int>(items.size())) {
            return items[index - 1];
        } else {
            return "Invalid";
        }
    }

    // Sell an item to the store
    void sellItem(Character& player, const std::string& item) {
        player.sellItem(item);
        items.push_back(item);
    }
};

// Game class
class Game {
private:
    Character player;
    std::vector<Enemy> enemies;
    Store store;

public:
    Game(std::string playerName)
        : player(playerName, 100, 20, 10) {
        // Initialize enemies
        enemies.push_back(Enemy("Goblin", R"(
            .-=-. 
           /  .  \
          |   .   |
           \  .  /
            '-=-' )", 50, 15, 5));

        enemies.push_back(Enemy("Orc", R"(
          ______
       .-"'  '--'.
      / .    .    \
     |  . .  . .   |
     |     .       |
      \   .   .  /
       '.______.' )", 80, 25, 10));

        enemies.push_back(Enemy("Dragon", R"(
              __
        .-"''////''"-.
      .'  .-.    .-.  '.
     /  .'/   \.'   \   \
     |  / /     \     \  |
      \ \ |     |      | /
       '\||     \     |/
          \\     |    /
           ))   .-.  (
          //    '-'   \
          \\__      _/
           ))|     |
          //  \    |
          \\   ) . |
           ((  ((  |
            \)) \)) ) ) )", 200, 40, 20));
    }

    void start() {
        std::cout << "Welcome to the RPG game!\n";
        std::cout << "Character: " << player.getName() << "\n";
        std::cout << "Health: " << player.getHealth() << "\n";
        std::cout << "Attack: " << player.getAttack() << "\n";
        std::cout << "Defense: " << player.getDefense() << "\n";

        while (player.getHealth() > 0 && !enemies.empty()) {
            Enemy& currentEnemy = enemies.back();
            std::cout << "\nYou encounter a " << currentEnemy.getName() << "!\n";
            std::cout << currentEnemy.getAsciiArt() << "\n";

            while (player.getHealth() > 0 && currentEnemy.getHealth() > 0) {
                std::cout << "\n1. Attack\n";
                std::cout << "2. Run\n";
                int choice;
                std::cin >> choice;

                if (choice == 1) {
                    player.attackEnemy(currentEnemy);
                    if (currentEnemy.getHealth() > 0) {
                        currentEnemy.attackPlayer(player);
                    }
                } else if (choice == 2) {
                    std::cout << "You run away from the " << currentEnemy.getName() << "!\n";
                    enemies.pop_back();
                    break;
                } else {
                    std::cout << "Invalid choice! Try again.\n";
                }
            }

            if (player.getHealth() <= 0) {
                std::cout << "Game over! You have been defeated.\n";
                return;
            }

            if (currentEnemy.getHealth() <= 0) {
                std::cout << "You defeated the " << currentEnemy.getName() << "!\n";
                enemies.pop_back();

                // Generate loot
                if (currentEnemy.getName() == "Dragon") {
                    player.buyItem("Legendary Sword");
                    player.buyItem("Dragon Scale Armor");
                } else {
                    player.buyItem("Health Potion");
                }

                // Display loot
                std::cout << "You found some loot!\n";
                player.displayInventory();
            }
        }

        if (enemies.empty()) {
            std::cout << "Congratulations! You have defeated all the enemies and won the game!\n";
        }

        // Visit the store
        std::cout << "\nYou arrive at the store.\n";
        store.displayItems();

        while (true) {
            std::cout << "\n1. Buy an item\n";
            std::cout << "2. Sell an item\n";
            std::cout << "3. Leave the store\n";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                std::cout << "Enter the index of the item you want to buy: ";
                int index;
                std::cin >> index;
                std::string item = store.buyItem(index);
                if (item == "Invalid") {
                    std::cout << "Invalid item index!\n";
                } else {
                    player.buyItem(item);
                }
            } else if (choice == 2) {
                std::cout << "Enter the name of the item you want to sell: ";
                std::string item;
                std::cin.ignore();
                std::getline(std::cin, item);
                player.sellItem(item);
                store.sellItem(player, item);
            } else if (choice == 3) {
                std::cout << "You leave the store.\n";
                break;
            } else {
                std::cout << "Invalid choice! Try again.\n";
            }
        }

        std::cout << "Thank you for playing the RPG game!\n";
    }
};

int main() {
    std::string playerName;
    std::cout << "Enter your character name: ";
    std::cin >> playerName;

    Game game(playerName);
    game.start();

    return 0;
}

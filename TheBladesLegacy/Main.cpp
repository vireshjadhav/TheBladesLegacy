#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace std;

namespace N_Utility
{
    class Utility
    {
    public:
        static void clearConsole()
        {
#ifdef _WIN32
            system("cls");
#else
            (void)system("clear");
#endif
        }

        static void waitForEnter()
        {
            cin.get();
        }

        static void clearInputBuffer()
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    };
}

namespace N_Character
{
    enum class Item
    {
        NONE = 1, MAP, SWORD, SHIELD, ARMOUR, BOW
    };

    enum class Enemies
    {
        GOBLIN = 1, TROLL = 2, ORGE = 3, DARK_ELF = 4, HOBGOBLIN = 5
    };

    class Character
    {
    protected:
        string name;
        int health;
        int maxHealth;
        int meleeDamage;
        int rangedDamage;
        int defence;

    public:
        Character()
        {
            name = "Unknown";
            health = 100;
            maxHealth = 100;
            meleeDamage = 20;
            rangedDamage = 10;
            defence = 8;
        }

        Character(string name, int health, int maxHealth, int meleeDamage, int rangedDamage, int defence) :
            name(name), health(health), maxHealth(maxHealth), meleeDamage(meleeDamage), rangedDamage(rangedDamage), defence(defence) {
        }

        Character(const Character& other)
        {
            name = other.name;
            health = other.health;
            maxHealth = other.maxHealth;
            meleeDamage = other.meleeDamage;
            rangedDamage = other.rangedDamage;
            defence = other.defence;
        }

        ~Character() {}

        void displayItems(int level) const
        {
            string item;
            switch ((Item)level)
            {
            case Item::NONE:
                item = "None";
                break;
            case Item::MAP:
                item = "Map";
                break;
            case Item::SWORD:
                item = "Sword";
                break;
            case Item::SHIELD:
                item = "Shield";
                break;
            case Item::ARMOUR:
                item = "Armour";
                break;
            case Item::BOW:
                item = "Bow";
                break;
            }
            std::cout << item;
        }

        virtual void attack(Character& target) = 0;

        string getName() { return name; }
        int getHealth() { return health; }
        int getMaxHealth() { return maxHealth; }
        int getMeleeDamage() { return meleeDamage; }
        int getRangedDamage() { return rangedDamage; }
        int getDefence() { return defence; }
        bool isFainted() const { return health <= 0; }

        int heal(int healAmount)
        {
            health += healAmount;
            if (health >= maxHealth) health = maxHealth;
            return health;
        }

        int takeDamage(int damage)
        {
            int actualDamage = damage - defence;
            if (actualDamage < 0) actualDamage = 0;
            health -= actualDamage;
            if (health < 0) health = 0;
            return health;
        }
    };
}

namespace N_Game
{
    using namespace N_Character;

    class Player : public Character
    {
    public:
        Player()
        {
            name = "Kael";
            health = 100;
            maxHealth = 100;
            rangedDamage = 20;
            meleeDamage = 10;
            defence = 8;
        }

        Player(string p_name, int p_health, int p_maxHealth, int p_meleeDamage, int p_rangedDamage, int p_defence) :
            Character(p_name, p_health, p_maxHealth, p_meleeDamage, p_rangedDamage, p_defence) {
        }

        void reset()
        {
            name = "Kael";
            health = 100;
            maxHealth = 100;
            rangedDamage = 20;
            meleeDamage = 10;
            defence = 8;
        }

        void displayPlayerStat(int level)
        {
            std::cout << "Player name:" << getName() << endl;
            std::cout << "Health: " << getHealth() << " hp" << endl;
            std::cout << "Max Health: " << getMaxHealth() << " hp" << endl;
            std::cout << "Ranged Damage: " << getRangedDamage() << " hp" << endl;
            std::cout << "Melee Damage: " << getMeleeDamage() << " hp" << endl;
            std::cout << "Defence: " << getDefence() << " hp" << endl;
            std::cout << "Item: ";
            displayItems(level);
            std::cout << "!" << endl;
        }

        void increasePlayerStat(int level)
        {
            switch (level)
            {
            case 1:
                maxHealth += 80;
                health = maxHealth;
                rangedDamage += 20;
                meleeDamage += 10;
                defence += 7;
                break;
            case 2:
                maxHealth += 120;
                health = maxHealth;
                rangedDamage += 40;
                meleeDamage += 25;
                defence += 15;
                break;
            case 3:
                maxHealth += 200;
                health = maxHealth;
                rangedDamage += 80;
                meleeDamage += 50;
                defence += 45;
                break;
            case 4:
                maxHealth += 300;
                health = maxHealth;
                rangedDamage += 160;
                meleeDamage += 100;
                defence += 135;
                break;
            case 5:
                maxHealth += 400;
                health = maxHealth;
                rangedDamage += 320;
                meleeDamage += 200;
                defence += 350;
                break;
            }
        }


        void attack(Character& target) override
        {
            int chooseDamage = rand() % 10;
            if (chooseDamage == 3)
            {
                cout << "The special move is ready and now in action." << endl;
                int tempDamage = 3 * getRangedDamage();
                target.takeDamage(tempDamage);
            }
            else if (chooseDamage == 2 || chooseDamage == 4 || chooseDamage == 6)
            {
                target.takeDamage(getMeleeDamage());
            }
            else
            {
                target.takeDamage(getRangedDamage());
            }
        }
    };

    class Enemy : public Character
    {
    public:
        vector<Enemy> minions;

        Enemy()
        {
            name = "Goblin";
            health = 55;
            maxHealth = 55;
            rangedDamage = 10;
            meleeDamage = 5;
            defence = 2;
        }

        Enemy(string e_name, int e_health, int e_maxHealth, int e_meleeDamage, int e_rangedDamage, int e_defence) :
            Character(e_name, e_health, e_maxHealth, e_meleeDamage, e_rangedDamage, e_defence) {
        }

        string getEnemyName() const
        {
            if (name == "Goblin")
            {
                return "Goblin";
            }
            else if (name == "Troll")
            {
                return "Troll";
            }
            else if (name == "Orge")
            {
                return "Orge";
            }
            else if (name == "Dark Elf")
            {
                return "Dark Elf";
            }
            else if (name == "Hobgoblin")
            {
                return "Hobgoblin";
            }
            else if (name == "Murlock")
            {
                return "Murlock";
            }
            else
                return "Unknonw";
        }

        void attack(Character& target) override
        {
            int chooseDamage = rand() % 3;
            if (chooseDamage == 2)
            {
                target.takeDamage(getRangedDamage());
            }
            else
            {
                target.takeDamage(getMeleeDamage());
            }
        }

        void displayEnemyStat(int level)
        {
            for (size_t i = 0; i < minions.size(); i++)
            {
                if (i < minions.size())
                {
                    std::cout << "Enemy name: " << minions[i].getEnemyName() << endl;
                    std::cout << "Health: " << minions[i].getHealth() << " hp" << endl;
                    std::cout << "Max Health: " << minions[i].getMaxHealth() << " hp" << endl;
                    std::cout << "Ranged Damage: " << minions[i].getRangedDamage() << " hp" << endl;
                    std::cout << "Melee Damage: " << minions[i].getMeleeDamage() << " hp" << endl;
                    std::cout << "Defence: " << minions[i].getDefence() << " hp" << endl;
                    std::cout << "**-----------------------------------------------------------------------**" << endl;
                }
            }
        }
    };

    class Game;

}

namespace N_Narration
{
    using namespace N_Utility;

    class Narration
    {
    public:
        void gameName()
        {
            std::cout << "**-----------------------------------------------------------------------**" << endl;
            std::cout << "                 Eclipse of Astralon: The Blade's Legacy                   " << endl;
            std::cout << "**-----------------------------------------------------------------------**" << endl;
            introMessage();
        }

        void introMessage()
        {
            std::cout << "\nWelcome, brave warrior. You are Kael of Astralon, known as Soulbinder\n"
                "a legendary hero of Luminara. You are about to embark on a journey that\n"
                "will test your courage, wisdom, and strength. The fate of a village-and\n"
                "perhaps the world-rests in your hands. Are you ready to rise to the challenge?" << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            Utility::waitForEnter();
        }

        void eldersMessage(N_Game::Game& game);
        void midStoryPart(N_Game::Game& game);
    };
}

namespace N_Game
{
    using namespace N_Character;
    using namespace N_Utility;

    class Game
    {
        N_Narration::Narration introStory;
        bool isPlaying = true;
        int level = 1;

    public:

        string getInput()
        {
            string input;
            getline(cin, input);
            transform(input.begin(), input.end(), input.begin(), ::toupper);
            return input;
        }

        void gameLoop(Player& player)
        {
            while (true)
            {
                introStory.gameName();
                introStory.eldersMessage(*this);
                while (isPlaying && level <= 6)
                {
                    enterLevel(level, player);
                    if (player.isFainted())
                    {
                        cout << "You have been defeated. Game Over!" << endl;
                        isPlaying = false;
                        break;
                    }
                    else
                    {
                        increasePlayerStat(level, player);
                    }
                    level++;
                }

                std::cout << "Press R to restart the game and E to exit" << endl;
                while (true)
                {
                    string input = getInput();
                    if (input == "R")
                    {
                        player.reset();
                        isPlaying = true;
                        level = 1;
                        N_Utility::Utility::clearConsole();
                        break;
                    }
                    else if (input == "E")
                    {
                        return;
                    }
                    else
                    {
                        cout << "Invalid input! Press R to restart the game and E to exit" << endl;
                    }
                }
            }
        }


        void handleStartInput();
        void handleRequestAccept();
        void handleContinueInput();
        void handleContinuePart();
        void enterLevel(int level, Player& player);
        void displayLevelStats(int level, Player& player, Enemy& enemy);
        void increasePlayerStat(int level, Player& player);
        void displayLevelTitle(int level);
        void restartGame(Game& game, Player& player);
    };
}

namespace N_Narration
{
    using namespace N_Game;
    using namespace N_Utility;

    void Narration::eldersMessage(Game& game)
    {
        std::cout << "\nElder's Message: \"Greetings, Kael. I am Elder Thalos, a guardian of the"
            "\nvillage of Luminara, nestled in the heart of the Kingdom of Astralon. Our"
            "\nvillage is under attack by a monstrous being known as Murlock, darkness and "
            "\ndestruction. He seeks to claim our most sacred artifact, the Eclipse Blade,"
            "\na weapon of unimaginable power forged by our ancestors. If Murlock succeeds,"
            "\nall of Aetheris will fall into darkness.\n"
            "\nYou, brave one, are our last hope. The villagers have fled, and our warriors"
            "\nhave fallen. Will you take up this quest to save Luminara and reclaim the"
            "\nEclipse Blade?\"" << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        game.handleStartInput();
    }

    void Narration::midStoryPart(Game& game)
    {
        Utility::clearConsole();
        std::cout << "Long ago, Luminara was a beacon of peace and prosperity in the Kingdom of Astralon."
            "\nBlessed by the stars themselves, our village thrived under the protection"
            "\nof the Eclipse Blade, a weapon said to harness the power of the celestial bodies."
            "\nIt was guarded by four legendary warriors, each stationed at a corner of the village."
            "\nBut one warrior, Kael, was different. Kael sought not only to protect but to understand"
            "\nthe world beyond Luminara. He embarked on a journey to the east, leaving the village"
            "\nin the hands of his comrades." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        std::cout << "\nOne fateful day, Murlock, a monster born of black magic, discovered the secret of"
            "\nthe Eclipse Blade. Consumed by greed, he marched upon Luminara with his minions,"
            "\ndefeating the guardians and plunging the village into chaos. The villagers cried out for"
            "\nhelp, and their pleas reached Kael, who now races back to save his home. But the journey"
            "\nis fraught with danger, and Kael must gather powerful artifacts to stand a chance against\n"
            "\nMurlock. Will you continue this journey and help Kael save Luminara? Press C to continue\nor Q to quit." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        game.handleContinueInput();
    }
}

namespace N_BattleManager
{
    using namespace N_Game;

    Player player;
    Enemy enemy;
    Game game;

    class BattleManager
    {
    private:
        bool battleOngoing;
        bool playerTurn;

    public:
        void startBattle(int level)
        {
            playerTurn = true;
            battleOngoing = true;
            battle(level);
        }

        void stopBattle()
        {
            battleOngoing = false;
        }

        void battle(int level)
        {
            for (size_t i = 0; i < enemy.minions.size(); i++)
            {
                while (battleOngoing)
                {
                    if (playerTurn)
                    {
                        std::cout << "Select form the following moves!" << endl;
                        std::cout << "1. Attack" << endl;
                        std::cout << "2. Heal" << endl;
                        string input = game.getInput();
                        if (input == "1")
                        {
                            player.attack(enemy.minions[i]);
                        }
                        else if (input == "2")
                        {
                            int healAmount = 20;
                            if (level <= 2)
                            {
                                player.heal(healAmount);

                            }
                            else if (level >= 3 && level <= 4)
                            {
                                healAmount += 60;

                            }
                            else if (level >= 5)
                            {
                                healAmount += 90;
                            }
                        }
                    }
                    else
                    {
                        int chooseMove = rand() % 2;
                        if (chooseMove == 1)
                        {
                            enemy.minions[i].attack(player);
                        }
                        else
                        {
                            enemy.minions[i].heal(25);
                            std::cout << enemy.minions[i].getEnemyName() << " healed itself for 25 hp!" << endl;
                        }
                    }

                    updateBattleState(i);

                    playerTurn = !playerTurn;

                    Utility::waitForEnter();
                }

                if (player.isFainted())
                {
                    break;
                }
            }
            handleBattleOutcome(level);
        }

        void updateBattleState(size_t i)
        {
            if (enemy.minions[i].isFainted())
            {
                battleOngoing = false;
            }
            else if (player.isFainted())
            {
                battleOngoing = false;
            }

        }

        void handleBattleOutcome(int level)
        {
            if (player.isFainted())
            {
                std::cout << player.getName() << "has defeated! You lose the Battle." << endl;
            }
            else
            {
                switch (level)
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    std::cout << "You defeated the Murlocks minions! You have unlocked ";
                    player.displayItems(level + 1);
                    std::cout << "!" << endl;
                    break;
                case 6:
                    std::cout << "You have defeated Murlock! The dark clouds over Astralon begin to dissipate, and the village"
                        "\nof Luminara is bathed in the warm light of the sun once more. The Eclipse Blade, now safe in"
                        "\nyour hands, glows with a radiant energy, its power preserved for the future of Aetheris." << endl;
                    std::cout << "\nYou’ve conquered the challenge! Through courage, strategy, and unwavering determination,"
                        "\nyou have saved Astralon and ensured peace for its people. The villagers cheer your name,"
                        "\nand your legend will be told for generations to come." << endl;
                    std::cout << "\nThank you for playing Eclipse of Eldoria: The Celestial Quest. Your journey has come to an end,"
                        "\nbut your legacy will live on forever." << endl;
                    break;
                default:
                    break;
                }
            }
        }
    };
}

namespace N_Game {

    using namespace N_Utility;
    using namespace N_Character;
    using namespace N_BattleManager;

    Enemy enemy;
    BattleManager battleManager;

    void Game::handleStartInput()
    {
        std::cout << "\nI see hesitation in your eyes, warrior. Will you accept (A) or decline (D)\nthe elder's request?" << endl;
        while (true)
        {
            string input = getInput();
            if (input == "A")
            {
                handleRequestAccept();
                break;
            }
            else if (input == "D")
            {
                std::cout << "You have chosen to turn away from this quest. The village of Luminara falls";
                std::cout << "\ninto darkness, and the Eclipse Blade is lost forever. Murlock's reign of";
                std::cout << "\nterror begins, and the world of Aetheris is plunged into chaos. The end." << endl;
                isPlaying = false;
                return;
            }
            else
            {
                std::cout << "Invalid input! Press A to accept the elder's request or D to deny it." << endl;
            }
        }
    }

    void Game::handleRequestAccept()
    {
        std::cout << "You have chosen to accept this perilous quest, Kael. Listen closely,"
            "\nfor the tale of Luminara is one of both beauty and tragedy." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        introStory.midStoryPart(*this);
    }

    void Game::handleContinueInput()
    {
        while (true)
        {
            string input = getInput();
            if (input == "C")
            {
                handleContinuePart();
                break;
            }
            else if (input == "Q")
            {
                std::cout << "Thank you for visiting Eldoria. May your next adventure be even grander!" << endl;
                isPlaying = false;
                return;
            }
            else
            {
                std::cout << "Invalid input! Press C to continue the journey or Q to quit." << endl;
            }
        }
    }

    void Game::handleContinuePart()
    {
        std::cout << "You have chosen to continue. Your quest will take you through six trials,"
            "\neach more challenging than the last. Along the way, you will face Murlock's"
            "\nminions and acquire powerful items to aid you in your final battle. Prepare"
            "\nyourself, brave one, for the path ahead is treacherous." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
    }

    void Game::displayLevelTitle(int level)
    {
        switch (level)
        {
        case 1:
            std::cout << "                  Level 1 : The Map to the Eclipse Weapon                  " << endl;
            break;
        case 2:
            std::cout << "                  Level 2: The Shattered Blade of Lumina                   " << endl;
            break;
        case 3:
            std::cout << "                 Level 3: The Aegis of the Eternal Guardian                " << endl;
            break;
        case 4:
            std::cout << "              Level 4: The Obsidian Armor of the Fallen Titan              " << endl;
            break;
        case 5:
            std::cout << "                Level 5: The Whispering Bow of the Windrider               " << endl;
            break;
        case 6:
            std::cout << " Final Boss Level: The Eclipse Citadel - Confrontation with the Void King  " << endl;
            break;
        }
    }
    void Game::enterLevel(int level, Player& player)
    {
        enemy.minions.clear();
        Utility::clearConsole();
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        displayLevelTitle(level);
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        switch (level)
        {
        case 1:
            std::cout << "\nKael,You have entered the Level " << level << ". You are about to retrieve ";
            player.displayItems(level + 1);
            std::cout << " a crucial "
                "\nartifact that reveals the path to the legendary weapon capable of defeating"
                "\nMurlock. But suddenly, one of Murlock’s minions emerges from the shadows,"
                "\n blocking your way." << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            std::cout << "\nKael,You have entered the Level " << level << ". You are about to retrieve ";
            player.displayItems(level + 1);
            std::cout << " a crucial artifact." << endl;
            break;
        }
        Utility::waitForEnter();
        if (level == 1)
        {
            std::cout << "\nMinion: You'll never lay your hands on the map prepare to die!" << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            Utility::waitForEnter();
            std::cout << "\nYou: You're just another obstacle in my path. Let's finish this!" << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            Utility::waitForEnter();
        }
        switch (level)
        {
        case 1:
            enemy.minions.push_back(Enemy("Goblin", 55, 55, 10, 5, 2));
            break;

        case 2:
            enemy.minions.push_back(Enemy("Goblin", 55, 55, 10, 5, 2));
            enemy.minions.push_back(Enemy("Troll", 70, 70, 20, 10, 4));
            break;
        case 3:
            enemy.minions.push_back(Enemy("Goblin", 55, 55, 10, 5, 2));
            enemy.minions.push_back(Enemy("Troll", 70, 70, 20, 10, 4));
            enemy.minions.push_back(Enemy("Orge", 100, 100, 40, 25, 8));
            break;
        case 4:
            enemy.minions.push_back(Enemy("Goblin", 55, 55, 10, 5, 2));
            enemy.minions.push_back(Enemy("Troll", 70, 70, 20, 12, 4));
            enemy.minions.push_back(Enemy("Orge", 100, 120, 40, 25, 8));
            enemy.minions.push_back(Enemy("Dark Elf", 160, 160, 80, 45, 15));
            break;
        case 5:
            enemy.minions.push_back(Enemy("Goblin", 55, 55, 10, 5, 2));
            enemy.minions.push_back(Enemy("Troll", 70, 70, 20, 10, 10));
            enemy.minions.push_back(Enemy("Orge", 100, 120, 40, 25, 20));
            enemy.minions.push_back(Enemy("Dark Elf", 160, 160, 80, 45, 45));
            enemy.minions.push_back(Enemy("Hobgoblin", 230, 230, 160, 70, 90));
            break;
        case 6:
            enemy.minions.push_back(Enemy("Murlock", 1300, 1300, 500, 350, 200));
            break;
        }
        displayLevelStats(level, player, enemy);
    }

    void Game::displayLevelStats(int level, Player& player, Enemy& enemy)
    {
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        std::cout << "                       Level " << level << " : stats                       " << endl;
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        player.displayPlayerStat(level);
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        enemy.displayEnemyStat(level);
        battleManager.startBattle(level);
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
    }

    void Game::increasePlayerStat(int level, Player& player)
    {
        player.increasePlayerStat(level);
    }
}

int main() {
    srand(time(0));
    N_Game::Player player;
    N_Game::Game game;

    game.gameLoop(player);

    return 0;
}
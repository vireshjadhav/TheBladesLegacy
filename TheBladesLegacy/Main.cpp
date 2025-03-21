#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace std;

//Namespace for utility functions such as clearing console, wait for input and clear input buffer  
namespace N_Utility
{
    class Utility
    {
    public:
        //Function to clear screen based on the operating system
        static void clearConsole()
        {
#ifdef _WIN32
            system("cls");          //for windows
#else
            (void)system("clear");      //for other systems
#endif
        }

        //Function to wait till user press Enter
        static void waitForEnter()
        {
            cin.get();
        }

        //Function to clear any buffer and handle any leftover input
        static void clearInputBuffer()
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    };
}

// Namespace for character related classes and enum classes
namespace N_Character
{
    //Enum class that contains item that player can acquire
    enum class Item
    {
        NONE = 1, MAP, SWORD, SHIELD, ARMOUR, BOW
    };

    //Base class For character (player and enemies derived classes
    class Character
    {
    protected:
        string name;                    //Name of the character
        int health;                     //Current health
        int maxHealth;                  //Maximum health
        int meleeDamage;                //Damage dealt by melee attack
        int rangedDamage;               //Damage dealt by ranged attack
        int defense;                    //Defense health points

    public:
        //Default constructor 
        Character()
        {
            name = "Unknown";
            health = 60;
            maxHealth = 60;
            rangedDamage = 20;
            meleeDamage = 10;
            defense = 3;
        }

        //Parameterized constructor
        Character(string name, int health, int maxHealth, int meleeDamage, int rangedDamage, int defense) :
            name(name), health(health), maxHealth(maxHealth), meleeDamage(meleeDamage), rangedDamage(rangedDamage), defense(defense) {
        }

        //Copy constructor
        Character(const Character& other)
        {
            name = other.name;
            health = other.health;
            maxHealth = other.maxHealth;
            meleeDamage = other.meleeDamage;
            rangedDamage = other.rangedDamage;
            defense = other.defense;
        }

        //destructor
        ~Character() {}

        //Display the items depending upon level
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

        //Pure virtual function for attacking 
        virtual void attack(Character& target, int index) = 0;

        //Getters for character atributes which are protected 
        string getName() { return name; }
        int getHealth() { return health; }
        int getMaxHealth() { return maxHealth; }
        int getMeleeDamage() { return meleeDamage; }
        int getRangedDamage() { return rangedDamage; }
        int getDefence() { return defense; }
        bool isFainted() const { return health <= 0; }

        //Function to heal
        void heal(int healAmount)
        {
            health += healAmount;
            if (health >= maxHealth) health = maxHealth;
        }

        //Function to take damage from target and return health
        int takeDamage(int damage)
        {
            int actualDamage = damage - defense;
            if (actualDamage < 0) actualDamage = 0;
            health -= actualDamage;
            if (health < 0) health = 0;
            return health;
        }
    };

    //Derived player class inherits from character class
    class Player : public Character
    {
    private:
        bool avoidNextAttack = false; //Flag to void next attack in level 5

    public:
        //Default constructor
        Player()
        {
            name = "Kael";
            health = 60;
            maxHealth = 60;
            rangedDamage = 20;
            meleeDamage = 10;
            defense = 3;
        }

        //Parameterized constructor
        Player(string p_name, int p_health, int p_maxHealth, int p_meleeDamage, int p_rangedDamage, int p_defence) :
            Character(p_name, p_health, p_maxHealth, p_meleeDamage, p_rangedDamage, p_defence) {
        }

        //Function to reset players stat to default values while restart
        void reset()
        {
            name = "Kael";
            health = 60;
            maxHealth = 60;
            rangedDamage = 20;
            meleeDamage = 10;
            defense = 3;
        }

        bool shouldAvoidAttack() const { return avoidNextAttack; }  //getter for avoid attack 

        void resetAvoidAttack() { avoidNextAttack = false; }  //Reset avoid attack flag

        //Function to display player stats based on levels and passing levels to displayItems method 
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

        //Function to increase player stat after each level
        void increasePlayerStat(int level)
        {
            switch (level)
            {
            case 1:
                maxHealth += 30;
                health = maxHealth;
                rangedDamage += 15;
                meleeDamage += 10;
                defense += 5;
                break;
            case 2:
                maxHealth += 40;
                health = maxHealth;
                rangedDamage += 20;
                meleeDamage += 15;
                defense += 7;
                break;
            case 3:
                maxHealth += 50;
                health = maxHealth;
                rangedDamage += 25;
                meleeDamage += 20;
                defense += 10;
                break;
            case 4:
                maxHealth += 60;
                health = maxHealth;
                rangedDamage += 30;
                meleeDamage += 20;
                defense += 12;
                break;
            case 5:
                maxHealth += 60;
                health = maxHealth;
                rangedDamage += 35;
                meleeDamage += 30;
                defense += 15;
                break;
            }
        }

        //Function to override attack method form base class Character
        void attack(Character& target, int index) override
        {
            int chooseRangeDamage = rand() % 20;         //Randomly chose to attack type 
            if (chooseRangeDamage == 0 && rand() % 5 == 0)   //Critical hit move logic (2 % chances)
            {
                cout << "The double ranged hit move is ready and now in action." << endl;
                int tempDamage = 2 * getRangedDamage();        //Variable to perform temperary dmage  and giving damage twite of the ranged damage
                target.takeDamage(tempDamage);      //passing temperary damage to takedamage method of current object 
            }
            else if (chooseRangeDamage == 2 || chooseRangeDamage == 5 || chooseRangeDamage == 11 || chooseRangeDamage ==17) //Ranged attack damage logic (20 % chances) 
            {
                cout << getName() << " launches a powerful ranged attack!" << endl;
                target.takeDamage(getRangedDamage());
                if (index >= 5 && (chooseRangeDamage ==  5 || chooseRangeDamage == 15))
                {
                    avoidNextAttack = true;
                    cout << getName() << "avoids the next enemy attack!" << endl;
                }
            }
            else //Default melee attack (70% chances)
            {
                int chooseMeleeDamage = rand() % 10;
                int damageDealt = getMeleeDamage();
                target.takeDamage(damageDealt);
                cout << getName() << " strikes with a devastating melee attack!" << endl;

                // Level 2 ability: Critical hit (10% chance)
                if (index >= 2 && chooseMeleeDamage == 0)
                {
                    int criticalDamage = damageDealt * 2;
                    cout << "Critical hit! " << getName() << " deals " << criticalDamage << " damage!" << endl;
                    target.takeDamage(criticalDamage);
                }

                // Level 4 ability: Life steal (10% chance)
                if (index >= 4 && chooseMeleeDamage == 2)
                {
                    int lifeStealAmount = damageDealt / 2;
                    heal(lifeStealAmount);
                    cout << getName() << " steals " << lifeStealAmount << " HP from the enemy!" << endl;
                }
            }
        }

    }; 

    //Derived Enemy class inherits from character class
    class Enemy : public Character
    {
    public:
        //Vector to store enemies
        vector<Enemy> minions;

        //Default constructor
        Enemy()
        {
            name = "Goblin";
            health = 50;
            maxHealth = 50;
            rangedDamage = 15;
            meleeDamage = 10;
            defense = 2;
        }

        //Parameterized constructor 
        Enemy(string e_name, int e_health, int e_maxHealth, int e_meleeDamage, int e_rangedDamage, int e_defence) :
            Character(e_name, e_health, e_maxHealth, e_meleeDamage, e_rangedDamage, e_defence) {
        }

        //Function to get enemies name 
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
                return "Unknown";
        }

        //Function to override attack method form base class Character
        void attack(Character& target, int index) override
        {
            if (getName() == "Murlock")//ability only for Murlock (final boss)
            {
                int murlockAbility = rand() % 3;
                switch (murlockAbility)
                {
                case 0: //Ground slash
                    cout << "Murlock uses Ground Slash!" << endl;
                    target.takeDamage(getMeleeDamage() * 1.5);
                    break;
                case 1:  //Speed Dash
                    cout << "Murlock uses Speed Dash!" << endl;
                    target.takeDamage(getMeleeDamage());
                    defense += 5;
                    break;
                case 2: // Health Regeneration
                    cout << "Murlock regenerates health!" << endl;
                    heal(50);
                    break;
                }
            }
            else  //Normal enemy attacks 
            {
                int chooseDamage = rand() % 3;  //Variable to chose attack type 
                if (chooseDamage == 2)  //Ranged attack type logic (33 % chances) 
                {
                    std::cout << getName() << " unleashes a ranged attack!" << endl;  //Getting name of object and providing type of attack with print function
                    target.takeDamage(getRangedDamage());
                }
                else
                {
                    //Setting melee attack as default attack type (67 % chances) 
                    std::cout << getName() << " unleashes a melee attack!" << endl;
                    target.takeDamage(getMeleeDamage());
                }
            }
        }

        //Fucntion to display enemy stat by level 
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
}

//Namespace game providing forward declaration of game class
namespace N_Game
{
    class Game;  //Forward declaration for Narration class
}

//Namespace for narration and story-related functions with narration class
namespace N_Narration
{
    class Narration
    {
    public:
        void gameTitle();       //Function to display title 
        void introMessage();    //Function to narrate introductory message
        void midStoryPart(N_Game::Game& game);  //Function to narrate story mid part
        void eldersMessage(N_Game::Game& game); //Function to display elders message
    };
}

//Namespace for the game class with main game logic
namespace N_Game
{
    using namespace N_Utility;
    using namespace N_Character;
    using namespace N_Narration;

    class Game
    {
        N_Narration::Narration* introStory; //pointer to the narration class object
        bool isPlaying = true;      //Flag to track if the game is still running
        int level = 1;            //Variable to define level in the game
        Player player;      //Player class object
        Enemy enemy;        //Enemy class object

    public:
        //Default constructor
        Game()
        {
            introStory = new Narration();
        }

        //Destructor to delete narration class object when it goes out of the scope
        ~Game()
        {
            delete introStory;
        }

        //Function to get input line and convert it to uppercase
        string getInput()
        {
            string input;
            getline(cin, input);
            input.erase(0, input.find_first_not_of(' '));  //Remove leading spaces
            input.erase(input.find_last_not_of(' ') + 1);   //Remove trailing spaces
            transform(input.begin(), input.end(), input.begin(), ::toupper);  //convert to uppercase
            return input;
        }

        //Getters for player and enemy object
        Player& getPlayer() { return player; }  
        Enemy& getEnemy() { return enemy; }

        //Main game loop
        void gameLoop()
        {
            while (true)
            {
                introStory->gameTitle();   //Display the game title and intro message
                introStory->eldersMessage(*this);   //Display the elder's message
                while (isPlaying && level <= 6)    //Loop through level 1 to 6 if player is still playing 
                {
                    enterLevel(level);  //enter the current level starting from level 1
                    if (player.isFainted())   //Check if the player is defeated and have health below or equal to 0
                    {
                        cout << "You have been defeated. Game Over!" << endl;
                        isPlaying = false; //If the player is defeated playing condition to false
                        break; //Exit the loop
                    }
                    else  //If player is not defeated or 
                    {
                        player.increasePlayerStat(level); //Function to increase the stat depending on level
                    }
                    level++;  //Plus one to keep loop going till the condition met
                }

                std::cout << "Press R to restart the game and E to exit" << endl;    //Message to restart or exit
                while (true)  //Loop to till press R o E 
                {
                    string input = getInput();
                    if (input == "R")  
                    {
                        player.reset(); //Reset function to reset all variables
                        isPlaying = true; 
                        level = 1;
                        N_Utility::Utility::clearConsole();  //Clear function to clear console befor restart
                        break;   //Exit the loop
                    }
                    else if (input == "E")
                    {
                        return; //Exit the game
                    }
                    else
                    {
                        cout << "Invalid input! Press R to restart the game and E to exit" << endl;   //Repeat the message if the condition is not met
                    }
                }
            }
        }

        //Declaration of Functions to handle user input and flow
        void handleStartInput(); //Function to handle players response to elder's request
        void handleRequestAccept();  //Function to display message and continue story if player accept the request
        void handleContinueInput();  //Function to handle the player's decision to continue or quit
        void handleContinuePart();   //Function to handle the continuation of the game
        void enterLevel(int level); //Function to enter the levels and initialize the enemy
        void displayLevelStats(int level);  //Function to display the stats depending on level
        void increasePlayerStat(int level);  //Fucntion to increase the stat of player with levels
        void displayLevelTitle(int level);  //Function to display level titles
    };
}

//Namespace for the narration and story related function
namespace N_Narration
{
    using namespace N_Utility;

    void Narration::gameTitle() // Function to display the game Title
    {
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        std::cout << "                 Eclipse of Astralon: The Blade's Legacy                   " << endl;
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        introMessage(); //Calling function to introducntion message
    }

    void Narration::introMessage() //Function to diplsay the introdunction message
    {
        std::cout << "\nWelcome, brave warrior. You are Kael of Astralon, known as Soulbinder\n"
            "a legendary hero of Luminara. You are about to embark on a journey that\n"
            "will test your courage, wisdom, and strength. The fate of a village-and\n"
            "perhaps the world-rests in your hands. Are you ready to rise to the challenge?" << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter(); //wait to pless enter
    }

    void Narration::eldersMessage(N_Game::Game& game) //Function to display the Elder's message
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
        game.handleStartInput(); //Calling function to handle start input 
    }

    void Narration::midStoryPart(N_Game::Game& game)  // Function to display the mid-story part
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
        game.handleContinueInput();  //Calling function to handle the continue input to continue or quit
    }
}

//Namespace for battle management
namespace N_BattleManager
{
    using namespace N_Game;
    using namespace N_Utility;

    //Class to manage the battle 
    class BattleManager
    {
    private:
        Game& game;  //Reference to the game object
        bool battleOngoing;  //Flag to track if the battle is ongoing
        bool playerTurn;  //Flag to track whose turn it is (Player's or Enemy's)

    public:
        //Parameterized construction
        BattleManager(Game& gameRef) : game(gameRef), battleOngoing(false), playerTurn(true) {}

        //Function to start a battle
        void startBattle(int level)
        {
            playerTurn = true; 
            battleOngoing = true;
            battle(level); //Calling battle function to begin the battle by passing levels to it
        }

        //Function to stop the battle
        void stopBattle()
        {
            battleOngoing = false;
        }

        //Function to handle the battle logic
        void battle(int level)
        {
            int healCooldown = 0;  //Variable to cooldown for healing ability

            for (size_t i = 0; i < game.getEnemy().minions.size(); i++)
            {
                battleOngoing = true; // Reset battle state for each minion
                while (battleOngoing)
                {
                    if (playerTurn)
                    {
                        // Prompt the player to select a move
                        std::cout << "Select from the following moves!" << endl;
                        std::cout << "1. Attack" << endl;
                        std::cout << "2. Heal" << endl;
                        string input;
                        while (true)
                        {
                            input = game.getInput();
                            if (input == "1" || input == "2")
                            {
                                break; // Valid input, exit the loop
                            }
                            else
                            {
                                std::cout << "Invalid input! Press 1 to Attack or 2 to Heal." << endl;
                            }
                        }

                        if (input == "1") 
                        {
                            // Player chooses to attack
                            game.getPlayer().attack(game.getEnemy().minions[i], i);
                            if (game.getEnemy().minions[i].isFainted())
                            {
                                std::cout << "The " << game.getEnemy().minions[i].getEnemyName() << " falls to the ground, defeated!" << endl;
                            }
                            else
                            {
                                std::cout << game.getEnemy().minions[i].getEnemyName() << " has " << game.getEnemy().minions[i].getHealth() << " HP left." << endl;
                            }
                        }
                        else if (input == "2")
                        {
                            // Player chooses to heal
                            if (healCooldown == 0)
                            {
                                int healAmount = 20;
                                switch (level)
                                {
                                case 1:
                                    healAmount = 20;
                                    break;
                                case 2:
                                    healAmount = 30;
                                    break;
                                case 3:
                                    healAmount = 40;
                                    break;
                                case 4:
                                    healAmount = 50;
                                    break;
                                case 5:
                                    healAmount = 70;
                                    break;
                                case 6:
                                    healAmount = 90;
                                    break;
                                }
                                game.getPlayer().heal(healAmount);
                                healCooldown = 3;  //Set cooldown for healing 3 turns
                                cout << game.getPlayer().getName() << " heals for " << healAmount << " HP!" << endl;
                            }
                            else
                            {
                                cout << game.getPlayer().getName() << " cannot heal yet. Cooldown: " << healCooldown << " turns remaining and wasted one turn." << endl;
                            }
                        }

                        if (healCooldown > 0)
                        {
                            healCooldown--; //Decrease cooldown each turn till it is 0
                        }
                    }
                    else
                    {
                        if(game.getPlayer().shouldAvoidAttack())
                        {
                            cout << game.getPlayer().getName() << " avoids the enemy's attack!" << endl;
                            game.getPlayer().resetAvoidAttack(); // Reset the flag
                        }
                        int chooseMove = rand() % 10; //For enemy choose move logic
                        if (chooseMove == 2) //Gives enemy the 10 % chances to choose heal
                        {
                            game.getEnemy().minions[i].heal(25);
                            std::cout << game.getEnemy().minions[i].getEnemyName() << " healed for 25 HP!" << endl;
                        }
                        else
                        {
                            //Setting attack move as default with 90% chances
                            game.getEnemy().minions[i].attack(game.getPlayer(), i);
                            if (!game.getPlayer().isFainted())
                            {
                                cout << game.getPlayer().getName() << " has " << game.getPlayer().getHealth() << " HP left." << endl;
                            }
                        }
                    }

                    // Update battle state
                    updateBattleState(i);

                    // Toggle player turn
                    playerTurn = !playerTurn;

                    // Wait for player input to continue
                    Utility::waitForEnter();
                }

                // Check if the player is defeated
                if (game.getPlayer().isFainted())
                {
                    break;
                }
            }

            // Handle battle outcome
            handleBattleOutcome(level);
        }

        //Function to update the battle state if player or enemy  fainted
        void updateBattleState(size_t i)
        {
            if (game.getEnemy().minions[i].isFainted()) //Enemy is defeated
            {
                battleOngoing = false;
            }
            else if (game.getPlayer().isFainted()) //Player is defeated
            {
                battleOngoing = false;
            }
        }

        //Function to handle the outcome of the battle
        void handleBattleOutcome(int level)
        {
            if (game.getPlayer().isFainted())  //If Player is defeated
            {
                std::cout << game.getPlayer().getName() << " has been defeated! You lose the Battle." << endl;
            }
            else //The enemy is defeated and player wins
            {
                switch (level) 
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    std::cout << "You defeated the Murlocks minions! You have unlocked ";
                    game.getPlayer().displayItems(level + 1);  //Display the unlock item
                    std::cout << "!" << endl;
                    break;
                case 6: //Win message after Final bass defeated
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

//Namespace for the main game logic
namespace N_Game {

    using namespace N_Utility;
    using namespace N_Character;
    using namespace N_BattleManager;

    //Function to handle the player's response to the enlder's request
    void Game::handleStartInput()
    {
        std::cout << "\nI see hesitation in your eyes, warrior. Will you accept (A) or decline (D)\nthe elder's request?" << endl;
        while (true)
        {
            string input = getInput();
            if (input == "A")  //Player accepts the request
            {
                handleRequestAccept();
                break;
            }
            else if (input == "D")   //Player declines the request
            {
                std::cout << "You have chosen to turn away from this quest. The village of Luminara falls";
                std::cout << "\ninto darkness, and the Eclipse Blade is lost forever. Murlock's reign of";
                std::cout << "\nterror begins, and the world of Aetheris is plunged into chaos. The end." << endl;
                isPlaying = false;  //Set flag to false
                return;
            }
            else
            {
                std::cout << "Invalid input! Press A to accept the elder's request or D to deny it." << endl;
            }
        }
    }

    //Function to handle the player accepting the elder's request
    void Game::handleRequestAccept()
    {
        std::cout << "You have chosen to accept this perilous quest, Kael. Listen closely,"
            "\nfor the tale of Luminara is one of both beauty and tragedy." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        introStory->midStoryPart(*this);  //Continues to the mid-story part
    }
    
    //Function to handle the player's decision to continue or quit
    void Game::handleContinueInput()
    {
        while (true)  //Keep the loop till valid input is provided
        {
            string input = getInput();
            if (input == "C")  //Player chooses to continue
            {
                handleContinuePart(); //Call function to handle continuation part
                break;
            }
            else if (input == "Q") //Player chooses to quit
            {
                std::cout << "Thank you for visiting Eldoria. May your next adventure be even grander!" << endl;
                isPlaying = false;
                return;
            }
            else  //Default message
            {
                std::cout << "Invalid input! Press C to continue the journey or Q to quit." << endl;
            }
        }
    }

    //Function to handle the continuation of the game
    void Game::handleContinuePart()
    {
        Utility::clearConsole();
        std::cout << "You have chosen to continue. Your quest will take you through six trials,"
            "\neach more challenging than the last. Along the way, you will face Murlock's"
            "\nminions and acquire powerful items to aid you in your final battle. Prepare"
            "\nyourself, brave one, for the path ahead is treacherous." << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        std::cout << "Before the battle begins, remember that healing has a cooldown. you must wait\nthree turns before using it again or you will loose one turn" << endl;
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
    }

    //Function to display the title of the current level
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

    //Function to enter a level and initialize enemies
    void Game::enterLevel(int level)
    {
        enemy.minions.clear(); //Clear previous enemies, usefull in loop
        Utility::clearConsole();
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        displayLevelTitle(level);
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        switch (level) //Message to the player depending on current level
        {
        case 1:
            std::cout << "\nKael,You have entered the Level " << level << ". You are about to retrieve ";
            player.displayItems(level + 1);
            std::cout << " a crucial "
                "\nartifact that reveals the path to the legendary weapon capable of defeating"
                "\nMurlock. But suddenly, one of Murlock’s minions emerges from the shadows,"
                "\nblocking your way." << endl;
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
        case 6:
            std::cout << "\nKael, the moment has come. You finally stand before the Murlock";
        }
        Utility::waitForEnter();
        if (level == 1) //Conversation of player and enemies 
        {
            std::cout << "\nMinion: You'll never lay your hands on the map prepare to die!" << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            Utility::waitForEnter();
            std::cout << "\nYou: You're just another obstacle in my path. Let's finish this!" << endl;
            std::cout << "\n[Press Enter to continue...]" << endl;
            Utility::waitForEnter();
        }
        else if (level == 6)  //Conversation of player and enemies 
        {
            std::cout << "\nMurlock: You’re persistent, Kael. But your journey ends here!" << std::endl;
            std::cout << "\n[Press Enter to continue...]" << std::endl;
            Utility::waitForEnter();

            std::cout << "\nKael: Hand over the Eclipse Blade, Murlock. It belongs to Luminara—you have \nno right to hold such a legendary weapon!" << std::endl;
            std::cout << "\n[Press Enter to continue...]" << std::endl;
            Utility::waitForEnter();

            std::cout << "\nMurlock: Hah! You think you’re worthy of its power? This blade will bring \nnothing but destruction!" << std::endl;
            std::cout << "\n[Press Enter to continue...]" << std::endl;
            Utility::waitForEnter();

            std::cout << "\nKael: I’ll decide its fate. But first, I have to deal with you!" << std::endl;
            std::cout << "\n[Press Enter to continue...]" << std::endl;
            Utility::waitForEnter();

        }
        switch (level)  //Add enemies in minions vector
        {
        case 1:
            enemy.minions.push_back(Enemy("Goblin", 70, 70, 10, 15, 3));
            break;

        case 2:
            enemy.minions.push_back(Enemy("Goblin", 70, 70, 10, 15, 3));
            enemy.minions.push_back(Enemy("Troll", 120, 120, 20, 25, 5));
            break;
        case 3:
            enemy.minions.push_back(Enemy("Goblin", 70, 70, 10, 15, 3));
            enemy.minions.push_back(Enemy("Troll", 120, 120, 20, 25, 5));
            enemy.minions.push_back(Enemy("Orge", 180, 180, 30, 35, 10));
            break;
        case 4:
            enemy.minions.push_back(Enemy("Goblin", 70, 70, 10, 15, 3));
            enemy.minions.push_back(Enemy("Troll", 120, 120, 20, 25, 5));
            enemy.minions.push_back(Enemy("Orge", 180, 180, 30, 35, 10));
            enemy.minions.push_back(Enemy("Dark Elf", 250, 250, 40, 45, 15));
            break;
        case 5:
            enemy.minions.push_back(Enemy("Goblin", 70, 70, 10, 15, 3));
            enemy.minions.push_back(Enemy("Troll", 120, 120, 20, 25, 5));
            enemy.minions.push_back(Enemy("Orge", 180, 180, 30, 35, 10));
            enemy.minions.push_back(Enemy("Dark Elf", 250, 250, 40, 45, 15));
            enemy.minions.push_back(Enemy("Hobgoblin", 350, 350, 50, 55, 20));
            break;
        case 6:
            enemy.minions.push_back(Enemy("Murlock", 600, 600, 70, 80, 30));
            break;
        }
        displayLevelStats(level);  //Display level stats
    }

    //Function to display the level stat on the basis of current level
    void Game::displayLevelStats(int level)
    {
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        std::cout << "                       Level " << level << " : stats                       " << endl;
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        player.displayPlayerStat(level);  //Calling display player stat function passing level variable
        std::cout << "**-----------------------------------------------------------------------**" << endl;
        enemy.displayEnemyStat(level);  //Calling display enemy stat function passing level variable
        BattleManager battleManager(*this);
        battleManager.startBattle(level);  //Calling start battle function passing level variable
        std::cout << "\n[Press Enter to continue...]" << endl;
        Utility::waitForEnter();
    }
}


//Main function to start the game
int main() {
    srand(time(0));  //Seed the random number generator
    N_Game::Game game;   //Create the game object

    game.gameLoop();   //Start the game loop

    return 0;
}
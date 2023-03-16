#include "config.h"

//std::vector & std::pair to store the position of the bullets
vector<pair<int, int>> bulletPos;
vector<pair<int, int>> enemyBulletPos;

//std::vector & std::pair to store the benefits brought by power up items for a certain duration
vector<pair<std::chrono::system_clock::time_point, string>> notifyList;

//std::vector & std::pair to store the power-up good positions
vector<pair<int, int>> hpUpPos;
vector<pair<int, int>> damagePowerUpPos;
vector<pair<int, int>> weaponPowerUpPos;

//std::vector to store the shape of the ship + enemy ship design
vector<const char *> shipMatrix;
vector<const char *> enemyShipMatrix;

//This function extract the ship design from .txt files and put it inside the vector.
void loadShip(int &shipSize, int &enemyShipSize, vector<const char *> &shipMatrix, vector<const char *> &enemyShipMatrix)
{
    string line, line2;
    ifstream in("playerShip.txt"), in2("enemyShip.txt");
    vector<string> rawPlayerShip, rawEnemyShip;
    getline(in, line);
    shipSize = stoi(line);
    for (int i = 0; i < shipSize; i++)
    {
        getline(in, line);
        rawPlayerShip.push_back(line.c_str());
    }
    for (int i = 0; i < rawPlayerShip.size(); i++)
    {
        shipMatrix.push_back(rawPlayerShip[i].c_str());
    }
    getline(in2, line2);
    enemyShipSize = stoi(line2);
    for (int i = 0; i < enemyShipSize; i++)
    {
        getline(in2, line2);
        rawEnemyShip.push_back(line2.c_str());
    }
    for (int i = 0; i < rawEnemyShip.size(); i++)
    {
        enemyShipMatrix.push_back(rawEnemyShip[i].c_str());
    }
}

//This function extracts the highest score recorded locally within the highScore.txt file.
void loadScore(int &highScore)
{
    ifstream in("highScore.txt");
    in >> highScore;
}

//This function saves the highest score to the .txt file.
void saveScore(int highScore)
{
    ofstream out("highScore.txt");
    out << highScore;
}

//A "struct" data structure to store an enemy ship as an object.
struct enemy_ship
{
    //Initializer of the variables for the object 
    int hp;
    int max_hp;
    int shipX_enemy;
    int shipY_enemy;
    int shipX_diag;
    int shipY_diag;
    int size;
    
    //Constructor
    enemy_ship(int x, int y, int z, int a)
    {
        shipX_enemy = x;
        shipY_enemy = y;
        size = z;
        hp = a;
        shipX_diag = shipX_enemy + 2;
        shipY_diag = shipY_diag + 2;
    }
    
    //A function to return the hp of the ship object
    int health() const
    {
        return hp;
    }
    
    //A function call to print the ship object
    void printShip(bool isHit)
    {
        for (int i = 0; i < size; i++)
        {
            if (isHit)
            {
                attron(COLOR_PAIR(1));
            }
            mvprintw(shipY_enemy + i, shipX_enemy, enemyShipMatrix[i]);
            attroff(COLOR_PAIR(1));
        }
    }
};

//It stores each of the ship objects
vector<enemy_ship> mobs;

//Print out the score of the player currently attained, and the highest score available.
void printScore(int x, int score, int highScore)
{
    mvprintw(1, 2, "PLAYER 1");
    mvprintw(1, 17, "HI-SCORE");
    string scoreStr = to_string(score);
    string highScoreStr = to_string(highScore);
    char const *scoreChar = scoreStr.c_str();
    char const *highScoreChar = highScoreStr.c_str();
    mvprintw(2, 2, scoreChar);
    mvprintw(2, 17, highScoreChar);
}

//Print out the notification of powered-up item status
void printNotify(int y, int hpRegen, int damageIncrease, int enemyHPIncrease, int enemyDamage)
{
    int x, value;
    string valueStr;
    for (int i = 0; i < notifyList.size(); i++)
    {
        auto elapsedTime = std::chrono::system_clock::now(); //mark the current time
        std::chrono::duration<double> elapsed_seconds = elapsedTime - notifyList[i].first; //calculate the time passed 
        auto numSeconds = elapsed_seconds.count();
        if (numSeconds > 1)
        {
            notifyList.erase(notifyList.begin() + i);
        }
        else
        {
            if (notifyList[i].second == "hp+")
            {
                x = 5;
                valueStr = "+" + to_string(hpRegen);
            }
            else if (notifyList[i].second == "hp-")
            {
                x = 5;
                valueStr = "-" + to_string(enemyDamage);
            }
            else if (notifyList[i].second == "damage")
            {
                x = 20;
                valueStr = "+" + to_string(damageIncrease);
            }
            else if (notifyList[i].second == "diff")
            {
                x = 35;
                valueStr = "+" + to_string(enemyHPIncrease);
            }
            char const *valueChar = valueStr.c_str();
            mvprintw(y - 2, x, valueChar);
        }
    }
}

//A function to print the statistics of the player ship
void printStats(int y, int hp, int damage, int enemyHP, bool equipWeapon)
{
    mvprintw(y - 3, 2, "HP");
    mvprintw(y - 3, 12, "MULTIPLIER");
    mvprintw(y - 3, 27, "DIFFICULTY");
    string hpStr = to_string(hp);
    string damageStr = to_string(damage);
    if (equipWeapon)
    {
        damageStr = "**" + damageStr + "**";
    }
    string enemyHPStr = to_string(enemyHP);
    char const *hpChar = hpStr.c_str();
    char const *damageChar = damageStr.c_str();
    char const *enemyHPChar = enemyHPStr.c_str();
    mvprintw(y - 2, 2, hpChar);
    mvprintw(y - 2, 12, damageChar);
    mvprintw(y - 2, 27, enemyHPChar);
}

//A function to print the player ship
void printShip(int shipSize, int shipX, int shipY)
{
    for (int i = 0; i < shipSize; i++)
        mvprintw(shipY + i, shipX, shipMatrix[i]);
}

//A function to print all the enemy ships
void printEnemyShip()
{
    for (int i = 0; i < mobs.size(); i++)
    {
        mobs[i].printShip(false);
    }
}

//A function to randomly generate the enemy ships
void spawnEnemyShip(int x, int y, int &current_ship, int enemy_hp, int enemyShipSize)
{
    int tempY, tempX;
    while (1)
    {
        tempY = rand() % (y / 2);
        tempX = rand() % x;
        if (mvinch(tempY, tempX) == ' ')
        {
            bool flag = true;

            for (int i = 1; i <= enemyShipSize + 1; i++)
            {
                for (int j = 1; j <= enemyShipSize + 1; j++)
                {
                    if (mvinch(tempY - i, tempX - j) != ' ')
                    {
                        flag = false;
                        break;
                    }
                }
            }
            if (tempX + enemyShipSize > x - 1)
                flag = false;
            if (flag)
                break;
        }
    }
    enemy_ship temp(tempX, tempY, enemyShipSize, enemy_hp);
    mobs.push_back(temp);
    current_ship = mobs.size();
}

//A function to print the locations of the damage power-up item
void printDamagePowerUp()
{
    for (int i = 0; i < damagePowerUpPos.size(); i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(damagePowerUpPos[i].second, damagePowerUpPos[i].first, "D");
        attroff(COLOR_PAIR(2));
    }
}

//A function to print the locations of the weapon power-up item
void printWeaponPowerUp()
{
    for (int i = 0; i < weaponPowerUpPos.size(); i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(weaponPowerUpPos[i].second, weaponPowerUpPos[i].first, "W");
        attroff(COLOR_PAIR(2));
    }
}

//A function to print the locations of Increase HP power-up item
void printHPUp()
{
    for (int i = 0; i < hpUpPos.size(); i++)
    {
        attron(COLOR_PAIR(2));
        mvprintw(hpUpPos[i].second, hpUpPos[i].first, "H");
        attroff(COLOR_PAIR(2));
    }
}

//A function to randomly generate power up weapons.
void spawnPowerUp(float period, int x, int y, std::chrono::system_clock::time_point &powerUpStartTime, vector<pair<int, int>> &powerUpPos)
{
    auto elapsedTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = elapsedTime - powerUpStartTime;
    auto numSeconds = elapsed_seconds.count();
    if (period < numSeconds)
    {
        powerUpStartTime = std::chrono::system_clock::now();
        powerUpPos.erase(powerUpPos.begin(), powerUpPos.end());
        while (true)
        {
            int tempX = rand() % x;
            int tempY = y / 2 + rand() % (y / 4);

            if (mvinch(tempY, tempX) == ' ') //search for a blank position to place the power up item
            {
                powerUpPos.push_back(std::make_pair(tempX, tempY));
                break;
            }
        }
    }
}

//A function to print the bullets made by player.
void printBullets(bool equipWeapon)
{
    for (int i = 0; i < bulletPos.size(); i++)
    {
        if (bulletPos[0].second > 0)
        {
            if (equipWeapon)
            {
                attron(COLOR_PAIR(5)); //Modification of color
                mvprintw(bulletPos[i].second, bulletPos[i].first, "O"); //Change the bullet indicator
                mvprintw(bulletPos[i].second, bulletPos[i].first + 2, "O");
                attroff(COLOR_PAIR(5));
            }
            else
            {
                attron(COLOR_PAIR(4)); //White color configuration
                mvprintw(bulletPos[i].second, bulletPos[i].first, "*");
                mvprintw(bulletPos[i].second, bulletPos[i].first + 2, "*");
                attroff(COLOR_PAIR(4));
            }
        }
    }
    if (bulletPos[0].second <= 0)
        bulletPos.erase(bulletPos.begin()); //Delete the bullet when it is out of boundary
}

//A function to print the bullets made by enemy.
void printEnemyBullets(int y)
{
    for (int i = 0; i < enemyBulletPos.size(); i++)
    {
        if (enemyBulletPos[i].second >= y - 1)
            remove(enemyBulletPos.begin(), enemyBulletPos.end(), std::make_pair(enemyBulletPos[i].first, enemyBulletPos[i].second));
        attron(COLOR_PAIR(3));
        mvprintw(enemyBulletPos[i].second, enemyBulletPos[i].first, "|");
        attroff(COLOR_PAIR(3));
    }
}

//A function to detect whether the bullet hit the ships, including player ship and enemy ship.
void hitDetection(int &score, int &damage, int &hp, int enemyDamage, int hpRegen, int damageIncrease, bool &equipWeapon, std::chrono::system_clock::time_point &equipWeaponStartTime, int shipSize, int shipX, int shipY)
{
    bool status = false;
    for (int i = 0; i < bulletPos.size(); i++)
    {
        status = false;
        if (mvinch(bulletPos[i].second - 1, bulletPos[i].first) != ' ')
        {
            for (int j = 0; j < mobs.size(); j++)
            {
                if (bulletPos[i].first >= mobs[j].shipX_enemy && bulletPos[i].first <= mobs[j].shipX_enemy + 2)
                {
                    mobs[j].hp = mobs[j].hp - damage;
                    mobs[j].printShip(true);
                    status = true;
                    break;
                }
            }
        }
        if (mvinch(bulletPos[i].second - 1, bulletPos[i].first + 2) != ' ')
        {
            for (int j = 0; j < mobs.size(); j++)
            {
                if (bulletPos[i].first + 2 >= mobs[j].shipX_enemy && bulletPos[i].first + 2 <= mobs[j].shipX_enemy + 2)
                {
                    mobs[j].hp = mobs[j].hp - damage;
                    mobs[j].printShip(true);
                    status = true;
                    break;
                }
            }
        }
        if (status)
        {
            score += 10;
            remove(bulletPos.begin(), bulletPos.end(), std::make_pair(bulletPos[i].first, bulletPos[i].second));
            break;
        }
    }

    for (int i = 0; i < enemyBulletPos.size(); i++)
    {
        if (mvinch(enemyBulletPos[i].second + 1, enemyBulletPos[i].first) != ' ')
        {
            if (enemyBulletPos[i].first >= shipX && enemyBulletPos[i].first < shipX + shipSize)
            {
                if (enemyBulletPos[i].second >= shipY && enemyBulletPos[i].second < shipY + shipSize)
                {
                    hp -= enemyDamage;
                    notifyList.push_back(make_pair(std::chrono::system_clock::now(), "hp-"));
                    remove(enemyBulletPos.begin(), enemyBulletPos.end(), std::make_pair(enemyBulletPos[i].first, enemyBulletPos[i].second));
                    break;
                }
            }
        }
    }

    for (int i = 0; i < damagePowerUpPos.size(); i++) //Detection of increased damage powered-up bullets
    {
        if (damagePowerUpPos[i].first >= shipX && damagePowerUpPos[i].first < shipX + shipSize)
        {
            if (damagePowerUpPos[i].second >= shipY && damagePowerUpPos[i].second < shipY + shipSize)
            {
                damage += damageIncrease;
                score += 100;
                notifyList.push_back(make_pair(std::chrono::system_clock::now(), "damage"));
                damagePowerUpPos.erase(damagePowerUpPos.begin() + i);
                break;
            }
        }
    }

    for (int i = 0; i < weaponPowerUpPos.size(); i++) //Detection of powered-up bullets hit
    {
        if (weaponPowerUpPos[i].first >= shipX && weaponPowerUpPos[i].first <= shipX + shipSize)
        {
            if (weaponPowerUpPos[i].second >= shipY && weaponPowerUpPos[i].second <= shipY + shipSize)
            {
                score += 500;
                equipWeapon = true;
                equipWeaponStartTime = std::chrono::system_clock::now();
                weaponPowerUpPos.erase(weaponPowerUpPos.begin() + i);
                break;
            }
        }
    }

    for (int i = 0; i < hpUpPos.size(); i++)
    {
        if (hpUpPos[i].first >= shipX && hpUpPos[i].first <= shipX + shipSize)
        {
            if (hpUpPos[i].second >= shipY && hpUpPos[i].second <= shipY + shipSize)
            {
                if (hp < 5)
                {
                    hp += hpRegen;
                    notifyList.push_back(make_pair(std::chrono::system_clock::now(), "hp+"));
                }
                score += 300;
                hpUpPos.erase(hpUpPos.begin() + i);
                break;
            }
        }
    }
}

//Check if the enemy ships are down, remove them and add points if they are
void checkShipStatus(int &current_ship, int &score)
{
    vector<int> val;
    for (int i = 0; i < mobs.size(); i++)
    {
        if (mobs[i].health() <= 0)
            val.push_back(i);
    }
    //A range-based for loop to iterate the downed ships
    for (auto i : val)
    {
        score += 100;
        mobs.erase(mobs.begin() + i);
        current_ship -= 1;
    }
}

//Add the bullets for player ship
void addBullets(bool equipWeapon, int shipX, int shipY, std::chrono::system_clock::time_point &bulletStartTime)
{
    int distance;
    float period;

    auto elapsedTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = elapsedTime - bulletStartTime;
    auto numSeconds = elapsed_seconds.count();
    if (equipWeapon)
    {
        period = 0.01;
        distance = 1;
    }
    else
    {
        period = 0.05;
        distance = 2;
    }
    if (period < numSeconds)
    {
        bulletStartTime = std::chrono::system_clock::now();
        for (auto &i : bulletPos)
        {
            i.second -= distance;
        }
        bulletPos.push_back(std::make_pair(shipX, shipY - 1));
    }
}

//Add the bullets for enemy ships
void addEnemyBullets(float period, float sleepTime, int shipX, int shipY, std::chrono::system_clock::time_point &enemyBulletStartTime, std::chrono::system_clock::time_point &enemyBulletSleepTime)
{
    auto elapsedTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = elapsedTime - enemyBulletStartTime;
    std::chrono::duration<double> elapsedSleepTime = elapsedTime - enemyBulletSleepTime;

    auto numSeconds = elapsed_seconds.count();
    auto sleepSeconds = elapsedSleepTime.count();
    if (period < numSeconds)
    {
        enemyBulletStartTime = std::chrono::system_clock::now();
        for (auto &i : enemyBulletPos)
        {
            i.second += 2;
        }
    }
    if (period < numSeconds && sleepSeconds < sleepTime)
    {
        for (int i = 0; i < mobs.size(); i++)
        {
            enemyBulletPos.push_back(std::make_pair(mobs[i].shipX_enemy + 1, mobs[i].shipY_enemy + 1));
        }
    }
    else if (sleepSeconds > sleepTime * 2)
        enemyBulletSleepTime = std::chrono::system_clock::now();
}

//Reset the game state when it finished
void resetGameState()
{
    mobs.clear();
    notifyList.clear();
    bulletPos.clear();
    enemyBulletPos.clear();
    hpUpPos.clear();
    damagePowerUpPos.clear();
    weaponPowerUpPos.clear();
}

//UI and the function for running the game
void gameBoard(int x, int y)
{
    while (true)
    {
        erase();
        //Initialization of necessary variables
        auto gameStartTime = std::chrono::system_clock::now();
        auto inputStartTime = std::chrono::system_clock::now();
        auto notifyStartTime = std::chrono::system_clock::now();
        auto bulletStartTime = std::chrono::system_clock::now();
        auto enemyBulletStartTime = std::chrono::system_clock::now();
        auto enemyBulletSleepTime = std::chrono::system_clock::now();

        auto hpUpStartTime = std::chrono::system_clock::now();
        auto damagePowerUpStartTime = std::chrono::system_clock::now();
        auto weaponPowerUpStartTime = std::chrono::system_clock::now();
        auto equipWeaponStartTime = std::chrono::system_clock::now();

        int shipSize, enemyShipSize;

        int hpRegen = 1;
        int enemyDamage = 1;
        int damageIncrease = 1;
        int enemyHPIncrease = 5;
        int enemyHPDuration = 5;

        int hp = 5;
        int score = 0;
        int damage = 10;
        int highScore = 0;
        int enemyHP = 300;
        bool equipWeapon = false;

        //Load the highest score locally and the ship designs.
        loadScore(highScore);
        loadShip(shipSize, enemyShipSize, shipMatrix, enemyShipMatrix);

        int shipX = x / 2;
        int shipY = y / 4 * 3;
        bulletPos.push_back(std::make_pair(shipX, shipY - 1));

        int equipWeaponDuration = 10;
        int number = 5;
        int counter = 0;
        int current_ship = 0;

        //Initialization of the game border
        box(stdscr, 0, 0);
        printShip(shipSize, shipX, shipY);
        for (int i = 0; i < 5; i++)
            spawnEnemyShip(x, y, current_ship, enemyHP, enemyShipSize);

        int prevCh = 0;
        int ch = getch();
        while (ch != 27) // if the player presses the escape button, it exits
        {
            auto nowTime = std::chrono::system_clock::now();
            std::chrono::duration<double> inputElapsedTime = nowTime - inputStartTime;
            auto inputElapsedSeconds = inputElapsedTime.count();
            if (ch == ERR)
            {
                ch = prevCh;
            }

            // WASD and arrow key controls
            if (ch == 'w' || ch == KEY_UP)
            {
                if (shipY > 2 && 0.05 < inputElapsedSeconds)
                {
                    shipY--;
                    inputStartTime = std::chrono::system_clock::now();
                }
            }
            else if (ch == 'a' || ch == KEY_LEFT)
            {
                if (shipX > 1 && 0.025 < inputElapsedSeconds)
                {
                    shipX--;
                    inputStartTime = std::chrono::system_clock::now();
                }
            }
            else if (ch == 's' || ch == KEY_DOWN)
            {
                if (shipY < y - 1 - shipSize && 0.05 < inputElapsedSeconds)
                {
                    shipY++;
                    inputStartTime = std::chrono::system_clock::now();
                }
            }
            else if (ch == 'd' || ch == KEY_RIGHT)
            {
                if (shipX < x - 1 - shipSize && 0.025 < inputElapsedSeconds)
                {
                    shipX++;
                    inputStartTime = std::chrono::system_clock::now();
                }
            }
            erase();
            box(stdscr, 0, 0);

            std::chrono::duration<double> gameElapsedTime = nowTime - gameStartTime;
            auto gameElapsedSeconds = gameElapsedTime.count();
            if (gameElapsedSeconds > enemyHPDuration)
            {
                enemyHP += enemyHPIncrease;
                gameStartTime = std::chrono::system_clock::now();
                notifyList.push_back(make_pair(std::chrono::system_clock::now(), "diff"));
            }
            if (hp <= 0)
            {
                break;
            }

            std::chrono::duration<double> equipWeaponElapsedTime = nowTime - equipWeaponStartTime;
            auto equipWeaponElapsedSeconds = equipWeaponElapsedTime.count();
            if (equipWeaponElapsedSeconds > equipWeaponDuration)
            {
                equipWeapon = false;
            }
            
            //Calling all functions for interaction
            printShip(shipSize, shipX, shipY);
            loadShip(shipSize, enemyShipSize, shipMatrix, enemyShipMatrix); // spegetii fix for ship disappearing
            printBullets(equipWeapon);
            printEnemyBullets(y);
            printEnemyShip();
            printHPUp();
            printDamagePowerUp();
            printWeaponPowerUp();
            printScore(x, score, highScore);
            printStats(y, hp, damage, enemyHP, equipWeapon);
            printNotify(y, hpRegen, damageIncrease, enemyHPIncrease, enemyDamage);

            addBullets(equipWeapon, shipX, shipY, bulletStartTime);
            addEnemyBullets(0.5, 3, shipX, shipY, enemyBulletStartTime, enemyBulletSleepTime);

            hitDetection(score, damage, hp, enemyDamage, hpRegen, damageIncrease, equipWeapon, equipWeaponStartTime, shipSize, shipX, shipY);
            checkShipStatus(current_ship, score);
            if (current_ship < number)
                spawnEnemyShip(x, y, current_ship, enemyHP, enemyShipSize);
            spawnPowerUp(8, x, y, damagePowerUpStartTime, damagePowerUpPos);
            spawnPowerUp(15, x, y, hpUpStartTime, hpUpPos);
            spawnPowerUp(30, x, y, weaponPowerUpStartTime, weaponPowerUpPos);

            usleep(1500);
            prevCh = ch;
            ch = getch();
        }
        if (score > highScore)
            saveScore(score);
        resetGameState();
        int choice = 0;
        tryAgain(choice, x, y);
        if (!choice)
        {
            ch = 0;
            erase();
            mvprintw(12, 12, "Loading...");
            break;
        }
    }
    int select = 1;
    menu(select, x, y);
    menuSelect(select, x, y);
}

/*
# Program name: Box Creatures
# Author: James Ramsey
# Date last updated: 12/??/23
# Purpose: Run the game box creatues
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <random>
#include <stdlib.h>
#include <windows.h>
#include <bits/stdc++.h>

using namespace std;
using namespace std::filesystem;

enum creatureColor
{
    R = 4,
    B = 1,
    G = 2,
    Y = 6,
    P = 5
};

void clearInput();
void startMenu();
void optionsMenu();
void characterMenu();
string createCharacter();
string chooseCharacter();
void deleteCharacter();
void runGame(string Name);
int runShop(int money, int win, int loss, string Name);
void createlvl1Creature(string Name);
void createlvl3Creature(string Name);
void createlvl5Creature(string Name);
void createEnemyCreature(int plvl);
void addCreature(int power, int speed, int health, creatureColor Color, int lvl, string Name);
void creatureInputFile(int power, int speed, int health, creatureColor Color, int lvl, string Name, int lineNum);
void printInventory(int choice, string Name);
void runInventory(string Name);
char runBattle(string Name);
void printBattleField(int Ecolor, int Pcolor);
int createStat(int min, int max);
int getAdvantage(creatureColor attacker, creatureColor defender);
int TypeAdv(int ECOLOR, int PCOLOR);
int playerAttack(int power, int lvl, int Attacker, int Defender);
int enemyAttack(int power, int lvl, int Attacker, int Defender);
int chooseLinNum(string Name);
bool validLine(string fileName, int lineChoice);

// classes and whatnot

struct inventory
{
    creatureColor Color;
    int Power;
    int Health;
    int Speed;
    int lvl;
};

creatureColor colorArray[5] = {R, G, B, Y, P};

const int BLOCK_MIN_WAIT = 3;
int pBlockCounter = 0;
int eBlockCounter = 0;
const int SPEC_ATT_MIN_WAIT = 3;
int pSpecAttackCounter = 0;
int eSpecAttackCounter = 0;

// attacker is the row, defender color is the column
int defattackGrid[5][5] = {
    {1, 2, 1, 1, 1},
    {1, 1, 2, 1, 1},
    {2, 1, 1, 1, 1},
    {1, 1, 1, 1, 2},
    {1, 1, 1, 2, 1}};

class Creature
{
private:
    int Power;
    int Speed;
    int Health;
    int Levels;
    creatureColor Color;

public:
    Creature(int pow, int sped, int heal, creatureColor col, int lvls) : Power(pow), Speed(sped), Health(heal), Color(col), Levels(lvls){};

    int getPower()
    {
        return Power;
    }

    int getSpeed()
    {
        return Speed;
    }

    int getHealth()
    {
        return Health;
    }

    string getColor()
    {
        string color;
        switch (Color)
        {
        case R:
            color = "Red";
            break;
        case G:
            color = "Green";
            break;
        case B:
            color = "Blue";
            break;
        case Y:
            color = "Yellow";
            break;
        case P:
            color = "Purple";
            break;

        default:
            break;
        }
        return color;
    }
};

class Player
{
private:
    int Money;
    int Wins;
    int Losses;

public:
    Player(int mon, int win, int loss) : Money(mon), Wins(win), Losses(loss){};

    int getMoney()
    {
        return Money;
    }

    int getWins()
    {
        return Wins;
    }

    int getLosses()
    {
        return Losses;
    }

    void gainMoney()
    {
        srand(time(0));
        int amount = createStat(6, 20);
        Money += amount;
    }

    void setMoney(int mon)
    {
        Money = mon;
    }

    void setWin()
    {
        Wins = Wins + 1;
    }

    void setLoss()
    {
        Losses = Losses + 1;
    }

    void saveCharacter(string Name)
    {
        string fileName = "Characters/" + Name + ".txt";
        fstream pf(fileName);
        pf << Money << " " << Wins << " " << Losses;
    }
};

class Inventory
{
private:
    inventory slot1;
    inventory slot2;
    inventory slot3;
    int Power;
    int Speed;
    int Health;
    int Level;
    creatureColor Color;

public:
    Inventory(int pow, int sped, int heal, creatureColor col, int lvls) : Power(pow), Speed(sped), Health(heal), Color(col), Level(lvls){};

    void fillSlot1()
    {
        slot1.Power = Power;
        slot1.Speed = Speed;
        slot1.Health = Health;
        slot1.Color = Color;
        slot1.lvl = Level;
    }

    void fillSlot2()
    {
        slot2.Power = Power;
        slot2.Speed = Speed;
        slot2.Health = Health;
        slot2.Color = Color;
        slot1.lvl = Level;
    }

    void fillSlot3()
    {
        slot3.Power = Power;
        slot3.Speed = Speed;
        slot3.Health = Health;
        slot3.Color = Color;
        slot1.lvl = Level;
    }

    string toString()
    {
        string color;
        switch (Color)
        {
        case R:
            color = "Red";
            break;
        case G:
            color = "Green";
            break;
        case B:
            color = "Blue";
            break;
        case Y:
            color = "Yellow";
            break;
        case P:
            color = "Purple";
            break;

        default:
            break;
        }
        return color;
    }

    void printCreature()
    {
        cout << "Color: " << toString() << endl;
        cout << "Power: " << Power << endl;
        cout << "Health: " << Health << endl;
        cout << "Speed: " << Speed << endl;
        cout << "Level: " << Level << endl;
    }
};

// main
int main()
{
    startMenu();
    return 0;
}

// functions
void clearInput()
{
    cin.clear();
    cin.ignore(INT_MAX, '\n');
}

void startMenu()
{
    int Choice;
    do
    {
        try
        {
            cout << "Welcome to box creatures!" << endl;
            cout << "1. Start battling"
                 << "\n2. Options"
                 << "\n3. Quit" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, or 3" << endl;
                cout << "1. Start"
                     << "\n2. Options"
                     << "\n3. Quit" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            system("cls");
            characterMenu();
            break;
        case 2:
            system("cls");
            optionsMenu();
            break;
        default:
            break;
        }
    } while (Choice != 3);
}

#pragma region optionsMenu

void optionsMenu()
{
    string Text;
    int Choice;
    ifstream MyReadFile("HTP.txt");
    cout << "Options Menu" << endl;
    while (Choice != 3)
    {
        try
        {
            cout << "1. How to play"
                 << "\n2. Delete characters"
                 << "\n3. Back" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, or 3" << endl;
                cout << "1. How to play"
                     << "\n2. Delete characters"
                     << "\n3. Back" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            cout << " " << endl;
            while (getline(MyReadFile, Text))
            {
                cout << Text << endl;
            }
            cout << " " << endl;
            break;
        case 2:
            deleteCharacter();
            break;
        default:
            break;
        }
    }
    system("cls");
}

void deleteCharacter()
{
    string filelocation;
    string searchfilename;
    string path = "Characters/";
    cout << "Please enter the name of the character you want to delete or hit 3 to go back:" << endl;

    for (const auto &file : directory_iterator(path))
    {
        filelocation = file.path().string();
        filelocation.erase(0, 11);
        filelocation.erase(filelocation.end() - 4, filelocation.end() - 0);
        if (filelocation == "Enemy")
        {
            ;
        }
        else
        {
            cout << filelocation << endl;
        }
    }

    cin >> searchfilename;

    if (searchfilename == "3")
    {
        return;
    }

    if (searchfilename == "Enemy")
    {
        ;
    }
    else if (remove("Characters/" + searchfilename + ".txt"))
    {
        cout << "Your character " << searchfilename << " has been deleted." << endl;
    }
    else
    {
        cout << "Character " << searchfilename << " not found." << endl;
    }
}

#pragma endregion

#pragma region characterMenu

void characterMenu()
{
    int Choice, count;
    string Name;
    string path = "Characters";
    while (Choice != 3)
    {
        try
        {
            cout << "1. Create new character"
                 << "\n2. Choose a character"
                 << "\n3. Back" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, or 3" << endl;
                cout << "1. Create new character"
                     << "\n2. Choose a character"
                     << "\n3. Back" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            Name = createCharacter();
            system("cls");
            runGame(Name);
            break;
        case 2:
            try
            {
                for (const auto &file : directory_iterator(path))
                {
                    count++;
                }
                if (count <= 1)
                {
                    throw("empty");
                }

                else
                {
                    Name = chooseCharacter();
                    system("cls");
                    runGame(Name);
                }
            }
            catch (...)
            {
                cout << "No characters have been created" << endl;
            }
            break;
        default:
            break;
        }
    }
    system("cls");
}

string createCharacter()
{
    string Name, fileName;
    try
    {
        cout << "Choose your character's name" << endl;
        cin >> Name;

        fileName = Name + ".txt";

        if (filesystem::exists("Characters/" + fileName))
        {
            throw(Name);
        }
        else if (fileName == "Enemy")
        {
            throw(Name);
        }
        else
        {
            ofstream file("Characters/" + fileName);
            file << 0 << " " << 0 << " " << 0 << endl
                 << endl
                 << endl
                 << endl
                 << endl;
            file.close();
        }
    }
    catch (string name)
    {
        while (filesystem::exists("Characters/" + fileName) || fileName == "Enemy")
        {
            clearInput();
            cout << "This character name already exists, please choose another" << endl;
            cin >> name;
            Name = name;
            fileName = Name + ".txt";
        }

        ofstream file("Characters/" + fileName);
        file << 0 << " " << 0 << " " << 0 << endl
             << endl
             << endl
             << endl
             << endl;
        file.close();
    }
    createlvl1Creature(Name);
    return Name;
}

string chooseCharacter()
{
    string Name;
    string path = "Characters/";
    string filelocation;

    cout << "What is the character's name" << endl;

    for (const auto &file : directory_iterator(path))
    {
        filelocation = file.path().string();
        filelocation.erase(0, 11);
        filelocation.erase(filelocation.end() - 4, filelocation.end() - 0);
        if (filelocation == "Enemy")
        {
            ;
        }
        else
        {
            cout << filelocation << endl;
        }
    }
    cout << " " << endl;
    cin >> Name;
    if (exists("Characters/" + Name + ".txt") && Name != "Enemy")
    {
        return Name;
    }

    while (!exists("Characters/" + Name + ".txt") || Name == "Enemy")
    {
        clearInput();
        cout << "Character does not exist please choose from the list:" << endl;
        for (const auto &file : directory_iterator(path))
        {
            filelocation = file.path().string();
            filelocation.erase(0, 11);
            filelocation.erase(filelocation.end() - 4, filelocation.end() - 0);
            if (filelocation == "Enemy")
            {
                ;
            }
            else
            {
                cout << filelocation << endl;
            }
        }
        cin >> Name;
    }
    return Name;
}

#pragma endregion

#pragma region Gamestuff
void runGame(string Name)
{
    int money, win, loss, Choice;
    char winLoss = 'N';
    string fileName = "Characters/" + Name + ".txt";
    srand(time(0));

    fstream pf(fileName);
    pf >> money >> win >> loss;
    Player player(money, win, loss);

    cout << "Welcome to the game!" << endl;
    cout << " " << endl;
    while (Choice != 4)
    {
        try
        {
            cout << "Player:" << Name << "   Wins/Loss:" << player.getWins() << "/" << player.getLosses() << "   Money:" << player.getMoney() << endl;
            cout << "1. battle"
                 << "\n2. Shop"
                 << "\n3. Inventory"
                 << "\n4. Save & Quit" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3 && Choice != 4) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3 && choice != 4) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, 3, or 4" << endl;
                cout << "1. battle"
                     << "\n2. Shop"
                     << "\n3. Inventory"
                     << "\n4. Save & Quit" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            winLoss = runBattle(Name);
            if (winLoss == 'W')
            {
                player.setWin();
                player.setMoney(money += rand() % 50 + 10);
            }
            if (winLoss == 'L')
            {
                player.setLoss();
            }

            break;
        case 2:
            player.setMoney(runShop(player.getMoney(), player.getWins(), player.getLosses(), Name));
            break;
        case 3:
            runInventory(Name);
            break;
        default:
            break;
        }
        player.saveCharacter(Name);
        system("cls");
    }
}

int runShop(int money, int win, int loss, string Name)
{
    system("cls");
    Player player(money, win, loss);
    int Choice;
    cout << "Welcome to the shop" << endl;
    while (Choice != 4)
    {
        cout << "Money: " << to_string(money) << endl;
        try
        {
            cout << "1. Buy level 1 Pet: $50"
                 << "\n2. Buy level 3 Pet: $300"
                 << "\n3. Buy level 5 Pet: $500"
                 << "\n4. Leave" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3 && Choice != 4) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3 && choice != 4) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, 3, or 4" << endl;
                cout << "1. Buy level 1 Pet: $50"
                     << "\n2. Buy level 3 Pet: $300"
                     << "\n3. Buy level 5 Pet: $500"
                     << "\n4. Leave" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            if (money >= 50)
            {
                money -= 50;
                player.setMoney(money);
                cout << "Bought level 1 Pet" << endl;
                createlvl1Creature(Name);
            }
            else
            {
                cout << "Not enough money" << endl;
            }

            break;
        case 2:
            if (money >= 300)
            {
                money -= 300;
                player.setMoney(money);
                cout << "Bought level 3 Pet" << endl;
                createlvl3Creature(Name);
            }
            else
            {
                cout << "Not enough money" << endl;
            }
            break;
        case 3:
            if (money >= 500)
            {
                money -= 500;
                player.setMoney(money);
                cout << "Bought level 5 Pet" << endl;
                createlvl5Creature(Name);
            }
            else
            {
                cout << "Not enough money" << endl;
            }
            break;
        default:
            break;
        }
    }
    system("cls");
    return money;
}

void runInventory(string Name)
{
    system("cls");
    int Choice;
    cout << "What slot do you want to see" << endl;
    while (Choice != 4)
    {
        try
        {
            cout << "1. slot 1"
                 << "\n2. slot 2"
                 << "\n3. slot 3"
                 << "\n4. back" << endl;
            cin >> Choice;
            if ((Choice != 1 && Choice != 2 && Choice != 3 && Choice != 4) || !Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3 && choice != 4) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, 3, 4" << endl;
                cout << "1. slot 1"
                     << "\n2. slot 2"
                     << "\n3. slot 3"
                     << "\n4. back" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            printInventory(Choice, Name);
            break;
        case 2:
            printInventory(Choice, Name);
            break;
        case 3:
            printInventory(Choice, Name);
            break;
        case 4:
            break;
        default:
            break;
        }
    }
    system("cls");
}

char runBattle(string Name)
{
    string fileName = "Characters/" + Name + ".txt", line, text;
    int currentLine = 0, lineNum, it=1;
    vector<int> alreadyUsed;
    char winLoss;
    bool validChoice = false;

    int Epower = 0, Espeed = 0, Ecolor = 0, Elvl = 0, Ehealth = 0, Eattack = 0, Eloss = 0;
    int Ppower = 0, Pspeed = 0, Pcolor = 0, Plvl = 0, Phealth = 0, Pattack = 0, Ploss = 0, Pamount = 0;
    fstream pf;

    pf.open(fileName);
    while (getline(pf, text))
    {
        if (it >= 3 && !text.size())
        {
            continue;
        }
        else if (it >= 3 && text.size())
        {
            Pamount++;
        }
        it++;
    }
    pf.close();

    it=1;

    while (Ploss < Pamount && Eloss < Pamount)
    {
        pBlockCounter = 0;
        eBlockCounter = 0;
        pSpecAttackCounter = 0;
        eSpecAttackCounter = 0;
        if (Phealth <= 0)
        {

            while (validChoice == false)
            {
                lineNum = chooseLinNum(Name);
                if (validLine(fileName, lineNum) == true)
                {
                    auto it = find(alreadyUsed.begin(), alreadyUsed.end(), lineNum);

                    // If element was found
                    if (it != alreadyUsed.end())
                    {
                        int index = it - alreadyUsed.begin();
                        cout << "Creature already used" << endl;
                    }
                    else
                    {
                        validChoice = true;
                    }
                }
            }
            validChoice = false;
            alreadyUsed.push_back(lineNum);

            currentLine = 0;
            pf.open(fileName);
            while (!pf.eof())
            {
                currentLine++;
                getline(pf, line);
                if (currentLine == lineNum)
                {
                    break;
                }
            }
            pf >> Pcolor >> Ppower >> Phealth >> Pspeed >> Plvl;
            pf.close();
        }

        if (Ehealth <= 0)
        {
            createEnemyCreature(Plvl);
            pf.open("Characters/Enemy.txt");
            pf >> Ecolor >> Epower >> Ehealth >> Espeed >> Elvl;
            pf.close();
        }

        do
        {
            system("cls");
            cout << "Enemy health: " << Ehealth << endl;
            printBattleField(Ecolor, Pcolor);
            cout << "Your health: " << Phealth << endl;
            cout << " " << endl;

            if (pSpecAttackCounter != 0)
            {
                pSpecAttackCounter--;
            }
            if (pBlockCounter != 0)
            {
                pBlockCounter--;
            }
            if (eSpecAttackCounter != 0)
            {
                eSpecAttackCounter--;
            }
            if (eBlockCounter != 0)
            {
                eBlockCounter--;
            }

            if (Pspeed >= Espeed)
            {
                Pattack = playerAttack(Ppower, Plvl, Pcolor, Ecolor);
                Eattack = enemyAttack(Epower, Elvl, Ecolor, Pcolor);

                if (Pattack == 0)
                {
                    continue;
                }

                if (Pattack == -1)
                {
                    cout << "You block the attack" << endl;
                    continue;
                }
                else if (Eattack == -1)
                {
                    cout << "The enemy blocks the attack" << endl;
                    continue;
                }
                else
                {

                    cout << "You attack for " << Pattack << endl;
                    Ehealth -= Pattack;

                    if (Ehealth > 0)
                    {
                        cout << "The enemy attacks for " << Eattack << endl;
                        Phealth -= Eattack;
                    }
                }
            }
            else
            {
                Pattack = playerAttack(Ppower, Plvl, Pcolor, Ecolor);
                Eattack = enemyAttack(Epower, Elvl, Ecolor, Pcolor);

                if (Pattack == 0)
                {
                    continue;
                }

                if (Pattack == -1)
                {
                    cout << "You block the attack" << endl;
                    continue;
                }
                else if (Eattack == -1)
                {
                    cout << "The enemy blocks the attack" << endl;
                    continue;
                }
                else
                {
                    cout << "The enemy attacks for " << Eattack << endl;
                    Phealth -= Eattack;

                    if (Phealth > 0)
                    {

                        cout << "You attack for " << Pattack << endl;
                        Ehealth -= Pattack;
                    }
                }
            }
            system("pause");
        } while (Phealth > 0 && Ehealth > 0);
        if (Phealth<=0)
        {
            Ploss++;
        }
        if (Ehealth<=0)
        {
            Eloss++;
        }
        
    }
    if (Eloss >= Pamount)
    {
        winLoss = 'W';
    }
    else if (Ploss >= Pamount)
    {
        winLoss = 'L';
    }

    return winLoss;
}

void printInventory(int choice, string Name)
{
    fstream pf;
    string line;
    string fileName = "Characters/" + Name + ".txt";
    creatureColor Color;
    int color, power, health, speed, lvl, currentLine = 0;
    int lineNum = choice + 1;

    pf.open(fileName);

    while (!pf.eof())
    {
        currentLine++;

        getline(pf, line);

        if (currentLine == lineNum)
            break;
    }
    pf >> color >> power >> health >> speed >> lvl;

    switch (color)
    {
    case 1:
        Color = R;
        break;
    case 2:
        Color = G;
        break;
    case 3:
        Color = B;
        break;
    case 4:
        Color = Y;
        break;
    case 5:
        Color = P;
        break;

    default:
        break;
    }

    Inventory inven(power, speed, health, Color, lvl);

    inven.printCreature();
}

#pragma endregion

#pragma region creatures

int createStat(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

void createlvl1Creature(string Name)
{
    int power = 0, speed = 0, health = 0, intcolor, lvl;

    srand(time(0));
    intcolor = createStat(1, 5);
    power = createStat(3, 10);
    speed = createStat(3, 10);
    health = createStat(3, 10);
    lvl = 1;

    creatureColor color = colorArray[intcolor - 1];

    Creature creature(power, speed, health, color, lvl);

    cout << "Congrat!!! You got a new creature" << endl;
    cout << "Color: " << creature.getColor() << endl;
    cout << "Power: " << creature.getPower() << endl;
    cout << "Health: " << creature.getHealth() << endl;
    cout << "Speed: " << creature.getSpeed() << endl;

    addCreature(power, speed, health, color, lvl, Name);
}

void createlvl3Creature(string Name)
{
    int power = 0, speed = 0, health = 0, intcolor, lvl;

    srand(time(0));
    for (int i = 0; i < 3; i++)
    {
        power += createStat(3, 10);
        speed += createStat(3, 10);
        health += createStat(3, 10);
    }
    intcolor = createStat(1, 5);
    lvl = 3;

    creatureColor color = colorArray[intcolor - 1];

    Creature creature(power, speed, health, color, lvl);

    cout << "Congrat!!! You got a new creature" << endl;
    cout << "Color: " << creature.getColor() << endl;
    cout << "Power: " << creature.getPower() << endl;
    cout << "Health: " << creature.getHealth() << endl;
    cout << "Speed: " << creature.getSpeed() << endl;

    addCreature(power, speed, health, color, lvl, Name);
}

void createlvl5Creature(string Name)
{
    int power = 0, speed = 0, health = 0, intcolor, lvl;

    srand(time(0));
    for (int i = 0; i < 5; i++)
    {

        power += createStat(3, 10);
        speed += createStat(3, 10);
        health += createStat(3, 10);
    }
    intcolor = createStat(1, 5);
    lvl = 5;

    creatureColor color = colorArray[intcolor - 1];

    Creature creature(power, speed, health, color, lvl);

    cout << "Congrat!!! You got a new creature" << endl;
    cout << "Color: " << creature.getColor() << endl;
    cout << "Power: " << creature.getPower() << endl;
    cout << "Health: " << creature.getHealth() << endl;
    cout << "Speed: " << creature.getSpeed() << endl;

    addCreature(power, speed, health, color, lvl, Name);
}

void createEnemyCreature(int plvl)
{
    string fileName = "Characters/Enemy.txt";
    fstream pf;
    int power = 0, speed = 0, health = 0, intcolor, lvl;

    srand(time(0));
    lvl = createStat(1, plvl);
    intcolor = createStat(1, 5);
    for (int i = 0; i < lvl; i++)
    {
        power += createStat(3, 10);
        speed += createStat(3, 10);
        health += createStat(3, 10);
    }

    pf.open(fileName);
    pf << intcolor << " " << power << " " << health << " " << speed << " " << lvl << endl;
    pf.close();
}

void addCreature(int power, int speed, int health, creatureColor Color, int lvl, string Name)
{

    int Choice, lineNum;

    cout << "What inventory slot would you like to put it in" << endl;

    try
    {
        cout << "1. slot 1"
             << "\n2. slot 2"
             << "\n3. slot 3"
             << "\n4. Don't get Creature" << endl;
        cin >> Choice;
        if ((Choice != 1 && Choice != 2 && Choice != 3 && Choice != 4) || !Choice)
        {
            throw(Choice);
        }
    }
    catch (int choice)
    {
        while ((choice != 1 && choice != 2 && choice != 3 && choice != 4) || !choice)
        {
            clearInput();
            cout << "Please Enter 1, 2, or 3" << endl;
            cout << "1. slot 1"
                 << "\n2. slot 2"
                 << "\n3. slot 3"
                 << "\n4. Don't get Creature" << endl;
            cin >> choice;
        }
        Choice = choice;
    }

    switch (Choice)
    {
    case 1:
        lineNum = 3;
        creatureInputFile(power, speed, health, Color, lvl, Name, lineNum);
        cout << "Added creature to slot 1" << endl;
        break;
    case 2:
        lineNum = 4;
        creatureInputFile(power, speed, health, Color, lvl, Name, lineNum);
        cout << "Added creature to slot 2" << endl;
        break;
    case 3:
        lineNum = 5;
        creatureInputFile(power, speed, health, Color, lvl, Name, lineNum);
        cout << "Added creature to slot 3" << endl;
        break;
    case 4:
        cout << "Creature was not added to inventory" << endl;
        break;
    default:
        break;
    }
}

void creatureInputFile(int power, int speed, int health, creatureColor Color, int lvl, string Name, int lineNum)
{
    fstream pf;
    vector<string> lines;
    string line;
    string fileName = "Characters/" + Name + ".txt";
    int color;

    switch (Color)
    {
    case R:
        color = 1;
        break;
    case G:
        color = 2;
        break;
    case B:
        color = 3;
        break;
    case Y:
        color = 4;
        break;
    case P:
        color = 5;
        break;

    default:
        break;
    }

    pf.open(fileName);

    while (getline(pf, line))
    {
        lines.push_back(line);
    }

    pf.close();

    pf.open(fileName);

    lineNum--;
    for (int i = 0; i < lines.size(); i++)
    {
        if (i != lineNum)
            pf << lines[i] << endl;
        else
            pf << color << " " << power << " " << health << " " << speed << " " << lvl << endl;
    }
}

#pragma endregion

#pragma region Battle

void printBattleField(int Ecolor, int Pcolor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 8);
    cout << "\n============";
    switch (Ecolor)
    {
    case 1:
        SetConsoleTextAttribute(hConsole, R);
        break;
    case 2:
        SetConsoleTextAttribute(hConsole, G);
        break;
    case 3:
        SetConsoleTextAttribute(hConsole, B);
        break;
    case 4:
        SetConsoleTextAttribute(hConsole, Y);
        break;
    case 5:
        SetConsoleTextAttribute(hConsole, P);
        break;
    default:
        break;
    }

    cout << "(;*-*;)";
    SetConsoleTextAttribute(hConsole, 8);
    cout << "===============\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 34; j++)
        {
            cout << "=";
        }
        cout << "\n";
    }
    cout << "==============";
    SetConsoleTextAttribute(hConsole, 13);
    cout << "[BC]";
    SetConsoleTextAttribute(hConsole, 8);
    cout << "================\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 34; j++)
        {
            cout << "=";
        }
        cout << "\n";
    }

    cout << "============";
    switch (Pcolor)
    {
    case 1:
        SetConsoleTextAttribute(hConsole, R);
        break;
    case 2:
        SetConsoleTextAttribute(hConsole, G);
        break;
    case 3:
        SetConsoleTextAttribute(hConsole, B);
        break;
    case 4:
        SetConsoleTextAttribute(hConsole, Y);
        break;
    case 5:
        SetConsoleTextAttribute(hConsole, P);
        break;
    default:
        break;
    }
    cout << "(;*-*;)";
    SetConsoleTextAttribute(hConsole, 8);
    cout << "===============" << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

int playerAttack(int power, int lvl, int Attacker, int Defender)
{
    // Variables
    int Choice;
    bool validChoice = false;
    int attack = 0;
    srand(time(0));

    do
    {
        try
        {
            cout << "What would you like to do" << endl;
            cout << "1. Attack  2. Color attack  3. Block" << endl;

            cin >> Choice;

            if ((Choice > 0 && Choice >= 3) && Choice)
            {
                throw(Choice);
            }
        }
        catch (int choice)
        {
            while ((choice != 1 && choice != 2 && choice != 3) || !choice)
            {
                clearInput();
                cout << "Please Enter 1, 2, or 3" << endl;
                cout << "1. Attack  2. Color attack  3. Block" << endl;
                cin >> choice;
            }
            Choice = choice;
        }

        switch (Choice)
        {
        case 1:
            attack = rand() % power + lvl;
            validChoice = true;
            break;
        case 2:
            if (pSpecAttackCounter == 0)
            {
                attack = (rand() % power + lvl);
                attack = attack * TypeAdv(Defender, Attacker);

                pSpecAttackCounter = SPEC_ATT_MIN_WAIT;
                validChoice = true;
            }
            else
            {
                cout << "You can't do that right now" << endl;
            }
            break;
        case 3:

            if (pBlockCounter == 0)
            {
                attack = -1;
                pBlockCounter = BLOCK_MIN_WAIT;
                validChoice = true;
            }
            else
            {
                cout << "You can't do that right now" << endl;
            }

            break;
        default:
            break;
        }
    } while (validChoice != true);

    return attack;
}

int enemyAttack(int power, int lvl, int Attacker, int Defender)
{
    // Variables
    int Choice;
    int attack = 0;
    srand(time(0));

    Choice = rand() % 2 + 1;

    switch (Choice)
    {
    case 1:
        attack = rand() % power + lvl;
        break;
    case 2:
        if (eSpecAttackCounter == 0)
        {
            attack = (rand() % power + lvl);
            attack = attack * TypeAdv(Defender, Attacker);

            eSpecAttackCounter = SPEC_ATT_MIN_WAIT;
        }
        else
        {
            attack = rand() % power + lvl;
        }

        break;
    case 3:

        if (eBlockCounter == 0)
        {
            attack = -1;
            eBlockCounter = BLOCK_MIN_WAIT;
        }
        else
        {
            attack = rand() % power + lvl;
        }
        break;
    default:
        break;
    }
    return attack;
}

int TypeAdv(int ECOLOR, int PCOLOR)
{
    creatureColor pcolor, ecolor;
    switch (PCOLOR)
    {
    case 1:
        pcolor = R;
        break;
    case 2:
        pcolor = G;
        break;
    case 3:
        pcolor = B;
        break;
    case 4:
        pcolor = Y;
        break;
    case 5:
        pcolor = P;
        break;
    default:
        break;
    }

    switch (ECOLOR)
    {
    case 1:
        ecolor = R;
        break;
    case 2:
        ecolor = G;
        break;
    case 3:
        ecolor = B;
        break;
    case 4:
        ecolor = Y;
        break;
    case 5:
        ecolor = P;
        break;
    default:
        break;
    }

    return getAdvantage(pcolor, ecolor);
}

int getAdvantage(creatureColor attacker, creatureColor defender)
{
    int attackerIndex = -1;
    int defenderIndex = -1;

    for (int i = 0; i < sizeof(colorArray); i++)
    {

        if (attackerIndex == -1 && colorArray[i] == attacker)
        {
            attackerIndex = i;
        }

        if (defenderIndex == -1 && colorArray[i] == defender)
        {
            defenderIndex = i;
        }

        if (attackerIndex > -1 && defenderIndex > -1)
        {
            break;
        }
    }

    // read from the defattackgrid
    int battleAdvantage = defattackGrid[attackerIndex][defenderIndex];
    return battleAdvantage;
}

int chooseLinNum(string Name)
{
    int lineNum, Choice, it = 1;
    string fileName = "Characters/" + Name + ".txt", text;
    ifstream pf;
    pf.open(fileName);
    try
    {
        cout << "Choose what creature you want to bring out" << endl;
        while (getline(pf, text))
        {

            if (it >= 3 && !text.size())
            {
                cout << "empty" << endl;
            }
            else if (it >= 3 && text.size())
            {
                cout << "Choose slot " << it - 2 << endl;
            }
            it++;
        }

        cin >> Choice;

        if ((Choice != 1 && Choice != 2 && Choice != 3) || !Choice)
        {
            throw(Choice);
        }
    }
    catch (int choice)
    {
        while ((Choice != 1 && Choice != 2 && Choice != 3) || !choice)
        {
            clearInput();
            cout << "Please Enter 1, 2, 3, or a vaild slot" << endl;
            for (int i = 3; i < 6; i++)
            {
                getline(pf, text);
                if (!text.size())
                {
                    cout << "empty" << endl;
                }
                else
                {
                    cout << "Choose slot " << i - 2 << endl;
                }
                it++;
            }

            cin >> choice;
        }
        Choice = choice;
    }
    system("cls");
    return Choice+1;
}

bool validLine(string fileName, int lineChoice)
{
    bool isvalid = false;
    int it=0;
    string text;
    fstream pf;

    pf.open(fileName);

    while (getline(pf, text))
    {

        if (it == lineChoice && !text.size())
        {
            isvalid = false;
        }
        if (it == lineChoice && text.size())
        {
            isvalid = true;
        }
        it++;
    }

    return isvalid;
}

#pragma endregion
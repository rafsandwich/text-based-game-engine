
#include <iostream>
#include <iomanip>
#include <memory>
#include <iterator>
#include <vector>
#include <forward_list>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"
#include <fstream>

// using declarations
using std::string;
using std::unique_ptr;
using std::cout;
using std::endl;

const unsigned int maxSize =512;
const string saveFileName = "save.txt";
string commandBuffer;
State *currentState;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

/**
 * Sets up the map.
 */
void initRooms() {
    //5 rooms in a plus shape, with a centre room leading to each exit
    Room* r1 = Room::addRoom(&r1name, &r1desc);
    Room* r2 = Room::addRoom(&r2name, &r2desc);
    Room* r3 = Room::addRoom(&r3name, &r3desc);
    Room* r4 = Room::addRoom(&r4name, &r4desc);
    Room* r5 = Room::addRoom(&r5name, &r5desc);
    //these objects are added to chosen rooms (see lines 52 - 53)
    GameObject* o1 = GameObject::addObject(&o1name, &o1desc, &o1key);
    GameObject* o2 = GameObject::addObject(&o2name, &o2desc, &o2key);
    GameObject* o4 = GameObject::addObject(&o4name, &o4desc, &o4key);
    r1->configSouth(r2);
    r1->configEast(r3);
    r1->configWest(r4);
    r1->configNorth(r5);
    r2->addObject(o1);
    r2->addObject(o4); //two objects in south room
    r3->addObject(o2); //one object in east room
}


/**
 * Sets up the game state.
 */
void initState() {
    currentState = new State(Room::rooms.front());
    //these objects are added to the inventory
    GameObject* o3 = GameObject::addObject(&o3name, &o3desc, &o3key);
    //GameObject* o4 = GameObject::addObject(&o4name, &o4desc, &o4key);
    currentState->addObject(o3);
    //currentState->addObject(o4);
}

bool loadInventory(std::list<string> keys) {
    try {
        for (string key : keys)
            for (auto & item : GameObject::gameObjects)
                if (key == item->getKeyword()) {
                    currentState->addObject(item);
                    break;
                }
    } catch (std::exception& e) {
        cout << "The save file has been written to incorrectly, starting new game... " << endl;
        return false;
    }
    return true;
}

bool loadRoom(string roomName, std::list<string> names){
    Room* currentRoom = nullptr;
    try {
        for (auto & room : Room::rooms) {
            if (roomName == room->getName()) {
                currentRoom = room;
                break;
            }
        }
        for (string name : names) {
            auto iter = GameObject::gameObjects.begin();
            for (int i = 0; i < GameObject::gameObjects.size(); i++) {
                if (name == (*iter)->getKeyword()) {
                    currentRoom->addObject(*iter);
                    break;
                }
                advance(iter, 1);
            }
        }
    } catch (std::exception& e) {
        cout << "The save file has been written to incorrectly, starting new game... " << endl;
        return false;
    }
    return true;
}

void load() {
    cout << "Loading..." << endl;
    auto* r1 = Room::addRoom(&r1name, &r1desc);
    auto* r2 = Room::addRoom(&r2name, &r2desc);
    auto* r3 = Room::addRoom(&r3name, &r3desc);
    auto* r4 = Room::addRoom(&r4name, &r4desc);
    auto* r5 = Room::addRoom(&r5name, &r5desc);
    r1->configSouth(r2);
    r1->configEast(r3);
    r1->configWest(r4);
    r1->configNorth(r5);
    GameObject::addObject(&o1name, &o1desc, &o1key);
    GameObject::addObject(&o2name, &o2desc, &o2key);
    GameObject::addObject(&o3name, &o3desc, &o3key);
    GameObject::addObject(&o4name, &o4desc, &o4key);

    std::ifstream file (saveFileName);
    if (file.is_open()) {
        bool fileLoadSuccess = false;
        string textLine;

        getline(file, textLine);
        for (auto & room : Room::rooms) {
            if (textLine == room->getName()) {
                currentState = new State(room);
                fileLoadSuccess = true;
                break;
            }
        }

        std::list<string> objectNames;
        while (getline(file, textLine) && fileLoadSuccess) {
            if (textLine == "Blorb") { //access inv
                getline(file, textLine);
                while (textLine != "Slorb") { //exit inv
                    objectNames.push_back(textLine);
                    getline(file, textLine);
                }
                fileLoadSuccess = loadInventory(objectNames);
            }

            else {
                string roomText = textLine;
                objectNames.clear();
                getline(file, textLine);
                while (textLine != "Jorb") { //exit room objects
                    objectNames.push_back(textLine);
                    getline(file, textLine);
                }
                fileLoadSuccess = loadRoom(roomText, objectNames);
            }

        }

        if (fileLoadSuccess)
            cout << "File successfully loaded!" << endl;
        else {
            cout << "Loading file failed, new game starting..." << endl;
            Room::rooms.clear();
            GameObject::gameObjects.clear();
            initRooms();
            initState();
        }
        cout << endl;
        file.close();
    }
    else {
        cout << "Unable to load file. Starting new game... " << endl;
        initRooms();
        initState();
    }
}

void save() {
    cout << "Saving..." << endl;
    std::ofstream file(saveFileName);
    if (file.is_open()) {
        file << currentState->getCurrentRoom()->getName() << endl;

        file << "Blorb" << endl; //key for accessing inventory
        for (GameObject* item : currentState->getInventory())
            file << item->getKeyword() << endl;
        file << "Slorb" << endl; //key for end of inventory

        for (Room* room : Room::rooms) {
            file << room->getName() << endl;
            for (GameObject* item : room->getObjects())
                file << item->getKeyword() << endl;
            file << "Jorb" << endl; //key for exit room objects
        }

        file.close();
        cout << "Save successful!" << endl;
        cout << endl;

    } else {
        cout << "Unable to save." << endl;
    }
}

/**
 * Nicer way of writing the choose a direction to go code
 * @param direction
 */
void goToRoom(char direction) {
    Room *roomDestination = nullptr;
    if (direction == 'n')
        roomDestination = currentState->getCurrentRoom()->getNorth();
    else if (direction == 's')
        roomDestination = currentState->getCurrentRoom()->getSouth();
    else if (direction == 'e')
        roomDestination = currentState->getCurrentRoom()->getEast();
    else if (direction == 'w')
        roomDestination = currentState->getCurrentRoom()->getWest();

    if (roomDestination == nullptr) { // room doesn't exist
        wrapOut(&badExit);      // cout cannot go there message
        wrapEndPara();
    } else {
        currentState->goTo(roomDestination); // this room state updated and room described
    }
}

int checkObject(std::vector<GameObject *> objects, string objectKey) {
    auto iter = objects.begin();
    for (int i = 0; i < objects.size(); i++) {
        if (objectKey == (*iter)->getKeyword())
            return i;
        advance(iter, 1);
    }
    return maxSize; //works as 'this object does not exist'
}

bool get(string objectKey) { //add object to inv if it is in the room (and remove from the room)
    std::vector<GameObject*> roomObjects = currentState->getCurrentRoom()->getObjects();
    int index = checkObject(roomObjects, objectKey);
    if (index != maxSize) {
        currentState->addObject(currentState->getCurrentRoom()->removeObject(index));
        cout << "You picked up the " << roomObjects.at(index)->getName() << endl;
        return true;
    }
    return false;
}

bool drop(string objectKey) { //remove object from inv and add to the current room objects if it's in inv
    std::vector<GameObject*> inventory = currentState->getInventory();
    int index = checkObject(inventory, objectKey);
    if (index != maxSize) {
        currentState->getCurrentRoom()->addObject(currentState->removeObject(index));
        cout << "You dropped the " << inventory.at(index)->getName() << endl;
        return true;
    }
    return false;
}

bool examine(string objectKey) { //cout description of object provided it is in the room
    std::vector<GameObject*> roomObjects = currentState->getCurrentRoom()->getObjects();
    int index = checkObject(roomObjects, objectKey);
    if (index != maxSize) {
        cout << (roomObjects.at(index))->getDescription() << endl;
        return true;
    }
    return false;
}

/**
 * The main game loop.
 */
void gameLoop() {
    //std::cout << "gameloop entered check";
    bool gameOver=false;
    while (!gameOver) {
        /* Ask for a command. */
        bool commandOk = false;
        inputCommand(&commandBuffer);

        /* The first word of a command would normally be the verb. The first word is the text before the first
         * space, or if there is no space, the whole string. */
        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));
        if (endOfVerb == 255) {
            if (commandBuffer.compare(0, endOfVerb, "inventory") == 0) {
                commandOk = true;
                if (currentState->getInventory().empty())
                    cout << "There is nothing in your inventory. " << endl;
                else {
                    std::vector < GameObject * > inventory = currentState->getInventory();
                    auto iter = inventory.begin();
                    cout << "In your bag you have: " << endl;
                    for (int i = 0; i < inventory.size(); i++) {
                        if (i == inventory.size() - 1 && i != 0) {
                            cout << "and a ";
                            cout << (*iter)->getName() << endl;
                            break;
                        }
                        cout << "a ";
                        cout << (*iter)->getName() << endl;
                        advance(iter, 1);
                    }
                }
            }
            else if (commandBuffer.compare(0, endOfVerb, "describe") == 0) {
                commandOk = true;
                currentState->getCurrentRoom()->describe();
            }

            if ((commandBuffer.compare(0, endOfVerb, "north") == 0) ||
                (commandBuffer.compare(0, endOfVerb, "n") == 0)) {
                commandOk = true;
                goToRoom('n');
            } else if ((commandBuffer.compare(0, endOfVerb, "south") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "s") == 0)) {
                commandOk = true;
                goToRoom('s');
            } else if ((commandBuffer.compare(0, endOfVerb, "east") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "e") == 0)) {
                commandOk = true;
                goToRoom('e');
            } else if ((commandBuffer.compare(0, endOfVerb, "west") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "w") == 0)) {
                commandOk = true;
                goToRoom('w');
            } else if ((commandBuffer.compare(0, endOfVerb, "save") == 0)) {
                commandOk = true;
                save();
            } else if ((commandBuffer.compare(0, endOfVerb, "quit") == 0)) {
                commandOk = true;
                gameOver = true;
            }
            else if (((commandBuffer.compare(0, endOfVerb, "") == 0))) {
                commandOk = true;
                cout << endl;
            }

        }//two word commands
        else {
            string objectKey = commandBuffer.substr(endOfVerb + 1);
            if (commandBuffer.compare(0, endOfVerb, "get") == 0) {
                commandOk = true;
                if (!get(objectKey))
                    cout << "You search the room, but cannot find that object anywhere. " << endl;
            }
            else if (commandBuffer.compare(0, endOfVerb, "drop") == 0) {
                commandOk = true;
                if (!drop(objectKey))
                    cout << "This object is not in your inventory. " << endl;
            }
            else if (commandBuffer.compare(0, endOfVerb, "examine") == 0) {
                commandOk = true;
                if (!examine(objectKey))
                    cout << "You want to examine this object, but cannot find it anywhere. " << endl;
            }
        }

        /* If commandOk hasn't been set, command wasn't understood, display error message */
        if(!commandOk) {
            wrapOut(&badCommand);
            wrapEndPara();
        }
    }
}


int main() {
//    std::cout <<"start check, ";
//    initWordWrap();
//    std::cout << "wordwrap check, ";
//    initRooms();
//    std::cout << "initrooms check, ";
//    initState();
//    std::cout << "initState check, ";
//    currentState->announceLoc();
//    std::cout << "announceloc check, ";
//    gameLoop();

    initWordWrap();

    string s;
    cout << "Load adventure? (y/n)" << endl;
    std::cin >> s;
    char c = s[0]; // variations of yes or no allowed, could change so only 'y' or 'n' are accepted
    bool loadDone = false;
    while (!loadDone) {
        switch (c) {
            case 'y' :
                load();
                loadDone = true;
                break;
            case 'n' :
                initRooms();
                initState();
                loadDone = true;
                break;
            default:
                cout << "Please enter y or n" << endl;
                std::cin >> s;
                c = s[0];
                break;
        }
    }

    currentState->announceLoc();
    gameLoop();
    return 0;
}
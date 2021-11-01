//
// Created by DrMark on 10/2/2017.
//

#include "State.h"

/**
 * Current state of the game.
 */

/**
 * Display the description of the room the player is in. */

void State::announceLoc() const {
    this->currentRoom->describe();
}

/**
 * Constructor.
 * @param startRoom Pointer to the room which the user starts in.
 */
State::State(Room *startRoom) : currentRoom(startRoom) {};

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;
    this->announceLoc();
}

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room* State::getCurrentRoom() {
    return this->currentRoom;
}

std::vector<GameObject*> State::getInventory() {
    return this->inventory;
}

void State::addObject(GameObject* _gameObject){
    this->inventory.push_back(_gameObject);
}

GameObject* State::removeObject(int index){
    GameObject* gameObject = inventory.at(index);
    this->inventory.erase(inventory.begin() + index);
    return gameObject;
}
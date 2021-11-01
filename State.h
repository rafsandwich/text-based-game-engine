//
// Created by DrMark on 10/2/2017.
//

#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H


#include "Room.h"

class State {
    Room *currentRoom;
    std::vector<GameObject*> inventory;
public:
    //std::list<GameObject> inventory;
    std::vector<GameObject*> getInventory();
    explicit State(Room *startRoom);
    void goTo(Room *target);
    void announceLoc() const;
    Room* getCurrentRoom();
    void addObject(GameObject* gameObject);
    GameObject* removeObject(int index);
};


#endif //TEXTADV_STATE_H

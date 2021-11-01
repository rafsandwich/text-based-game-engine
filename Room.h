//
// Created by DrMark on 10/2/2017.
//


#ifndef TEXTADV_ROOM_H
#define TEXTADV_ROOM_H

#include <string>
#include <forward_list>
#include <list>
#include "GameObject.h"
#include <vector>

using std::string;

/**
 * Represents a room (accessible location in the game).
 */
class Room {
    /**
     * Short name used as a header.
     */
    const string* name;
    /**
     * Full description of the room.
     */
    const string* description;

    std::vector<GameObject*> gameObjects;
    /**
     * Pointer to room that is north of this one.
     */
    Room* north;
    Room* east;
    Room* south;
    Room* west;
public:
    /**
     * Constructs a new Room.
     * @param _name Name of the room.
     * @param _desc Description of the room.
     */
    Room(const string *_name, const string *_desc);

    /**
     * Removes a destroyed room from the global list if it's there.
     */
    ~Room();

    /**
     * Outputs the name and description of the room
     * in standard format and names of all objects in the room.
     */
    void describe();

    /**
     * List storing all rooms that have been registered via addRoom().
     */
    static std::list<Room*> rooms;

    /**
     * Creates a new Room with the given parameters and register it with the static list.
     * @param _name Name of the room.
     * @param _desc Description of the room.
     */
    static Room* addRoom(const string* _name, const string* _desc);
    static Room* addRoom(Room* room);
    static std::list<Room*> getRooms();

    std::vector<GameObject*> getObjects() const;
    void addObject(GameObject* gameObject);
    GameObject* removeObject(int index);

    string getName() const;

    //better than set as it also sets the opposite for the room opposite it, reducing code
    void configNorth(Room *_north);
    void configSouth(Room *_south);
    void configEast(Room *_east);
    void configWest(Room *_west);

    Room* getNorth() const;
    void setNorth(Room* _north);

    Room* getEast() const;
    void setEast(Room* _east);

    Room* getSouth() const;
    void setSouth(Room* _south);

    Room* getWest() const;
    void setWest(Room* _west);

};

#endif //TEXTADV_ROOM_H

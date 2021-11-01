//
// Created by rafha on 13/11/2020.
//

#ifndef TEXTADV_GAMEOBJECT_H
#define TEXTADV_GAMEOBJECT_H

#include <string>
#include <list>

using std::string;

class GameObject {
    const string* name;
    const string* description;
    const string* keyword;

public:
    GameObject(const string* _name, const string* _description, const string* _keyword );
    GameObject(const GameObject &g2);
    ~GameObject();

    static GameObject* addObject(const string* _name, const string* _description, const string* _keyword);

    std::string getName() const;
    std::string getDescription() const;
    std::string getKeyword() const;

    static std::list<GameObject*> gameObjects;

};


#endif //TEXTADV_GAMEOBJECT_H

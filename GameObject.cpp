//
// Created by rafha on 13/11/2020.
//

#include <list>
#include "GameObject.h"

std::list<GameObject*> GameObject::gameObjects;

GameObject::GameObject(const string* _name, const string* _description, const string* _keyword) :
    name(_name), description(_description), keyword(_keyword){}

GameObject::GameObject(const GameObject &g2) :
        name(g2.name), description(g2.description), keyword(g2.keyword){};

GameObject::~GameObject() {
    GameObject::gameObjects.remove(this);
}

std::string GameObject::getName() const {
    return *this->name;
}

std::string GameObject::getDescription() const {
    return *this->description;
}

std::string GameObject::getKeyword() const {
    return *this->keyword;
}

GameObject *GameObject::addObject(const string* _name, const string* _description, const string* _keyword) {
    auto *newObject = new GameObject(_name, _description, _keyword);
    GameObject::gameObjects.push_back(newObject);
    return newObject;
}




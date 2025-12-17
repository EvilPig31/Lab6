#include "../include/npc.h"
#include "../include/visitor.h"
#include <cmath>
#include <iostream>

NPC::NPC(const std::string& name, double x, double y) : name(name), x(x), y(y), alive(true){}

std::string NPC::getName() const { return name; }
double NPC::getX() const { return x; }
double NPC::getY() const { return y; }
bool NPC::isAlive() const { return alive; }
void NPC::setAlive(bool status) { alive = status; }
double NPC::calculateDistance(const NPC* other) const {
    if (!other) return 999999.0;
    double dx = x - other->x;
    double dy = y - other->y;
    return std::sqrt(dx * dx + dy * dy);
}
bool NPC::isValidCoordinates(double x, double y){
    return (x > 0 && x <= 500 && y > 0 && y <= 500);
}
std::string NPC::getType() const {
    if (dynamic_cast<const Squirrel*>(this)) return "Squirrel";
    if (dynamic_cast<const Werewolf*>(this)) return "Werewolf";
    if (dynamic_cast<const Druid*>(this)) return "Druid";
    return "Unknown";
}
Squirrel::Squirrel(const std::string& name, double x, double y) : NPC(name, x, y){}
void Squirrel::accept(NPCVisitor& visitor){
    visitor.visit(this);
}
bool Squirrel::canAttack(NPC* other) const{
    std::string type = other->getType();
    return (type == "Werewolf" || type == "Druid");
}
std::string Squirrel::attack(NPC* other){
    if (canAttack(other)) {
        other->setAlive(false);
        return name + " (Squirrel) kills " + other->getName() + " (" + other->getType() + ")";
    }
    return name + " (Squirrel) cannot attack " + other->getName();
}
Werewolf::Werewolf(const std::string& name, double x, double y) : NPC(name, x, y){}
void Werewolf::accept(NPCVisitor& visitor){
    visitor.visit(this);
}
bool Werewolf::canAttack(NPC* other) const{
    return other->getType() == "Druid";
}
std::string Werewolf::attack(NPC* other){
    if (canAttack(other)) {
        other->setAlive(false);
        return name + " (Werewolf) kills " + other->getName() + " (" + other->getType() + ")";
    }
    return name + " (Werewolf) cannot attack " + other->getName();
}
Druid::Druid(const std::string& name, double x, double y) : NPC(name, x, y){}

void Druid::accept(NPCVisitor& visitor){
    visitor.visit(this);
}
bool Druid::canAttack(NPC* other) const{
    return false;
}
std::string Druid::attack(NPC* other){
    return name + " (Druid) is peaceful and does not attack anyone";
}
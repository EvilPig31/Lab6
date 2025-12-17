#include "../include/visitor.h"
#include "../include/npc.h"
#include "../include/observer.h"
#include <iostream>
#include <algorithm>

BattleVisitor::BattleVisitor(std::vector<std::shared_ptr<NPC>>& npcs, double range, BattleLogger* logger) : npcs(npcs), battleRange(range), logger(logger){}
void BattleVisitor::visit(Squirrel* squirrel){
    if (!squirrel->isAlive()) return;
    for (auto& target : npcs){
        if (!target->isAlive() || target.get() == squirrel) continue;
        double distance = squirrel->calculateDistance(target.get());
        if (distance <= battleRange){
            resolveBattle(squirrel, target.get());
        }
    }
}
void BattleVisitor::visit(Werewolf* werewolf){
    if (!werewolf->isAlive()) return;
    for (auto& target : npcs){
        if (!target->isAlive() || target.get() == werewolf) continue;
        double distance = werewolf->calculateDistance(target.get());
        if (distance <= battleRange){
            resolveBattle(werewolf, target.get());
        }
    }
}
void BattleVisitor::visit(Druid* druid){
}

void BattleVisitor::resolveBattle(NPC* npc1, NPC* npc2) {
    bool npc1Can = npc1->canAttack(npc2);
    bool npc2Can = npc2->canAttack(npc1);
    std:: string event;
    if (npc1Can && npc2Can){
        npc1->setAlive(false);
        npc2->setAlive(false);
        event = npc1->getName() + "and" + npc2->getName() + "killed each other";
        if (logger) logger->logBattleEvent(event);
    }
    else if (npc1Can){
        npc2->setAlive(false);
        event = npc1->getName() + " (" + npc1->getType() + ") killed " + npc2->getName() + " (" + npc2->getType() + ")"; 
        if (logger) logger->logBattleEvent(event);
    }
    else if (npc2Can){
        npc1->setAlive(false);
        event = npc2->getName() + " (" + npc2->getType() + ") killed " + npc1->getName() + " (" + npc1->getType() + ")"; 
        if (logger) logger->logBattleEvent(event);
    }
}

void BattleVisitor::executeBattle(){
    std::vector<std::pair<NPC*, NPC*>> battlePairs;
    for (size_t i = 0; i < npcs.size(); i++){
        if (!npcs[i]->isAlive()) continue;
        for (size_t j = i + 1; j < npcs.size(); j++){
            if (!npcs[j]->isAlive()) continue;
            double distance = npcs[i]->calculateDistance(npcs[j].get());
            if (distance <= battleRange) {
                battlePairs.push_back({npcs[i].get(), npcs[j].get()});
            }
        }
    }
    for (auto& pair : battlePairs) {
        resolveBattle(pair.first, pair.second);
    }
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(),
        [](const std::shared_ptr<NPC>& npc) {
            return !npc->isAlive();
        }), npcs.end());
}
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

void BattleVisitor::resolveBattle(NPC* attacker, NPC* target) {
    std::string attackResult = attacker->attack(target);
    if (logger && attackResult.find("kills") != std::string::npos) {
        logger->logBattleEvent(attackResult);
    }
    if (!target->isAlive()) {
        std::string counterAttackResult = target->attack(attacker);
        if (counterAttackResult.find("kills") != std::string::npos) {
            attacker->setAlive(false);
            if (logger) {
                logger->logBattleEvent(counterAttackResult + " (counter-attack)");
            }
        }
    }
}

void BattleVisitor::executeBattle() {
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            npc->accept(*this);
        }
    }
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(),
        [](const std::shared_ptr<NPC>& npc) {
            return !npc->isAlive();
        }), npcs.end());
}
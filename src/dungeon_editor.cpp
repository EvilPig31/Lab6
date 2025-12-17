#include "../include/dungeon_editor.h"
#include "../include/npc_factory.h"
#include "../include/visitor.h"
#include <iostream>
#include <iomanip>

DungeonEditor::DungeonEditor(){
    attachConsoleLogger();
}

bool DungeonEditor::addNPC(const std::string& type, const std::string& name, double x, double y){
    for (const auto& npc : npcs){
        if (npc->getName() == name){
            std::cerr << "Error: NPC with name '" << name << "' already exists" << std::endl;
            return false;
        }
    }
    NPCFactory::NPCType npcType;
    if (type == "squirrel" || type == "SQUIRREL"){
        npcType = NPCFactory::NPCType::SQUIRREL;
    } else if (type == "werewolf" || type == "WEREWOLF"){
        npcType = NPCFactory::NPCType::WEREWOLF;
    } else if (type == "druid" || type == "DRUID"){
        npcType = NPCFactory::NPCType::DRUID;
    } else {
        std::cerr << "Error: Unknown NPC type '" << type << "'" << std::endl;
        return false;
    }
    auto npc = NPCFactory::createNPC(npcType, name, x, y);
    if (npc) {
        npcs.push_back(npc);
        std::cout << "Added " << type << " '" << name << "' at (" << x << ", " << y << ")" << std::endl;
        return true;
    }
    return false;
}
void DungeonEditor::printAllNPCs() const{
    std::cout << "\n=== NPC List ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Type" 
              << std::setw(20) << "Name" 
              << std::setw(10) << "X" 
              << std::setw(10) << "Y" 
              << "Status" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    for (const auto& npc : npcs){
        std::cout << std::left << std::setw(15) << npc->getType()
                  << std::setw(20) << npc->getName()
                  << std::setw(10) << npc->getX()
                  << std::setw(10) << npc->getY()
                  << (npc->isAlive() ? "Alive" : "Dead") << std::endl;
    }
    std::cout << "Total: " << npcs.size() << " NPCs" << std::endl;
}
void DungeonEditor::startBattle(double range){
    std::cout << "\n=== Starting Battle (Range: " << range << "m) ===" << std::endl;
    BattleVisitor visitor(npcs, range, &battleLogger);
    visitor.executeBattle();
    std::cout << "Battle finished. Remaining NPCs: " << npcs.size() << std::endl;
}
bool DungeonEditor::saveToFile(const std::string& filename) const {
    return NPCFactory::saveToFile(npcs, filename);
}
bool DungeonEditor::loadFromFile(const std::string& filename){
    auto loadedNPCs = NPCFactory::loadFromFile(filename);
    if (!loadedNPCs.empty()) {
        npcs = loadedNPCs;
        return true;
    }
    return false;
}
void DungeonEditor::attachConsoleLogger(){
    static ConsoleLogger consoleLogger;
    battleLogger.attach(&consoleLogger);
}
void DungeonEditor::attachFileLogger(const std::string& filename){
    static FileLogger fileLogger(filename);
    battleLogger.attach(&fileLogger);
}
size_t DungeonEditor::getNPCCount() const{
    return npcs.size();
}
void DungeonEditor::clearAll() {
    npcs.clear();
    std::cout << "All NPCs cleared" << std::endl;
}
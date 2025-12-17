#ifndef DUNGEON_EDITOR_H
#define DUNGEON_EDITOR_H

#include <vector>
#include <memory>
#include "npc.h"
#include "observer.h"

class DungeonEditor{
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    BattleLogger battleLogger;
public:
    DungeonEditor();
    bool addNPC(const std::string& type, const std::string& name, double x, double y);
    void printAllNPCs() const;
    void startBattle(double range);
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void attachConsoleLogger();
    void attachFileLogger(const std::string& filename = "log.txt");
    size_t getNPCCount() const;
    void clearAll();
};

#endif
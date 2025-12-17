#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <memory>

class NPC;
class Squirrel;
class Werewolf;
class Druid;

class NPCVisitor{
public:
    virtual ~NPCVisitor() = default;
    virtual void visit(Squirrel* squirrel) = 0;
    virtual void visit(Werewolf* werewolf) = 0;
    virtual void visit(Druid* druid) = 0;
};
class BattleVisitor : public NPCVisitor{
private:
    std::vector<std::shared_ptr<NPC>> &npcs;
    double battleRange;
    class BattleLogger* logger;
    
public:
    BattleVisitor(std::vector<std::shared_ptr<NPC>>& npcs, double range, class BattleLogger* logger = nullptr);
    void visit(Squirrel* squirrel) override;
    void visit(Werewolf* werewolf) override;
    void visit(Druid* druid) override;
    
    void executeBattle();
    
private:
    void resolveBattle(NPC* attacker, NPC* target);
};

#endif
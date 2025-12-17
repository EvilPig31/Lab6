#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>

class NPCVisitor;

class NPC{
protected:
    std::string name;
    double x;
    double y;
    bool alive;

public:
    NPC(const std::string& name, double x, double y);
    virtual ~NPC() = default;
    std::string getName() const;
    std::string getType() const;
    double getX() const;
    double getY() const;
    bool isAlive() const;
    void setAlive(bool status);
    virtual void accept(NPCVisitor& visitor) = 0;
    virtual bool canAttack(NPC* other) const = 0;
    virtual std::string attack(NPC* other) = 0;
    double calculateDistance(const NPC* other) const;
    static bool isValidCoordinates(double x, double y);
};

class Squirrel : public NPC{
public:
    Squirrel(const std::string& name, double x, double y);
    void accept(NPCVisitor& visitor) override;
    bool canAttack(NPC* other) const override;
    std::string attack(NPC* other) override;
};

class Werewolf : public NPC{
public:
    Werewolf(const std::string& name, double x, double y);
    void accept(NPCVisitor& visitor) override;
    bool canAttack(NPC* other) const override;
    std::string attack(NPC* other) override;
};

class Druid : public NPC{
public:
    Druid(const std::string& name, double x, double y);
    void accept(NPCVisitor& visitor) override;
    bool canAttack(NPC* other) const override;
    std::string attack(NPC* other) override;
};

#endif
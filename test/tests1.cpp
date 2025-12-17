#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/npc_factory.h"
#include "../include/visitor.h"
#include "../include/observer.h"
#include "../include/dungeon_editor.h"
#include <fstream>
#include <filesystem>

using namespace std;

TEST(NPCTest, NPCConstructor){
    Squirrel squirrel("TestSquirrel", 100.0, 200.0);
    EXPECT_EQ(squirrel.getName(), "TestSquirrel");
    EXPECT_DOUBLE_EQ(squirrel.getX(), 100.0);
    EXPECT_DOUBLE_EQ(squirrel.getY(), 200.0);
    EXPECT_TRUE(squirrel.isAlive());
    EXPECT_EQ(squirrel.getType(), "Squirrel");
}
TEST(NPCTest, CoordinateValidation){
    EXPECT_TRUE(NPC::isValidCoordinates(1.0, 1.0));
    EXPECT_TRUE(NPC::isValidCoordinates(500.0, 500.0));
    EXPECT_TRUE(NPC::isValidCoordinates(250.0, 250.0));
    EXPECT_FALSE(NPC::isValidCoordinates(0.0, 100.0));
    EXPECT_FALSE(NPC::isValidCoordinates(100.0, 0.0));
    EXPECT_FALSE(NPC::isValidCoordinates(501.0, 100.0));
    EXPECT_FALSE(NPC::isValidCoordinates(100.0, 501.0));
    EXPECT_FALSE(NPC::isValidCoordinates(-10.0, 100.0));
    EXPECT_FALSE(NPC::isValidCoordinates(100.0, -10.0));
}
TEST(NPCTest, DistanceCalculation){
    Squirrel s1("S1", 0.0, 0.0);
    Squirrel s2("S2", 3.0, 4.0);
    EXPECT_DOUBLE_EQ(s1.calculateDistance(&s2), 5.0);
    EXPECT_DOUBLE_EQ(s2.calculateDistance(&s1), 5.0);
    
    Squirrel s3("S3", 10.0, 10.0);
    Squirrel s4("S4", 10.0, 10.0);
    
    EXPECT_DOUBLE_EQ(s3.calculateDistance(&s4), 0.0);
}

TEST(NPCTest, SquirrelAttackRules){
    Squirrel squirrel("Sq", 100, 100);
    Werewolf wolf("Wolf", 101, 101);
    Druid druid("Dru", 102, 102);
    Squirrel squirrel2("Sq2", 103, 103);
    
    EXPECT_TRUE(squirrel.canAttack(&wolf));
    
    EXPECT_TRUE(squirrel.canAttack(&druid));

    EXPECT_FALSE(squirrel.canAttack(&squirrel2));
}

TEST(NPCTest, WerewolfAttackRules){
    Werewolf wolf("Wolf", 100, 100);
    Druid druid("Dru", 101, 101);
    Squirrel squirrel("Sq", 102, 102);

    EXPECT_TRUE(wolf.canAttack(&druid));
    EXPECT_FALSE(wolf.canAttack(&squirrel));
    
    Werewolf wolf2("Wolf2", 103, 103);
    EXPECT_FALSE(wolf.canAttack(&wolf2));
}

TEST(NPCTest, DruidAttackRules){
    Druid druid("Dru", 100, 100);
    Squirrel squirrel("Sq", 101, 101);
    Werewolf wolf("Wolf", 102, 102);
    Druid druid2("Dru2", 103, 103);
    EXPECT_FALSE(druid.canAttack(&squirrel));
    EXPECT_FALSE(druid.canAttack(&wolf));
    EXPECT_FALSE(druid.canAttack(&druid2));
}


TEST(FactoryTest, CreateNPC){
    auto squirrel = NPCFactory::createNPC(NPCFactory::NPCType::SQUIRREL, "TestSquirrel", 100, 200);
    EXPECT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    
    auto werewolf = NPCFactory::createNPC(NPCFactory::NPCType::WEREWOLF, "TestWolf", 150, 250);
    EXPECT_NE(werewolf, nullptr);
    EXPECT_EQ(werewolf->getType(), "Werewolf");
    
    auto druid = NPCFactory::createNPC(NPCFactory::NPCType::DRUID, "TestDruid", 200, 300);
    EXPECT_NE(druid, nullptr);
    EXPECT_EQ(druid->getType(), "Druid");
}

TEST(FactoryTest, InvalidCoordinates){
    auto npc = NPCFactory::createNPC(NPCFactory::NPCType::SQUIRREL, "BadNPC", 0, 0);
    EXPECT_EQ(npc, nullptr);
    
    npc = NPCFactory::createNPC(NPCFactory::NPCType::SQUIRREL, "BadNPC2", 501, 500);
    EXPECT_EQ(npc, nullptr);
}

TEST(FactoryTest, StringTypeConversion){
    EXPECT_EQ(NPCFactory::stringToType("SQUIRREL"), NPCFactory::NPCType::SQUIRREL);
    EXPECT_EQ(NPCFactory::stringToType("WEREWOLF"), NPCFactory::NPCType::WEREWOLF);
    EXPECT_EQ(NPCFactory::stringToType("DRUID"), NPCFactory::NPCType::DRUID);
    EXPECT_EQ(NPCFactory::stringToType("UNKNOWN"), NPCFactory::NPCType::SQUIRREL);
    
    EXPECT_EQ(NPCFactory::typeToString(NPCFactory::NPCType::SQUIRREL), "SQUIRREL");
    EXPECT_EQ(NPCFactory::typeToString(NPCFactory::NPCType::WEREWOLF), "WEREWOLF");
    EXPECT_EQ(NPCFactory::typeToString(NPCFactory::NPCType::DRUID), "DRUID");
}

TEST(VisitorTest, BattleVisitorCreation){
    vector<shared_ptr<NPC>> npcs;
    BattleLogger logger;
    
    BattleVisitor visitor(npcs, 50.0, &logger);
    EXPECT_TRUE(true);
}

TEST(VisitorTest, BattleWithinRange){
    vector<shared_ptr<NPC>> npcs;
    BattleLogger logger;
    
    auto squirrel = make_shared<Squirrel>("Sq", 100, 100);
    auto wolf = make_shared<Werewolf>("Wolf", 101, 101);
    
    npcs.push_back(squirrel);
    npcs.push_back(wolf);
    
    BattleVisitor visitor(npcs, 10.0, &logger);
    visitor.executeBattle();
    
    EXPECT_FALSE(wolf->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
}

TEST(VisitorTest, BattleOutOfRange){
    vector<shared_ptr<NPC>> npcs;
    BattleLogger logger;
    
    auto squirrel = make_shared<Squirrel>("Sq", 100, 100);
    auto wolf = make_shared<Werewolf>("Wolf", 400, 400);
    
    npcs.push_back(squirrel);
    npcs.push_back(wolf);
    
    BattleVisitor visitor(npcs, 50.0, &logger);
    visitor.executeBattle();
    
    EXPECT_TRUE(wolf->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
}

TEST(VisitorTest, ComplexBattleScenario){
    vector<shared_ptr<NPC>> npcs;
    BattleLogger logger;
    auto squirrel = make_shared<Squirrel>("Sq", 100, 100);
    auto wolf = make_shared<Werewolf>("Wolf", 101, 101);
    auto druid = make_shared<Druid>("Dru", 102, 102);
    
    npcs.push_back(squirrel);
    npcs.push_back(wolf);
    npcs.push_back(druid);
    
    BattleVisitor visitor(npcs, 10.0, &logger);
    visitor.executeBattle();
    
    EXPECT_FALSE(wolf->isAlive());
    EXPECT_FALSE(druid->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
    EXPECT_EQ(npcs.size(), 1);
}

TEST(VisitorTest, EachWithEachBattleLogic) {
    vector<shared_ptr<NPC>> npcs;
    BattleLogger logger;
    
    // Создаем 3 NPC в одном месте
    auto squirrel = make_shared<Squirrel>("Sq", 100, 100);
    auto wolf = make_shared<Werewolf>("Wolf", 100, 100);
    auto druid = make_shared<Druid>("Dru", 100, 100);
    
    npcs.push_back(squirrel);
    npcs.push_back(wolf);
    npcs.push_back(druid);
    
    BattleVisitor visitor(npcs, 10.0, &logger);
    visitor.executeBattle();
    EXPECT_FALSE(wolf->isAlive());
    EXPECT_FALSE(druid->isAlive());
    EXPECT_TRUE(squirrel->isAlive());
    EXPECT_EQ(npcs.size(), 1);
}

TEST(ObserverTest, FileLoggerCreatesFile){
    string filename = "test_log.txt";
    FileLogger logger(filename);
    
    logger.update("Test event 1");
    logger.update("Test event 2");
    
    ifstream file(filename);
    EXPECT_TRUE(file.is_open());
    
    string line;
    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        EXPECT_TRUE(line.find("Test event") != string::npos);
    }
    
    EXPECT_GE(lineCount, 1);
    file.close();
    
    remove(filename.c_str());
}

TEST(ObserverTest, BattleLoggerNotifiesObservers){
    BattleLogger logger;
    
    class MockObserver : public BattleObserver {
    public:
        int notificationCount = 0;
        string lastEvent;
        
        void update(const string& event) override {
            notificationCount++;
            lastEvent = event;
        }
    };
    
    MockObserver obs1, obs2;
    
    logger.attach(&obs1);
    logger.attach(&obs2);
    
    logger.logBattleEvent("Test battle event");
    
    EXPECT_EQ(obs1.notificationCount, 1);
    EXPECT_EQ(obs2.notificationCount, 1);
    EXPECT_EQ(obs1.lastEvent, "Test battle event");
    EXPECT_EQ(obs2.lastEvent, "Test battle event");

    logger.detach(&obs1);
    logger.logBattleEvent("Second event");
    EXPECT_EQ(obs1.notificationCount, 1);
    EXPECT_EQ(obs2.notificationCount, 2);
}

TEST(DungeonEditorTest, AddNPC){
    DungeonEditor editor;
    EXPECT_TRUE(editor.addNPC("squirrel", "TestSq", 100, 200));
    EXPECT_EQ(editor.getNPCCount(), 1);
    
    EXPECT_TRUE(editor.addNPC("werewolf", "TestWolf", 150, 250));
    EXPECT_EQ(editor.getNPCCount(), 2);

    EXPECT_FALSE(editor.addNPC("squirrel", "BadNPC", 0, 0));
    EXPECT_EQ(editor.getNPCCount(), 2);
    
    EXPECT_FALSE(editor.addNPC("druid", "TestSq", 200, 300));
    EXPECT_EQ(editor.getNPCCount(), 2);
}

TEST(DungeonEditorTest, ClearAll){
    DungeonEditor editor;
    
    editor.addNPC("squirrel", "Sq1", 100, 200);
    editor.addNPC("werewolf", "Wolf1", 150, 250);
    
    EXPECT_EQ(editor.getNPCCount(), 2);
    
    editor.clearAll();
    
    EXPECT_EQ(editor.getNPCCount(), 0);
}

TEST(DungeonEditorTest, BattleSimulation){
    DungeonEditor editor;
    editor.addNPC("squirrel", "Sq", 100, 100);
    editor.addNPC("werewolf", "Wolf", 101, 101);
    editor.addNPC("druid", "Dru", 400, 400);
    
    EXPECT_EQ(editor.getNPCCount(), 3);
    
    editor.startBattle(100.0);
    
    EXPECT_EQ(editor.getNPCCount(), 2);
}

TEST(DungeonEditorTest, FileOperations) {
    DungeonEditor editor;
    editor.addNPC("squirrel", "FileSq", 100, 200);
    editor.addNPC("werewolf", "FileWolf", 150, 250);
    editor.addNPC("druid", "FileDru", 200, 300);
    
    string filename = "test_dungeon.txt";
    
    EXPECT_TRUE(editor.saveToFile(filename));
    
    editor.clearAll();
    EXPECT_EQ(editor.getNPCCount(), 0);
    
    EXPECT_TRUE(editor.loadFromFile(filename));
    EXPECT_EQ(editor.getNPCCount(), 3);
    
    remove(filename.c_str());
}
TEST(IntegrationTest, FullBattleScenario){
    DungeonEditor editor;
    editor.addNPC("squirrel", "Squirrel1", 100, 100);
    editor.addNPC("werewolf", "Wolf1", 110, 110);
    editor.addNPC("werewolf", "Wolf2", 400, 400);
    editor.addNPC("druid", "Druid1", 105, 105);
    editor.addNPC("druid", "Druid2", 200, 200);
    
    EXPECT_EQ(editor.getNPCCount(), 5);
    
    editor.startBattle(50.0);
    
    EXPECT_EQ(editor.getNPCCount(), 3);
}


TEST(EdgeCasesTest, SingleNPCBattle){
    DungeonEditor editor;
    
    editor.addNPC("squirrel", "LonelySq", 100, 200);
    editor.startBattle(100.0);
    
    EXPECT_EQ(editor.getNPCCount(), 1);
}

TEST(EdgeCasesTest, SameTypeNPCBattle){
    DungeonEditor editor;
    editor.addNPC("squirrel", "Sq1", 100, 100);
    editor.addNPC("squirrel", "Sq2", 101, 101);
    
    editor.startBattle(10.0);
    EXPECT_EQ(editor.getNPCCount(), 2);
}
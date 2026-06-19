#include "EnemyFactory.h"

#include "GameException.h"
#include "Goblin.h"
#include "Hobbit.h"
#include "Orc.h"
#include "Troll.h"

std::unique_ptr<Enemy> EnemyFactory::create(const std::string& type,
                                            const std::string& name) {
    if (type == "goblin") return std::make_unique<Goblin>(name);
    if (type == "orc")    return std::make_unique<Orc>(name);
    if (type == "troll")  return std::make_unique<Troll>(name);
    if (type == "hobbit") return std::make_unique<Hobbit>(name);
    throw UnknownEnemyTypeException(type);
}
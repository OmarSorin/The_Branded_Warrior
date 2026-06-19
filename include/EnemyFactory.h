#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include <memory>
#include <string>

#include "Enemy.h"

class EnemyFactory {
public:
    static std::unique_ptr<Enemy> create(const std::string& type,
                                         const std::string& name);
};

#endif // ENEMYFACTORY_H
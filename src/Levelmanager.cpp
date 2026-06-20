#include "Levelmanager.h"
// cppcheck-suppress unusedFunction
LevelConfig Levelmanager::configFor(int depth) {
    // The dungeon gets bigger and more cluttered each level
    LevelConfig cfg(6 + depth * 2,
                    8 + depth * 4);

    // goblins on each level
    cfg.addSpawn(EnemySpawn("goblin", util::Range<int>(2, 3 + depth)));

    // Ffrom the 2nd floor there are also hobbits and orcs
    if (depth >= 1) {
        cfg.addSpawn(EnemySpawn("hobbit", util::Range<int>(1, 1 + depth / 2)));
        cfg.addSpawn(EnemySpawn("orc", util::Range<int>(1, 1 + depth)));
    }

    // Trolls only on last level
    if (depth >= 2) {
        cfg.addSpawn(EnemySpawn("troll", util::Range<int>(1, depth)));
    }

    return cfg;
}
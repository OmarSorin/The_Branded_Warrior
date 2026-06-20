#include "Levelmanager.h"

LevelConfig Levelmanager::configFor(int depth) const {
    LevelConfig cfg(depth,
                    6 + depth * 2,
                    8 + depth * 4);

    cfg.addSpawn(EnemySpawn("goblin", util::Range<int>(2, 3 + depth)));

    if (depth >= 1) {
        cfg.addSpawn(EnemySpawn("hobbit", util::Range<int>(1, 1 + depth / 2)));
        cfg.addSpawn(EnemySpawn("orc", util::Range<int>(1, 1 + depth)));
    }

    if (depth >= 2) {
        cfg.addSpawn(EnemySpawn("troll", util::Range<int>(1, depth)));
    }

    return cfg;
}
#ifndef COMBATSUBJECT_H
#define COMBATSUBJECT_H

#include <vector>

#include "Combatobserver.h"

class CombatSubject {
    std::vector<CombatObserver *> observers;

public:
    void addObserver(CombatObserver *observer) {
        if (observer)
            observers.push_back(observer);
    }

protected:
    void notify(const std::string &text, MessageType type) const {
        for (auto *observer : observers)
            observer->onCombatEvent(text, type);
    }
};

#endif // COMBATSUBJECT_H
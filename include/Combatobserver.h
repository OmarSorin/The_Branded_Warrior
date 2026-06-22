#ifndef COMBATOBSERVER_H
#define COMBATOBSERVER_H

#include <string>

//same message type, im just refactoring the messagelog to use an
// observer design pattern to not have 8928934289049023 shared logic that
// can go up in flames when you add a new class, etc.
enum class MessageType { Dealt, Taken, Xp, Info };

class CombatObserver {
public:
    virtual ~CombatObserver() = default;
    virtual void onCombatEvent(const std::string &text, MessageType type) = 0;
};

#endif // COMBATOBSERVER_H
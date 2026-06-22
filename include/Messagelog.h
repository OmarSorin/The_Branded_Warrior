#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <deque>
#include <string>
#include <utility>

#include "Combatobserver.h"

// 1 line in log
class LogEntry {
    std::string text;
    MessageType type;

public:
    LogEntry(std::string text, MessageType type)
        : text(std::move(text)), type(type) {}
    // cppcheck-suppress unusedFunction
    const std::string &getText() const { return text; }
    // cppcheck-suppress unusedFunction
    MessageType getType() const { return type; }
};

class MessageLog : public CombatObserver {
    std::deque<LogEntry> entries;
    static constexpr std::size_t MAX_ENTRIES = 6;

public:
    // appends a msg, drops old
    void onCombatEvent(const std::string &text, MessageType type) override;

    //FIFO
    // cppcheck-suppress unusedFunction
    const std::deque<LogEntry> &getEntries() const { return entries; }
};

#endif // MESSAGELOG_H
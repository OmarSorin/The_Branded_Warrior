#include "Messagelog.h"

void MessageLog::onCombatEvent(const std::string &text, MessageType type) {
    entries.emplace_back(text, type);
    while (entries.size() > MAX_ENTRIES)
        entries.pop_front();
}
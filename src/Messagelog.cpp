#include "Messagelog.h"

void MessageLog::add(const std::string &text, MessageType type) {
    entries.emplace_back(text, type);
    while (entries.size() > MAX_ENTRIES)
        entries.pop_front();
}
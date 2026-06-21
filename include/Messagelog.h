#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <deque>
#include <string>
#include <utility>


enum class MessageType { Dealt, Taken, Xp, Info };

// 1 line in log
class LogEntry {
    std::string text;
    MessageType type;

public:
    LogEntry(std::string text, MessageType type)
        : text(std::move(text)), type(type) {}

    const std::string &getText() const { return text; }
    MessageType getType() const { return type; }
};

class MessageLog {
    std::deque<LogEntry> entries;
    static constexpr std::size_t MAX_ENTRIES = 6;

public:
    // appends a msg, drops old
    void add(const std::string &text, MessageType type = MessageType::Info);

    //FIFO
    const std::deque<LogEntry> &getEntries() const { return entries; }
};

#endif // MESSAGELOG_H
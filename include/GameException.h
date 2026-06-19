#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class InvalidPositionException : public GameException {
public:
    InvalidPositionException(int x, int y)
        : GameException("Invalid position: (" + std::to_string(x) +
                        ", " + std::to_string(y) + ")") {}
};

class DeadCharacterException : public GameException {
public:
    explicit DeadCharacterException(const std::string& name)
        : GameException(name + " is dead and cannot act") {}
};

class InvalidItemException : public GameException {
public:
    explicit InvalidItemException(int index)
        : GameException("Invalid inventory index: " + std::to_string(index)) {}
};

class UnknownEnemyTypeException : public GameException {
public:
    explicit UnknownEnemyTypeException(const std::string& type)
        : GameException("Unknown enemy type: '" + type + "'") {}
};

#endif // GAMEEXCEPTION_H
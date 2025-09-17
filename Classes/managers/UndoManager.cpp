#include "managers/UndoManager.h"

using namespace MyCardGame;

void UndoManager::addAction(UndoCommand&& command) {
    _actions.emplace_back(std::move(command));
}

bool UndoManager::undo() {
    if (_actions.empty()) {
        return false;
    }

    _actions.back().undo();
    _actions.pop_back();

    return true;
}

void UndoManager::clear() {
    _actions.clear();
}

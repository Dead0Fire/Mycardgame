#pragma once

#include <vector>
#include <functional>

namespace MyCardGame {

    /**
     * @class UndoCommand
     * @brief Represents a command that can be undone.
     *
     * This class encapsulates an action that can be reversed. The specific logic
     * for undoing the action is provided as a lambda function or other callable
     * target during its creation.
     */
    class UndoCommand {
    public:
        // The function that will be called to undo the action.
        std::function<void()> undo;
    };

    /**
     * @class UndoManager
     * @brief Manages a stack of undoable commands.
     *
     * This manager follows the Command Pattern to handle undo functionality.
     * It maintains a history of actions performed by the user and can reverse
     * them one by one. It does not contain any game logic itself but simply
     * executes the undo commands it has stored.
     */
    class UndoManager {
    public:
        /**
         * @brief Adds a new undoable action to the history.
         * @param command The command containing the logic to undo an action.
         */
        void addAction(UndoCommand&& command);

        /**
         * @brief Reverts the most recent action if available.
         * @return True if an action was undone, false if the history was empty.
         */
        bool undo();

        /**
         * @brief Clears the entire undo history.
         */
        void clear();

    private:
        std::vector<UndoCommand> _actions;
    };

}

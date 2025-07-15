#include <iostream>
#include <string>
#include "Stack.h"  // bringing in the stack
using namespace std;

// op types for undo/redo
enum OperationType { INSERT, DELETE };

// used to track actions for undo/redo
struct Operation {
    OperationType type;
    int position;
    string content;   // what got inserted/deleted
};

int main() {
    Stack<Operation> undoStack;
    Stack<Operation> redoStack;
    string fulltext = "";    // working text
    string cmd;

    // main loop,for cmds
    while (cin >> cmd) {
        if (cmd == "INSERT") {
            int pos;
            string txt;
            cin >> pos;
            cin.ignore();     // skip newline or space
            getline(cin, txt); // grab full text

            // bound check
            if (pos < 0 || pos > fulltext.length()) {
                cout << "Invalid pos for insert!\n";
                continue;
            }

            fulltext.insert(pos, txt); // inserting
            undoStack.push({ INSERT, pos, txt });  // save it for undo later

            // we changed stuff, so redo no longer valid
            while (!redoStack.isEmpty()) {
                redoStack.pop();
            }

        } else if (cmd == "DELETE") {
            int pos, len;
            cin >> pos >> len;

            // bounds check again
            if (pos < 0 || len < 0 || pos + len > fulltext.length()) {
                cout << "Invalid pos or len for delete!\n";
                continue;
            }

            string removed = fulltext.substr(pos, len); // save for undo
            fulltext.erase(pos, len); // delete it
            undoStack.push({ DELETE, pos, removed });

            // any new change invalidates redo history
            while (!redoStack.isEmpty()) {
                redoStack.pop();
            }

        } else if (cmd == "UNDO") {
            if (undoStack.isEmpty()) {
                cout << "Nothig to undo!\n";
                continue;
            }
            Operation last = undoStack.pop();
            if (last.type == INSERT) {
                // undo insert: just delete it back
                fulltext.erase(last.position, last.content.length());
            } else if (last.type == DELETE) {
                // undo delete: put it back in
                fulltext.insert(last.position, last.content);
            }
            redoStack.push(last); // so we can redo it later if needed
        } else if (cmd == "REDO") {
            if (redoStack.isEmpty()) {
                cout << "Nothing to redo!\n";
                continue;
            }
            Operation redoAct = redoStack.pop();
            if (redoAct.type == INSERT) {
                fulltext.insert(redoAct.position, redoAct.content);
            } else if (redoAct.type == DELETE) {
                fulltext.erase(redoAct.position, redoAct.content.length());
            }
            undoStack.push(redoAct); // move it back to undo
        } else if (cmd == "SHOW") {
            cout << fulltext << '\n';  // print out the text

        } else if (cmd == "EXIT") {
            break;   // finish

        } else {
            // unknown input
            cout << "Unknown cmd. Try again?\n";
        }
    }

    return 0;
}

#include <iostream>
#include <string>
#include "list.h"
using namespace std;

// patient class for keeping name and condition info
class Patient {
public:
    string name;
    string condition;
    Patient(string name, string condition) : name(name), condition(condition) {
        // initializer for the variables
    }
};

// every ward can hold up to 4 ppl
class Ward {
public:
    LinkedList<Patient*> beds;

    bool isFull() const {
        int bedCount = 0;
        Node<Patient*>* temp = beds.pfirst;
        while (temp != nullptr) {
            bedCount++;
            temp = temp->next;
        }
        return bedCount >= 4;  // 4 beds max
    }

    void addPatient(Patient* newPatient) {
        if (!isFull()) {
            beds.addLast(newPatient);  // put them in the next open bed
        }
        // no else,to silently fail if full
    }

    void displayWard() const {
        auto* walker = beds.pfirst;
        while (walker) {
            cout << walker->info->name << " (" << walker->info->condition << ") ";
            walker = walker->next;
        }
        cout << endl;
    }
};

class HospitalManagementSystem {
private:
    LinkedList<Ward*> wards;

    // puts all patients from all wards into one list
    LinkedList<Patient*> flattenPatients() {
        LinkedList<Patient*> patientList;
        Node<Ward*>* wardIt = wards.pfirst;
        while (wardIt) {
            Node<Patient*>* bedWalker = wardIt->info->beds.pfirst;
            while (bedWalker) {
                patientList.addLast(bedWalker->info);
                bedWalker = bedWalker->next;
            }
            wardIt = wardIt->next;
        }
        return patientList;
    }

    // to rebuild the ward after discharging or moving around patients
    void rebuildWards(LinkedList<Patient*>& patients) {
        // clear existing wards first
        while (!wards.isEmpty()) {
            delete wards.pfirst->info;
            wards.removeFirst();
        }

        // start filling wards again from scratch
        Ward* current = new Ward;
        Node<Patient*>* iter = patients.pfirst;

        while (iter) {
            if (current->isFull()) {
                wards.addLast(current);
                current = new Ward;
            }
            current->addPatient(iter->info);
            iter = iter->next;
        }

        if (!current->beds.isEmpty()) {
            wards.addLast(current);
        } else {
            delete current; // nothing in this one
        }
    }

public:
    void admitPatient(const string& name, const string& condition) {
        auto* incoming = new Patient(name, condition);

        // add to a new ward if needed
        if (wards.isEmpty() || wards.plast->info->isFull()) {
            Ward* freshWard = new Ward;
            freshWard->addPatient(incoming);
            wards.addLast(freshWard);
        } else {
            wards.plast->info->addPatient(incoming);
        }
    }

    void dischargePatient(int bedIndex) {
        LinkedList<Patient*> all = flattenPatients();
        Node<Patient*>* current = all.pfirst;

        int pos = 0;
        while (current && pos < bedIndex) {
            current = current->next;
            pos++;
        }

        if (!current) {
            cout << "Invalid bed index.\n";
            return;
        }

        // delete patient memory and remove from list
        delete current->info;
        all.removeFirstOccurrence(current->info);

        // rebuild the whole ward like patient position
        rebuildWards(all);
    }

    void transferPatient(int sourceBedIndex, int targetBedIndex) {
        if (sourceBedIndex == targetBedIndex) return;

        LinkedList<Patient*> flatList = flattenPatients();

        // find source
        Node<Patient*>* source = flatList.pfirst;
        int pos = 0;
        while (source && pos < sourceBedIndex) {
            source = source->next;
            pos++;
        }

        if (!source) {
            cout << "Invalid source index.\n";
            return;
        }

        Patient* toMove = source->info;
        flatList.removeFirstOccurrence(toMove);

        // locate the target position (bed)
        Node<Patient*>* target = flatList.pfirst;
        pos = 0;
        while (target && pos < targetBedIndex) {
            target = target->next;
            pos++;
        }

        if (targetBedIndex >= pos) {
            flatList.addLast(toMove);  // put it at the end
        } else {
            // insert in between manually
            Node<Patient*>* before = target ? target->prev : nullptr;
            auto* nodeToInsert = new Node<Patient*>{toMove, nullptr, nullptr};

            nodeToInsert->next = target;
            nodeToInsert->prev = before;

            if (target) target->prev = nodeToInsert;
            if (before) before->next = nodeToInsert;

            if (!before) flatList.pfirst = nodeToInsert;
            if (!target) flatList.plast = nodeToInsert;
        }

        rebuildWards(flatList);
    }

    void displaySystem() {
        int wardCounter = 1;
        auto* walker = wards.pfirst;

        while (walker) {
            cout << "Ward " << wardCounter++ << ": ";
            walker->info->displayWard();
            walker = walker->next;
        }
    }

    string getPatientInfo(int bedIndex) {
        auto all = flattenPatients();
        auto* iter = all.pfirst;

        int idx = 0;
        while (iter && idx < bedIndex) {
            iter = iter->next;
            idx++;
        }

        if (!iter) return "Invalid bed index.";

        return iter->info->name + " - " + iter->info->condition;
    }

    ~HospitalManagementSystem() {
        // big cleanup to release all patients and wards
        Node<Ward*>* currWard = wards.pfirst;
        while (currWard) {
            Node<Patient*>* bed = currWard->info->beds.pfirst;
            while (bed) {
                delete bed->info;
                bed = bed->next;
            }
            delete currWard->info;
            currWard = currWard->next;
        }
    }
};

int main() {
    HospitalManagementSystem hms;
    string cmd;

    cout << "Hospital Management System\n";
    cout << "Commands: admit, discharge, transfer, info, display, exit\n";

    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "admit") {
            string patientName, condition;
            cin >> patientName;
            getline(cin, condition);

            // i tried to trim
            if (!condition.empty() && condition[0] == ' ') {
                condition.erase(0, 1);
            }

            hms.admitPatient(patientName, condition);

        } else if (cmd == "discharge") {
            int idx;
            cin >> idx;
            hms.dischargePatient(idx);

        } else if (cmd == "transfer") {
            int from, to;
            cin >> from >> to;
            hms.transferPatient(from, to);

        } else if (cmd == "info") {
            int idx;
            cin >> idx;
            cout << hms.getPatientInfo(idx) << endl;

        } else if (cmd == "display") {
            hms.displaySystem();

        } else if (cmd == "exit") {
            break;

        } else {
            cout << "Unknown command.\n";
        }
    }

    return 0;
}

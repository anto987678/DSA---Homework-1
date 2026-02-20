# Big Homework 1 – Data Structures and Algorithms  
### **Stacks, Queues & Lists**

This repository contains three exercises focused on implementing and using **custom data structures** — **Stack**, **Queue**, and **List** — in C++ (without STL containers). Each exercise demonstrates practical applications of these structures through unique real-world problems.

---

## Exercises Overview

### **Exercise 1 – Text Editor (Stack)**
A simple console-based text editor that supports:
- `INSERT` – add text at a given position  
- `DELETE` – remove a substring  
- `UNDO` – revert the last operation  
- `REDO` – reapply an undone operation  

Uses **two stacks** (`undo` and `redo`) to manage operation history and demonstrates stack-based command management.

---

### **Exercise 2 – Hospital Management System (List)**
Simulates patient management in a hospital where each ward can hold up to **4 patients**. Uses **linked lists** to manage wards and patients dynamically.

#### Main operations:
- `admitPatient(name, condition)` – adds a new patient  
- `dischargePatient(index)` – removes and shifts patients  
- `transferPatient(sourceIndex, targetIndex)` – transfers a patient  
- `displaySystem()` – shows ward occupancy  
- `getPatientInfo(index)` – retrieves patient info by bed index  

Ensures no empty beds remain after discharges or transfers.

---

### **Exercise 3 – Playlist Manager (Queue)**
A dynamic **music playlist manager** that handles songs by priority level:
1. **VIP**  
2. **Regular**  
3. **Low**

Implements **queues** for each priority and a **history queue** for played songs.

#### Supported commands:
- `ADD <pref> <duration> <name>` – adds a song  
- `PLAY` – plays next song in queue  
- `SKIP <seconds>` – skip part of a song  
- `REPLAY` – replay last played song  
- `REMOVE <name>` – remove a song  
- `SHOW` – display upcoming playlist  
- `HISTORY` – list played songs  

Input/output is handled via **files** for this exercise.

---

## Technical Details
- **Language**: C++  
- Only **custom implementations** of stack, queue, and list allowed (no STL containers)  
- Each exercise in its folder:  

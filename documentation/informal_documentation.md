# GRAIN_DB 2.0

GrainDB is a lightweight database engine that treats every piece of data as a fundamental building block, ensuring correctness, structure, and control from the smallest unit upward.

**GRAIN_DB 2.0 focuses on correctness, structure, and separation of responsibilities.**

## 📌 Overview

**GRAIN_DB 2.0** is a command-driven database engine that:

- Accepts SQL-like commands  
- Parses user intent  
- Executes logic through a clean control layer  
- Stores data persistently in files  

This project represents the **core backend storage layer** — not a UI, not a web app, and not a full DBMS.

## 🎯 Key Features

- 📂 File-based persistent storage  
- 🔢 Automatic ID generation (database-owned)  
- 🧠 Clear separation of concerns  
- 🧾 SQL-inspired command syntax  
- 🛡️ Safe DELETE with mandatory `WHERE` clause  
- 🧱 Modular architecture (`parser`, `executor`, `storage`)  

## 🧠 Architecture (Mental Model)

Think of the engine as a disciplined office:

| Role | File | Responsibility |
|----|----|----|
| Reception Desk | `main.c` | Input handling & prompt |
| Translator | `parser.c` | Command understanding |
| Decision Manager | `executor.c` | Execution control |
| Store Room | `storage.c` | File I/O & persistence |
| File Cabinets | `data/*.db` | Stored records |

Each module has **one responsibility only**.  
No module interferes with another.

## 🔁 Execution Flow
User
↓
main.c        (input controller)
↓
parser.c      (WHAT is requested?)
↓
executor.c    (HOW to execute?)
↓
storage.c     (WHERE data lives)
↓
executor.c
↓
main.c
↓
User

This flow defines the entire engine.

## 🧾 Supported Commands

### INSERT

insert students Ali 19

* Database automatically assigns a unique ID
* Users do **not** control IDs

Stored as: 1 Ali 19

### SELECT

select students
select students where id=1

Returns stored records matching the query.

### DELETE

delete students where id=1

⚠️ `DELETE` **requires a WHERE clause** to prevent accidental data loss.

## 📂 Storage Model

Each table is stored as a separate file: data/<table_name>.db
Example:
data/students.db

Record format (space-separated):
1 Ali 19
2 Sara 20

## 💾 Persistence
* Data survives program termination
* No in-memory-only storage
* Files act as permanent storage

This persistence is the **core feature of any real database**.

## 🚫 What This Project Is NOT
* ❌ No authentication
* ❌ No passwords
* ❌ No permissions
* ❌ No networking
* ❌ No GUI
* ❌ No web layer
These belong to applications — not database engines.

## ✅ What This Project IS

* A backend database engine
* A learning-focused DB core
* A foundation for advanced database features

# 🧪 Build Instructions
### Requirements
* GCC (Linux/macOS) or MinGW (Windows)
### Compile

```bash
gcc main.c parser/parser.c executor/executor.c storage/storage.c -Iinclude -o grain_db

### Run

```bash
./grain_db
```

## 📈 Versioning

### DB 2.0 (Current)
* Stable `INSERT`, `SELECT`, `DELETE`
* Automatic ID generation
* WHERE filtering by ID
* Modular engine architecture

### Planned (DB 2.1+)
* `help` command (similar to `git help`)
* Built-in command documentation

## 🧠 Core Philosophy
A database does not think.
It only remembers — accurately and consistently.

## 🏷️ Project Status
**Educational · Experimental · Actively Improving**

## ⭐ One-Line Description
*A lightweight C-based database engine that parses commands, executes logic, and persists structured data to disk.*

## 👤 Author : 
**Anousha Asadullah**
Database Engine Developer (C)

# What I've Done

A collection of programming projects, university coursework, development tooling, and technical experiments that I can showcase.

This repository serves as an index for my work. Larger projects and development environments are maintained in their own repositories and linked below.

---

## Development & Infrastructure

### Development Environment Test

A multi-language development environment built and verified across Windows 11 and WSL 2.

The project documents and tests a development setup covering:

- Python
- C
- C++
- Java
- JavaScript
- TypeScript
- C#
- x86-64 Assembly
- Git and GitHub
- CMake
- Visual Studio Code
- WSL 2 / Ubuntu
- GDB and native debugging
- Language-specific debugging workflows

The repository includes small working programs for each language, build configurations, debugging configurations, and documentation for reproducing the environment.

[View the Development Environment Test repository](https://github.com/OmerDeSaban/dev-environment-test)

---

### Dev Services

A reusable local development-services environment built with Docker Desktop and Docker Compose.

The stack includes:

- PostgreSQL
- MySQL
- MariaDB
- Redis
- SQLite
- Docker Compose
- Persistent Docker volumes
- Environment-based configuration
- Local-only service networking
- DBeaver database management
- Redis Insight
- WSL 2 integration

The environment is designed to provide common backend services for future projects without requiring permanent database-server installations directly on Windows.

[View the Dev Services repository](https://github.com/OmerDeSaban/dev-services)

---

## Game Mods

Personal game modifications focused on extending or improving game systems through code and custom tooling.

### Mount & Blade II: Bannerlord — Relation Commands

A lightweight C# mod that adds developer-console commands for changing the player's relation with large groups of characters across a campaign.

The mod supports:

- Setting relation with every living hero in the campaign
- Setting relation with all notables
- Setting relation with all wanderers
- Setting relation with all active clan leaders
- Handling Bannerlord's direct and effective relationship systems
- Supporting living storyline characters that Bannerlord stores as disabled heroes
- Automatic deployment into the Bannerlord module directory during development

The implementation accounts for Bannerlord's shared effective relationships between nobles and clan representatives, ensuring that the final relation displayed in-game matches the requested value.

**Technologies:** C#, .NET Framework 4.7.2, Mount & Blade II: Bannerlord modding API

[View Relation Commands](./Game-Mods/Mount-and-Blade-II-Bannerlord/RelationCommands)

---

## University Projects

Selected coursework demonstrating algorithms, data structures, systems programming, object-oriented design, concurrency, networking, and integration between multiple programming languages.

### Software Project — Clustering Algorithms & Python/C Integration

A multi-stage software project focused on clustering algorithms and integration between Python and native C code.

The coursework includes:

- K-Means implementations in both Python and C
- A native C extension module callable from Python
- K-Means++ initialization
- Symmetric Non-negative Matrix Factorization (SymNMF)
- Python/C interoperability using custom extension modules
- Numerical and data-analysis code

**Technologies:** Python, C, Python C extensions

[View Software Project coursework](./University/Software%20Project)

---

### Operating Systems

A collection of systems-programming assignments implemented primarily in both C and C++.

The coursework includes:

- Low-level operating-system programming exercises
- Command-shell implementations
- Message reader/sender and message-slot components
- Thread-safe queue implementation
- Parallel file-search functionality
- Client/server networking applications
- Multiple implementations in both C and C++

**Technologies:** C, C++, Linux, multithreading, inter-process communication, networking

[View Operating Systems coursework](./University/Operating%20Systems)

---

### Advanced Topics in Programming — Tank Simulation & Algorithms

C++ projects centered around a tank-battle simulation and algorithmic decision-making framework.

The projects include:

- Board and game-state management
- Tank and player abstractions
- Multiple tank/player strategies
- BFS-based navigation and pathfinding
- Factory-based creation of players and tank algorithms
- Separation between interfaces and implementations
- Extensible C++ class architecture

**Technologies:** C++, object-oriented programming, BFS, design patterns, algorithms

[View Advanced Topics in Programming coursework](./University/Advanced%20Topics%20In%20Programming)

---

### Data Structures

Implementations of fundamental advanced data structures in multiple languages.

Projects include:

- AVL Tree implementation in Python
- Binomial Heap implementation in Java

**Technologies:** Python, Java, trees, heaps, algorithmic data structures

[View Data Structures coursework](./University/Data%20Structures)

---

### Additional Java Coursework

Earlier Java assignments covering foundational programming and software-development concepts.

Examples include:

- Array utilities
- String-processing utilities
- Bigram language model implementation
- General Java programming exercises

**Technologies:** Java

[View Software 1 coursework](./University/Software%201)

---

Additional assignments and source code are available throughout the [`University`](./University) directory.

---

## Technologies & Tools

Languages:

- Python
- C
- C++
- C#
- Java
- JavaScript
- TypeScript
- x86-64 Assembly
- SQL

Development tools and platforms:

- Git
- GitHub
- GitHub CLI
- Visual Studio Code
- Visual Studio Build Tools
- CMake
- Ninja
- .NET
- Node.js / npm
- WSL 2
- Ubuntu Linux
- Docker
- Docker Compose
- GDB
- NASM

Databases and data services:

- PostgreSQL
- MySQL
- MariaDB
- SQLite
- Redis

---

## About This Repository

This repository is intended to provide a central overview of work that I can showcase.

Some projects are stored directly within this repository, while larger or independently maintained projects have their own repositories and are linked from here.

The collection will continue to grow as I complete additional university, personal, game-modding, systems, backend, and software-development projects.

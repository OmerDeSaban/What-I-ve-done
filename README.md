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

A lightweight C# mod that adds developer-console commands for changing and persistently locking the player's relation with large groups of characters across a campaign.

The mod supports:

- Setting relation with every living hero in the campaign
- Setting relation with all notables
- Setting relation with all wanderers
- Setting relation with all active clan leaders
- Handling Bannerlord's direct and effective relationship systems
- Supporting living storyline characters that Bannerlord stores as disabled heroes
- Automatic deployment into the Bannerlord module directory during development
- Persistent relation locks with save/load support, lock hierarchy, and automatic re-enforcement

The implementation accounts for Bannerlord's shared effective relationships between nobles and clan representatives, ensuring that the final relation displayed in-game matches the requested value.

**Technologies:** C#, .NET Framework 4.7.2, Mount & Blade II: Bannerlord modding API

[View Relation Commands](./Game-Mods/Mount-and-Blade-II-Bannerlord/RelationCommands)

---

### Mount & Blade II: Bannerlord — Clan Commands

A C# utility mod that adds developer-console commands for discovering, inspecting, listing, and safely destroying clans in a campaign.

The mod supports:

- Finding clans and kingdoms by partial display name or internal ID
- Displaying exact `StringId` values for clans, kingdoms, and leaders
- Inspecting detailed clan information by exact ID
- Listing every active clan in a specified kingdom
- Listing clans currently at war with the player
- Destroying a single clan by exact ID
- Destroying all eligible clans in another kingdom
- Destroying eligible clans currently at war with the player
- Hard safety protections for the player's clan and the player's kingdom
- Automatic deployment into the Bannerlord module directory during development

The destructive commands are intentionally ID-based, while the discovery and list commands expose the IDs needed to target campaign entities precisely.

**Technologies:** C#, .NET Framework 4.7.2, Mount & Blade II: Bannerlord modding API

[View Clan Commands](./Game-Mods/Mount-and-Blade-II-Bannerlord/ClanCommands)

---

### Mount & Blade II: Bannerlord — Character Commands

A C# utility mod that adds developer-console commands for setting, inspecting, and persistently locking the player's and player clan's personality traits.

The mod supports:

- Setting Calculating, Generosity, Honor, Mercy, and Valor
- Automatic normalization of decimal and out-of-range inputs to Bannerlord's valid `-2` to `2` trait levels
- Persistent player-specific trait locks
- Persistent clan-wide trait locks for every living member of the player's clan
- `lock_all` hierarchy with automatic splitting into individual trait locks
- Per-trait values through `lock_all_by_values`
- Player-specific locks overriding clan-wide locks for the player
- Save/load persistence and hourly re-enforcement
- Inspection commands for the player and every living player-clan hero
- Automatic deployment into the Bannerlord module directory during development

**Technologies:** C#, .NET Framework 4.7.2, Mount & Blade II: Bannerlord modding API

[View Character Commands](./Game-Mods/Mount-and-Blade-II-Bannerlord/CharacterCommands)

---

### Mount & Blade II: Bannerlord — Notable Support Commands

A C# utility mod that controls notable-support cost and manages notable relationships while preserving the player's ability to recruit notables as supporters.

The mod supports:

- Finding and inspecting living notables by name or internal ID
- Inspecting raw and effective relations between a notable and any living hero
- Locking notable support cost to a fixed configurable value, defaulting to `10,000`
- Keeping notable-to-notable relations at the campaign maximum
- Keeping notable-to-player-clan relations at the campaign maximum
- Applying a configurable ceiling to other non-player relations without raising values below the ceiling
- Using a dynamic default ceiling of `95` when the campaign relation maximum is `100`
- Persistent relation and support-cost settings with save/load support
- Event-based relation enforcement with a staggered hourly fallback that completes a full sweep roughly every five in-game days
- Automatic deployment into the Bannerlord module directory during development

The implementation accounts for Bannerlord's effective-relation system and the v1.4.5 notable-support rules, including the maximum-relation hard block that can prevent a notable from switching support to the player's clan.

**Technologies:** C#, .NET Framework 4.7.2, Mount & Blade II: Bannerlord modding API

[View Notable Support Commands](./Game-Mods/Mount-and-Blade-II-Bannerlord/NotableSupportCommands)

---

### Total War: ROME REMASTERED — OP Julii

A campaign-focused data mod that turns the Julii into a deliberately overpowered faction while also applying several global campaign, economy, recruitment, diplomacy, and quality-of-life changes.

The mod includes:

- A heavily expanded Julii starting position with additional regions, characters, armies, settlement development, and infrastructure
- Near-free Roman unit recruitment, upkeep, and equipment upgrades for the modified units
- One-turn, one-denarius building construction across the modified building definitions
- Greatly increased Roman recruitment availability
- Powerful Julii-specific trait and ancillary triggers
- Julii exclusions from many existing trait triggers
- Campaign resource quantities raised to the maximum configured value used by the modified campaign file
- Brigand and pirate spawn values reduced to zero
- Strengthened diplomacy, reputation, and merchant-embargo effects
- Revised building text for building variants exposed by the expanded availability
- A Feral launcher-compatible mod-folder structure with `modinfo.json` and a launcher-generated manifest workflow

**Technologies:** Total War: ROME REMASTERED data files, Feral mod launcher and manifest system, JSON, Git

[View OP Julii](./Game-Mods/Total-War-ROME-REMASTERED/OP-Julii)

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

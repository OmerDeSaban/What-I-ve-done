# Character Commands

A lightweight C# mod for **Mount & Blade II: Bannerlord** that adds developer-console commands for setting and persistently locking the five personality traits used by the player and the player's clan.

**Current version:** v1.0.0

Game versions tested on:

- **Bannerlord:** v1.4.5
- **War Sails:** v1.2.5

## Personality Traits

The mod manages:

- Calculating
- Generosity
- Honor
- Mercy
- Valor

Bannerlord uses integer levels from `-2` to `2` for these personality traits.

Character Commands accepts decimal or out-of-range input and normalizes it automatically.

The normalization rules are:

1. Clamp the input to `[-2, 2]`.
2. Round to the nearest integer.
3. Exact `.5` midpoints round toward the numerically higher integer.

Examples:

```text
2.7  ->  2
1.7  ->  2
1.5  ->  2
0.5  ->  1
-0.5 ->  0
-1.5 -> -1
-8   -> -2
```

## Player Commands

### One-Time Setters

```text
trait.set_calculating <value>
trait.set_generosity <value>
trait.set_honor <value>
trait.set_mercy <value>
trait.set_valor <value>
```

### Individual Locks

```text
trait.lock_calculating <value>
trait.lock_generosity <value>
trait.lock_honor <value>
trait.lock_mercy <value>
trait.lock_valor <value>

trait.unlock_calculating
trait.unlock_generosity
trait.unlock_honor
trait.unlock_mercy
trait.unlock_valor
```

### All-Trait Locks

```text
trait.lock_all <value>
trait.lock_all_by_values <calculating> <generosity> <honor> <mercy> <valor>

trait.unlock_all
trait.lock_status
```

## Player-Clan Commands

Clan-wide commands affect every living hero currently belonging to `Clan.PlayerClan`, including the player and living underage clan members.

### One-Time Setters

```text
trait.clan_set_calculating <value>
trait.clan_set_generosity <value>
trait.clan_set_honor <value>
trait.clan_set_mercy <value>
trait.clan_set_valor <value>
```

### Individual Locks

```text
trait.clan_lock_calculating <value>
trait.clan_lock_generosity <value>
trait.clan_lock_honor <value>
trait.clan_lock_mercy <value>
trait.clan_lock_valor <value>

trait.clan_unlock_calculating
trait.clan_unlock_generosity
trait.clan_unlock_honor
trait.clan_unlock_mercy
trait.clan_unlock_valor
```

### All-Trait Locks

```text
trait.clan_lock_all <value>
trait.clan_lock_all_by_values <calculating> <generosity> <honor> <mercy> <valor>

trait.clan_unlock_all
trait.clan_lock_status
```

## Inspection Commands

```text
trait.values
trait.clan_values
```

`trait.values` displays the player's current values for all five managed personality traits.

`trait.clan_values` displays every living hero currently in `Clan.PlayerClan`, including the hero's internal ID and current values for Calculating, Generosity, Honor, Mercy, and Valor.

These commands are read-only and are intended for verification and debugging of one-time setters and persistent locks.

## Lock Hierarchy

Within each scope, `lock_all` sits above the five individual trait locks.

For example:

```text
trait.lock_all 1
```

stores one global player-trait lock at `1`.

Running:

```text
trait.lock_honor 1
```

is redundant because Honor is already effectively locked to `1`.

Running:

```text
trait.lock_honor 2
```

splits the global lock into five individual locks:

```text
Calculating: 1
Generosity: 1
Honor: 2
Mercy: 1
Valor: 1
All: OFF
```

Similarly:

```text
trait.unlock_honor
```

while `trait.lock_all 1` is active splits the global lock and leaves the other four traits locked to `1`.

Running `trait.lock_all` again replaces all player-specific individual locks with a single global lock.

The clan-wide scope follows the same rules with `trait.clan_lock_all`.

## `lock_all_by_values`

The command:

```text
trait.lock_all_by_values 2 1 0 -1 -2
```

creates five individual locks in this fixed order:

```text
Calculating
Generosity
Honor
Mercy
Valor
```

If all five normalized values are identical, Character Commands automatically collapses the state into the simpler `lock_all` representation.

The same behavior applies to:

```text
trait.clan_lock_all_by_values
```

## Player vs Clan Lock Priority

Player-specific locks override clan-wide locks for the player.

For example:

```text
trait.clan_lock_all 1
trait.lock_honor 2
```

produces:

```text
Other living player-clan heroes:
    all five traits = 1

Player:
    Calculating = 1
    Generosity = 1
    Honor = 2
    Mercy = 1
    Valor = 1
```

This prevents the player and clan lock systems from fighting over the same trait.

## Persistence and Re-Enforcement

Trait lock state is stored in the campaign save.

The mod:

- reacts immediately to player personality-trait changes
- performs an hourly fallback check for all active locks
- automatically applies clan-wide locks to new living clan members on the next hourly check
- preserves player-specific overrides when clan-wide locks are active

## Verified Behavior

Character Commands has been tested successfully for:

- decimal and out-of-range trait normalization
- midpoint rounding toward the numerically higher valid trait value
- one-time player trait setters
- `trait.lock_all` hierarchy and redundant-lock detection
- splitting `lock_all` into individual locks when one trait receives a different value
- splitting `lock_all` when one trait is unlocked
- `lock_all_by_values` with five independent trait values
- automatic collapse of `lock_all_by_values` into `lock_all` when all normalized values are identical
- equivalent clan-wide lock hierarchy through the `trait.clan_*` commands
- player-specific locks overriding clan-wide locks for the player
- hourly re-enforcement of clan-wide trait locks
- save/load persistence of both player and clan lock state
- `trait.values` inspection of the player's current traits
- `trait.clan_values` inspection of every living player-clan hero and their current traits

In the tested campaign, the player remained at the player-specific locked values while the rest of the living player clan remained at the separate clan-wide locked values, confirming the intended cross-scope precedence.

## Project Structure

```text
CharacterCommands/
├── Module/
│   └── SubModule.xml
├── src/
│   ├── CharacterCommands.cs
│   ├── TraitLockBehavior.cs
│   └── SubModule.cs
├── CharacterCommands.csproj
└── README.md
```

## Requirements

- Mount & Blade II: Bannerlord
- .NET SDK capable of building .NET Framework 4.7.2 projects
- Bannerlord developer console

The project references TaleWorlds assemblies directly from the local Bannerlord installation.

## Development Setup

The project expects an environment variable named:

```text
BANNERLORD_GAME_DIR
```

pointing to the Bannerlord installation directory.

Example:

```powershell
[Environment]::SetEnvironmentVariable(
    "BANNERLORD_GAME_DIR",
    "C:\Program Files (x86)\Steam\steamapps\common\Mount & Blade II Bannerlord",
    "User"
)
```

After setting the variable, restart the terminal or development environment.

Verify it with:

```powershell
echo $env:BANNERLORD_GAME_DIR
```

## Building

From the `CharacterCommands` directory:

```powershell
dotnet build
```

The project automatically deploys the compiled DLL and `SubModule.xml` into:

```text
Mount & Blade II Bannerlord/
└── Modules/
    └── CharacterCommands/
        ├── SubModule.xml
        └── bin/
            └── Win64_Shipping_Client/
                └── CharacterCommands.dll
```

Bannerlord and the Bannerlord launcher should be closed while building because they can lock the deployed DLL.

## Installation

If building from source, deployment is handled automatically by the project file.

To enable the mod:

1. Launch Mount & Blade II: Bannerlord.
2. Open the Mods section of the launcher.
3. Enable **Character Commands**.
4. Start or load a campaign.
5. Open the developer console.
6. Run one of the available `trait.*` commands.

## Repository

This project is intended to live under the **What I've Done** repository:

```text
Game-Mods/
└── Mount-and-Blade-II-Bannerlord/
    └── CharacterCommands/
```

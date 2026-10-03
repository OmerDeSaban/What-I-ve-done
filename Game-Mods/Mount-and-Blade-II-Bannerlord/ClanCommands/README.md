# Clan Commands

A lightweight C# mod for **Mount & Blade II: Bannerlord** that adds developer-console commands for finding, inspecting, listing, and destroying clans.

The mod is intended for campaign cleanup and debugging, particularly in long-running or heavily modded campaigns.

**Current version:** v1.0.0

Game versions tested on:

- **Bannerlord:** v1.4.5
- **War Sails:** v1.2.5

## Features

Clan Commands provides discovery commands that expose both in-game names and internal `StringId` values, plus destructive commands that use exact IDs.

| Command | Description |
| --- | --- |
| `clan.find <name-or-id>` | Finds clans by partial in-game name or partial ID and displays their IDs |
| `clan.info <clan_id>` | Displays detailed information for one clan using its exact ID |
| `clan.destroy <clan_id>` | Destroys one clan using its exact ID |
| `clan.find_kingdom <name-or-id>` | Finds kingdoms by partial in-game name or partial ID and displays their IDs |
| `clan.list_kingdom <kingdom_id>` | Lists all active clans belonging to a kingdom, including clan IDs |
| `clan.destroy_kingdom_clans <kingdom_id>` | Destroys all active clans belonging to another kingdom |
| `clan.list_war` | Lists active non-bandit clans currently at war with the player, including IDs |
| `clan.destroy_war_clans` | Destroys all eligible clans currently at war with the player |

## ID-Based Workflow

The destructive commands intentionally use exact internal IDs rather than display names.

The find and list commands always print IDs alongside in-game names. This makes them the discovery and preview layer for destructive commands.

For example:

```text
clan.find Maneolis
```

can return output in this form:

```text
- Maneolis | ID: clan_empire_west_8 | Kingdom: Independent | Leader: Vipon (ID: lord_1_71) | Eliminated: No
```

You can then inspect the exact clan:

```text
clan.info clan_empire_west_8
```

and, if desired, destroy it:

```text
clan.destroy clan_empire_west_8
```

## Safety Rules

The mod includes hard safety checks around destructive operations.

### Player Clan

`Clan.PlayerClan` can never be destroyed by any command in this mod.

```text
clan.destroy <clan_id>
```

refuses to continue when the supplied ID belongs to the player's clan.

### Player Kingdom

```text
clan.destroy_kingdom_clans <kingdom_id>
```

refuses to target the player's current kingdom.

The player's clan is also explicitly excluded again inside the bulk-destruction loop.

### War Cleanup

`clan.destroy_war_clans` excludes:

- the player's clan
- every clan belonging to the player's current kingdom
- already eliminated clans
- bandit factions

War state is evaluated through the campaign's faction-war state so clans belonging to hostile kingdoms can be identified alongside independent hostile clans.

## Commands

### Find Clan

```text
clan.find <name-or-id>
```

Searches both the clan's displayed name and its internal `StringId`.

The search is case-insensitive and supports partial matches.

Every result includes:

- clan name and ID
- kingdom name and ID, when applicable
- leader name and ID, when applicable
- eliminated status

### Clan Info

```text
clan.info <clan_id>
```

Looks up one clan by exact `StringId` and displays:

- clan name and ID
- leader name and ID
- kingdom name and ID
- tier
- renown
- gold
- eliminated status
- whether it is the player's clan

### Destroy Clan

```text
clan.destroy <clan_id>
```

Destroys one active clan by exact `StringId`.

The player's own clan is always protected.

Clan elimination is handled through Bannerlord's normal clan-destruction logic. In testing, this correctly marked both adult and underage clan members as dead when the clan was eliminated.

### Find Kingdom

```text
clan.find_kingdom <name-or-id>
```

Searches kingdom display names and internal IDs.

Every result includes:

- kingdom name and ID
- ruling-clan name and ID
- eliminated status

### List Kingdom Clans

```text
clan.list_kingdom <kingdom_id>
```

Lists every active clan currently belonging to the specified kingdom.

The kingdom ID is printed in the heading, and every clan result includes its clan ID.

This is an informational command, so it also includes the player's clan when listing the player's own kingdom.

### Destroy Kingdom Clans

```text
clan.destroy_kingdom_clans <kingdom_id>
```

Destroys all active clans currently belonging to the specified kingdom.

The target kingdom cannot be the player's current kingdom.

The complete target list is collected before destruction begins so campaign collection changes do not alter the target set halfway through the command.

### List War Clans

```text
clan.list_war
```

Lists active non-bandit clans whose current faction is at war with the player's faction.

Clans belonging to the player's own kingdom are always excluded.

Every result includes the clan ID, kingdom ID when applicable, and leader ID when applicable.

### Destroy War Clans

```text
clan.destroy_war_clans
```

Destroys the same eligible set of clans identified by `clan.list_war`.

The player clan and all clans in the player's kingdom are protected by repeated hard checks.

## Verified Behavior

The mod has been tested successfully for:

- partial clan-name lookup with clan IDs in the results
- exact clan lookup by `StringId`
- partial kingdom-name lookup with kingdom IDs in the results
- kingdom clan listing with clan, kingdom, and leader IDs
- war-target listing with clan IDs
- refusal to destroy the player's clan
- refusal to destroy clans belonging to the player's kingdom
- destruction of a single target clan
- destruction of all clans in a target kingdom
- destruction of eligible clans currently at war with the player
- correct clan elimination in the Encyclopedia after destruction

During testing, eliminated underage heroes were correctly marked as dead by the campaign even though Bannerlord's generic baby portrait can remain colored rather than grayscale. This is cosmetic and does not indicate that the hero is still alive.

## Project Structure

```text
ClanCommands/
├── Module/
│   └── SubModule.xml
├── src/
│   ├── ClanCommands.cs
│   └── SubModule.cs
├── ClanCommands.csproj
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

From the `ClanCommands` directory:

```powershell
dotnet build
```

The project automatically deploys the compiled DLL and `SubModule.xml` into:

```text
Mount & Blade II Bannerlord/
└── Modules/
    └── ClanCommands/
        ├── SubModule.xml
        └── bin/
            └── Win64_Shipping_Client/
                └── ClanCommands.dll
```

Bannerlord and the Bannerlord launcher should be closed while building because they can lock the deployed DLL.

## Installation

If building from source, deployment is handled automatically by the project file.

To enable the mod:

1. Launch Mount & Blade II: Bannerlord.
2. Open the Mods section of the launcher.
3. Enable **Clan Commands**.
4. Start or load a campaign.
5. Open the developer console.
6. Use a find/list command before destructive operations.

## Repository

This project is part of the **What I've Done** repository, a collection of programming projects, tools, mods, and other development work.

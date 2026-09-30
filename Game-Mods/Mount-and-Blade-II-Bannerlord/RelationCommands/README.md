# Relation Commands

A lightweight C# mod for **Mount & Blade II: Bannerlord** that adds console commands for setting the player's relation with large groups of characters at once.

The mod was created to make campaign relationship management easier, particularly in long-running or heavily modded campaigns where manually changing relations with hundreds or thousands of heroes is impractical.

## Features

Relation Commands adds four console commands:

| Command | Description |
| --- | --- |
| `relation.set_everyone <value>` | Sets relation with every living hero in the campaign |
| `relation.set_notables <value>` | Sets relation with all living notables |
| `relation.set_wanderers <value>` | Sets relation with all living wanderers |
| `relation.set_clans <value>` | Sets relation with the leader of every active clan |

Relation values must be between:

```text
-100 and 100
```

For example:

```text
relation.set_everyone 100
relation.set_notables 50
relation.set_wanderers -25
relation.set_clans 75
```

## How It Works

Bannerlord distinguishes between a hero's direct personal relation and the effective relation displayed to the player.

For many characters, setting the direct personal relation is sufficient. However, nobles can use an effective relationship pair shared through their clan or another representative hero.

Because of this, the mod uses a multi-step process:

1. Set the direct personal relation for every target hero.
2. Resolve Bannerlord's effective relationship pair for each hero.
3. Correct each unique effective relationship pair once.
4. Verify the final effective relation.

This prevents shared clan relationships from being repeatedly overwritten while ensuring that the value displayed in-game matches the requested value.

The mod also accounts for living storyline characters that Bannerlord stores in `DeadOrDisabledHeroes` rather than `AllAliveHeroes`.

This includes characters that remain alive but have been disabled after their role in a storyline or quest has changed.

## Verified Behavior

The `relation.set_everyone` command has been tested successfully on a campaign containing more than 2,400 living heroes.

Example:

```text
relation.set_everyone 100
```

Result:

```text
Processed 2466 living heroes. 2466 now have relation 100.
```

The command has also been tested successfully with intermediate values such as:

```text
relation.set_everyone 50
```

## Project Structure

```text
RelationCommands/
├── Module/
│   └── SubModule.xml
├── src/
│   ├── RelationCommands.cs
│   └── SubModule.cs
├── RelationCommands.csproj
└── README.md
```

## Requirements

- Mount & Blade II: Bannerlord
- .NET SDK capable of building .NET Framework 4.7.2 projects
- Bannerlord developer console

The project references the TaleWorlds assemblies directly from the local Bannerlord installation.

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

From the `RelationCommands` directory:

```powershell
dotnet build
```

The project automatically deploys the compiled DLL and `SubModule.xml` into:

```text
Mount & Blade II Bannerlord/
└── Modules/
    └── RelationCommands/
        ├── SubModule.xml
        └── bin/
            └── Win64_Shipping_Client/
                └── RelationCommands.dll
```

Bannerlord and the Bannerlord launcher should be closed while building because they can lock the deployed DLL.

## Installation

If building from source, deployment is handled automatically by the project file.

To enable the mod:

1. Launch Mount & Blade II: Bannerlord.
2. Open the Mods section of the launcher.
3. Enable **Relation Commands**.
4. Start or load a campaign.
5. Open the developer console.
6. Run one of the available commands.

Example:

```text
relation.set_everyone 100
```

## Commands

### Set Everyone

```text
relation.set_everyone <value>
```

Sets the effective relation with every living hero in the campaign.

This includes:

- nobles
- faction rulers
- clan leaders
- notables
- wanderers
- companions
- minor-faction heroes
- living storyline heroes
- living disabled heroes

The player's own character is excluded.

### Set Notables

```text
relation.set_notables <value>
```

Sets relation with every living notable.

### Set Wanderers

```text
relation.set_wanderers <value>
```

Sets relation with every living wanderer.

### Set Clans

```text
relation.set_clans <value>
```

Sets relation with the leader of every active clan other than the player's own clan.

## Notes

The requested relation must be between `-100` and `100`.

The mod intentionally operates on the effective relationship displayed and used by Bannerlord rather than assuming that every character's direct personal relation is the final displayed value.

Dead heroes are not modified.

## Repository

This project is part of the **What I've Done** repository, a collection of programming projects, tools, mods, and other development work.

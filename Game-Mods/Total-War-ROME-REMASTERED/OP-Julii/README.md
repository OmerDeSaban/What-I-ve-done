# OP Julii

A personal **Total War: ROME REMASTERED** campaign overhaul built around making the Julii deliberately overpowered while reshaping a large part of the Roman campaign experience.

The mod began as direct edits to the game's data files and has since been repackaged as a proper Feral launcher-compatible mod. It modifies the Imperial Campaign starting setup, settlement development, recruitment, unit costs, Roman culture rules, traits, ancillaries, diplomacy/reputation values, campaign resources, and building localization.

This is intentionally not a balanced mod. Its purpose is to create a heavily accelerated, high-power Julii campaign with an enormous starting advantage and very low economic friction.

## Current Status

The mod is packaged for the ROME REMASTERED launcher under:

```text
launcher/OP_Julii/
```

The repository currently contains:

- `modinfo.json`
- a generated `filelist.json` manifest
- all eight modified game-data files in their correct launcher-relative paths
- documentation of the original vanilla file locations

The generated manifest currently contains all eight modded data files and their checksums, so the repository represents a complete launcher package rather than the earlier direct-file-replacement setup.

The original vanilla backup files are intentionally not stored in Git.

## Overall Effect of the Mod

OP Julii changes much more than the Julii starting army. Its main effects are:

- expands the Julii from two starting regions to four;
- gives the Julii a very large starting family, agent network, navy, and collection of armies;
- heavily develops all Julii starting settlements and raises their populations;
- deliberately reduces the other three Roman factions to extremely small starting forces;
- makes every campaign-map resource use the maximum quantity value found in the vanilla file;
- disables normal brigand and pirate spawning;
- makes every building level cost `1` denarius and take `1` turn to construct;
- makes 39 modified Roman unit entries recruit in `0` turns for `1` denarius with `1` denarius upkeep and `1` denarius equipment-upgrade costs;
- makes a large Roman roster recruit at experience level `9` from applicable buildings;
- gives the Julii access to additional gladiator units;
- removes the one-temple restriction and greatly expands the temple system;
- gives Julii characters and agents extremely powerful trait packages and repeatedly restores many of those traits if they fall below their intended values;
- gives Julii characters and agents curated ancillary packages and restores missing ancillaries over time;
- makes Roman agents and basic campaign infrastructure almost free;
- makes Rome the only culture in this configuration able to reach the `huge_city` settlement tier;
- strengthens several diplomacy/reputation rewards and greatly increases the merchant-trade relationship income multiplier;
- adds usable building descriptions for combinations exposed by the expanded building system.

Some of the building and diplomacy changes are **global**, not Julii-only. The Julii receive the largest advantage because the Roman culture, Roman unit roster, starting campaign setup, traits, and ancillaries are all modified in their favor.

## Campaign Starting Position — `descr_strat.txt`

The Imperial Campaign setup is heavily rewritten.

### Julii Territory and Settlements

The Julii start with four settlements instead of the vanilla two:

| Region | Settlement level | Population |
| --- | --- | ---: |
| Etruria | City | 9,509 |
| Umbria | City | 9,509 |
| Apulia | City | 9,509 |
| Sicilia Romanus | City | 9,509 |

Apulia is transferred from the Brutii and Sicilia Romanus is transferred from the Scipii.

Each Julii settlement starts with **38 buildings**, including advanced government, walls, military recruitment, economic infrastructure, roads, sanitation, entertainment, education, and the full set of temple chains at `large_temple` level.

The Julii treasury remains at the vanilla value of `5000` denarii; their advantage comes from territory, infrastructure, characters, armies, resources, and the near-free economy defined elsewhere in the mod.

### Julii Characters, Agents, Armies, and Fleets

The starting Julii presence is expanded dramatically.

| Starting content | Vanilla | OP Julii |
| --- | ---: | ---: |
| Named characters | 4 | 14 |
| General-type agents | — | 7 |
| Admirals | 1 | 5 |
| Diplomats | 1 | 10 |
| Spies | 1 | 17 |
| Assassins | 0 | 4 |
| Character records | 9 | 54 |
| Armies | 5 | 26 |
| Units in those armies | 17 | 405 |

The result is a Julii faction that begins with enough military and agent coverage to operate across several fronts immediately.

### Other Roman Factions

The other Roman factions are intentionally reduced so the Julii dominate the Roman side of the campaign from the beginning.

- **Brutii** lose Apulia and retain Bruttium.
- **Scipii** lose Sicilia Romanus and retain Campania.
- **Senate / SPQR** retain Latium.
- Bruttium, Campania, and Latium are each raised to roughly 9,528 population and receive the same broad 38-building starting infrastructure package.
- Each of the Brutii, Scipii, and Senate is reduced to one named character, one associated character record, one army, and one bodyguard unit in the modified starting setup.

### Campaign Resources and Spawning

`descr_strat.txt` also changes global campaign conditions:

- `brigand_spawn_value` is changed from `10` to `0`;
- `pirate_spawn_value` is changed from `28` to `0`;
- all **688** campaign-map resource entries are set to quantity `3`.

In the vanilla file, most resources use quantity `1`, some use `2`, and only a small number use `3`. OP Julii normalizes every resource entry to `3`.

### Core Attitudes Toward the Julii

The file also changes the campaign AI's core-attitude values toward `romans_julii` for a broad range of non-Roman factions. Macedon, Egypt, the Seleucids, Carthage, Parthia, Pontus, Gaul, Germania, Britannia, Armenia, Dacia, the Greek Cities, Numidia, Scythia, Spain, and Thrace are given a core-attitude value of `100` toward the Julii.

This changes their underlying diplomatic disposition toward the Julii; it does **not** directly create starting alliances.

## Buildings and Recruitment — `export_descr_buildings.txt`

This is one of the largest parts of the mod.

### Near-Free, One-Turn Construction

All **170 building levels** present in the modified file use:

```text
construction 1
cost 1
```

This means building construction is reduced to one turn and one denarius throughout the modified building database.

### Experience-9 Roman Recruitment

A large portion of the Roman recruitment tree is modified so applicable Roman units are recruited at **experience level 9**.

The file contains **136 Roman recruitment lines** using experience `9`, covering **34 distinct Roman unit types**, plus additional experience-9 naval recruitment lines.

Government buildings also gain high-experience Roman general/bodyguard recruitment.

### Expanded Building Availability

Many building chains have their faction/culture requirements broadened. Low- and mid-tier military, defensive, economic, entertainment, infrastructure, and temple buildings can therefore appear for more cultures than in vanilla.

The highest levels of some chains remain restricted, and the separate `descr_cultures.txt` changes mean Roman settlements retain an important advantage because Rome is still allowed to reach `huge_city` while other cultures are capped below it.

### Reworked Temple System

The temple system is changed especially heavily:

- all **21** temple chains have the vanilla `no_other_temple` restriction removed;
- multiple different temple chains can therefore coexist in a settlement;
- many temple capabilities are expanded or standardized across farming, happiness, law, population growth, health, trade, morale, agent capacity, and recruitment;
- temple recruitment pools are greatly expanded and can include special units from multiple cultures;
- high-tier temples commonly provide very strong bonuses and additional elite recruitment;
- many temple/building `ai_destruction_hint` entries are removed, helping the expanded building combinations persist after conquest;
- five additional high-tier temple levels are present in the modified file:
  - `temple_of_battleforge_pantheon`
  - `temple_of_viking_awesome_temple`
  - `temple_of_viking_pantheon`
  - `temple_of_horse_2_awesome_temple`
  - `temple_of_horse_2_pantheon`

The 21 temple chains represented in the file are Battle, Battleforge, Farming, Fertility, Forge, Fun, Governors, Healing, Horse, Hunting, Justice, Law, Leadership, Love, Naval, One God, Trade, Victory, Violence, Viking, and Horse 2.

This is why the starting Roman settlements in `descr_strat.txt` can contain many temples simultaneously instead of being limited to a single temple chain.

## Units — `export_descr_unit.txt`

The unit database contains **39 modified unit entries**.

For those entries, the `stat_cost` values are changed so that:

```text
recruitment time = 0
recruitment cost = 1
upkeep = 1
weapon upgrade cost = 1
armour upgrade cost = 1
```

The final custom-battle cost value is retained from the original unit definition.

### Modified Infantry and Special Units

`roman peasant`, `roman archer`, `roman archer auxillia`, `roman velite`, `roman light infantry auxillia`, `roman city militia`, `roman hastati`, `roman princeps`, `roman triarii`, `roman infantry auxillia`, both standard and first-cohort legionary variants, praetorian and urban cohorts, `roman arcani`, war dogs, incendiary pigs, and the Roman gladiator entries are included.

### Modified Cavalry

The modified cavalry entries include Roman light cavalry, auxiliary cavalry, medium cavalry, heavy cavalry, praetorian cavalry, and both early and later Roman general bodyguard cavalry.

### Modified Siege Units

Ballistae, scorpions, onagers, heavy onagers, and repeating ballistae receive the same near-free cost treatment.

### Modified Naval Units

Biremes, triremes, quinqueremes, corvus quinqueremes, and deceres are also modified.

### Additional Julii Unit Ownership

Two gladiator ownership lists are expanded:

- `roman velite gladiator` adds `romans_julii`;
- `roman mirmillo gladiator` adds `romans_julii`.

This gives the Julii access to gladiator types that were associated with other Roman factions in the vanilla unit file, provided the relevant recruitment building is available.

## Roman Culture Rules — `descr_cultures.txt`

Roman campaign infrastructure and agents are made almost free:

| Roman item | Vanilla cost | OP Julii cost |
| --- | ---: | ---: |
| Fort | 500 | 1 |
| Watchtower | 200 | 1 |
| Spy | 350 | 1 |
| Assassin | 500 | 1 |
| Diplomat | 250 | 1 |
| Merchant | 250 | 1 |
| Admiral | 100 | 1 |

The corresponding `recruitment points` field for the five Roman agent/naval recruitment entries is also changed from `1` to `0`.

Settlement progression is changed as well:

- Roman maximum settlement level remains `huge_city`.
- Barbarian maximum level is increased from `city` to `large_city`.
- Carthaginian, Greek, Egyptian, and Eastern maximum levels are reduced from `huge_city` to `large_city`.

This leaves Roman culture as the only culture in this configuration that can reach the `huge_city` tier.

## Julii Traits — `export_descr_character_traits.txt`

The trait file contains a large custom Julii system rather than a few isolated bonuses.

There are **181 new `OP_Julii_*` triggers**:

- 15 `AgentCreated` triggers;
- 17 `CharacterComesOfAge` triggers;
- 149 `CharacterTurnEnd` triggers.

In addition, **121 existing vanilla triggers** are modified to exclude the Julii from their normal outcomes, usually through a `not FactionType romans_julii` condition.

### Agent Trait Packages

New Julii agents receive very strong role-specific packages at creation.

- Merchants receive high merchant ability, monopoly, security, resource knowledge, guild/training, collector, discipline, energy, and logistical traits.
- Assassins receive very high assassin and conspirator ability plus security and discipline support traits.
- Spies receive very high spy and conspirator ability plus the same supporting traits.
- Diplomats receive very high diplomatic, smooth-talking, and natural diplomatic skill plus security and logistical traits.

### Family Member / General Package

Characters coming of age receive an enormous curated package including very high command, attack, defense, administration, politics, military skill, night-battle ability, trade/building/agent-management skill, Roman offices, loyalty/security/fertility traits, and a number of flavor or high-dread traits.

This package is intentionally overpowered rather than designed around natural trait progression.

### Persistent Trait Enforcement

Many of the `CharacterTurnEnd` triggers act as a self-repair system. If a Julii character or agent falls below the intended threshold for a trait, the mod grants that trait again at a 100% trigger chance.

For example, commander, merchant, spy, assassin, diplomat, and admiral traits can be re-applied when their values fall below the configured thresholds. This means much of the Julii trait advantage is persistent instead of being a one-time starting bonus.

## Julii Ancillaries — `export_descr_ancillaries.txt`

The ancillary file adds **58 Julii-specific triggers**:

- 8 `AgentCreated` triggers;
- 2 `CharacterComesOfAge` triggers;
- 48 `CharacterTurnEnd` triggers.

Characters receive role-specific ancillary packages, and the turn-end triggers can restore missing ancillaries later.

Examples of the packages include:

- **Merchant:** merchant, Numismatist, equestrian, scribe, silk merchant, and multiple trade-related priests.
- **Assassin:** dancer, partner-in-crime, poisoner, pet monkey, networker, bodyguard, food taster, and cook.
- **Spy:** dancer, partner-in-crime, poisoner, pet monkey, networker, scout, intrepid explorer, and caravan driver.
- **Diplomat:** linguist, rhetorician, foreign dignitary, bodyguard, food taster, cook, wrestler, and pet lion.
- **General / family member:** bodyguard, drillmaster, military engineer, siege engineer, spymaster, praetorian guardsman, orator, and soothsayer.
- **Admiral:** navigator, seamaster, priest of Neptune, shipwright, drillmaster, praetorian guardsman, intrepid explorer, and caravan driver.

Like the trait system, this is designed to keep Julii characters consistently powerful over the course of the campaign.

## Diplomacy, Reputation, and Trade — `feral_descr_reputations_and_relations.txt`

Several global reputation and relationship values are increased:

| Effect | Vanilla | OP Julii |
| --- | ---: | ---: |
| Long-term alliance reputation | 5 | 20 |
| Accepted ceasefire | 50 | 100 |
| Accepted military assistance | 100 | 200 |
| Ally-of-an-ally relationship | 50 | 100 |
| Merchant-trade income multiplier | 1 | 5 |
| Merchant-trade relationship cap | 30 | 100 |

These changes make successful peaceful/diplomatic interactions more rewarding and make the merchant-trade relationship system substantially more valuable.

These values are global campaign-system changes rather than Julii-only modifiers.

## Building Text — `data/text/export_buildings.txt`

The building localization file is expanded because the modified building database exposes many building/culture combinations that the vanilla campaign normally never needs to display.

The mod replaces approximately **1,348** placeholder warning-description lines such as:

```text
WARNING! This baseline description should never appear on screen!
```

with usable building descriptions and adds many culture-specific localization keys for the newly exposed variants.

This prevents the expanded building system from displaying internal placeholder text to the player.

## File-by-File Summary

| File | What OP Julii changes |
| --- | --- |
| `world/maps/campaign/imperial_campaign/descr_strat.txt` | Gives the Julii four starting regions, heavily developed settlements, a vastly larger family/agent roster, 26 armies and 405 starting units; strips down the other Roman factions; sets all 688 resources to quantity 3; disables brigand/pirate spawning; changes core attitudes toward the Julii. |
| `export_descr_buildings.txt` | Sets all building construction to 1 turn / 1 denarius; makes much of the Roman roster recruit at experience 9; broadens building availability; removes the one-temple restriction; massively expands temple capabilities/recruitment; adds several missing high temple tiers. |
| `export_descr_unit.txt` | Changes 39 Roman unit entries to 0-turn recruitment, 1-denarius recruitment/upkeep/equipment upgrades, while retaining custom-battle prices; adds Julii ownership to Velite and Mirmillo Gladiators. |
| `export_descr_character_traits.txt` | Adds 181 Julii-specific trait triggers, including creation/coming-of-age packages and persistent turn-end enforcement; excludes Julii from 121 normal vanilla trait triggers. |
| `export_descr_ancillaries.txt` | Adds 58 Julii-specific triggers that grant and restore strong role-based ancillary packages for merchants, assassins, spies, diplomats, generals, and admirals. |
| `descr_cultures.txt` | Makes Roman forts, watchtowers, agents, merchants, and admirals cost 1; changes their recruitment-point fields; changes culture settlement caps so only Romans can reach `huge_city`. |
| `text/export_buildings.txt` | Replaces large numbers of hidden/placeholder descriptions and adds localization for building/culture combinations exposed by the expanded building system. |
| `feral_descr_reputations_and_relations.txt` | Strengthens alliance/ceasefire/military-assistance reputation effects and ally relationships; multiplies merchant-trade income and increases its relationship cap. |

## Project Structure

```text
OP-Julii/
├── launcher/
│   └── OP_Julii/
│       ├── modinfo.json
│       ├── filelist.json
│       └── data/
│           ├── descr_cultures.txt
│           ├── export_descr_ancillaries.txt
│           ├── export_descr_buildings.txt
│           ├── export_descr_character_traits.txt
│           ├── export_descr_unit.txt
│           ├── feral_descr_reputations_and_relations.txt
│           ├── text/
│           │   └── export_buildings.txt
│           └── world/
│               └── maps/
│                   └── campaign/
│                       └── imperial_campaign/
│                           └── descr_strat.txt
├── docs/
│   └── source-paths.md
└── README.md
```

## Installation

The mod is intended to be loaded through the ROME REMASTERED Feral launcher rather than by overwriting the base game.

Copy:

```text
launcher\OP_Julii
```

into:

```text
%LOCALAPPDATA%\Feral Interactive\Total War ROME REMASTERED\Mods\My Mods\
```

The installed result should therefore be:

```text
%LOCALAPPDATA%\Feral Interactive\Total War ROME REMASTERED\Mods\My Mods\OP_Julii\
```

Before testing the launcher version, the base-game files should be restored to vanilla so there is no ambiguity about whether the launcher mod is actually providing the changes.

Enable **OP Julii** in the mod manager and start a **new Imperial Campaign**. Changes made by `descr_strat.txt`, such as starting territories, armies, agents, populations, and buildings, only apply when a new campaign is created.

## Maintaining the Launcher Manifest

`filelist.json` is already present in the repository and contains the eight current mod-data files.

Whenever any file under `launcher/OP_Julii/data/` is changed, added, or removed, regenerate the manifest through the ROME REMASTERED launcher before treating the package as final. The regenerated `filelist.json` should then be committed with the data changes.

## Technical Notes

The gameplay files are plain-text **Total War data/configuration files using Creative Assembly/Feral's game-specific syntax**, rather than source code written in a general-purpose language such as C++ or C#.

The launcher metadata files, `modinfo.json` and `filelist.json`, use **JSON**.

**Technologies:** Total War: ROME REMASTERED data modding, Creative Assembly/Feral configuration formats, Feral launcher mod packaging, JSON, Git

## Repository Location

This project is part of the **What I've Done** repository:

```text
Game-Mods/
└── Total-War-ROME-REMASTERED/
    └── OP-Julii/
```

Only the modified mod files, launcher metadata, manifest, and documentation are stored here. Vanilla game files and backup archives should remain outside the repository.

## Modding References

- Feral Interactive's ROME REMASTERED modding repository: <https://github.com/FeralInteractive/romeremastered>
- Total War Center's ROME REMASTERED modding documentation and data-file references

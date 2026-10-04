# OP All

A personal **Total War: ROME REMASTERED** Imperial Campaign overhaul built around one premise:

> Every faction is overpowered, diplomacy is unstable, and the campaign should keep escalating until only one major power remains.

OP All is the all-factions successor to the earlier OP Julii project. Instead of making one Roman faction overwhelmingly stronger than the rest, the long-term goal is to give every playable faction the same broad structural advantages while preserving each faction's original unit-stat identity.

The campaign-start conversion is intentionally being done **incrementally**. The Julii are the current test faction in `descr_strat.txt`; additional factions should be converted in small steps and tested after each step so campaign-loading failures are easy to isolate.

## Current Status

**Status: pre-alpha / launcher-test scaffold**

The current launcher package lives under:

```text
launcher/OP-All/
```

At this stage:

- all 21 Imperial Campaign factions are listed as playable;
- brigand and pirate spawning are disabled;
- all 688 campaign-map resource entries use quantity `3`;
- the Julii have received most of the current `descr_strat.txt` test changes;
- the other factions should be converted gradually rather than all at once;
- building construction and unit recruitment are globally accelerated and nearly free;
- all recruitment entries in the current building file grant experience `9`;
- all cultures use the same high-end settlement/campaign-cost rules;
- all-faction OP trait packages are implemented with creation/coming-of-age and persistent turn-end repair triggers;
- all-faction curated ancillary packages are implemented for generals, admirals, spies, assassins, diplomats, and merchants;
- the campaign AI is configured for maximum diplomatic instability and aggressive military behavior;
- reputation recovery from peaceful diplomacy and merchant trade has effectively been removed.

`filelist.json` is **not yet present** and still needs to be generated/refreshed through the ROME REMASTERED launcher after the final files for a test build are in place.

## Design Goals

OP All deliberately does **not** try to make every unit identical.

The intended balance is:

- make money, recruitment, construction, agents, traits, ancillaries, and campaign infrastructure extremely generous for everyone;
- keep the original internal unit combat stats so faction rosters retain their normal strengths and weaknesses;
- preserve cultural differences where the data files require them;
- remove economic friction so the AI can field large armies quickly;
- make diplomacy volatile enough that powerful factions do not settle into permanent peaceful blocs;
- change `descr_strat.txt` one faction at a time and test after each meaningful edit.

## Implemented Files

### `data/world/maps/campaign/imperial_campaign/descr_strat.txt`

The campaign file currently acts as the staged testing area.

Implemented global changes include:

- all 21 campaign factions moved into the `playable` block;
- `brigand_spawn_value 0`;
- `pirate_spawn_value 0`;
- all **688** resource entries set to quantity `3`.

The Julii currently contain most of the custom starting-position work. The rest of the factions should be upgraded incrementally and tested after each batch rather than converted simultaneously.

Because `descr_strat.txt` controls starting settlements, characters, traits, ancillaries, armies, and ownership, its changes require a **new Imperial Campaign** to test.

Exact culture/role-specific starting trait and ancillary lines are documented in:

```text
docs/descr-strat-character-packages.md
```

### `data/export_descr_buildings.txt`

The current building database contains **170** building levels, all configured with:

```text
construction 1
cost 1
```

All **1,732** active recruitment lines currently use experience level `9`.

The file also contains the expanded all-culture building/temple setup inherited from the OP conversion work, including broad recruitment and temple bonuses.

### `data/export_descr_unit.txt`

All **299** active `stat_cost` lines — base units plus Remastered `rebalance_statblock` entries — use:

```text
stat_cost        0, 1, 1, 1, 1, 1
```

This means:

- 0-turn recruitment;
- 1 denarius recruitment cost;
- 1 denarius upkeep;
- 1 denarius weapon-upgrade cost;
- 1 denarius armour-upgrade cost;
- 1 denarius custom-battle cost.

No unit combat stats were intentionally changed as part of this cost pass.

### `data/descr_cultures.txt`

All six campaign cultures currently share the OP campaign rules:

- maximum settlement level: `huge_city`;
- fort cost: `1`;
- watchtower cost: `1`;
- spy, assassin, diplomat, merchant, and admiral recruitment cost: `1`;
- agent population cost: `1`;
- agent recruitment points: `0`;
- very high population/squalor limits and effectively removed distance-from-capital pressure.

The purpose is to remove culture-based economic handicaps while retaining the cultures themselves for traits, ancillaries, visuals, and faction identity.

### `data/export_descr_character_traits.txt`

The trait file contains **270 `OP_ALL_*` triggers**.

The system is role- and culture-aware:

- admirals, assassins, spies, diplomats, and merchants receive their packages through `AgentCreated`;
- generals/family members receive their package through `CharacterComesOfAge`;
- `CharacterTurnEnd` triggers continually repair the intended OP traits;
- culture exclusions are respected;
- deliberately lower-than-maximum traits have upper-bound correction logic so harmful higher levels do not persist;
- faction-leader/heir marker traits are not part of the OP package.

The exact `descr_strat.txt` trait lines generated from these current packages are in `docs/descr-strat-character-packages.md`.

### `data/export_descr_ancillaries.txt`

The old Julii-only ancillary system has been replaced with an all-factions system.

Each role receives a curated **8-ancillary** package:

- General
- Admiral
- Spy
- Assassin
- Diplomat
- Merchant

Rules used for the packages:

- no `Unique` / one-off ancillaries;
- exactly eight followers, matching the character retinue limit;
- OP triggers are placed before vanilla ancillary triggers so the curated package fills the available slots first;
- non-Barbarian and Barbarian variants are used where exclusions or role usefulness differ;
- effects that are already effectively capped by the trait package are deprioritized;
- movement, line of sight, personal security, surgery, training, trading, and other role-relevant effects are preferred.

The file contains both initial-grant triggers and `CharacterTurnEnd` repair triggers.

### `data/feral_descr_ai_personality.txt`

The shared diplomatic personality is explicitly configured as:

```text
dogmatism 100
openness 100
flexibility 100
aggresiveness 100
nationalism 0
stability 0
independence 100
```

Every military-priority profile also uses:

```text
sally_agression 100
sally_desperate 100
attack_risk_taker 4
subterfuge_risk_taker 7
```

Army-composition weights and the existing personality mappings are otherwise retained.

The goal is aggressive expansion, risky invasions, opportunistic treaty breaking, and frequent agent activity without making every faction recruit the same army composition.

### `data/feral_descr_reputations_and_relations.txt`

Positive reputation recovery has been removed:

- time: `0`;
- long-term alliances: `0`;
- cancelling embargoes: `0`;
- accepted compensation: `0`;
- accepted ceasefires: `0`;
- accepted military assistance: `0`;
- ally-of-an-ally bonus: `0`.

Rebel and non-rebel bribery use negative reputation values, merchant trade gives no positive relationship growth, and embargoes are strongly punitive:

```text
income_multiplier 0
cap 100
embargo_penalty 100
```

This complements the unstable AI personality so diplomacy does not naturally settle into permanent friendly blocs.

### `data/text/export_buildings.txt`

Contains the expanded building localization needed by the broader building/temple availability setup.

### `docs/source-paths.md`

Maps each mod file back to its original ROME REMASTERED game-data path.

### `docs/descr-strat-character-packages.md`

Contains copy/paste-ready `traits ...` and `ancillaries ...` lines for every supported character role and culture.

## Project Structure

```text
OP-All/
├── launcher/
│   └── OP-All/
│       ├── modinfo.json
│       ├── filelist.json              # generate/refresh before testing
│       └── data/
│           ├── descr_cultures.txt
│           ├── export_descr_ancillaries.txt
│           ├── export_descr_buildings.txt
│           ├── export_descr_character_traits.txt
│           ├── export_descr_unit.txt
│           ├── feral_descr_ai_personality.txt
│           ├── feral_descr_reputations_and_relations.txt
│           ├── text/
│           │   └── export_buildings.txt
│           └── world/
│               └── maps/
│                   └── campaign/
│                       └── imperial_campaign/
│                           └── descr_strat.txt
├── docs/
│   ├── descr-strat-character-packages.md
│   └── source-paths.md
└── README.md
```

## First Launcher Test

For the next test build, use the current Julii-focused `descr_strat.txt` rather than changing every faction first.

1. Copy `launcher\OP-All` to:

   ```text
   %LOCALAPPDATA%\Feral Interactive\Total War ROME REMASTERED\Mods\My Mods\OP-All\
   ```

2. Make sure any base-game files previously edited directly are restored to vanilla. Otherwise it becomes impossible to tell whether the launcher mod is supplying a change.

3. Open the ROME REMASTERED launcher/mod manager and generate or refresh `filelist.json` **after** the final test files have been copied. Commit that generated manifest with the same data-file revision.

4. Enable the mod and add these Advanced launch options while testing:

   ```text
   enable_logging enable_dialogs
   ```

5. Start a **new Imperial Campaign**. Existing saves cannot test `descr_strat.txt` starting-position changes correctly.

6. Test one system at a time:
   - campaign loads to the faction-selection screen;
   - Julii campaign starts successfully;
   - buildings/recruitment appear and cost the intended values;
   - units recruit at the intended speed/cost/experience;
   - starting Julii traits and ancillaries display correctly;
   - a newly created agent receives the correct OP package;
   - a coming-of-age family member receives the correct OP package;
   - turn-end repair restores a deliberately removed trait/ancillary;
   - AI factions actually expand and betray agreements as intended.

7. If the campaign fails to load, inspect:

   ```text
   /VFS/Local/Rome/logs/message_log.txt
   ```

   Fix the first relevant parser/data error before making another large `descr_strat.txt` change.

## What Is Still Missing / Not Yet Finished

Before calling the mod feature-complete, the following work remains:

- generate and commit a current `launcher/OP-All/filelist.json`;
- continue converting the remaining factions in `descr_strat.txt`, one small faction/batch at a time;
- replace starting-character trait/ancillary lines with the packages from `docs/descr-strat-character-packages.md` as each faction is converted;
- verify faction starting settlements, buildings, armies, fleets, and agent positions for every faction;
- test the all-faction ancillary triggers in-game, especially the first turn and newly created agents;
- test the trait upper-bound logic in-game over multiple turns;
- test AI campaign behavior for several dozen turns to confirm the chaos settings produce expansion rather than pathological passivity or self-destruction;
- decide whether the playable `slave` faction should have a fully functioning family tree/reproduction system. ROME REMASTERED 2.0.4 supports this, but it must be enabled through faction data; the current package does not yet include a modified `descr_sm_factions.txt`;
- regenerate `filelist.json` every time a file under `launcher/OP-All/data/` is added, removed, or changed.

## Incremental `descr_strat.txt` Workflow

The safest workflow is the one already being used:

1. keep the current global data files stable;
2. modify one faction or one small `descr_strat.txt` section;
3. regenerate the manifest if needed;
4. start a new campaign with logging enabled;
5. confirm campaign creation and the edited faction;
6. commit the working step;
7. move to the next faction.

This makes it much easier to identify the exact edit that causes a campaign-load failure.

## Installation

The mod is intended to be loaded through the ROME REMASTERED Feral launcher rather than by overwriting the base game.

Copy:

```text
launcher\OP-All
```

into:

```text
%LOCALAPPDATA%\Feral Interactive\Total War ROME REMASTERED\Mods\My Mods\
```

The installed result should be:

```text
%LOCALAPPDATA%\Feral Interactive\Total War ROME REMASTERED\Mods\My Mods\OP-All\
```

Then refresh/generate the launcher manifest, enable **OP All**, and start a new Imperial Campaign.

## Modding References

- Feral Interactive ROME REMASTERED modding documentation:
  <https://github.com/FeralInteractive/romeremastered>
- Feral logging/debugging documentation:
  <https://github.com/FeralInteractive/romeremastered/blob/main/documentation/feature_guides/logging/logging.md>

## Repository Location

This project is intended to live under:

```text
Game-Mods/
└── Total-War-ROME-REMASTERED/
    └── OP-All/
```

Only modified mod files, launcher metadata, generated manifests, and project documentation should be committed. Vanilla backups should remain outside the repository.

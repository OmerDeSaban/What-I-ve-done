# Notable Support Commands

A C# utility mod for **Mount & Blade II: Bannerlord** that controls notable support cost and manages notable relationships without preventing notables from being recruited as supporters of the player's clan.

**Current version:** v1.0.0

Game versions tested on:

- **Bannerlord:** v1.4.5
- **War Sails:** v1.2.5

## Why This Mod Exists

Bannerlord v1.4.5 uses two mechanics that matter here.

### Support cost

The vanilla `DefaultNotablePowerModel` calculates the one-time cost of gaining a notable supporter as:

```text
20,000 + (10,000 × current number of player-clan supporter notables)
```

This means the price continually rises as more notables support the player.

Notable Support Commands can replace that increasing cost with a fixed configurable value.

The default command value is:

```text
10,000
```

### Support eligibility

In Bannerlord v1.4.5, a player needs at least `50` relation with a notable to request their support.

If the notable already supports another clan, the game also compares the notable's relation with that clan's leader.

A notable is hard-blocked from switching when their relation with the current supported clan leader reaches the campaign's maximum relation limit, which is `100` in the tested game version.

When that relation is below the maximum, a player at `100` relation can still request the notable's support.

Because of this, the default non-player relation **cap** is calculated dynamically as:

```text
floor(MaxRelationLimit / 5) × 5 - 5
```

With Bannerlord's normal maximum of `100`, the default is therefore:

```text
95
```

This creates a five-point safety margin below the hard-block value.

## Relationship Policy

The configured non-player value is a **ceiling / maximum**, not a target.

For integer Bannerlord relations, `95` is an attainable maximum rather than merely a mathematical supremum.

### Notable ↔ Player Clan

Relations between living notables and living members of `Clan.PlayerClan` are kept exactly at the campaign's maximum relation value.

In the tested v1.4.5 support-request logic, relations with non-player members of the player's clan do not interfere with a notable choosing to support the player's clan.

For the normal maximum:

```text
Notable <-> player-clan hero = 100
```

### Notable ↔ Notable

Relations between two living notables are also kept exactly at the maximum relation value.

```text
Notable <-> notable = 100
```

### Notable ↔ Other Non-Player Hero

For every other living hero outside the player's clan who is not a notable, the configured value is used only as an upper bound.

With the default cap of `95`:

```text
Current relation 20  -> remains 20
Current relation 80  -> remains 80
Current relation 95  -> remains 95
Current relation 96  -> lowered to 95
Current relation 100 -> lowered to 95
```

The mod therefore never raises one of these relations merely because the cap is `95`.

This category includes non-player nobles as well as other non-notable heroes outside `Clan.PlayerClan`.

The mod rejects a cap of `100`, because maximum relation with the leader of a clan currently supported by a notable can hard-block that notable from switching support to the player's clan.

## Automatic Non-Player Support

Bannerlord has a separate automatic notable-support system.

An unsupported notable can automatically begin supporting a non-player clan when their relation with that clan's leader is greater than `50`.

The player clan is explicitly excluded from that automatic assignment; player support is acquired through the support interaction.

Because the configurable value in this mod is only a ceiling, the mod does **not** raise low relations toward `95`. It therefore does not itself make unsupported notables more likely to support other clans automatically.

If a notable's relation with a non-player clan leader naturally rises above the configured cap, the mod lowers it back to the cap.

Once a notable supports the player's clan and their relation with the player is kept at `100`, the normal deterioration logic does not cause them to leave.

## Recruitment Relation Thresholds

Bannerlord v1.4.5 uses relation bands when deciding how many volunteer slots a hero can access from a notable.

The relation contribution rises at:

```text
0
5
10
20
40
60
80
100
```

For a non-player noble, normal recruitment also receives a base slot and an AI-hero bonus.

As a result:

- `20` relation can be enough for all six slots when the noble and notable are in the same map faction.
- `40` relation is enough for all six slots under ordinary peaceful conditions even without the same-faction bonus.
- Hostile/war conditions apply additional penalties.

The fallback recruitment threshold is not needed as the default in this game version because a clear support-switch hard-block value exists at the maximum relation limit.

## Commands

### Find Notable

```text
notable.find <name-or-id>
```

Searches living notables by partial display name or partial internal `StringId`.

Each result includes:

- notable name and ID
- current settlement
- currently supported clan, when applicable
- current player relation

### Notable Info

```text
notable.info <hero_id>
```

Displays detailed support information for one notable, including:

- notable ID
- current settlement
- player relation
- current supported clan and ID
- supported clan leader and ID
- relation with the supported clan leader
- current support cost
- current support-request eligibility reason

### Clean Stale Support Assignments

```text
notable.cleanup_stale_support
```

Immediately scans living notables and clears support assignments that still point to an **eliminated clan**.

This repairs a Bannerlord edge case where a notable can remain recorded as supporting a clan after that clan has been destroyed. If the notable had `100` relation with that clan's leader, Bannerlord's normal support-request check can otherwise continue to hard-block the notable from switching to the player's clan.

The cleanup changes only the notable's `SupporterOf` assignment. It does **not** change the notable's stored relation with the former clan leader.

When Bannerlord raises `OnClanDestroyedEvent`, the mod clears supporters of that destroyed clan immediately. A lightweight hourly scan remains as a safety fallback for stale assignments that were created or preserved by another mod or by an unusual campaign state. Both mechanisms are independent of whether relation locking is enabled.

### Inspect Relation

```text
notable.relation <notable_id> <hero_id>
```

Inspects the current relation between one living notable and any other living hero by exact internal IDs.

The command displays:

- notable name and ID
- other hero name and ID
- raw/direct relation
- effective relation used by Bannerlord
- the effective hero pair used by the diplomacy model
- whether notable relation locking is currently enabled
- the policy rule that applies to this pair

For example:

```text
notable.relation CharacterObject_2091 main_hero
```

If the other hero is a member of `Clan.PlayerClan`, the applicable rule is an exact maximum relation target.

If the other hero is another notable, the applicable rule is also an exact maximum relation target.

For ordinary non-player, non-notable heroes outside `Clan.PlayerClan`, the command reports the configured ceiling. Values below that ceiling remain untouched.

### Apply Relation Policy Once

```text
notable.apply_relations
notable.apply_relations <nonplayer_relation_cap>
```

Without a value, the command uses the dynamic safe default.

With the normal relation maximum of `100`, that cap is `95`.

This command:

- sets notable-to-notable relations to the maximum
- sets notable-to-player-clan relations to the maximum
- lowers other non-player relations only when they exceed the configured cap
- leaves other non-player relations below the cap unchanged

It applies the policy once but does not keep it locked.

### Lock Relation Policy

```text
notable.lock_relations
notable.lock_relations <nonplayer_relation_cap>
```

Enables persistent relation enforcement.

The policy is applied immediately and reacts to normal relation-change events. Changes that bypass Bannerlord's normal relation event are handled by a staggered hourly fallback sweep.

The cap remains a ceiling: ordinary non-player relations below it are never raised by the cap rule.

To avoid a large recurring freeze in very large campaigns, the fallback does **not** rescan every notable in one daily pass. Instead, it dynamically processes enough living notables each in-game hour to complete one full fallback sweep every `120` in-game hours. Normal relation-change events are still enforced immediately.

The configured cap is persisted with the campaign save.

### Unlock Relation Policy

```text
notable.unlock_relations
```

Disables future relation enforcement.

Existing relation values are left unchanged.

### Lock Support Cost

```text
notable.lock_support_cost
notable.lock_support_cost <cost>
```

Without a value, the fixed cost defaults to:

```text
10000
```

A custom non-negative cost may also be supplied.

The cost lock is persisted with the campaign save.

### Unlock Support Cost

```text
notable.unlock_support_cost
```

Returns support-cost calculation to the previous/base `NotablePowerModel`.

### Status

```text
notable.status
```

Displays:

- whether relation enforcement is active
- configured non-player relation cap
- notable-to-notable exact target
- notable-to-player-clan exact target
- dynamic default safe cap
- hard support-switch block value
- minimum player relation required for support
- support-cost lock state and value

## Verified Behavior

The mod has been tested successfully for:

- discovery of living notables by partial name with internal IDs
- detailed notable inspection by exact hero ID
- fixed support-cost locking at the default `10000`
- custom fixed support-cost values
- one-time relation-policy application
- persistent relation locking with a custom ceiling such as `60`
- restoration of notable-to-player relation to `100` after an external relation command lowered it
- switching the persistent non-player ceiling back to the default `95`
- save/load persistence of the relation lock and support-cost lock
- disabling relation enforcement without resetting existing relations
- disabling the support-cost override and returning control to the base notable-power model

The relation ceiling was also verified to behave as a ceiling rather than a target: the mod lowers managed ordinary non-player relations that exceed the configured value but does not raise relations that are already below it.

## Eliminated-Clan Support Cleanup

Bannerlord can leave a living notable's `SupporterOf` reference pointing to a clan after that clan has been eliminated. The normal support-request condition still evaluates that stale clan and its leader, so a stored relation of `100` can continue to trigger the hard support-switch block.

Notable Support Commands treats support for an eliminated clan as stale state and clears it automatically. Clan destruction is handled immediately through Bannerlord's `OnClanDestroyedEvent`, with an hourly full-notable safety scan as fallback. The cleanup is intentionally independent of the relation lock and support-cost lock because an eliminated clan should no longer retain active supporters.

The fallback scan is inexpensive: it checks each living notable once per in-game hour and only mutates heroes whose supported clan is already eliminated. The normal path after a clan is destroyed uses a targeted cleanup for that clan immediately.

## Performance Design

A full relation-policy pass can be expensive in unusually large or heavily modded campaigns because every living notable may need to be checked against many living heroes.

The explicit commands:

```text
notable.apply_relations
notable.lock_relations
```

still perform an immediate full policy application so the requested state takes effect right away.

Persistent fallback enforcement is intentionally staggered across five in-game days.

The mod dynamically calculates:

```text
ceil(living_notables / 120)
```

and processes that many notables each in-game hour.

Normal `HeroRelationChanged` events are still corrected immediately. The staggered sweep exists only as a safety net for relation changes made by systems or mods that bypass the normal event. Separately, clan destruction triggers an immediate targeted cleanup of stale `SupporterOf` assignments, backed by a lightweight hourly safety scan.

In a campaign with roughly `1,200` living notables, the fallback processes about `10` notables per in-game hour and completes one full safety sweep every `120` in-game hours, or roughly five in-game days. This spreads the work across the day instead of doing the entire multi-million-pair scan at once.

The full explicit pass is also processed internally in smaller batches to reduce temporary allocation pressure.

## Compatibility Design

The support-cost model derives from `NotablePowerModel` and delegates all unrelated calculations to its `BaseModel`.

Only `GetInitialNotableSupporterCost` is replaced while the cost lock is enabled.

This is intended to preserve the previous model in Bannerlord's model chain rather than replacing unrelated notable-power behavior.

## Relation Enforcement

Bannerlord's `Hero.GetRelation` uses the campaign diplomacy model's effective relation rather than only the raw direct hero-to-hero relation.

The mod therefore evaluates the effective relation used by Bannerlord.

For exact rules, it raises or lowers the effective relation until it matches the configured value.

For the non-player cap rule, it changes the relation only when the effective value exceeds the cap.

This is especially important when one side of a managed relationship is a noble whose effective relation may be represented through a clan leader.

## Project Structure

```text
NotableSupportCommands/
├── Module/
│   └── SubModule.xml
├── src/
│   ├── NotableSupportBehavior.cs
│   ├── NotableSupportCommands.cs
│   ├── NotableSupportPowerModel.cs
│   └── SubModule.cs
├── NotableSupportCommands.csproj
└── README.md
```

## Requirements

- Mount & Blade II: Bannerlord
- .NET SDK capable of building .NET Framework 4.7.2 projects
- Bannerlord developer console

The project references TaleWorlds assemblies directly from the local Bannerlord installation.

## Development Setup

The project expects:

```text
BANNERLORD_GAME_DIR
```

to point to the Bannerlord installation directory.

Example:

```powershell
[Environment]::SetEnvironmentVariable(
    "BANNERLORD_GAME_DIR",
    "C:\Program Files (x86)\Steam\steamapps\common\Mount & Blade II Bannerlord",
    "User"
)
```

## Building

From the `NotableSupportCommands` directory:

```powershell
dotnet build
```

The project automatically deploys into:

```text
Mount & Blade II Bannerlord/
└── Modules/
    └── NotableSupportCommands/
        ├── SubModule.xml
        └── bin/
            └── Win64_Shipping_Client/
                └── NotableSupportCommands.dll
```

Bannerlord and the Bannerlord launcher should be closed while building because they can lock the deployed DLL.

## Repository

This project is intended to live under:

```text
Game-Mods/
└── Mount-and-Blade-II-Bannerlord/
    └── NotableSupportCommands/
```

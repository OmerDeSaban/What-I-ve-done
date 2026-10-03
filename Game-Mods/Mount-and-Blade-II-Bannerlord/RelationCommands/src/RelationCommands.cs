using System;
using System.Collections.Generic;
using TaleWorlds.CampaignSystem;
using TaleWorlds.Library;

namespace RelationCommands
{
    internal enum RelationGroup
    {
        Everyone,
        Notables,
        Wanderers,
        Clans
    }

    public static class RelationConsoleCommands
    {
        // ------------------------------------------------------------
        // One-time relation commands
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_everyone",
            "relation")]
        public static string SetEveryone(List<string> args)
        {
            return ExecuteSetCommand(
                args,
                "relation.set_everyone",
                RelationGroup.Everyone);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_notables",
            "relation")]
        public static string SetNotables(List<string> args)
        {
            return ExecuteSetCommand(
                args,
                "relation.set_notables",
                RelationGroup.Notables);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_wanderers",
            "relation")]
        public static string SetWanderers(List<string> args)
        {
            return ExecuteSetCommand(
                args,
                "relation.set_wanderers",
                RelationGroup.Wanderers);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_clans",
            "relation")]
        public static string SetClans(List<string> args)
        {
            return ExecuteSetCommand(
                args,
                "relation.set_clans",
                RelationGroup.Clans);
        }

        // ------------------------------------------------------------
        // Relation lock commands
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_everyone",
            "relation")]
        public static string LockEveryone(List<string> args)
        {
            if (!TryPrepareValueCommand(
                    args,
                    "relation.lock_everyone",
                    out int value,
                    out string error))
            {
                return error;
            }

            RelationLockBehavior behavior =
                RelationLockBehavior.Current;

            if (behavior == null)
            {
                return "Relation lock behavior is not available.";
            }

            return behavior.LockEveryone(value);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_notables",
            "relation")]
        public static string LockNotables(List<string> args)
        {
            return ExecuteLockCommand(
                args,
                "relation.lock_notables",
                RelationGroup.Notables);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_wanderers",
            "relation")]
        public static string LockWanderers(List<string> args)
        {
            return ExecuteLockCommand(
                args,
                "relation.lock_wanderers",
                RelationGroup.Wanderers);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_clans",
            "relation")]
        public static string LockClans(List<string> args)
        {
            return ExecuteLockCommand(
                args,
                "relation.lock_clans",
                RelationGroup.Clans);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_everyone",
            "relation")]
        public static string UnlockEveryone(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "relation.unlock_everyone",
                    out string error))
            {
                return error;
            }

            RelationLockBehavior behavior =
                RelationLockBehavior.Current;

            if (behavior == null)
            {
                return "Relation lock behavior is not available.";
            }

            return behavior.UnlockEveryone();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_notables",
            "relation")]
        public static string UnlockNotables(List<string> args)
        {
            return ExecuteUnlockCommand(
                args,
                "relation.unlock_notables",
                RelationGroup.Notables);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_wanderers",
            "relation")]
        public static string UnlockWanderers(List<string> args)
        {
            return ExecuteUnlockCommand(
                args,
                "relation.unlock_wanderers",
                RelationGroup.Wanderers);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_clans",
            "relation")]
        public static string UnlockClans(List<string> args)
        {
            return ExecuteUnlockCommand(
                args,
                "relation.unlock_clans",
                RelationGroup.Clans);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_status",
            "relation")]
        public static string LockStatus(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "relation.lock_status",
                    out string error))
            {
                return error;
            }

            RelationLockBehavior behavior =
                RelationLockBehavior.Current;

            if (behavior == null)
            {
                return "Relation lock behavior is not available.";
            }

            return behavior.GetStatus();
        }

        // ------------------------------------------------------------
        // Shared command handling
        // ------------------------------------------------------------

        private static string ExecuteSetCommand(
            List<string> args,
            string commandName,
            RelationGroup group)
        {
            if (!TryPrepareValueCommand(
                    args,
                    commandName,
                    out int value,
                    out string error))
            {
                return error;
            }

            return ApplyGroupRelation(
                group,
                value);
        }

        private static string ExecuteLockCommand(
            List<string> args,
            string commandName,
            RelationGroup group)
        {
            if (!TryPrepareValueCommand(
                    args,
                    commandName,
                    out int value,
                    out string error))
            {
                return error;
            }

            RelationLockBehavior behavior =
                RelationLockBehavior.Current;

            if (behavior == null)
            {
                return "Relation lock behavior is not available.";
            }

            return behavior.LockGroup(
                group,
                value);
        }

        private static string ExecuteUnlockCommand(
            List<string> args,
            string commandName,
            RelationGroup group)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    commandName,
                    out string error))
            {
                return error;
            }

            RelationLockBehavior behavior =
                RelationLockBehavior.Current;

            if (behavior == null)
            {
                return "Relation lock behavior is not available.";
            }

            return behavior.UnlockGroup(group);
        }

        internal static string ApplyGroupRelation(
            RelationGroup group,
            int value)
        {
            switch (group)
            {
                case RelationGroup.Everyone:
                    return SetRelations(
                        GetAllLivingHeroes(),
                        value,
                        "living heroes");

                case RelationGroup.Notables:
                    return SetRelations(
                        GetLivingNotables(),
                        value,
                        "notables");

                case RelationGroup.Wanderers:
                    return SetRelations(
                        GetLivingWanderers(),
                        value,
                        "wanderers");

                case RelationGroup.Clans:
                    return SetRelations(
                        GetClanLeaders(),
                        value,
                        "clan leaders");

                default:
                    return "Unknown relation group.";
            }
        }

        internal static void EnforceSingleHero(
            Hero hero,
            int value)
        {
            SetRelations(
                new[] { hero },
                value,
                "hero");
        }

        internal static bool HeroMatchesGroup(
            Hero hero,
            RelationGroup group)
        {
            if (!IsValidLivingTarget(hero))
            {
                return false;
            }

            switch (group)
            {
                case RelationGroup.Everyone:
                    return true;

                case RelationGroup.Notables:
                    return hero.IsNotable;

                case RelationGroup.Wanderers:
                    return hero.IsWanderer;

                case RelationGroup.Clans:
                    return IsActiveClanLeader(hero);

                default:
                    return false;
            }
        }

        // ------------------------------------------------------------
        // Proven effective-relation setter
        // ------------------------------------------------------------

        private static string SetRelations(
            IEnumerable<Hero> targets,
            int targetValue,
            string targetDescription)
        {
            Hero player = Hero.MainHero;

            var diplomacyModel =
                Campaign.Current.Models.DiplomacyModel;

            List<Hero> targetList =
                new List<Hero>();

            HashSet<Hero> uniqueHeroes =
                new HashSet<Hero>();

            foreach (Hero hero in targets)
            {
                if (hero == null ||
                    hero == player ||
                    !uniqueHeroes.Add(hero))
                {
                    continue;
                }

                targetList.Add(hero);
            }

            /*
             * PASS 1:
             * Set every hero's direct personal relation.
             */
            foreach (Hero hero in targetList)
            {
                player.SetPersonalRelation(
                    hero,
                    targetValue);
            }

            /*
             * PASS 2:
             * Correct each unique effective relation pair once.
             */
            HashSet<string> correctedPairs =
                new HashSet<string>();

            foreach (Hero hero in targetList)
            {
                diplomacyModel.GetHeroesForEffectiveRelation(
                    player,
                    hero,
                    out Hero effectivePlayer,
                    out Hero effectiveHero);

                if (effectivePlayer == null ||
                    effectiveHero == null)
                {
                    continue;
                }

                string pairKey =
                    GetEffectivePairKey(
                        effectivePlayer,
                        effectiveHero);

                if (!correctedPairs.Add(pairKey))
                {
                    continue;
                }

                int currentEffective =
                    diplomacyModel.GetEffectiveRelation(
                        player,
                        hero);

                if (currentEffective == targetValue)
                {
                    continue;
                }

                int currentRawPairRelation =
                    CharacterRelationManager.GetHeroRelation(
                        effectivePlayer,
                        effectiveHero);

                int difference =
                    targetValue - currentEffective;

                int correctedRawPairRelation =
                    currentRawPairRelation + difference;

                CharacterRelationManager.SetHeroRelation(
                    effectivePlayer,
                    effectiveHero,
                    correctedRawPairRelation);
            }

            /*
             * PASS 3:
             * Verify the final effective relation.
             */
            int exactCount = 0;

            foreach (Hero hero in targetList)
            {
                int displayedRelation =
                    diplomacyModel.GetEffectiveRelation(
                        player,
                        hero);

                if (displayedRelation == targetValue)
                {
                    exactCount++;
                }
            }

            return
                $"Processed {targetList.Count} {targetDescription}. " +
                $"{exactCount} now have relation {targetValue}.";
        }

        // ------------------------------------------------------------
        // Target discovery
        // ------------------------------------------------------------

        private static IEnumerable<Hero> GetLivingNotables()
        {
            List<Hero> targets =
                new List<Hero>();

            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (hero.IsNotable)
                {
                    targets.Add(hero);
                }
            }

            return targets;
        }

        private static IEnumerable<Hero> GetLivingWanderers()
        {
            List<Hero> targets =
                new List<Hero>();

            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (hero.IsWanderer)
                {
                    targets.Add(hero);
                }
            }

            return targets;
        }

        private static IEnumerable<Hero> GetClanLeaders()
        {
            List<Hero> targets =
                new List<Hero>();

            foreach (Clan clan in Clan.All)
            {
                if (!IsValidClan(clan))
                {
                    continue;
                }

                if (IsValidLivingTarget(clan.Leader))
                {
                    targets.Add(clan.Leader);
                }
            }

            return targets;
        }

        private static IEnumerable<Hero> GetAllLivingHeroes()
        {
            HashSet<Hero> heroes =
                new HashSet<Hero>();

            foreach (Hero hero in Hero.AllAliveHeroes)
            {
                if (IsValidLivingTarget(hero))
                {
                    heroes.Add(hero);
                }
            }

            foreach (Hero hero in Hero.DeadOrDisabledHeroes)
            {
                if (IsValidLivingTarget(hero))
                {
                    heroes.Add(hero);
                }
            }

            return heroes;
        }

        private static bool IsValidLivingTarget(
            Hero hero)
        {
            return
                hero != null &&
                hero != Hero.MainHero &&
                hero.IsAlive;
        }

        private static bool IsActiveClanLeader(
            Hero hero)
        {
            return
                hero != null &&
                hero.Clan != null &&
                IsValidClan(hero.Clan) &&
                hero.Clan.Leader == hero;
        }

        private static bool IsValidClan(
            Clan clan)
        {
            return
                clan != null &&
                clan != Clan.PlayerClan &&
                !clan.IsEliminated &&
                clan.Leader != null;
        }

        // ------------------------------------------------------------
        // Validation
        // ------------------------------------------------------------

        private static bool TryPrepareValueCommand(
            List<string> args,
            string commandName,
            out int value,
            out string error)
        {
            value = 0;
            error = string.Empty;

            if (Campaign.Current == null ||
                Hero.MainHero == null)
            {
                error = "A campaign must be loaded.";
                return false;
            }

            if (args.Count != 1 ||
                !int.TryParse(args[0], out value))
            {
                error =
                    $"Usage: {commandName} <value>";
                return false;
            }

            if (value < -100 || value > 100)
            {
                error =
                    "Relation must be between -100 and 100.";
                return false;
            }

            return true;
        }

        private static bool TryPrepareNoValueCommand(
            List<string> args,
            string commandName,
            out string error)
        {
            error = string.Empty;

            if (Campaign.Current == null ||
                Hero.MainHero == null)
            {
                error = "A campaign must be loaded.";
                return false;
            }

            if (args.Count != 0)
            {
                error =
                    $"Usage: {commandName}";
                return false;
            }

            return true;
        }

        private static string GetEffectivePairKey(
            Hero firstHero,
            Hero secondHero)
        {
            string firstId = firstHero.StringId;
            string secondId = secondHero.StringId;

            if (string.CompareOrdinal(
                    firstId,
                    secondId) <= 0)
            {
                return firstId + "|" + secondId;
            }

            return secondId + "|" + firstId;
        }
    }
}

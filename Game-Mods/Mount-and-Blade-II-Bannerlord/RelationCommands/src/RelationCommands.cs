using System;
using System.Collections.Generic;
using TaleWorlds.CampaignSystem;
using TaleWorlds.Library;

namespace RelationCommands
{
    public static class RelationConsoleCommands
    {
        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_everyone",
            "relation")]
        public static string SetEveryone(List<string> args)
        {
            if (!TryPrepareCommand(
                    args,
                    "relation.set_everyone",
                    out int value,
                    out string error))
            {
                return error;
            }

            return SetRelations(
                GetAllLivingHeroes(),
                value,
                "living heroes");
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_notables",
            "relation")]
        public static string SetNotables(List<string> args)
        {
            if (!TryPrepareCommand(
                    args,
                    "relation.set_notables",
                    out int value,
                    out string error))
            {
                return error;
            }

            List<Hero> targets = new List<Hero>();

            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (hero.IsNotable)
                {
                    targets.Add(hero);
                }
            }

            return SetRelations(
                targets,
                value,
                "notables");
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_wanderers",
            "relation")]
        public static string SetWanderers(List<string> args)
        {
            if (!TryPrepareCommand(
                    args,
                    "relation.set_wanderers",
                    out int value,
                    out string error))
            {
                return error;
            }

            List<Hero> targets = new List<Hero>();

            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (hero.IsWanderer)
                {
                    targets.Add(hero);
                }
            }

            return SetRelations(
                targets,
                value,
                "wanderers");
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_clans",
            "relation")]
        public static string SetClans(List<string> args)
        {
            if (!TryPrepareCommand(
                    args,
                    "relation.set_clans",
                    out int value,
                    out string error))
            {
                return error;
            }

            List<Hero> targets = new List<Hero>();

            foreach (Clan clan in Clan.All)
            {
                if (!IsValidClan(clan))
                {
                    continue;
                }

                if (IsValidTarget(clan.Leader))
                {
                    targets.Add(clan.Leader);
                }
            }

            return SetRelations(
                targets,
                value,
                "clan leaders");
        }

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
             * First set the direct personal relation for every target.
             *
             * This handles heroes whose relation is stored directly,
             * including wanderers, notables, companions, and living
             * storyline heroes that are currently disabled.
             */
            foreach (Hero hero in targetList)
            {
                player.SetPersonalRelation(
                    hero,
                    targetValue);
            }

            /*
             * Some heroes, particularly nobles, use an effective
             * relationship pair rather than their individual raw
             * relationship.
             *
             * Multiple heroes can resolve to the same effective pair,
             * so each pair must be corrected only once.
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
             * Verify the final effective relation that the player
             * actually sees in-game.
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

        private static string GetEffectivePairKey(
            Hero firstHero,
            Hero secondHero)
        {
            string firstId = firstHero.StringId;
            string secondId = secondHero.StringId;

            if (string.CompareOrdinal(firstId, secondId) <= 0)
            {
                return firstId + "|" + secondId;
            }

            return secondId + "|" + firstId;
        }

        private static bool TryPrepareCommand(
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

        private static bool IsValidTarget(Hero hero)
        {
            return
                hero != null &&
                hero != Hero.MainHero;
        }

        private static bool IsValidClan(Clan clan)
        {
            return
                clan != null &&
                clan != Clan.PlayerClan &&
                !clan.IsEliminated &&
                clan.Leader != null;
        }

        private static IEnumerable<Hero> GetAllLivingHeroes()
        {
            HashSet<Hero> heroes =
                new HashSet<Hero>();

            foreach (Hero hero in Hero.AllAliveHeroes)
            {
                if (hero != null &&
                    hero != Hero.MainHero &&
                    hero.IsAlive)
                {
                    heroes.Add(hero);
                }
            }

            /*
             * Some living storyline heroes are moved into
             * DeadOrDisabledHeroes after their campaign role changes.
             * Include those heroes while still excluding anyone
             * who is actually dead.
             */
            foreach (Hero hero in Hero.DeadOrDisabledHeroes)
            {
                if (hero != null &&
                    hero != Hero.MainHero &&
                    hero.IsAlive)
                {
                    heroes.Add(hero);
                }
            }

            return heroes;
        }
    }
}

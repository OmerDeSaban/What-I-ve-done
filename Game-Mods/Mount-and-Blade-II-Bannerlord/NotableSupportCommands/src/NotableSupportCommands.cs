using System;
using System.Collections.Generic;
using System.Text;
using TaleWorlds.CampaignSystem;
using TaleWorlds.Library;

namespace NotableSupportCommands
{
    public static class NotableSupportConsoleCommands
    {
        private const int DefaultSupportCost = 10000;

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "find",
            "notable")]
        public static string FindNotable(List<string> args)
        {
            if (!TryPrepareCampaignCommand(
                    out string error))
            {
                return error;
            }

            string query =
                args == null
                    ? string.Empty
                    : string.Join(" ", args).Trim();

            if (string.IsNullOrWhiteSpace(query))
            {
                return
                    "Usage: notable.find <name-or-id>";
            }

            List<Hero> matches =
                new List<Hero>();

            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (!hero.IsNotable)
                {
                    continue;
                }

                if (ContainsIgnoreCase(
                        hero.Name.ToString(),
                        query) ||
                    ContainsIgnoreCase(
                        hero.StringId,
                        query))
                {
                    matches.Add(hero);
                }
            }

            matches.Sort(
                delegate (Hero first, Hero second)
                {
                    return string.Compare(
                        first.Name.ToString(),
                        second.Name.ToString(),
                        StringComparison.OrdinalIgnoreCase);
                });

            if (matches.Count == 0)
            {
                return
                    $"No living notables found matching \"{query}\".";
            }

            StringBuilder result =
                new StringBuilder();

            result.AppendLine(
                $"Found {matches.Count} notable(s):");

            foreach (Hero hero in matches)
            {
                result.AppendLine(
                    FormatNotableSummary(hero));
            }

            return result
                .ToString()
                .TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "info",
            "notable")]
        public static string NotableInfo(List<string> args)
        {
            if (!TryPrepareCampaignCommand(
                    out string error))
            {
                return error;
            }

            if (args.Count != 1)
            {
                return
                    "Usage: notable.info <hero_id>";
            }

            Hero notable =
                FindLivingNotableById(args[0]);

            if (notable == null)
            {
                return
                    $"No living notable found with ID \"{args[0]}\".";
            }

            return FormatNotableDetails(notable);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "relation",
            "notable")]
        public static string Relation(List<string> args)
        {
            if (!TryPrepareCampaignCommand(
                    out string error))
            {
                return error;
            }

            if (args.Count != 2)
            {
                return
                    "Usage: notable.relation <notable_id> <hero_id>";
            }

            Hero notable =
                FindLivingNotableById(args[0]);

            if (notable == null)
            {
                return
                    $"No living notable found with ID \"{args[0]}\".";
            }

            Hero otherHero =
                FindLivingHeroById(args[1]);

            if (otherHero == null)
            {
                return
                    $"No living hero found with ID \"{args[1]}\".";
            }

            if (otherHero == notable)
            {
                return
                    "A notable cannot be inspected against itself.";
            }

            var diplomacyModel =
                Campaign.Current.Models.DiplomacyModel;

            int rawRelation =
                CharacterRelationManager.GetHeroRelation(
                    notable,
                    otherHero);

            int effectiveRelation =
                diplomacyModel.GetEffectiveRelation(
                    notable,
                    otherHero);

            diplomacyModel.GetHeroesForEffectiveRelation(
                notable,
                otherHero,
                out Hero effectiveFirst,
                out Hero effectiveSecond);

            string effectivePair =
                effectiveFirst == null ||
                effectiveSecond == null
                    ? "Unavailable"
                    : $"{effectiveFirst.Name} " +
                      $"(ID: {effectiveFirst.StringId}) <-> " +
                      $"{effectiveSecond.Name} " +
                      $"(ID: {effectiveSecond.StringId})";

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            int configuredCap =
                behavior != null &&
                behavior.RelationsLocked
                    ? behavior.NonPlayerRelationCap
                    : GetDefaultNonPlayerRelationCap();

            string policyState =
                behavior != null &&
                behavior.RelationsLocked
                    ? "LOCKED"
                    : "OFF";

            string ruleDescription;

            if (IsPlayerClanHero(otherHero))
            {
                ruleDescription =
                    $"Exact target {GetMaxRelation()} " +
                    "(player-clan hero)";
            }
            else if (otherHero.IsNotable)
            {
                ruleDescription =
                    $"Exact target {GetMaxRelation()} " +
                    "(notable-to-notable)";
            }
            else
            {
                ruleDescription =
                    $"Ceiling {configuredCap}; " +
                    "values below the ceiling are left unchanged";
            }

            return
                $"Notable: {notable.Name} " +
                $"(ID: {notable.StringId})\n" +
                $"Other hero: {otherHero.Name} " +
                $"(ID: {otherHero.StringId})\n" +
                $"Raw/direct relation: {rawRelation}\n" +
                $"Effective relation: {effectiveRelation}\n" +
                $"Effective pair: {effectivePair}\n" +
                $"Relation-lock state: {policyState}\n" +
                $"Applicable rule: {ruleDescription}";
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "apply_relations",
            "notable")]
        public static string ApplyRelations(List<string> args)
        {
            if (!TryPrepareNonPlayerCapCommand(
                    args,
                    "notable.apply_relations",
                    out int value,
                    out string error))
            {
                return error;
            }

            RelationPolicyResult result =
                ApplyRelationPolicy(value);

            return
                $"Applied notable relation policy with non-player " +
                $"relation cap {value}. " +
                $"{result.CompliantPairs}/{result.TotalPairs} managed " +
                "relation pair(s) now comply with the policy.";
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_relations",
            "notable")]
        public static string LockRelations(List<string> args)
        {
            if (!TryPrepareNonPlayerCapCommand(
                    args,
                    "notable.lock_relations",
                    out int value,
                    out string error))
            {
                return error;
            }

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior == null)
            {
                return
                    "Notable support behavior is not available.";
            }

            return behavior.LockRelations(value);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_relations",
            "notable")]
        public static string UnlockRelations(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "notable.unlock_relations",
                    out string error))
            {
                return error;
            }

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior == null)
            {
                return
                    "Notable support behavior is not available.";
            }

            return behavior.UnlockRelations();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_support_cost",
            "notable")]
        public static string LockSupportCost(List<string> args)
        {
            if (!TryPrepareSupportCostCommand(
                    args,
                    "notable.lock_support_cost",
                    out int value,
                    out string error))
            {
                return error;
            }

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior == null)
            {
                return
                    "Notable support behavior is not available.";
            }

            return behavior.LockSupportCost(value);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_support_cost",
            "notable")]
        public static string UnlockSupportCost(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "notable.unlock_support_cost",
                    out string error))
            {
                return error;
            }

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior == null)
            {
                return
                    "Notable support behavior is not available.";
            }

            return behavior.UnlockSupportCost();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "status",
            "notable")]
        public static string Status(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "notable.status",
                    out string error))
            {
                return error;
            }

            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior == null)
            {
                return
                    "Notable support behavior is not available.";
            }

            return behavior.GetStatus();
        }

        internal static RelationPolicyResult ApplyRelationPolicy(
            int nonPlayerRelationCap)
        {
            const int fullPassBatchSize = 25;

            List<Hero> livingHeroes =
                GetAllLivingHeroesSnapshot();

            List<Hero> notables =
                GetLivingNotablesSnapshot(
                    livingHeroes);

            int totalPairs = 0;
            int compliantPairs = 0;

            for (int startIndex = 0;
                 startIndex < notables.Count;
                 startIndex += fullPassBatchSize)
            {
                int count =
                    Math.Min(
                        fullPassBatchSize,
                        notables.Count - startIndex);

                List<Hero> batch =
                    notables.GetRange(
                        startIndex,
                        count);

                RelationPolicyResult batchResult =
                    ApplyRelationPolicyForNotables(
                        batch,
                        livingHeroes,
                        nonPlayerRelationCap);

                totalPairs +=
                    batchResult.TotalPairs;

                compliantPairs +=
                    batchResult.CompliantPairs;
            }

            return
                new RelationPolicyResult(
                    totalPairs,
                    compliantPairs);
        }

        internal static RelationPolicyResult ApplyRelationPolicyBatch(
            int nonPlayerRelationCap,
            int startIndex,
            int targetSweepHours,
            out int nextIndex,
            out int processedNotables,
            out int totalNotables)
        {
            List<Hero> livingHeroes =
                GetAllLivingHeroesSnapshot();

            List<Hero> notables =
                GetLivingNotablesSnapshot(
                    livingHeroes);

            totalNotables =
                notables.Count;

            if (totalNotables == 0 ||
                targetSweepHours <= 0)
            {
                nextIndex = 0;
                processedNotables = 0;

                return
                    new RelationPolicyResult(
                        0,
                        0);
            }

            if (startIndex < 0 ||
                startIndex >= totalNotables)
            {
                startIndex = 0;
            }

            int notablesPerHour =
                Math.Max(
                    1,
                    (totalNotables +
                     targetSweepHours - 1) /
                    targetSweepHours);

            processedNotables =
                Math.Min(
                    notablesPerHour,
                    totalNotables);

            List<Hero> batch =
                new List<Hero>(
                    processedNotables);

            for (int offset = 0;
                 offset < processedNotables;
                 offset++)
            {
                int index =
                    (startIndex + offset) %
                    totalNotables;

                batch.Add(
                    notables[index]);
            }

            nextIndex =
                (startIndex + processedNotables) %
                totalNotables;

            return
                ApplyRelationPolicyForNotables(
                    batch,
                    livingHeroes,
                    nonPlayerRelationCap);
        }

        private static RelationPolicyResult
            ApplyRelationPolicyForNotables(
                List<Hero> notables,
                List<Hero> livingHeroes,
                int nonPlayerRelationCap)
        {
            int maxRelation =
                GetMaxRelation();

            Dictionary<string, RelationRequest> requests =
                new Dictionary<string, RelationRequest>();

            foreach (Hero notable in notables)
            {
                foreach (Hero otherHero in livingHeroes)
                {
                    if (otherHero == notable)
                    {
                        continue;
                    }

                    /*
                     * A notable-to-notable pair would otherwise be
                     * visited from both directions. Process it only
                     * when the current notable has the lower ID.
                     */
                    if (otherHero.IsNotable &&
                        string.CompareOrdinal(
                            notable.StringId,
                            otherHero.StringId) > 0)
                    {
                        continue;
                    }

                    if (!TryGetManagedRelationRule(
                            notable,
                            otherHero,
                            nonPlayerRelationCap,
                            maxRelation,
                            out int value,
                            out RelationRuleMode mode,
                            out int priority))
                    {
                        continue;
                    }

                    string pairKey =
                        GetHeroPairKey(
                            notable,
                            otherHero);

                    if (!requests.TryGetValue(
                            pairKey,
                            out RelationRequest existing) ||
                        priority > existing.Priority)
                    {
                        requests[pairKey] =
                            new RelationRequest(
                                notable,
                                otherHero,
                                value,
                                mode,
                                priority);
                    }
                }
            }

            var diplomacyModel =
                Campaign.Current.Models.DiplomacyModel;

            Dictionary<string, EffectiveRelationRequest>
                effectiveRequests =
                    new Dictionary<string, EffectiveRelationRequest>();

            foreach (RelationRequest request
                     in requests.Values)
            {
                diplomacyModel.GetHeroesForEffectiveRelation(
                    request.FirstHero,
                    request.SecondHero,
                    out Hero effectiveFirst,
                    out Hero effectiveSecond);

                if (effectiveFirst == null ||
                    effectiveSecond == null)
                {
                    continue;
                }

                string pairKey =
                    GetHeroPairKey(
                        effectiveFirst,
                        effectiveSecond);

                if (!effectiveRequests.TryGetValue(
                        pairKey,
                        out EffectiveRelationRequest existing) ||
                    request.Priority > existing.Priority)
                {
                    effectiveRequests[pairKey] =
                        new EffectiveRelationRequest(
                            request.FirstHero,
                            request.SecondHero,
                            effectiveFirst,
                            effectiveSecond,
                            request.Value,
                            request.Mode,
                            request.Priority);
                }
            }

            foreach (EffectiveRelationRequest request
                     in effectiveRequests.Values)
            {
                int currentEffective =
                    diplomacyModel.GetEffectiveRelation(
                        request.OriginalFirst,
                        request.OriginalSecond);

                if (!ShouldAdjust(
                        currentEffective,
                        request.Value,
                        request.Mode))
                {
                    continue;
                }

                int currentRaw =
                    CharacterRelationManager.GetHeroRelation(
                        request.EffectiveFirst,
                        request.EffectiveSecond);

                int correctedRaw =
                    currentRaw +
                    (request.Value -
                     currentEffective);

                CharacterRelationManager.SetHeroRelation(
                    request.EffectiveFirst,
                    request.EffectiveSecond,
                    correctedRaw);
            }

            int compliantCount = 0;

            foreach (RelationRequest request
                     in requests.Values)
            {
                int currentEffective =
                    diplomacyModel.GetEffectiveRelation(
                        request.FirstHero,
                        request.SecondHero);

                if (IsCompliant(
                        currentEffective,
                        request.Value,
                        request.Mode))
                {
                    compliantCount++;
                }
            }

            return
                new RelationPolicyResult(
                    requests.Count,
                    compliantCount);
        }

        internal static bool TryGetManagedRelationRule(
            Hero firstHero,
            Hero secondHero,
            int nonPlayerRelationCap,
            int maxRelation,
            out int value,
            out RelationRuleMode mode,
            out int priority)
        {
            value = 0;
            mode = RelationRuleMode.Maximum;
            priority = 0;

            if (firstHero == null ||
                secondHero == null ||
                firstHero == secondHero ||
                !firstHero.IsAlive ||
                !secondHero.IsAlive)
            {
                return false;
            }

            if (!firstHero.IsNotable &&
                !secondHero.IsNotable)
            {
                return false;
            }

            Hero notable =
                firstHero.IsNotable
                    ? firstHero
                    : secondHero;

            Hero otherHero =
                notable == firstHero
                    ? secondHero
                    : firstHero;

            /*
             * Highest priority:
             * notable <-> living player-clan hero is kept exactly
             * at the campaign maximum.
             */
            if (IsPlayerClanHero(otherHero))
            {
                value = maxRelation;
                mode = RelationRuleMode.Exact;
                priority = 3;
                return true;
            }

            /*
             * Second priority:
             * notable <-> notable is kept exactly at the maximum.
             */
            if (otherHero.IsNotable)
            {
                value = maxRelation;
                mode = RelationRuleMode.Exact;
                priority = 2;
                return true;
            }

            /*
             * Everyone else:
             * this is a CEILING, not a target.
             *
             * If the current effective relation is already below
             * the configured value, it is left untouched.
             * If it rises above the configured value, it is lowered
             * back to the cap.
             */
            value = nonPlayerRelationCap;
            mode = RelationRuleMode.Maximum;
            priority = 1;
            return true;
        }

        internal static void EnforceManagedPair(
            Hero firstHero,
            Hero secondHero,
            int nonPlayerRelationCap)
        {
            int maxRelation =
                GetMaxRelation();

            if (!TryGetManagedRelationRule(
                    firstHero,
                    secondHero,
                    nonPlayerRelationCap,
                    maxRelation,
                    out int value,
                    out RelationRuleMode mode,
                    out int priority))
            {
                return;
            }

            var diplomacyModel =
                Campaign.Current.Models.DiplomacyModel;

            diplomacyModel.GetHeroesForEffectiveRelation(
                firstHero,
                secondHero,
                out Hero effectiveFirst,
                out Hero effectiveSecond);

            if (effectiveFirst == null ||
                effectiveSecond == null)
            {
                return;
            }

            int currentEffective =
                diplomacyModel.GetEffectiveRelation(
                    firstHero,
                    secondHero);

            if (!ShouldAdjust(
                    currentEffective,
                    value,
                    mode))
            {
                return;
            }

            int currentRaw =
                CharacterRelationManager.GetHeroRelation(
                    effectiveFirst,
                    effectiveSecond);

            CharacterRelationManager.SetHeroRelation(
                effectiveFirst,
                effectiveSecond,
                currentRaw +
                (value - currentEffective));
        }

        private static bool ShouldAdjust(
            int currentValue,
            int configuredValue,
            RelationRuleMode mode)
        {
            if (mode == RelationRuleMode.Exact)
            {
                return currentValue != configuredValue;
            }

            return currentValue > configuredValue;
        }

        private static bool IsCompliant(
            int currentValue,
            int configuredValue,
            RelationRuleMode mode)
        {
            if (mode == RelationRuleMode.Exact)
            {
                return currentValue == configuredValue;
            }

            return currentValue <= configuredValue;
        }

        internal static int GetDefaultNonPlayerRelationCap()
        {
            int hardBlockValue =
                GetMaxRelation();

            return
                (hardBlockValue / 5) * 5 - 5;
        }

        internal static int GetMaxSafeNonPlayerRelationCap()
        {
            return
                GetMaxRelation() - 1;
        }

        internal static int GetMaxRelation()
        {
            return
                Campaign.Current.Models
                    .DiplomacyModel
                    .MaxRelationLimit;
        }

        internal static int GetMinRelation()
        {
            return
                Campaign.Current.Models
                    .DiplomacyModel
                    .MinRelationLimit;
        }

        private static bool TryPrepareNonPlayerCapCommand(
            List<string> args,
            string commandName,
            out int value,
            out string error)
        {
            value = 0;
            error = string.Empty;

            if (!TryPrepareCampaignCommand(
                    out error))
            {
                return false;
            }

            if (args.Count == 0)
            {
                value =
                    GetDefaultNonPlayerRelationCap();

                return true;
            }

            if (args.Count != 1 ||
                !int.TryParse(
                    args[0],
                    out value))
            {
                error =
                    $"Usage: {commandName} [nonplayer_relation_cap]";
                return false;
            }

            int minimum =
                GetMinRelation();

            int maximum =
                GetMaxSafeNonPlayerRelationCap();

            if (value < minimum ||
                value > maximum)
            {
                error =
                    $"Non-player relation cap must be between " +
                    $"{minimum} and {maximum}. " +
                    $"{GetMaxRelation()} is intentionally rejected " +
                    "because it can hard-block a notable from " +
                    "switching support to the player's clan.";
                return false;
            }

            return true;
        }

        private static bool TryPrepareSupportCostCommand(
            List<string> args,
            string commandName,
            out int value,
            out string error)
        {
            value = 0;
            error = string.Empty;

            if (!TryPrepareCampaignCommand(
                    out error))
            {
                return false;
            }

            if (args.Count == 0)
            {
                value =
                    DefaultSupportCost;

                return true;
            }

            if (args.Count != 1 ||
                !int.TryParse(
                    args[0],
                    out value) ||
                value < 0)
            {
                error =
                    $"Usage: {commandName} [non_negative_cost]";
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

            if (!TryPrepareCampaignCommand(
                    out error))
            {
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

        private static bool TryPrepareCampaignCommand(
            out string error)
        {
            error = string.Empty;

            if (Campaign.Current == null ||
                Hero.MainHero == null ||
                Clan.PlayerClan == null)
            {
                error =
                    "A campaign must be loaded.";
                return false;
            }

            return true;
        }

        private static Hero FindLivingHeroById(
            string heroId)
        {
            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (string.Equals(
                        hero.StringId,
                        heroId,
                        StringComparison.OrdinalIgnoreCase))
                {
                    return hero;
                }
            }

            return null;
        }

        private static Hero FindLivingNotableById(
            string heroId)
        {
            foreach (Hero hero in GetAllLivingHeroes())
            {
                if (hero.IsNotable &&
                    string.Equals(
                        hero.StringId,
                        heroId,
                        StringComparison.OrdinalIgnoreCase))
                {
                    return hero;
                }
            }

            return null;
        }

        private static string FormatNotableSummary(
            Hero notable)
        {
            string supporter =
                notable.SupporterOf == null
                    ? "None"
                    : $"{notable.SupporterOf.Name} " +
                      $"(ID: {notable.SupporterOf.StringId})";

            return
                $"- {notable.Name} " +
                $"| ID: {notable.StringId} " +
                $"| Settlement: " +
                $"{GetSettlementName(notable)} " +
                $"| Supports: {supporter} " +
                $"| Player relation: " +
                $"{notable.GetRelationWithPlayer()}";
        }

        private static string FormatNotableDetails(
            Hero notable)
        {
            float playerRelation =
                notable.GetRelationWithPlayer();

            int maxRelation =
                GetMaxRelation();

            string supporterName =
                "None";

            string supporterId =
                "N/A";

            string supporterLeaderName =
                "N/A";

            string supporterLeaderId =
                "N/A";

            string supporterLeaderRelation =
                "N/A";

            bool canRequestSupport =
                playerRelation >= 50;

            string supportReason =
                canRequestSupport
                    ? "Eligible"
                    : "Player relation is below 50";

            if (notable.SupporterOf != null)
            {
                supporterName =
                    notable.SupporterOf.Name.ToString();

                supporterId =
                    notable.SupporterOf.StringId;

                Hero supporterLeader =
                    notable.SupporterOf.Leader;

                if (supporterLeader != null)
                {
                    int relation =
                        notable.GetRelation(
                            supporterLeader);

                    supporterLeaderName =
                        supporterLeader.Name.ToString();

                    supporterLeaderId =
                        supporterLeader.StringId;

                    supporterLeaderRelation =
                        relation.ToString();

                    if (notable.SupporterOf ==
                        Clan.PlayerClan)
                    {
                        canRequestSupport = false;
                        supportReason =
                            "Already supports the player's clan";
                    }
                    else if (relation ==
                             maxRelation)
                    {
                        canRequestSupport = false;
                        supportReason =
                            $"Hard-blocked by relation " +
                            $"{maxRelation} with the current " +
                            "supported clan leader";
                    }
                    else if (playerRelation <
                             relation)
                    {
                        canRequestSupport = false;
                        supportReason =
                            $"Player relation {playerRelation} is " +
                            $"below current supported clan-leader " +
                            $"relation {relation}";
                    }
                    else if (playerRelation < 50)
                    {
                        canRequestSupport = false;
                        supportReason =
                            "Player relation is below 50";
                    }
                }
            }

            int currentCost =
                Campaign.Current.Models
                    .NotablePowerModel
                    .GetInitialNotableSupporterCost(
                        notable);

            return
                $"Notable: {notable.Name}\n" +
                $"ID: {notable.StringId}\n" +
                $"Settlement: {GetSettlementName(notable)}\n" +
                $"Player relation: {playerRelation}\n" +
                $"Supports clan: {supporterName}\n" +
                $"Supports clan ID: {supporterId}\n" +
                $"Supported clan leader: {supporterLeaderName}\n" +
                $"Supported leader ID: {supporterLeaderId}\n" +
                $"Relation with supported leader: " +
                $"{supporterLeaderRelation}\n" +
                $"Current support cost: {currentCost}\n" +
                $"Support request status: {supportReason}";
        }

        private static string GetSettlementName(
            Hero hero)
        {
            if (hero.CurrentSettlement != null)
            {
                return
                    $"{hero.CurrentSettlement.Name} " +
                    $"(ID: {hero.CurrentSettlement.StringId})";
            }

            return "None";
        }

        private static bool IsPlayerClanHero(
            Hero hero)
        {
            return
                hero != null &&
                hero.Clan == Clan.PlayerClan;
        }

        private static bool IsNonPlayerLord(
            Hero hero)
        {
            return
                hero != null &&
                hero.IsLord &&
                hero.Clan != null &&
                hero.Clan != Clan.PlayerClan;
        }

        private static string GetHeroPairKey(
            Hero firstHero,
            Hero secondHero)
        {
            string firstId =
                firstHero.StringId;

            string secondId =
                secondHero.StringId;

            if (string.CompareOrdinal(
                    firstId,
                    secondId) <= 0)
            {
                return
                    firstId + "|" + secondId;
            }

            return
                secondId + "|" + firstId;
        }

        private static bool ContainsIgnoreCase(
            string source,
            string value)
        {
            if (source == null ||
                value == null)
            {
                return false;
            }

            return
                source.IndexOf(
                    value,
                    StringComparison.OrdinalIgnoreCase) >= 0;
        }

        internal static List<Hero>
            GetAllLivingHeroesSnapshot()
        {
            List<Hero> heroes =
                new List<Hero>(
                    GetAllLivingHeroes());

            heroes.Sort(
                delegate (Hero first, Hero second)
                {
                    return string.CompareOrdinal(
                        first.StringId,
                        second.StringId);
                });

            return heroes;
        }

        internal static List<Hero>
            GetLivingNotablesSnapshot()
        {
            return
                GetLivingNotablesSnapshot(
                    GetAllLivingHeroesSnapshot());
        }

        private static List<Hero>
            GetLivingNotablesSnapshot(
                List<Hero> livingHeroes)
        {
            List<Hero> notables =
                new List<Hero>();

            foreach (Hero hero in livingHeroes)
            {
                if (hero.IsNotable)
                {
                    notables.Add(hero);
                }
            }

            notables.Sort(
                delegate (Hero first, Hero second)
                {
                    return string.CompareOrdinal(
                        first.StringId,
                        second.StringId);
                });

            return notables;
        }

        internal static IEnumerable<Hero>
            GetAllLivingHeroes()
        {
            HashSet<Hero> heroes =
                new HashSet<Hero>();

            foreach (Hero hero in Hero.AllAliveHeroes)
            {
                if (hero != null &&
                    hero.IsAlive)
                {
                    heroes.Add(hero);
                }
            }

            foreach (Hero hero in Hero.DeadOrDisabledHeroes)
            {
                if (hero != null &&
                    hero.IsAlive)
                {
                    heroes.Add(hero);
                }
            }

            return heroes;
        }
    }

    internal enum RelationRuleMode
    {
        Exact,
        Maximum
    }

    internal sealed class RelationRequest
    {
        internal Hero FirstHero { get; }
        internal Hero SecondHero { get; }
        internal int Value { get; }
        internal RelationRuleMode Mode { get; }
        internal int Priority { get; }

        internal RelationRequest(
            Hero firstHero,
            Hero secondHero,
            int value,
            RelationRuleMode mode,
            int priority)
        {
            FirstHero = firstHero;
            SecondHero = secondHero;
            Value = value;
            Mode = mode;
            Priority = priority;
        }
    }

    internal sealed class EffectiveRelationRequest
    {
        internal Hero OriginalFirst { get; }
        internal Hero OriginalSecond { get; }
        internal Hero EffectiveFirst { get; }
        internal Hero EffectiveSecond { get; }
        internal int Value { get; }
        internal RelationRuleMode Mode { get; }
        internal int Priority { get; }

        internal EffectiveRelationRequest(
            Hero originalFirst,
            Hero originalSecond,
            Hero effectiveFirst,
            Hero effectiveSecond,
            int value,
            RelationRuleMode mode,
            int priority)
        {
            OriginalFirst = originalFirst;
            OriginalSecond = originalSecond;
            EffectiveFirst = effectiveFirst;
            EffectiveSecond = effectiveSecond;
            Value = value;
            Mode = mode;
            Priority = priority;
        }
    }

    internal struct RelationPolicyResult
    {
        internal int TotalPairs { get; }
        internal int CompliantPairs { get; }

        internal RelationPolicyResult(
            int totalPairs,
            int compliantPairs)
        {
            TotalPairs = totalPairs;
            CompliantPairs = compliantPairs;
        }
    }
}

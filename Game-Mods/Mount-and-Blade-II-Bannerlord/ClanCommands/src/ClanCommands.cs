using System;
using System.Collections.Generic;
using System.Text;
using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.Actions;
using TaleWorlds.Library;

namespace ClanCommands
{
    public static class ClanConsoleCommands
    {
        [CommandLineFunctionality.CommandLineArgumentFunction(
            "find",
            "clan")]
        public static string FindClan(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            string query = JoinArguments(args);

            if (string.IsNullOrWhiteSpace(query))
            {
                return "Usage: clan.find <name-or-id>";
            }

            List<Clan> matches = new List<Clan>();

            foreach (Clan clan in Clan.All)
            {
                if (clan == null)
                {
                    continue;
                }

                if (ContainsIgnoreCase(GetClanName(clan), query) ||
                    ContainsIgnoreCase(clan.StringId, query))
                {
                    matches.Add(clan);
                }
            }

            SortClans(matches);

            if (matches.Count == 0)
            {
                return $"No clans found matching \"{query}\".";
            }

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"Found {matches.Count} clan(s) matching \"{query}\":");

            foreach (Clan clan in matches)
            {
                result.AppendLine(FormatClanSummary(clan));
            }

            return result.ToString().TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "info",
            "clan")]
        public static string ClanInfo(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 1)
            {
                return "Usage: clan.info <clan_id>";
            }

            Clan clan = FindClanById(args[0]);

            if (clan == null)
            {
                return $"No clan found with ID \"{args[0]}\".";
            }

            return FormatClanDetails(clan);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "destroy",
            "clan")]
        public static string DestroyClan(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 1)
            {
                return "Usage: clan.destroy <clan_id>";
            }

            Clan clan = FindClanById(args[0]);

            if (clan == null)
            {
                return $"No clan found with ID \"{args[0]}\".";
            }

            if (clan == Clan.PlayerClan)
            {
                return "Refusing to destroy the player's clan.";
            }

            if (clan.IsEliminated)
            {
                return
                    $"{GetClanName(clan)} (ID: {clan.StringId}) " +
                    "is already eliminated.";
            }

            string clanName = GetClanName(clan);
            string clanId = clan.StringId;

            DestroyClanAction.Apply(clan);

            return $"Destroyed clan {clanName} (ID: {clanId}).";
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "find_kingdom",
            "clan")]
        public static string FindKingdom(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            string query = JoinArguments(args);

            if (string.IsNullOrWhiteSpace(query))
            {
                return "Usage: clan.find_kingdom <name-or-id>";
            }

            List<Kingdom> matches = new List<Kingdom>();

            foreach (Kingdom kingdom in Kingdom.All)
            {
                if (kingdom == null)
                {
                    continue;
                }

                if (ContainsIgnoreCase(GetKingdomName(kingdom), query) ||
                    ContainsIgnoreCase(kingdom.StringId, query))
                {
                    matches.Add(kingdom);
                }
            }

            matches.Sort(
                delegate (Kingdom first, Kingdom second)
                {
                    return string.Compare(
                        GetKingdomName(first),
                        GetKingdomName(second),
                        StringComparison.OrdinalIgnoreCase);
                });

            if (matches.Count == 0)
            {
                return $"No kingdoms found matching \"{query}\".";
            }

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"Found {matches.Count} kingdom(s) matching \"{query}\":");

            foreach (Kingdom kingdom in matches)
            {
                result.AppendLine(FormatKingdomSummary(kingdom));
            }

            return result.ToString().TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "list_kingdom",
            "clan")]
        public static string ListKingdomClans(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 1)
            {
                return "Usage: clan.list_kingdom <kingdom_id>";
            }

            Kingdom kingdom = FindKingdomById(args[0]);

            if (kingdom == null)
            {
                return $"No kingdom found with ID \"{args[0]}\".";
            }

            List<Clan> clans = GetActiveClansInKingdom(kingdom);
            SortClans(clans);

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"{GetKingdomName(kingdom)} " +
                $"(ID: {kingdom.StringId})");

            result.AppendLine($"Active clans: {clans.Count}");

            foreach (Clan clan in clans)
            {
                result.AppendLine(FormatClanSummary(clan));
            }

            return result.ToString().TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "destroy_kingdom_clans",
            "clan")]
        public static string DestroyKingdomClans(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 1)
            {
                return
                    "Usage: clan.destroy_kingdom_clans <kingdom_id>";
            }

            Kingdom kingdom = FindKingdomById(args[0]);

            if (kingdom == null)
            {
                return $"No kingdom found with ID \"{args[0]}\".";
            }

            Kingdom playerKingdom = Clan.PlayerClan.Kingdom;

            if (playerKingdom != null &&
                kingdom == playerKingdom)
            {
                return
                    "Refusing to destroy clans belonging to " +
                    "the player's kingdom.";
            }

            List<Clan> targets = GetActiveClansInKingdom(kingdom);
            SortClans(targets);

            if (targets.Count == 0)
            {
                return
                    $"No active clans found in " +
                    $"{GetKingdomName(kingdom)} " +
                    $"(ID: {kingdom.StringId}).";
            }

            List<string> destroyed = new List<string>();

            /*
             * Work from a copied list. Clan destruction can mutate
             * campaign faction collections while this command runs.
             */
            foreach (Clan clan in targets)
            {
                if (clan == null ||
                    clan == Clan.PlayerClan ||
                    clan.IsEliminated)
                {
                    continue;
                }

                string clanName = GetClanName(clan);
                string clanId = clan.StringId;

                DestroyClanAction.Apply(clan);

                destroyed.Add($"{clanName} (ID: {clanId})");
            }

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"Destroyed {destroyed.Count} clan(s) from " +
                $"{GetKingdomName(kingdom)} " +
                $"(ID: {kingdom.StringId}):");

            foreach (string item in destroyed)
            {
                result.AppendLine(item);
            }

            return result.ToString().TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "list_war",
            "clan")]
        public static string ListWarClans(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 0)
            {
                return "Usage: clan.list_war";
            }

            List<Clan> targets = GetWarClanTargets();
            SortClans(targets);

            if (targets.Count == 0)
            {
                return
                    "No active non-bandit clans outside the player's " +
                    "kingdom are currently at war with the player.";
            }

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"Active clans currently at war with the player: " +
                $"{targets.Count}");

            foreach (Clan clan in targets)
            {
                result.AppendLine(FormatClanSummary(clan));
            }

            return result.ToString().TrimEnd();
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "destroy_war_clans",
            "clan")]
        public static string DestroyWarClans(List<string> args)
        {
            if (!TryPrepareCampaignCommand(out string error))
            {
                return error;
            }

            if (args.Count != 0)
            {
                return "Usage: clan.destroy_war_clans";
            }

            List<Clan> targets = GetWarClanTargets();
            SortClans(targets);

            if (targets.Count == 0)
            {
                return
                    "No active non-bandit clans outside the player's " +
                    "kingdom are currently at war with the player.";
            }

            List<string> destroyed = new List<string>();

            /*
             * Determine the complete target set before destroying
             * anything. Each target is checked again immediately
             * before destruction as an additional safety guard.
             */
            foreach (Clan clan in targets)
            {
                if (!CanDestroyWarTarget(clan))
                {
                    continue;
                }

                string clanName = GetClanName(clan);
                string clanId = clan.StringId;

                DestroyClanAction.Apply(clan);

                destroyed.Add($"{clanName} (ID: {clanId})");
            }

            StringBuilder result = new StringBuilder();

            result.AppendLine(
                $"Destroyed {destroyed.Count} clan(s) that were " +
                "at war with the player:");

            foreach (string item in destroyed)
            {
                result.AppendLine(item);
            }

            return result.ToString().TrimEnd();
        }

        private static List<Clan> GetActiveClansInKingdom(
            Kingdom kingdom)
        {
            List<Clan> result = new List<Clan>();

            foreach (Clan clan in Clan.All)
            {
                if (clan == null ||
                    clan.IsEliminated ||
                    clan.Kingdom != kingdom)
                {
                    continue;
                }

                result.Add(clan);
            }

            return result;
        }

        private static List<Clan> GetWarClanTargets()
        {
            List<Clan> result = new List<Clan>();

            foreach (Clan clan in Clan.All)
            {
                if (CanDestroyWarTarget(clan))
                {
                    result.Add(clan);
                }
            }

            return result;
        }

        private static bool CanDestroyWarTarget(Clan clan)
        {
            if (clan == null ||
                clan == Clan.PlayerClan ||
                clan.IsEliminated ||
                clan.IsBanditFaction)
            {
                return false;
            }

            Clan playerClan = Clan.PlayerClan;

            if (playerClan == null ||
                Hero.MainHero == null)
            {
                return false;
            }

            Kingdom playerKingdom = playerClan.Kingdom;

            /*
             * Hard safety rule:
             * never include any clan belonging to the player's
             * current kingdom.
             */
            if (playerKingdom != null &&
                clan.Kingdom == playerKingdom)
            {
                return false;
            }

            IFaction playerMapFaction = Hero.MainHero.MapFaction;
            IFaction targetMapFaction = clan.MapFaction;

            if (playerMapFaction == null ||
                targetMapFaction == null ||
                playerMapFaction == targetMapFaction)
            {
                return false;
            }

            return FactionManager.IsAtWarAgainstFaction(
                playerMapFaction,
                targetMapFaction);
        }

        private static Clan FindClanById(string clanId)
        {
            foreach (Clan clan in Clan.All)
            {
                if (clan != null &&
                    string.Equals(
                        clan.StringId,
                        clanId,
                        StringComparison.OrdinalIgnoreCase))
                {
                    return clan;
                }
            }

            return null;
        }

        private static Kingdom FindKingdomById(
            string kingdomId)
        {
            foreach (Kingdom kingdom in Kingdom.All)
            {
                if (kingdom != null &&
                    string.Equals(
                        kingdom.StringId,
                        kingdomId,
                        StringComparison.OrdinalIgnoreCase))
                {
                    return kingdom;
                }
            }

            return null;
        }

        private static string FormatClanSummary(Clan clan)
        {
            string kingdomText = clan.Kingdom == null
                ? "Independent"
                : $"{GetKingdomName(clan.Kingdom)} " +
                  $"(ID: {clan.Kingdom.StringId})";

            string leaderText = clan.Leader == null
                ? "None"
                : $"{clan.Leader.Name} " +
                  $"(ID: {clan.Leader.StringId})";

            return
                $"- {GetClanName(clan)} " +
                $"| ID: {clan.StringId} " +
                $"| Kingdom: {kingdomText} " +
                $"| Leader: {leaderText} " +
                $"| Eliminated: {(clan.IsEliminated ? "Yes" : "No")}";
        }

        private static string FormatClanDetails(Clan clan)
        {
            string kingdomName = clan.Kingdom == null
                ? "Independent"
                : GetKingdomName(clan.Kingdom);

            string kingdomId = clan.Kingdom == null
                ? "N/A"
                : clan.Kingdom.StringId;

            string leaderName = clan.Leader == null
                ? "None"
                : clan.Leader.Name.ToString();

            string leaderId = clan.Leader == null
                ? "N/A"
                : clan.Leader.StringId;

            return
                $"Clan: {GetClanName(clan)}\n" +
                $"ID: {clan.StringId}\n" +
                $"Leader: {leaderName}\n" +
                $"Leader ID: {leaderId}\n" +
                $"Kingdom: {kingdomName}\n" +
                $"Kingdom ID: {kingdomId}\n" +
                $"Tier: {clan.Tier}\n" +
                $"Renown: {clan.Renown:0.##}\n" +
                $"Gold: {clan.Gold}\n" +
                $"Eliminated: {(clan.IsEliminated ? "Yes" : "No")}\n" +
                $"Player clan: {(clan == Clan.PlayerClan ? "Yes" : "No")}";
        }

        private static string FormatKingdomSummary(
            Kingdom kingdom)
        {
            string rulingClanText = kingdom.RulingClan == null
                ? "None"
                : $"{GetClanName(kingdom.RulingClan)} " +
                  $"(ID: {kingdom.RulingClan.StringId})";

            return
                $"- {GetKingdomName(kingdom)} " +
                $"| ID: {kingdom.StringId} " +
                $"| Ruling clan: {rulingClanText} " +
                $"| Eliminated: {(kingdom.IsEliminated ? "Yes" : "No")}";
        }

        private static void SortClans(List<Clan> clans)
        {
            clans.Sort(
                delegate (Clan first, Clan second)
                {
                    int nameComparison = string.Compare(
                        GetClanName(first),
                        GetClanName(second),
                        StringComparison.OrdinalIgnoreCase);

                    if (nameComparison != 0)
                    {
                        return nameComparison;
                    }

                    return string.Compare(
                        first.StringId,
                        second.StringId,
                        StringComparison.OrdinalIgnoreCase);
                });
        }

        private static string GetClanName(Clan clan)
        {
            if (clan == null || clan.Name == null)
            {
                return "<unnamed clan>";
            }

            return clan.Name.ToString();
        }

        private static string GetKingdomName(Kingdom kingdom)
        {
            if (kingdom == null || kingdom.Name == null)
            {
                return "<unnamed kingdom>";
            }

            return kingdom.Name.ToString();
        }

        private static string JoinArguments(List<string> args)
        {
            if (args == null || args.Count == 0)
            {
                return string.Empty;
            }

            return string.Join(" ", args).Trim();
        }

        private static bool ContainsIgnoreCase(
            string source,
            string value)
        {
            if (source == null || value == null)
            {
                return false;
            }

            return source.IndexOf(
                value,
                StringComparison.OrdinalIgnoreCase) >= 0;
        }

        private static bool TryPrepareCampaignCommand(
            out string error)
        {
            error = string.Empty;

            if (Campaign.Current == null ||
                Hero.MainHero == null ||
                Clan.PlayerClan == null)
            {
                error = "A campaign must be loaded.";
                return false;
            }

            return true;
        }
    }
}

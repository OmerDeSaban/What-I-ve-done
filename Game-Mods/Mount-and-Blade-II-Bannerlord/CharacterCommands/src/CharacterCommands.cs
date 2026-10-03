using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.CharacterDevelopment;
using TaleWorlds.Core;
using TaleWorlds.Library;

namespace CharacterCommands
{
    internal enum TraitKind
    {
        Calculating = 0,
        Generosity = 1,
        Honor = 2,
        Mercy = 3,
        Valor = 4
    }

    internal enum TraitScope
    {
        Player,
        Clan
    }

    public static class CharacterConsoleCommands
    {
        // ------------------------------------------------------------
        // Player: one-time setters
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_calculating",
            "trait")]
        public static string SetCalculating(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.set_calculating",
                TraitScope.Player,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_generosity",
            "trait")]
        public static string SetGenerosity(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.set_generosity",
                TraitScope.Player,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_honor",
            "trait")]
        public static string SetHonor(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.set_honor",
                TraitScope.Player,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_mercy",
            "trait")]
        public static string SetMercy(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.set_mercy",
                TraitScope.Player,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "set_valor",
            "trait")]
        public static string SetValor(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.set_valor",
                TraitScope.Player,
                TraitKind.Valor);
        }

        // ------------------------------------------------------------
        // Player: individual locks
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_calculating",
            "trait")]
        public static string LockCalculating(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.lock_calculating",
                TraitScope.Player,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_generosity",
            "trait")]
        public static string LockGenerosity(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.lock_generosity",
                TraitScope.Player,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_honor",
            "trait")]
        public static string LockHonor(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.lock_honor",
                TraitScope.Player,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_mercy",
            "trait")]
        public static string LockMercy(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.lock_mercy",
                TraitScope.Player,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_valor",
            "trait")]
        public static string LockValor(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.lock_valor",
                TraitScope.Player,
                TraitKind.Valor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_calculating",
            "trait")]
        public static string UnlockCalculating(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.unlock_calculating",
                TraitScope.Player,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_generosity",
            "trait")]
        public static string UnlockGenerosity(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.unlock_generosity",
                TraitScope.Player,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_honor",
            "trait")]
        public static string UnlockHonor(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.unlock_honor",
                TraitScope.Player,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_mercy",
            "trait")]
        public static string UnlockMercy(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.unlock_mercy",
                TraitScope.Player,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_valor",
            "trait")]
        public static string UnlockValor(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.unlock_valor",
                TraitScope.Player,
                TraitKind.Valor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_all",
            "trait")]
        public static string LockAll(List<string> args)
        {
            return ExecuteLockAll(
                args,
                "trait.lock_all",
                TraitScope.Player);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_all_by_values",
            "trait")]
        public static string LockAllByValues(List<string> args)
        {
            return ExecuteLockAllByValues(
                args,
                "trait.lock_all_by_values",
                TraitScope.Player);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "unlock_all",
            "trait")]
        public static string UnlockAll(List<string> args)
        {
            return ExecuteUnlockAll(
                args,
                "trait.unlock_all",
                TraitScope.Player);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "lock_status",
            "trait")]
        public static string LockStatus(List<string> args)
        {
            return ExecuteStatus(
                args,
                "trait.lock_status",
                TraitScope.Player);
        }

        // ------------------------------------------------------------
        // Clan: one-time setters
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_set_calculating",
            "trait")]
        public static string ClanSetCalculating(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.clan_set_calculating",
                TraitScope.Clan,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_set_generosity",
            "trait")]
        public static string ClanSetGenerosity(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.clan_set_generosity",
                TraitScope.Clan,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_set_honor",
            "trait")]
        public static string ClanSetHonor(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.clan_set_honor",
                TraitScope.Clan,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_set_mercy",
            "trait")]
        public static string ClanSetMercy(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.clan_set_mercy",
                TraitScope.Clan,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_set_valor",
            "trait")]
        public static string ClanSetValor(List<string> args)
        {
            return ExecuteSetTrait(
                args,
                "trait.clan_set_valor",
                TraitScope.Clan,
                TraitKind.Valor);
        }

        // ------------------------------------------------------------
        // Clan: locks
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_calculating",
            "trait")]
        public static string ClanLockCalculating(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.clan_lock_calculating",
                TraitScope.Clan,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_generosity",
            "trait")]
        public static string ClanLockGenerosity(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.clan_lock_generosity",
                TraitScope.Clan,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_honor",
            "trait")]
        public static string ClanLockHonor(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.clan_lock_honor",
                TraitScope.Clan,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_mercy",
            "trait")]
        public static string ClanLockMercy(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.clan_lock_mercy",
                TraitScope.Clan,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_valor",
            "trait")]
        public static string ClanLockValor(List<string> args)
        {
            return ExecuteLockTrait(
                args,
                "trait.clan_lock_valor",
                TraitScope.Clan,
                TraitKind.Valor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_calculating",
            "trait")]
        public static string ClanUnlockCalculating(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.clan_unlock_calculating",
                TraitScope.Clan,
                TraitKind.Calculating);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_generosity",
            "trait")]
        public static string ClanUnlockGenerosity(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.clan_unlock_generosity",
                TraitScope.Clan,
                TraitKind.Generosity);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_honor",
            "trait")]
        public static string ClanUnlockHonor(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.clan_unlock_honor",
                TraitScope.Clan,
                TraitKind.Honor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_mercy",
            "trait")]
        public static string ClanUnlockMercy(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.clan_unlock_mercy",
                TraitScope.Clan,
                TraitKind.Mercy);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_valor",
            "trait")]
        public static string ClanUnlockValor(List<string> args)
        {
            return ExecuteUnlockTrait(
                args,
                "trait.clan_unlock_valor",
                TraitScope.Clan,
                TraitKind.Valor);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_all",
            "trait")]
        public static string ClanLockAll(List<string> args)
        {
            return ExecuteLockAll(
                args,
                "trait.clan_lock_all",
                TraitScope.Clan);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_all_by_values",
            "trait")]
        public static string ClanLockAllByValues(List<string> args)
        {
            return ExecuteLockAllByValues(
                args,
                "trait.clan_lock_all_by_values",
                TraitScope.Clan);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_unlock_all",
            "trait")]
        public static string ClanUnlockAll(List<string> args)
        {
            return ExecuteUnlockAll(
                args,
                "trait.clan_unlock_all",
                TraitScope.Clan);
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_lock_status",
            "trait")]
        public static string ClanLockStatus(List<string> args)
        {
            return ExecuteStatus(
                args,
                "trait.clan_lock_status",
                TraitScope.Clan);
        }

        // ------------------------------------------------------------
        // Inspection commands
        // ------------------------------------------------------------

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "values",
            "trait")]
        public static string Values(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "trait.values",
                    out string error))
            {
                return error;
            }

            return FormatHeroTraitValues(
                Hero.MainHero,
                "Player");
        }

        [CommandLineFunctionality.CommandLineArgumentFunction(
            "clan_values",
            "trait")]
        public static string ClanValues(List<string> args)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    "trait.clan_values",
                    out string error))
            {
                return error;
            }

            StringBuilder result =
                new StringBuilder();

            int count = 0;

            foreach (Hero hero in GetLivingPlayerClanHeroes())
            {
                if (count > 0)
                {
                    result.AppendLine();
                }

                result.Append(
                    FormatHeroTraitValues(
                        hero,
                        "Clan hero"));

                count++;
            }

            if (count == 0)
            {
                return
                    "No living heroes were found in the player's clan.";
            }

            return result
                .ToString()
                .TrimEnd();
        }

        // ------------------------------------------------------------
        // Shared command execution
        // ------------------------------------------------------------

        private static string ExecuteSetTrait(
            List<string> args,
            string commandName,
            TraitScope scope,
            TraitKind traitKind)
        {
            if (!TryPrepareSingleValueCommand(
                    args,
                    commandName,
                    out double rawValue,
                    out int normalizedValue,
                    out string error))
            {
                return error;
            }

            int affectedCount =
                ApplyTraitValue(
                    scope,
                    traitKind,
                    normalizedValue);

            string targetDescription =
                scope == TraitScope.Player
                    ? "player"
                    : $"{affectedCount} living player-clan hero(s)";

            return
                $"{GetTraitName(traitKind)} set to " +
                $"{normalizedValue} for {targetDescription}" +
                FormatNormalizationSuffix(
                    rawValue,
                    normalizedValue) +
                ".";
        }

        private static string ExecuteLockTrait(
            List<string> args,
            string commandName,
            TraitScope scope,
            TraitKind traitKind)
        {
            if (!TryPrepareSingleValueCommand(
                    args,
                    commandName,
                    out double rawValue,
                    out int normalizedValue,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            string result =
                behavior.LockTrait(
                    scope,
                    traitKind,
                    normalizedValue);

            return result +
                FormatNormalizationSuffix(
                    rawValue,
                    normalizedValue);
        }

        private static string ExecuteUnlockTrait(
            List<string> args,
            string commandName,
            TraitScope scope,
            TraitKind traitKind)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    commandName,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            return behavior.UnlockTrait(
                scope,
                traitKind);
        }

        private static string ExecuteLockAll(
            List<string> args,
            string commandName,
            TraitScope scope)
        {
            if (!TryPrepareSingleValueCommand(
                    args,
                    commandName,
                    out double rawValue,
                    out int normalizedValue,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            string result =
                behavior.LockAll(
                    scope,
                    normalizedValue);

            return result +
                FormatNormalizationSuffix(
                    rawValue,
                    normalizedValue);
        }

        private static string ExecuteLockAllByValues(
            List<string> args,
            string commandName,
            TraitScope scope)
        {
            if (!TryPrepareFiveValueCommand(
                    args,
                    commandName,
                    out double[] rawValues,
                    out int[] normalizedValues,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            string result =
                behavior.LockAllByValues(
                    scope,
                    normalizedValues);

            List<string> normalizationNotes =
                new List<string>();

            for (int index = 0;
                 index < normalizedValues.Length;
                 index++)
            {
                if (!ValuesEquivalent(
                        rawValues[index],
                        normalizedValues[index]))
                {
                    normalizationNotes.Add(
                        $"{GetTraitName((TraitKind)index)} " +
                        $"{FormatRawValue(rawValues[index])}" +
                        $" -> {normalizedValues[index]}");
                }
            }

            if (normalizationNotes.Count == 0)
            {
                return result;
            }

            return result +
                " Normalized: " +
                string.Join(
                    ", ",
                    normalizationNotes) +
                ".";
        }

        private static string ExecuteUnlockAll(
            List<string> args,
            string commandName,
            TraitScope scope)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    commandName,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            return behavior.UnlockAll(scope);
        }

        private static string ExecuteStatus(
            List<string> args,
            string commandName,
            TraitScope scope)
        {
            if (!TryPrepareNoValueCommand(
                    args,
                    commandName,
                    out string error))
            {
                return error;
            }

            TraitLockBehavior behavior =
                TraitLockBehavior.Current;

            if (behavior == null)
            {
                return "Trait lock behavior is not available.";
            }

            return behavior.GetStatus(scope);
        }

        // ------------------------------------------------------------
        // Trait application helpers
        // ------------------------------------------------------------

        internal static int ApplyTraitValue(
            TraitScope scope,
            TraitKind traitKind,
            int value)
        {
            if (scope == TraitScope.Player)
            {
                Hero.MainHero.SetTraitLevel(
                    GetTraitObject(traitKind),
                    value);

                return 1;
            }

            int affectedCount = 0;

            foreach (Hero hero in GetLivingPlayerClanHeroes())
            {
                hero.SetTraitLevel(
                    GetTraitObject(traitKind),
                    value);

                affectedCount++;
            }

            return affectedCount;
        }

        internal static void ApplyTraitValue(
            Hero hero,
            TraitKind traitKind,
            int value)
        {
            if (hero == null ||
                !hero.IsAlive)
            {
                return;
            }

            hero.SetTraitLevel(
                GetTraitObject(traitKind),
                value);
        }

        internal static IEnumerable<Hero>
            GetLivingPlayerClanHeroes()
        {
            if (Clan.PlayerClan == null)
            {
                yield break;
            }

            foreach (Hero hero in Clan.PlayerClan.Heroes)
            {
                if (hero != null &&
                    hero.IsAlive)
                {
                    yield return hero;
                }
            }
        }

        internal static TraitObject GetTraitObject(
            TraitKind traitKind)
        {
            switch (traitKind)
            {
                case TraitKind.Calculating:
                    return DefaultTraits.Calculating;

                case TraitKind.Generosity:
                    return DefaultTraits.Generosity;

                case TraitKind.Honor:
                    return DefaultTraits.Honor;

                case TraitKind.Mercy:
                    return DefaultTraits.Mercy;

                case TraitKind.Valor:
                    return DefaultTraits.Valor;

                default:
                    throw new ArgumentOutOfRangeException(
                        nameof(traitKind));
            }
        }

        internal static string GetTraitName(
            TraitKind traitKind)
        {
            switch (traitKind)
            {
                case TraitKind.Calculating:
                    return "Calculating";

                case TraitKind.Generosity:
                    return "Generosity";

                case TraitKind.Honor:
                    return "Honor";

                case TraitKind.Mercy:
                    return "Mercy";

                case TraitKind.Valor:
                    return "Valor";

                default:
                    return traitKind.ToString();
            }
        }

        private static string FormatHeroTraitValues(
            Hero hero,
            string heading)
        {
            if (hero == null)
            {
                return
                    $"{heading}: <unavailable>";
            }

            StringBuilder result =
                new StringBuilder();

            result.AppendLine(
                $"{heading}: {hero.Name} " +
                $"(ID: {hero.StringId})");

            foreach (TraitKind traitKind in
                     new[]
                     {
                         TraitKind.Calculating,
                         TraitKind.Generosity,
                         TraitKind.Honor,
                         TraitKind.Mercy,
                         TraitKind.Valor
                     })
            {
                int value =
                    hero.GetTraitLevel(
                        GetTraitObject(traitKind));

                result.AppendLine(
                    $"{GetTraitName(traitKind)}: {value}");
            }

            return result
                .ToString()
                .TrimEnd();
        }

        // ------------------------------------------------------------
        // Validation and normalization
        // ------------------------------------------------------------

        private static bool TryPrepareSingleValueCommand(
            List<string> args,
            string commandName,
            out double rawValue,
            out int normalizedValue,
            out string error)
        {
            rawValue = 0;
            normalizedValue = 0;
            error = string.Empty;

            if (!TryPrepareCampaignCommand(
                    out error))
            {
                return false;
            }

            if (args.Count != 1 ||
                !TryParseDouble(
                    args[0],
                    out rawValue))
            {
                error =
                    $"Usage: {commandName} <value>";
                return false;
            }

            normalizedValue =
                NormalizeTraitValue(rawValue);

            return true;
        }

        private static bool TryPrepareFiveValueCommand(
            List<string> args,
            string commandName,
            out double[] rawValues,
            out int[] normalizedValues,
            out string error)
        {
            rawValues = new double[5];
            normalizedValues = new int[5];
            error = string.Empty;

            if (!TryPrepareCampaignCommand(
                    out error))
            {
                return false;
            }

            if (args.Count != 5)
            {
                error =
                    $"Usage: {commandName} " +
                    "<calculating> <generosity> " +
                    "<honor> <mercy> <valor>";

                return false;
            }

            for (int index = 0;
                 index < 5;
                 index++)
            {
                if (!TryParseDouble(
                        args[index],
                        out rawValues[index]))
                {
                    error =
                        $"Usage: {commandName} " +
                        "<calculating> <generosity> " +
                        "<honor> <mercy> <valor>";

                    return false;
                }

                normalizedValues[index] =
                    NormalizeTraitValue(
                        rawValues[index]);
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

        private static bool TryParseDouble(
            string text,
            out double value)
        {
            if (double.TryParse(
                    text,
                    NumberStyles.Float,
                    CultureInfo.InvariantCulture,
                    out value))
            {
                return true;
            }

            return double.TryParse(
                text,
                NumberStyles.Float,
                CultureInfo.CurrentCulture,
                out value);
        }

        internal static int NormalizeTraitValue(
            double value)
        {
            double clampedValue =
                Math.Max(
                    -2.0,
                    Math.Min(
                        2.0,
                        value));

            return
                (int)Math.Floor(
                    clampedValue + 0.5);
        }

        private static bool ValuesEquivalent(
            double rawValue,
            int normalizedValue)
        {
            return
                Math.Abs(
                    rawValue - normalizedValue) <
                0.000001;
        }

        private static string FormatNormalizationSuffix(
            double rawValue,
            int normalizedValue)
        {
            if (ValuesEquivalent(
                    rawValue,
                    normalizedValue))
            {
                return string.Empty;
            }

            return
                $" (input {FormatRawValue(rawValue)} " +
                $"normalized to {normalizedValue})";
        }

        private static string FormatRawValue(
            double rawValue)
        {
            return rawValue.ToString(
                "0.###",
                CultureInfo.InvariantCulture);
        }
    }
}

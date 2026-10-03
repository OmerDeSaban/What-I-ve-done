using System.Text;
using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.CharacterDevelopment;

namespace CharacterCommands
{
    public sealed class TraitLockBehavior :
        CampaignBehaviorBase
    {
        internal static TraitLockBehavior Current
        {
            get;
            private set;
        }

        private bool _playerAllLocked;
        private int _playerAllValue;

        private bool _playerCalculatingLocked;
        private int _playerCalculatingValue;
        private bool _playerGenerosityLocked;
        private int _playerGenerosityValue;
        private bool _playerHonorLocked;
        private int _playerHonorValue;
        private bool _playerMercyLocked;
        private int _playerMercyValue;
        private bool _playerValorLocked;
        private int _playerValorValue;

        private bool _clanAllLocked;
        private int _clanAllValue;

        private bool _clanCalculatingLocked;
        private int _clanCalculatingValue;
        private bool _clanGenerosityLocked;
        private int _clanGenerosityValue;
        private bool _clanHonorLocked;
        private int _clanHonorValue;
        private bool _clanMercyLocked;
        private int _clanMercyValue;
        private bool _clanValorLocked;
        private int _clanValorValue;

        private bool _isApplyingLocks;

        public TraitLockBehavior()
        {
            Current = this;
        }

        public override void RegisterEvents()
        {
            CampaignEvents.PlayerTraitChangedEvent
                .AddNonSerializedListener(
                    this,
                    OnPlayerTraitChanged);

            CampaignEvents.HourlyTickEvent
                .AddNonSerializedListener(
                    this,
                    OnHourlyTick);
        }

        public override void SyncData(
            IDataStore dataStore)
        {
            dataStore.SyncData(
                "CharacterCommands_PlayerAllLocked",
                ref _playerAllLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerAllValue",
                ref _playerAllValue);

            dataStore.SyncData(
                "CharacterCommands_PlayerCalculatingLocked",
                ref _playerCalculatingLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerCalculatingValue",
                ref _playerCalculatingValue);

            dataStore.SyncData(
                "CharacterCommands_PlayerGenerosityLocked",
                ref _playerGenerosityLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerGenerosityValue",
                ref _playerGenerosityValue);

            dataStore.SyncData(
                "CharacterCommands_PlayerHonorLocked",
                ref _playerHonorLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerHonorValue",
                ref _playerHonorValue);

            dataStore.SyncData(
                "CharacterCommands_PlayerMercyLocked",
                ref _playerMercyLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerMercyValue",
                ref _playerMercyValue);

            dataStore.SyncData(
                "CharacterCommands_PlayerValorLocked",
                ref _playerValorLocked);

            dataStore.SyncData(
                "CharacterCommands_PlayerValorValue",
                ref _playerValorValue);

            dataStore.SyncData(
                "CharacterCommands_ClanAllLocked",
                ref _clanAllLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanAllValue",
                ref _clanAllValue);

            dataStore.SyncData(
                "CharacterCommands_ClanCalculatingLocked",
                ref _clanCalculatingLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanCalculatingValue",
                ref _clanCalculatingValue);

            dataStore.SyncData(
                "CharacterCommands_ClanGenerosityLocked",
                ref _clanGenerosityLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanGenerosityValue",
                ref _clanGenerosityValue);

            dataStore.SyncData(
                "CharacterCommands_ClanHonorLocked",
                ref _clanHonorLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanHonorValue",
                ref _clanHonorValue);

            dataStore.SyncData(
                "CharacterCommands_ClanMercyLocked",
                ref _clanMercyLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanMercyValue",
                ref _clanMercyValue);

            dataStore.SyncData(
                "CharacterCommands_ClanValorLocked",
                ref _clanValorLocked);

            dataStore.SyncData(
                "CharacterCommands_ClanValorValue",
                ref _clanValorValue);
        }

        internal string LockTrait(
            TraitScope scope,
            TraitKind traitKind,
            int value)
        {
            if (IsAllLocked(scope))
            {
                int inheritedValue =
                    GetAllValue(scope);

                if (inheritedValue == value)
                {
                    return
                        $"{GetScopePrefix(scope)}" +
                        $"{CharacterConsoleCommands.GetTraitName(traitKind)} " +
                        $"is already locked to {value} by " +
                        GetLockAllCommandName(scope) +
                        ".";
                }

                SplitAllLock(
                    scope,
                    inheritedValue);
            }

            if (IsTraitLocked(
                    scope,
                    traitKind) &&
                GetTraitValue(
                    scope,
                    traitKind) == value)
            {
                return
                    $"{GetScopePrefix(scope)}" +
                    $"{CharacterConsoleCommands.GetTraitName(traitKind)} " +
                    $"is already locked to {value}.";
            }

            SetTraitLock(
                scope,
                traitKind,
                true,
                value);

            ApplyActiveLocks();

            return
                $"Locked {GetScopePrefix(scope).ToLower()}" +
                $"{CharacterConsoleCommands.GetTraitName(traitKind)} " +
                $"to {value}.";
        }

        internal string UnlockTrait(
            TraitScope scope,
            TraitKind traitKind)
        {
            if (IsAllLocked(scope))
            {
                int inheritedValue =
                    GetAllValue(scope);

                SplitAllLock(
                    scope,
                    inheritedValue);

                SetTraitLock(
                    scope,
                    traitKind,
                    false,
                    0);

                ApplyActiveLocks();

                return
                    $"Unlocked {GetScopePrefix(scope).ToLower()}" +
                    $"{CharacterConsoleCommands.GetTraitName(traitKind)}. " +
                    $"The previous {GetLockAllCommandName(scope)} " +
                    $"value {inheritedValue} remains active for the " +
                    "other traits.";
            }

            if (!IsTraitLocked(
                    scope,
                    traitKind))
            {
                return
                    $"{GetScopePrefix(scope)}" +
                    $"{CharacterConsoleCommands.GetTraitName(traitKind)} " +
                    "is already unlocked.";
            }

            SetTraitLock(
                scope,
                traitKind,
                false,
                0);

            ApplyActiveLocks();

            return
                $"Unlocked {GetScopePrefix(scope).ToLower()}" +
                $"{CharacterConsoleCommands.GetTraitName(traitKind)}.";
        }

        internal string LockAll(
            TraitScope scope,
            int value)
        {
            if (IsAllLocked(scope) &&
                GetAllValue(scope) == value)
            {
                return
                    $"{GetScopeDisplayName(scope)} traits " +
                    $"are already locked to {value} by " +
                    GetLockAllCommandName(scope) +
                    ".";
            }

            SetAllLock(
                scope,
                true,
                value);

            ClearIndividualLocks(scope);

            ApplyActiveLocks();

            return
                $"Locked all {GetScopeDisplayName(scope).ToLower()} " +
                $"traits to {value}.";
        }

        internal string LockAllByValues(
            TraitScope scope,
            int[] values)
        {
            if (values == null ||
                values.Length != 5)
            {
                return
                    "Exactly five trait values are required.";
            }

            bool allEqual = true;

            for (int index = 1;
                 index < values.Length;
                 index++)
            {
                if (values[index] != values[0])
                {
                    allEqual = false;
                    break;
                }
            }

            if (allEqual)
            {
                return LockAll(
                    scope,
                    values[0]);
            }

            SetAllLock(
                scope,
                false,
                0);

            for (int index = 0;
                 index < values.Length;
                 index++)
            {
                SetTraitLock(
                    scope,
                    (TraitKind)index,
                    true,
                    values[index]);
            }

            ApplyActiveLocks();

            return
                $"Locked all {GetScopeDisplayName(scope).ToLower()} " +
                "traits to individual values: " +
                $"Calculating {values[0]}, " +
                $"Generosity {values[1]}, " +
                $"Honor {values[2]}, " +
                $"Mercy {values[3]}, " +
                $"Valor {values[4]}.";
        }

        internal string UnlockAll(
            TraitScope scope)
        {
            if (!HasAnyLock(scope))
            {
                return
                    $"No {GetScopeDisplayName(scope).ToLower()} " +
                    "trait locks are currently active.";
            }

            SetAllLock(
                scope,
                false,
                0);

            ClearIndividualLocks(scope);

            ApplyActiveLocks();

            return
                $"All {GetScopeDisplayName(scope).ToLower()} " +
                "trait locks have been disabled.";
        }

        internal string GetStatus(
            TraitScope scope)
        {
            StringBuilder result =
                new StringBuilder();

            result.AppendLine(
                $"{GetScopeDisplayName(scope)} trait locks:");

            if (IsAllLocked(scope))
            {
                int value =
                    GetAllValue(scope);

                result.AppendLine(
                    $"All: LOCKED at {value}");

                foreach (TraitKind traitKind
                         in GetAllTraitKinds())
                {
                    result.AppendLine(
                        $"{CharacterConsoleCommands.GetTraitName(traitKind)}: " +
                        $"inherited {value}");
                }
            }
            else
            {
                result.AppendLine(
                    "All: OFF");

                foreach (TraitKind traitKind
                         in GetAllTraitKinds())
                {
                    result.AppendLine(
                        $"{CharacterConsoleCommands.GetTraitName(traitKind)}: " +
                        FormatTraitLock(
                            scope,
                            traitKind));
                }
            }

            if (scope == TraitScope.Player &&
                HasAnyLock(TraitScope.Clan))
            {
                result.AppendLine(
                    "Player-specific locks override clan-wide locks " +
                    "for the player.");
            }

            return result
                .ToString()
                .TrimEnd();
        }

        private void OnHourlyTick()
        {
            ApplyActiveLocks();
        }

        private void OnPlayerTraitChanged(
            TraitObject trait,
            int previousLevel)
        {
            if (_isApplyingLocks ||
                Hero.MainHero == null)
            {
                return;
            }

            if (!TryGetTraitKind(
                    trait,
                    out TraitKind traitKind))
            {
                return;
            }

            if (!TryGetEffectivePlayerLock(
                    traitKind,
                    out int targetValue))
            {
                return;
            }

            int currentValue =
                Hero.MainHero.GetTraitLevel(
                    CharacterConsoleCommands
                        .GetTraitObject(traitKind));

            if (currentValue == targetValue)
            {
                return;
            }

            _isApplyingLocks = true;

            try
            {
                CharacterConsoleCommands.ApplyTraitValue(
                    Hero.MainHero,
                    traitKind,
                    targetValue);
            }
            finally
            {
                _isApplyingLocks = false;
            }
        }

        private void ApplyActiveLocks()
        {
            if (_isApplyingLocks ||
                Campaign.Current == null ||
                Hero.MainHero == null ||
                Clan.PlayerClan == null)
            {
                return;
            }

            _isApplyingLocks = true;

            try
            {
                foreach (Hero hero
                         in CharacterConsoleCommands
                             .GetLivingPlayerClanHeroes())
                {
                    foreach (TraitKind traitKind
                             in GetAllTraitKinds())
                    {
                        if (hero == Hero.MainHero)
                        {
                            if (TryGetEffectivePlayerLock(
                                    traitKind,
                                    out int playerValue))
                            {
                                ApplyIfDifferent(
                                    hero,
                                    traitKind,
                                    playerValue);
                            }

                            continue;
                        }

                        if (TryGetScopeLock(
                                TraitScope.Clan,
                                traitKind,
                                out int clanValue))
                        {
                            ApplyIfDifferent(
                                hero,
                                traitKind,
                                clanValue);
                        }
                    }
                }
            }
            finally
            {
                _isApplyingLocks = false;
            }
        }

        private static void ApplyIfDifferent(
            Hero hero,
            TraitKind traitKind,
            int value)
        {
            int currentValue =
                hero.GetTraitLevel(
                    CharacterConsoleCommands
                        .GetTraitObject(traitKind));

            if (currentValue == value)
            {
                return;
            }

            CharacterConsoleCommands.ApplyTraitValue(
                hero,
                traitKind,
                value);
        }

        private bool TryGetEffectivePlayerLock(
            TraitKind traitKind,
            out int value)
        {
            if (TryGetScopeLock(
                    TraitScope.Player,
                    traitKind,
                    out value))
            {
                return true;
            }

            return TryGetScopeLock(
                TraitScope.Clan,
                traitKind,
                out value);
        }

        private bool TryGetScopeLock(
            TraitScope scope,
            TraitKind traitKind,
            out int value)
        {
            value = 0;

            if (IsAllLocked(scope))
            {
                value =
                    GetAllValue(scope);

                return true;
            }

            if (!IsTraitLocked(
                    scope,
                    traitKind))
            {
                return false;
            }

            value =
                GetTraitValue(
                    scope,
                    traitKind);

            return true;
        }

        private void SplitAllLock(
            TraitScope scope,
            int inheritedValue)
        {
            SetAllLock(
                scope,
                false,
                0);

            foreach (TraitKind traitKind
                     in GetAllTraitKinds())
            {
                SetTraitLock(
                    scope,
                    traitKind,
                    true,
                    inheritedValue);
            }
        }

        private bool HasAnyLock(
            TraitScope scope)
        {
            if (IsAllLocked(scope))
            {
                return true;
            }

            foreach (TraitKind traitKind
                     in GetAllTraitKinds())
            {
                if (IsTraitLocked(
                        scope,
                        traitKind))
                {
                    return true;
                }
            }

            return false;
        }

        private bool IsAllLocked(
            TraitScope scope)
        {
            return scope == TraitScope.Player
                ? _playerAllLocked
                : _clanAllLocked;
        }

        private int GetAllValue(
            TraitScope scope)
        {
            return scope == TraitScope.Player
                ? _playerAllValue
                : _clanAllValue;
        }

        private void SetAllLock(
            TraitScope scope,
            bool locked,
            int value)
        {
            if (scope == TraitScope.Player)
            {
                _playerAllLocked = locked;
                _playerAllValue = value;
            }
            else
            {
                _clanAllLocked = locked;
                _clanAllValue = value;
            }
        }

        private bool IsTraitLocked(
            TraitScope scope,
            TraitKind traitKind)
        {
            if (scope == TraitScope.Player)
            {
                switch (traitKind)
                {
                    case TraitKind.Calculating:
                        return _playerCalculatingLocked;

                    case TraitKind.Generosity:
                        return _playerGenerosityLocked;

                    case TraitKind.Honor:
                        return _playerHonorLocked;

                    case TraitKind.Mercy:
                        return _playerMercyLocked;

                    case TraitKind.Valor:
                        return _playerValorLocked;
                }
            }
            else
            {
                switch (traitKind)
                {
                    case TraitKind.Calculating:
                        return _clanCalculatingLocked;

                    case TraitKind.Generosity:
                        return _clanGenerosityLocked;

                    case TraitKind.Honor:
                        return _clanHonorLocked;

                    case TraitKind.Mercy:
                        return _clanMercyLocked;

                    case TraitKind.Valor:
                        return _clanValorLocked;
                }
            }

            return false;
        }

        private int GetTraitValue(
            TraitScope scope,
            TraitKind traitKind)
        {
            if (scope == TraitScope.Player)
            {
                switch (traitKind)
                {
                    case TraitKind.Calculating:
                        return _playerCalculatingValue;

                    case TraitKind.Generosity:
                        return _playerGenerosityValue;

                    case TraitKind.Honor:
                        return _playerHonorValue;

                    case TraitKind.Mercy:
                        return _playerMercyValue;

                    case TraitKind.Valor:
                        return _playerValorValue;
                }
            }
            else
            {
                switch (traitKind)
                {
                    case TraitKind.Calculating:
                        return _clanCalculatingValue;

                    case TraitKind.Generosity:
                        return _clanGenerosityValue;

                    case TraitKind.Honor:
                        return _clanHonorValue;

                    case TraitKind.Mercy:
                        return _clanMercyValue;

                    case TraitKind.Valor:
                        return _clanValorValue;
                }
            }

            return 0;
        }

        private void SetTraitLock(
            TraitScope scope,
            TraitKind traitKind,
            bool locked,
            int value)
        {
            if (scope == TraitScope.Player)
            {
                switch (traitKind)
                {
                    case TraitKind.Calculating:
                        _playerCalculatingLocked = locked;
                        _playerCalculatingValue = value;
                        break;

                    case TraitKind.Generosity:
                        _playerGenerosityLocked = locked;
                        _playerGenerosityValue = value;
                        break;

                    case TraitKind.Honor:
                        _playerHonorLocked = locked;
                        _playerHonorValue = value;
                        break;

                    case TraitKind.Mercy:
                        _playerMercyLocked = locked;
                        _playerMercyValue = value;
                        break;

                    case TraitKind.Valor:
                        _playerValorLocked = locked;
                        _playerValorValue = value;
                        break;
                }

                return;
            }

            switch (traitKind)
            {
                case TraitKind.Calculating:
                    _clanCalculatingLocked = locked;
                    _clanCalculatingValue = value;
                    break;

                case TraitKind.Generosity:
                    _clanGenerosityLocked = locked;
                    _clanGenerosityValue = value;
                    break;

                case TraitKind.Honor:
                    _clanHonorLocked = locked;
                    _clanHonorValue = value;
                    break;

                case TraitKind.Mercy:
                    _clanMercyLocked = locked;
                    _clanMercyValue = value;
                    break;

                case TraitKind.Valor:
                    _clanValorLocked = locked;
                    _clanValorValue = value;
                    break;
            }
        }

        private void ClearIndividualLocks(
            TraitScope scope)
        {
            foreach (TraitKind traitKind
                     in GetAllTraitKinds())
            {
                SetTraitLock(
                    scope,
                    traitKind,
                    false,
                    0);
            }
        }

        private string FormatTraitLock(
            TraitScope scope,
            TraitKind traitKind)
        {
            if (!IsTraitLocked(
                    scope,
                    traitKind))
            {
                return "OFF";
            }

            return
                $"LOCKED at " +
                GetTraitValue(
                    scope,
                    traitKind);
        }

        private static bool TryGetTraitKind(
            TraitObject trait,
            out TraitKind traitKind)
        {
            if (trait == DefaultTraits.Calculating)
            {
                traitKind = TraitKind.Calculating;
                return true;
            }

            if (trait == DefaultTraits.Generosity)
            {
                traitKind = TraitKind.Generosity;
                return true;
            }

            if (trait == DefaultTraits.Honor)
            {
                traitKind = TraitKind.Honor;
                return true;
            }

            if (trait == DefaultTraits.Mercy)
            {
                traitKind = TraitKind.Mercy;
                return true;
            }

            if (trait == DefaultTraits.Valor)
            {
                traitKind = TraitKind.Valor;
                return true;
            }

            traitKind = TraitKind.Calculating;
            return false;
        }

        private static TraitKind[] GetAllTraitKinds()
        {
            return new[]
            {
                TraitKind.Calculating,
                TraitKind.Generosity,
                TraitKind.Honor,
                TraitKind.Mercy,
                TraitKind.Valor
            };
        }

        private static string GetScopeDisplayName(
            TraitScope scope)
        {
            return scope == TraitScope.Player
                ? "Player"
                : "Clan";
        }

        private static string GetScopePrefix(
            TraitScope scope)
        {
            return scope == TraitScope.Player
                ? string.Empty
                : "Clan ";
        }

        private static string GetLockAllCommandName(
            TraitScope scope)
        {
            return scope == TraitScope.Player
                ? "trait.lock_all"
                : "trait.clan_lock_all";
        }
    }
}

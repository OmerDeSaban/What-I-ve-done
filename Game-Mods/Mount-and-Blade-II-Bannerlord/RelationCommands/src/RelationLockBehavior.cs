using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.Actions;
using TaleWorlds.CampaignSystem.Party;

namespace RelationCommands
{
    public sealed class RelationLockBehavior : CampaignBehaviorBase
    {
        internal static RelationLockBehavior Current { get; private set; }

        private bool _everyoneLocked;
        private int _everyoneValue;

        private bool _notablesLocked;
        private int _notablesValue;

        private bool _wanderersLocked;
        private int _wanderersValue;

        private bool _clansLocked;
        private int _clansValue;

        private bool _isApplyingLocks;

        public RelationLockBehavior()
        {
            Current = this;
        }

        public override void RegisterEvents()
        {
            CampaignEvents.HeroRelationChanged.AddNonSerializedListener(
                this,
                OnHeroRelationChanged);

            CampaignEvents.HourlyTickEvent.AddNonSerializedListener(
                this,
                OnHourlyTick);
        }

        public override void SyncData(IDataStore dataStore)
        {
            dataStore.SyncData(
                "RelationCommands_EveryoneLocked",
                ref _everyoneLocked);

            dataStore.SyncData(
                "RelationCommands_EveryoneValue",
                ref _everyoneValue);

            dataStore.SyncData(
                "RelationCommands_NotablesLocked",
                ref _notablesLocked);

            dataStore.SyncData(
                "RelationCommands_NotablesValue",
                ref _notablesValue);

            dataStore.SyncData(
                "RelationCommands_WanderersLocked",
                ref _wanderersLocked);

            dataStore.SyncData(
                "RelationCommands_WanderersValue",
                ref _wanderersValue);

            dataStore.SyncData(
                "RelationCommands_ClansLocked",
                ref _clansLocked);

            dataStore.SyncData(
                "RelationCommands_ClansValue",
                ref _clansValue);
        }

        internal string LockEveryone(int value)
        {
            if (_everyoneLocked &&
                _everyoneValue == value)
            {
                return
                    $"Everyone is already locked to relation {value}.";
            }

            _everyoneLocked = true;
            _everyoneValue = value;

            ClearCategoryLocks();

            ApplyActiveLocks();

            return
                $"Locked everyone to relation {value}.";
        }

        internal string LockGroup(
            RelationGroup group,
            int value)
        {
            if (group == RelationGroup.Everyone)
            {
                return LockEveryone(value);
            }

            /*
             * lock_everyone has higher hierarchy.
             *
             * If the requested category is already effectively locked
             * to the same value, dismiss the command.
             */
            if (_everyoneLocked)
            {
                if (_everyoneValue == value)
                {
                    return
                        $"{GetGroupDisplayName(group)} are already " +
                        $"locked to relation {value} by " +
                        "relation.lock_everyone.";
                }

                /*
                 * Split the global lock into all three category locks,
                 * preserving the old global value.
                 */
                int inheritedValue =
                    _everyoneValue;

                SplitEveryoneLock(
                    inheritedValue);
            }

            if (IsGroupLocked(group) &&
                GetGroupValue(group) == value)
            {
                return
                    $"{GetGroupDisplayName(group)} are already " +
                    $"locked to relation {value}.";
            }

            SetGroupLock(
                group,
                true,
                value);

            ApplyActiveLocks();

            return
                $"Locked {GetGroupDisplayName(group).ToLower()} " +
                $"to relation {value}.";
        }

        internal string UnlockEveryone()
        {
            if (!_everyoneLocked &&
                !_notablesLocked &&
                !_wanderersLocked &&
                !_clansLocked)
            {
                return "No relation locks are currently active.";
            }

            _everyoneLocked = false;
            ClearCategoryLocks();

            return "All relation locks have been disabled.";
        }

        internal string UnlockGroup(
            RelationGroup group)
        {
            if (group == RelationGroup.Everyone)
            {
                return UnlockEveryone();
            }

            /*
             * If lock_everyone is active, unlocking one category
             * splits the global lock first.
             *
             * Example:
             *
             * everyone = 50
             * unlock_wanderers
             *
             * becomes:
             *
             * notables = 50
             * wanderers = OFF
             * clans = 50
             */
            if (_everyoneLocked)
            {
                int inheritedValue =
                    _everyoneValue;

                SplitEveryoneLock(
                    inheritedValue);

                SetGroupLock(
                    group,
                    false,
                    0);

                ApplyActiveLocks();

                return
                    $"Unlocked {GetGroupDisplayName(group).ToLower()}. " +
                    $"The previous lock_everyone value " +
                    $"{inheritedValue} remains active for the other " +
                    "relation groups.";
            }

            if (!IsGroupLocked(group))
            {
                return
                    $"{GetGroupDisplayName(group)} are already unlocked.";
            }

            SetGroupLock(
                group,
                false,
                0);

            return
                $"Unlocked {GetGroupDisplayName(group).ToLower()}.";
        }

        internal string GetStatus()
        {
            if (_everyoneLocked)
            {
                return
                    $"Everyone: LOCKED at {_everyoneValue}\n" +
                    $"Notables: inherited {_everyoneValue}\n" +
                    $"Wanderers: inherited {_everyoneValue}\n" +
                    $"Clans: inherited {_everyoneValue}";
            }

            return
                $"Everyone: OFF\n" +
                $"Notables: {FormatLock(_notablesLocked, _notablesValue)}\n" +
                $"Wanderers: {FormatLock(_wanderersLocked, _wanderersValue)}\n" +
                $"Clans: {FormatLock(_clansLocked, _clansValue)}";
        }

        private void OnHourlyTick()
        {
            ApplyActiveLocks();
        }

        private void OnHeroRelationChanged(
            Hero effectiveHero,
            Hero effectiveHeroGainedRelationWith,
            int relationChange,
            bool showNotification,
            ChangeRelationAction.ChangeRelationDetail detail,
            Hero originalHero,
            Hero originalGainedRelationWith)
        {
            if (_isApplyingLocks ||
                Hero.MainHero == null)
            {
                return;
            }

            Hero targetHero =
                GetOtherHeroIfPlayerRelation(
                    originalHero,
                    originalGainedRelationWith);

            if (targetHero == null)
            {
                targetHero =
                    GetOtherHeroIfPlayerRelation(
                        effectiveHero,
                        effectiveHeroGainedRelationWith);
            }

            if (targetHero == null)
            {
                return;
            }

            if (!TryGetLockedValueForHero(
                    targetHero,
                    out int targetValue))
            {
                return;
            }

            _isApplyingLocks = true;

            try
            {
                RelationConsoleCommands.EnforceSingleHero(
                    targetHero,
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
                Hero.MainHero == null)
            {
                return;
            }

            _isApplyingLocks = true;

            try
            {
                if (_everyoneLocked)
                {
                    RelationConsoleCommands.ApplyGroupRelation(
                        RelationGroup.Everyone,
                        _everyoneValue);

                    return;
                }

                if (_notablesLocked)
                {
                    RelationConsoleCommands.ApplyGroupRelation(
                        RelationGroup.Notables,
                        _notablesValue);
                }

                if (_wanderersLocked)
                {
                    RelationConsoleCommands.ApplyGroupRelation(
                        RelationGroup.Wanderers,
                        _wanderersValue);
                }

                /*
                 * Clans are applied last. This also gives clan locks
                 * deterministic precedence in the rare case that a
                 * modded hero belongs to more than one category.
                 */
                if (_clansLocked)
                {
                    RelationConsoleCommands.ApplyGroupRelation(
                        RelationGroup.Clans,
                        _clansValue);
                }
            }
            finally
            {
                _isApplyingLocks = false;
            }
        }

        private bool TryGetLockedValueForHero(
            Hero hero,
            out int value)
        {
            value = 0;

            if (_everyoneLocked)
            {
                value = _everyoneValue;
                return true;
            }

            /*
             * Use the same precedence as ApplyActiveLocks().
             */
            if (_clansLocked &&
                RelationConsoleCommands.HeroMatchesGroup(
                    hero,
                    RelationGroup.Clans))
            {
                value = _clansValue;
                return true;
            }

            if (_wanderersLocked &&
                RelationConsoleCommands.HeroMatchesGroup(
                    hero,
                    RelationGroup.Wanderers))
            {
                value = _wanderersValue;
                return true;
            }

            if (_notablesLocked &&
                RelationConsoleCommands.HeroMatchesGroup(
                    hero,
                    RelationGroup.Notables))
            {
                value = _notablesValue;
                return true;
            }

            return false;
        }

        private void SplitEveryoneLock(
            int inheritedValue)
        {
            _everyoneLocked = false;

            _notablesLocked = true;
            _notablesValue = inheritedValue;

            _wanderersLocked = true;
            _wanderersValue = inheritedValue;

            _clansLocked = true;
            _clansValue = inheritedValue;
        }

        private void ClearCategoryLocks()
        {
            _notablesLocked = false;
            _wanderersLocked = false;
            _clansLocked = false;
        }

        private bool IsGroupLocked(
            RelationGroup group)
        {
            switch (group)
            {
                case RelationGroup.Notables:
                    return _notablesLocked;

                case RelationGroup.Wanderers:
                    return _wanderersLocked;

                case RelationGroup.Clans:
                    return _clansLocked;

                default:
                    return _everyoneLocked;
            }
        }

        private int GetGroupValue(
            RelationGroup group)
        {
            switch (group)
            {
                case RelationGroup.Notables:
                    return _notablesValue;

                case RelationGroup.Wanderers:
                    return _wanderersValue;

                case RelationGroup.Clans:
                    return _clansValue;

                default:
                    return _everyoneValue;
            }
        }

        private void SetGroupLock(
            RelationGroup group,
            bool locked,
            int value)
        {
            switch (group)
            {
                case RelationGroup.Notables:
                    _notablesLocked = locked;
                    _notablesValue = value;
                    break;

                case RelationGroup.Wanderers:
                    _wanderersLocked = locked;
                    _wanderersValue = value;
                    break;

                case RelationGroup.Clans:
                    _clansLocked = locked;
                    _clansValue = value;
                    break;
            }
        }

        private static Hero GetOtherHeroIfPlayerRelation(
            Hero firstHero,
            Hero secondHero)
        {
            if (firstHero == Hero.MainHero)
            {
                return secondHero;
            }

            if (secondHero == Hero.MainHero)
            {
                return firstHero;
            }

            return null;
        }

        private static string FormatLock(
            bool locked,
            int value)
        {
            return locked
                ? $"LOCKED at {value}"
                : "OFF";
        }

        private static string GetGroupDisplayName(
            RelationGroup group)
        {
            switch (group)
            {
                case RelationGroup.Notables:
                    return "Notables";

                case RelationGroup.Wanderers:
                    return "Wanderers";

                case RelationGroup.Clans:
                    return "Clans";

                default:
                    return "Everyone";
            }
        }
    }
}

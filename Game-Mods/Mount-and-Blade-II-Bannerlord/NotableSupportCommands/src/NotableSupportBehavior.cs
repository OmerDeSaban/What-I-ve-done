using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.Actions;

namespace NotableSupportCommands
{
    public sealed class NotableSupportBehavior :
        CampaignBehaviorBase
    {
        internal static NotableSupportBehavior Current
        {
            get;
            private set;
        }

        private bool _relationsLocked;
        private int _nonPlayerRelationCap;

        private bool _supportCostLocked;
        private int _supportCost;

        private const int FallbackSweepHours = 120;

        private bool _isApplyingRelations;
        private int _fallbackNotableIndex;

        internal bool SupportCostLocked
        {
            get
            {
                return _supportCostLocked;
            }
        }

        internal int SupportCost
        {
            get
            {
                return _supportCost;
            }
        }

        internal bool RelationsLocked
        {
            get
            {
                return _relationsLocked;
            }
        }

        internal int NonPlayerRelationCap
        {
            get
            {
                return _nonPlayerRelationCap;
            }
        }

        public NotableSupportBehavior()
        {
            Current = this;
        }

        public override void RegisterEvents()
        {
            CampaignEvents.HeroRelationChanged
                .AddNonSerializedListener(
                    this,
                    OnHeroRelationChanged);

            CampaignEvents.HourlyTickEvent
                .AddNonSerializedListener(
                    this,
                    OnHourlyTick);
        }

        public override void SyncData(
            IDataStore dataStore)
        {
            dataStore.SyncData(
                "NotableSupportCommands_RelationsLocked",
                ref _relationsLocked);

            dataStore.SyncData(
                "NotableSupportCommands_NonPlayerRelationCap",
                ref _nonPlayerRelationCap);

            dataStore.SyncData(
                "NotableSupportCommands_SupportCostLocked",
                ref _supportCostLocked);

            dataStore.SyncData(
                "NotableSupportCommands_SupportCost",
                ref _supportCost);
        }

        internal string LockRelations(
            int nonPlayerRelationCap)
        {
            if (_relationsLocked &&
                _nonPlayerRelationCap ==
                nonPlayerRelationCap)
            {
                return
                    $"Notable relations are already locked with " +
                    $"non-player relation cap {nonPlayerRelationCap}.";
            }

            _relationsLocked = true;
            _nonPlayerRelationCap =
                nonPlayerRelationCap;

            _fallbackNotableIndex = 0;

            RelationPolicyResult result =
                ApplyRelations();

            return
                $"Locked notable relations. " +
                $"Notable-to-notable and notable-to-player-clan " +
                $"relations are kept at " +
                $"{NotableSupportConsoleCommands.GetMaxRelation()}, " +
                $"while other non-player relations are capped at " +
                $"{nonPlayerRelationCap} and are never raised by the cap. " +
                $"{result.CompliantPairs}/{result.TotalPairs} managed " +
                "pair(s) currently comply with the policy.";
        }

        internal string UnlockRelations()
        {
            if (!_relationsLocked)
            {
                return
                    "Notable relation locking is already disabled.";
            }

            _relationsLocked = false;
            _fallbackNotableIndex = 0;

            return
                "Notable relation locking has been disabled. " +
                "Existing relations are left unchanged.";
        }

        internal string LockSupportCost(
            int supportCost)
        {
            if (_supportCostLocked &&
                _supportCost == supportCost)
            {
                return
                    $"Notable support cost is already locked to " +
                    $"{supportCost}.";
            }

            _supportCostLocked = true;
            _supportCost = supportCost;

            return
                $"Locked notable support cost to {supportCost}.";
        }

        internal string UnlockSupportCost()
        {
            if (!_supportCostLocked)
            {
                return
                    "Notable support cost locking is already disabled.";
            }

            _supportCostLocked = false;

            return
                "Notable support cost locking has been disabled. " +
                "The active base NotablePowerModel will determine " +
                "the support cost again.";
        }

        internal string GetStatus()
        {
            int maxRelation =
                NotableSupportConsoleCommands.GetMaxRelation();

            int defaultOtherNoble =
                NotableSupportConsoleCommands
                    .GetDefaultNonPlayerRelationCap();

            string relationStatus =
                _relationsLocked
                    ? $"LOCKED; non-player relation cap = " +
                      $"{_nonPlayerRelationCap}"
                    : "OFF";

            string costStatus =
                _supportCostLocked
                    ? $"LOCKED at {_supportCost}"
                    : "OFF (base model controls cost)";

            return
                $"Notable support settings:\n" +
                $"Relations: {relationStatus}\n" +
                $"Notable <-> notable target: {maxRelation}\n" +
                $"Notable <-> player-clan target: {maxRelation}\n" +
                $"Non-player relation rule: ceiling only\n" +
                $"Default safe non-player cap: " +
                $"{defaultOtherNoble}\n" +
                $"Hard support-switch block value: " +
                $"{maxRelation}\n" +
                $"Minimum player relation to request support: 50\n" +
                $"Fallback: full staggered sweep every " +
                $"{FallbackSweepHours} in-game hours\n" +
                $"Support cost: {costStatus}";
        }

        private void OnHourlyTick()
        {
            if (!_relationsLocked ||
                _isApplyingRelations ||
                Campaign.Current == null ||
                Hero.MainHero == null ||
                Clan.PlayerClan == null)
            {
                return;
            }

            _isApplyingRelations = true;

            try
            {
                NotableSupportConsoleCommands
                    .ApplyRelationPolicyBatch(
                        _nonPlayerRelationCap,
                        _fallbackNotableIndex,
                        FallbackSweepHours,
                        out _fallbackNotableIndex,
                        out int processedNotables,
                        out int totalNotables);
            }
            finally
            {
                _isApplyingRelations = false;
            }
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
            if (_isApplyingRelations ||
                !_relationsLocked ||
                Campaign.Current == null)
            {
                return;
            }

            Hero firstHero =
                originalHero ??
                effectiveHero;

            Hero secondHero =
                originalGainedRelationWith ??
                effectiveHeroGainedRelationWith;

            if (firstHero == null ||
                secondHero == null)
            {
                return;
            }

            int maxRelation =
                NotableSupportConsoleCommands.GetMaxRelation();

            if (!NotableSupportConsoleCommands
                    .TryGetManagedRelationRule(
                        firstHero,
                        secondHero,
                        _nonPlayerRelationCap,
                        maxRelation,
                        out int configuredValue,
                        out RelationRuleMode mode,
                        out int priority))
            {
                return;
            }

            _isApplyingRelations = true;

            try
            {
                NotableSupportConsoleCommands
                    .EnforceManagedPair(
                        firstHero,
                        secondHero,
                        _nonPlayerRelationCap);
            }
            finally
            {
                _isApplyingRelations = false;
            }
        }

        private RelationPolicyResult ApplyRelations()
        {
            if (_isApplyingRelations ||
                Campaign.Current == null ||
                Hero.MainHero == null ||
                Clan.PlayerClan == null)
            {
                return
                    new RelationPolicyResult(
                        0,
                        0);
            }

            _isApplyingRelations = true;

            try
            {
                return
                    NotableSupportConsoleCommands
                        .ApplyRelationPolicy(
                            _nonPlayerRelationCap);
            }
            finally
            {
                _isApplyingRelations = false;
            }
        }
    }
}

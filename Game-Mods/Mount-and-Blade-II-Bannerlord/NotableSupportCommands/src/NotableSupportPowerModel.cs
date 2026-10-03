using TaleWorlds.CampaignSystem;
using TaleWorlds.CampaignSystem.ComponentInterfaces;
using TaleWorlds.CampaignSystem.GameComponents;
using TaleWorlds.Localization;

namespace NotableSupportCommands
{
    public sealed class NotableSupportPowerModel :
        NotablePowerModel
    {
        private readonly DefaultNotablePowerModel _fallbackModel =
            new DefaultNotablePowerModel();

        private NotablePowerModel PreviousModel
        {
            get
            {
                return BaseModel ?? _fallbackModel;
            }
        }

        public override int RegularNotableMaxPowerLevel
        {
            get
            {
                return PreviousModel.RegularNotableMaxPowerLevel;
            }
        }

        public override int NotableDisappearPowerLimit
        {
            get
            {
                return PreviousModel.NotableDisappearPowerLimit;
            }
        }

        public override ExplainedNumber CalculateDailyPowerChangeForHero(
            Hero hero,
            bool includeDescriptions = false)
        {
            return PreviousModel.CalculateDailyPowerChangeForHero(
                hero,
                includeDescriptions);
        }

        public override TextObject GetPowerRankName(
            Hero hero)
        {
            return PreviousModel.GetPowerRankName(hero);
        }

        public override float GetInfluenceBonusToClan(
            Hero hero)
        {
            return PreviousModel.GetInfluenceBonusToClan(hero);
        }

        public override int GetInitialPower(
            Hero hero)
        {
            return PreviousModel.GetInitialPower(hero);
        }

        public override int GetInitialNotableSupporterCost(
            Hero hero)
        {
            NotableSupportBehavior behavior =
                NotableSupportBehavior.Current;

            if (behavior != null &&
                behavior.SupportCostLocked)
            {
                return behavior.SupportCost;
            }

            return PreviousModel.GetInitialNotableSupporterCost(
                hero);
        }
    }
}

using TaleWorlds.CampaignSystem;
using TaleWorlds.Core;
using TaleWorlds.MountAndBlade;

namespace RelationCommands
{
    public sealed class SubModule : MBSubModuleBase
    {
        protected override void OnGameStart(
            Game game,
            IGameStarter gameStarter)
        {
            base.OnGameStart(
                game,
                gameStarter);

            if (game.GameType is Campaign &&
                gameStarter is CampaignGameStarter campaignStarter)
            {
                campaignStarter.AddBehavior(
                    new RelationLockBehavior());
            }
        }
    }
}

# OP All — `descr_strat.txt` Character Packages

These lines are generated from the current `export_descr_character_traits.txt` OP_ALL creation/coming-of-age triggers and the curated ancillary packages in `export_descr_ancillaries.txt`.

Use the character's **actual culture**. Trait values in `descr_strat.txt` are trait **levels**, not threshold points. The ancillary package uses the non-Barbarian variant for Roman, Carthaginian, Eastern, Egyptian, and Greek characters, and the Barbarian variant for Barbarian characters.

For a staged test, replace an existing character's old `traits` / `ancillaries` lines with the relevant pair below. Do not append a second `ancillaries` line: the game only supports eight retinue slots.

## Culture: `roman`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , PhilosophySkill 3 , GoodEngineer 3 , LogisticalSkill 3 , TacticalSkill 3 , StrategicSkill 3 , GoodInfantryGeneral 3 , GoodTaxman 3 , ArchitectSkill 3 , MathematicsSkill 3 , GoodAdministrator 3 , Sobriety 3 , HatesBarbarian 3 , PoeticSkill 3 , HatesCarthaginian 3 , HatesEastern 3 , Pious 1 , HatesEgyptian 3 , HatesGreek 3 , Austere 3 , Stoic 3 , RomanHero 5 , DeceiverVirtue 3 , Disciplinarian 3 , NaturalPhilosophySkill 3 , RhetoricSkill 3 , PlainRomanVirtue 3 , PoliticsSkill 3 , Rabblerouser 4 , InspiringSpeaker 3 , VictorRomanVirtue 3 , Secretive 3 , GamesFanRomanVice 2 , RacesFanRomanVice 2 , Prim 1 , RegainedEagle 2 , WellConnectedWife 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , chirugeon , decorated_hero , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , seamaster , priest_of_Neptune , Navigator
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , bodyguard , heroic_saviour , foodtaster , wrestler , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , honest_man , merchant , scribe_ancillary , spice_merchant
```

## Culture: `barbarian`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , VictorOthersVirtue 3 , Feck 4 , HatesRoman 3 , Brave 4 , HatesCarthaginian 3 , HatesEastern 3 , Pious 1 , HatesEgyptian 3 , HatesGreek 3 , Austere 3 , Stoic 3 , Warlord 4 , Berserker 2 , Drink 1
ancillaries Caravan_Driver , scout , bodyguard , priest_of_Andrasta , priest_of_Brigantia , priest_of_Woden , famous_warrior , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4
ancillaries Caravan_Driver , scout , bodyguard , heroic_saviour , seamaster , priest_of_Neptune , shipwright , priest_of_Andrasta
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4
ancillaries Caravan_Driver , priest_of_Andrasta , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4
ancillaries Caravan_Driver , priest_of_Andrasta , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4
ancillaries Caravan_Driver , priest_of_Andrasta , scout , bodyguard , heroic_saviour , foodtaster , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4
ancillaries Caravan_Driver , priest_of_Andrasta , scout , bodyguard , honest_man , merchant , spice_merchant , priest_of_Britannia2
```

## Culture: `carthaginian`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , VictorOthersVirtue 3 , Feck 4 , HatesRoman 3 , Brave 4 , PhilosophySkill 3 , GoodEngineer 3 , LogisticalSkill 3 , TacticalSkill 3 , StrategicSkill 3 , GoodInfantryGeneral 3 , GoodTaxman 3 , ArchitectSkill 3 , MathematicsSkill 3 , GoodAdministrator 3 , Sobriety 3 , HatesBarbarian 3 , PoeticSkill 3 , HatesEastern 3 , Pious 1 , HatesEgyptian 3 , HatesGreek 3 , Austere 3 , Stoic 3 , GoodCavalryGeneral 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , chirugeon , decorated_hero , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , seamaster , priest_of_Neptune , Navigator
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , bodyguard , heroic_saviour , foodtaster , wrestler , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , honest_man , merchant , scribe_ancillary , spice_merchant
```

## Culture: `eastern`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , VictorOthersVirtue 3 , Feck 4 , HatesRoman 3 , Brave 4 , PhilosophySkill 3 , GoodEngineer 3 , LogisticalSkill 3 , TacticalSkill 3 , StrategicSkill 3 , GoodInfantryGeneral 3 , GoodTaxman 3 , ArchitectSkill 3 , MathematicsSkill 3 , GoodAdministrator 3 , Sobriety 3 , HatesBarbarian 3 , PoeticSkill 3 , HatesCarthaginian 3 , HatesEgyptian 3 , HatesGreek 3 , Epicurean 3 , GoodCavalryGeneral 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , chirugeon , decorated_hero , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , seamaster , priest_of_Neptune , Navigator
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , bodyguard , heroic_saviour , foodtaster , wrestler , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , honest_man , merchant , scribe_ancillary , spice_merchant
```

## Culture: `egyptian`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , VictorOthersVirtue 3 , Feck 4 , HatesRoman 3 , Brave 4 , PhilosophySkill 3 , GoodEngineer 3 , LogisticalSkill 3 , TacticalSkill 3 , StrategicSkill 3 , GoodInfantryGeneral 3 , GoodTaxman 3 , ArchitectSkill 3 , MathematicsSkill 3 , GoodAdministrator 3 , Sobriety 3 , HatesBarbarian 3 , PoeticSkill 3 , HatesCarthaginian 3 , HatesEastern 3 , Pious 1 , HatesGreek 3 , Epicurean 3 , GoodCavalryGeneral 3 , NaturalPhilosophySkill 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , chirugeon , decorated_hero , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , seamaster , priest_of_Neptune , Navigator
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , bodyguard , heroic_saviour , foodtaster , wrestler , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4 , LogisticalSkill 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , honest_man , merchant , scribe_ancillary , spice_merchant
```

## Culture: `greek`

### General

```text
traits Upright 3 , Loyal 4 , BattleScarred 4 , AssassinMaster 3 , HaleAndHearty 3 , AssassinCatcher 3 , CounterSpy 3 , HatesSlave 3 , HighPersonalSecurity 3 , Xenophobia 3 , Intelligent 3 , GoodCommander 5 , GoodAttacker 5 , GoodDefender 5 , NaturalMilitarySkill 4 , GoodRiskyAttacker 3 , GoodRiskyDefender 3 , GoodSiegeAttacker 4 , GoodSiegeDefender 4 , Noctophilia 3 , GoodAmbusher 3 , NightBattleCapable 1 , Energetic 4 , GoodMiner 3 , GoodTrader 3 , GoodBuilder 3 , Just 3 , SpyMaster 3 , GoodFarmer 3 , Pragmatic 3 , Scout 3 , Fertile 3 , Cheapskate 3 , KindRuler 3 , PhlegmHumour 3 , Miserly 3 , Despoiler 3 , Genocide 3 , Paranoia 4 , Handsome 3 , PublicFaith 3 , NonAuthoritarian 3 , Bloodthirsty 1 , ReligiousMania 2 , Censor 1 , CapturedEagle 3 , Consul 1 , Praetor 1 , PontifexMaximus 1 , Aedile 1 , Quaestor 1 , FaithfulWife 1 , VictorOthersVirtue 3 , Feck 4 , HatesRoman 3 , Brave 4 , PhilosophySkill 3 , GoodEngineer 3 , LogisticalSkill 3 , TacticalSkill 3 , StrategicSkill 3 , GoodInfantryGeneral 3 , GoodTaxman 3 , ArchitectSkill 3 , MathematicsSkill 3 , GoodAdministrator 3 , Sobriety 3 , HatesBarbarian 3 , PoeticSkill 3 , HatesCarthaginian 3 , HatesEastern 3 , Pious 1 , HatesEgyptian 3 , Austere 3 , Stoic 3 , Disciplinarian 3 , GoodCavalryGeneral 3 , NaturalPhilosophySkill 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , chirugeon , decorated_hero , soothsayer
```

### Admiral

```text
traits HighPersonalSecurity 3 , Sailor 5 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , drillmaster , intrepid_explorer , scout , bodyguard , seamaster , priest_of_Neptune , Navigator
```

### Assassin

```text
traits HighPersonalSecurity 3 , GoodAssassin 5 , GoodConspirator 3 , NaturalAssassinSkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Spy

```text
traits HighPersonalSecurity 3 , GoodSpy 5 , GoodConspirator 3 , NaturalSpySkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , heroic_saviour , foodtaster , pet_lion , honest_man
```

### Diplomat

```text
traits HighPersonalSecurity 3 , SmoothTalker 3 , GoodDiplomat 5 , NaturalDiplomatSkill 3 , Energetic 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , bodyguard , heroic_saviour , foodtaster , wrestler , linguist , honest_man
```

### Merchant

```text
traits HighPersonalSecurity 3 , Monopolist 3 , GoodMerchant 4 , NaturalMerchantSkill 3 , SharedKnowledgeAllMinerals 1 , GlassCollector 1 , LegalDealer 2 , SecureMerchant 2 , SharedKnowledgeRarities 1 , WorldlyMerchant 2 , SpiritualMilieuGovernors 2 , SpiritualMilieuJustice 2 , SpiritualMilieuLaw 2 , SpiritualMilieuLeadership 2 , MerchantsGuildMember 1 , MerchantsGuildTrained 1 , PotteryCollector 1 , SharedKnowledgeAgriculturalGoods 1 , SharedKnowledgeBaseMinerals 1 , SharedKnowledgeBuildingSupplies 1 , SharedKnowledgeFabricsDyes 1 , SharedKnowledgeHomewares 1 , SharedKnowledgeLiveGoods 1 , SharedKnowledgePreciousMinerals 1 , Energetic 4 , SecurityMerchant 2 , SpiritualMilieuBattle 4 , SpiritualMilieuBattleforge 4 , SpiritualMilieuFarming 4 , SpiritualMilieuFertility 4 , SpiritualMilieuForge 4 , SpiritualMilieuFun 4 , SpiritualMilieuHealing 4 , SpiritualMilieuHorse 4 , SpiritualMilieuHorse2 4 , SpiritualMilieuHunting 4 , SpiritualMilieuLove 4 , SpiritualMilieuNaval 4 , SpiritualMilieuOneGod 4 , SpiritualMilieuVictory 4 , SpiritualMilieuViking 4 , SpiritualMilieuViolence 4 , LogisticalSkill 3 , Disciplinarian 3
ancillaries Caravan_Driver , intrepid_explorer , scout , bodyguard , honest_man , merchant , scribe_ancillary , spice_merchant
```

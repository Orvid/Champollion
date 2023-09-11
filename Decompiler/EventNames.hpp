#pragma once
#include <vector>
#include <string>

namespace Decompiler{
	namespace Skyrim {
        static const std::vector<std::string> NativeClasses ={};
		static const std::vector<std::string> EventNames = {
			"OnAnimationEvent", // ActiveMagicEffect
			"OnAnimationEventUnregistered", // ActiveMagicEffect
			"OnEffectFinish", // ActiveMagicEffect
			"OnEffectStart", // ActiveMagicEffect
			"OnGainLOS", // ActiveMagicEffect
			"OnLostLOS", // ActiveMagicEffect
			"OnUpdate", // ActiveMagicEffect
			"OnCombatStateChanged", // Actor
			"OnDeath", // Actor
			"OnDying", // Actor
			"OnEnterBleedout", // Actor
			"OnGetUp", // Actor
			"OnLocationChange", // Actor
			"OnLycanthropyStateChanged", // Actor
			"OnObjectEquipped", // Actor
			"OnObjectUnequipped", // Actor
			"OnPackageChange", // Actor
			"OnPackageEnd", // Actor
			"OnPackageStart", // Actor
			"OnPlayerBowShot", // Actor
			"OnPlayerFastTravelEnd", // Actor
			"OnPlayerLoadGame", // Actor
			"OnRaceSwitchComplete", // Actor
			"OnSit", // Actor
			"OnVampireFeed", // Actor
			"OnVampirismStateChanged", // Actor
			"OnAnimationEvent", // Alias
			"OnAnimationEventUnregistered", // Alias
			"OnGainLOS", // Alias
			"OnLostLOS", // Alias
			"OnReset", // Alias
			"OnUpdate", // Alias
			"OnAnimationEvent", // Form
			"OnAnimationEventUnregistered", // Form
			"OnGainLOS", // Form
			"OnLostLOS", // Form
			"OnSleepStart", // Form
			"OnSleepStop", // Form
			"OnTrackedStatsEvent", // Form
			"OnUpdate", // Form
			"OnUpdateGameTime", // Form
			"OnActivate", // ObjectReference
			"OnAttachedToCell", // ObjectReference
			"OnCellAttach", // ObjectReference
			"OnCellDetach", // ObjectReference
			"OnCellLoad", // ObjectReference
			"OnClose", // ObjectReference
			"OnContainerChanged", // ObjectReference
			"OnDestructionStageChanged", // ObjectReference
			"OnDetachedFromCell", // ObjectReference
			"OnEquipped", // ObjectReference
			"OnGrab", // ObjectReference
			"OnHit", // ObjectReference
			"OnItemAdded", // ObjectReference
			"OnItemRemoved", // ObjectReference
			"OnLoad", // ObjectReference
			"OnLockStateChanged", // ObjectReference
			"OnMagicEffectApply", // ObjectReference
			"OnOpen", // ObjectReference
			"OnRead", // ObjectReference
			"OnRelease", // ObjectReference
			"OnReset", // ObjectReference
			"OnSell", // ObjectReference
			"OnSpellCast", // ObjectReference
			"OnTranslationAlmostComplete", // ObjectReference
			"OnTranslationComplete", // ObjectReference
			"OnTranslationFailed", // ObjectReference
			"OnTrapHit", // ObjectReference
			"OnTrapHitStart", // ObjectReference
			"OnTrapHitStop", // ObjectReference
			"OnTrigger", // ObjectReference
			"OnTriggerEnter", // ObjectReference
			"OnTriggerLeave", // ObjectReference
			"OnUnequipped", // ObjectReference
			"OnUnload", // ObjectReference
			"OnWardHit", // ObjectReference
			"OnReset", // Quest
			"OnStoryActivateActor", // Quest
			"OnStoryAddToPlayer", // Quest
			"OnStoryArrest", // Quest
			"OnStoryAssaultActor", // Quest
			"OnStoryBribeNPC", // Quest
			"OnStoryCastMagic", // Quest
			"OnStoryChangeLocation", // Quest
			"OnStoryCraftItem", // Quest
			"OnStoryCrimeGold", // Quest
			"OnStoryCure", // Quest
			"OnStoryDialogue", // Quest
			"OnStoryDiscoverDeadBody", // Quest
			"OnStoryEscapeJail", // Quest
			"OnStoryFlatterNPC", // Quest
			"OnStoryHello", // Quest
			"OnStoryIncreaseLevel", // Quest
			"OnStoryIncreaseSkill", // Quest
			"OnStoryInfection", // Quest
			"OnStoryIntimidateNPC", // Quest
			"OnStoryJail", // Quest
			"OnStoryKillActor", // Quest
			"OnStoryNewVoicePower", // Quest
			"OnStoryPayFine", // Quest
			"OnStoryPickLock", // Quest
			"OnStoryPlayerGetsFavor", // Quest
			"OnStoryRelationshipChange", // Quest
			"OnStoryRemoveFromPlayer", // Quest
			"OnStoryScript", // Quest
			"OnStoryServedTime", // Quest
			"OnStoryTrespass", // Quest
			"OnBeginState",
			"OnEndState",
			"OnInit"
		};
        static const std::vector<std::string> EventNamesLowerCase = {
			"onanimationevent", // ActiveMagicEffect
			"onanimationeventunregistered", // ActiveMagicEffect
			"oneffectfinish", // ActiveMagicEffect
			"oneffectstart", // ActiveMagicEffect
			"ongainlos", // ActiveMagicEffect
			"onlostlos", // ActiveMagicEffect
			"onupdate", // ActiveMagicEffect
			"oncombatstatechanged", // Actor
			"ondeath", // Actor
			"ondying", // Actor
			"onenterbleedout", // Actor
			"ongetup", // Actor
			"onlocationchange", // Actor
			"onlycanthropystatechanged", // Actor
			"onobjectequipped", // Actor
			"onobjectunequipped", // Actor
			"onpackagechange", // Actor
			"onpackageend", // Actor
			"onpackagestart", // Actor
			"onplayerbowshot", // Actor
			"onplayerfasttravelend", // Actor
			"onplayerloadgame", // Actor
			"onraceswitchcomplete", // Actor
			"onsit", // Actor
			"onvampirefeed", // Actor
			"onvampirismstatechanged", // Actor
			"onanimationevent", // Alias
			"onanimationeventunregistered", // Alias
			"ongainlos", // Alias
			"onlostlos", // Alias
			"onreset", // Alias
			"onupdate", // Alias
			"onanimationevent", // Form
			"onanimationeventunregistered", // Form
			"ongainlos", // Form
			"onlostlos", // Form
			"onsleepstart", // Form
			"onsleepstop", // Form
			"ontrackedstatsevent", // Form
			"onupdate", // Form
			"onupdategametime", // Form
			"onactivate", // ObjectReference
			"onattachedtocell", // ObjectReference
			"oncellattach", // ObjectReference
			"oncelldetach", // ObjectReference
			"oncellload", // ObjectReference
			"onclose", // ObjectReference
			"oncontainerchanged", // ObjectReference
			"ondestructionstagechanged", // ObjectReference
			"ondetachedfromcell", // ObjectReference
			"onequipped", // ObjectReference
			"ongrab", // ObjectReference
			"onhit", // ObjectReference
			"onitemadded", // ObjectReference
			"onitemremoved", // ObjectReference
			"onload", // ObjectReference
			"onlockstatechanged", // ObjectReference
			"onmagiceffectapply", // ObjectReference
			"onopen", // ObjectReference
			"onread", // ObjectReference
			"onrelease", // ObjectReference
			"onreset", // ObjectReference
			"onsell", // ObjectReference
			"onspellcast", // ObjectReference
			"ontranslationalmostcomplete", // ObjectReference
			"ontranslationcomplete", // ObjectReference
			"ontranslationfailed", // ObjectReference
			"ontraphit", // ObjectReference
			"ontraphitstart", // ObjectReference
			"ontraphitstop", // ObjectReference
			"ontrigger", // ObjectReference
			"ontriggerenter", // ObjectReference
			"ontriggerleave", // ObjectReference
			"onunequipped", // ObjectReference
			"onunload", // ObjectReference
			"onwardhit", // ObjectReference
			"onreset", // Quest
			"onstoryactivateactor", // Quest
			"onstoryaddtoplayer", // Quest
			"onstoryarrest", // Quest
			"onstoryassaultactor", // Quest
			"onstorybribenpc", // Quest
			"onstorycastmagic", // Quest
			"onstorychangelocation", // Quest
			"onstorycraftitem", // Quest
			"onstorycrimegold", // Quest
			"onstorycure", // Quest
			"onstorydialogue", // Quest
			"onstorydiscoverdeadbody", // Quest
			"onstoryescapejail", // Quest
			"onstoryflatternpc", // Quest
			"onstoryhello", // Quest
			"onstoryincreaselevel", // Quest
			"onstoryincreaseskill", // Quest
			"onstoryinfection", // Quest
			"onstoryintimidatenpc", // Quest
			"onstoryjail", // Quest
			"onstorykillactor", // Quest
			"onstorynewvoicepower", // Quest
			"onstorypayfine", // Quest
			"onstorypicklock", // Quest
			"onstoryplayergetsfavor", // Quest
			"onstoryrelationshipchange", // Quest
			"onstoryremovefromplayer", // Quest
			"onstoryscript", // Quest
			"onstoryservedtime", // Quest
			"onstorytrespass", // Quest
			"onbeginstate",
			"onendstate",
			"oninit"
		};
	}
	namespace Fallout4{
		static const std::vector<std::string> EventNames = {
			"OnEffectFinish", // ActiveMagicEffect
			"OnEffectStart", // ActiveMagicEffect
			"OnCombatStateChanged", // Actor
			"OnCommandModeCompleteCommand", // Actor
			"OnCommandModeEnter", // Actor
			"OnCommandModeExit", // Actor
			"OnCommandModeGiveCommand", // Actor
			"OnCompanionDismiss", // Actor
			"OnConsciousnessStateChanged", // Actor
			"OnCripple", // Actor
			"OnDeath", // Actor
			"OnDeferredKill", // Actor
			"OnDifficultyChanged", // Actor
			"OnDying", // Actor
			"OnEnterBleedout", // Actor
			"OnEnterSneaking", // Actor
			"OnEscortWaitStart", // Actor
			"OnEscortWaitStop", // Actor
			"OnGetUp", // Actor
			"OnItemEquipped", // Actor
			"OnItemUnequipped", // Actor
			"OnKill", // Actor
			"OnLocationChange", // Actor
			"OnPackageChange", // Actor
			"OnPackageEnd", // Actor
			"OnPackageStart", // Actor
			"OnPartialCripple", // Actor
			"OnPickpocketFailed", // Actor
			"OnPlayerCreateRobot", // Actor
			"OnPlayerEnterVertibird", // Actor
			"OnPlayerFallLongDistance", // Actor
			"OnPlayerFireWeapon", // Actor
			"OnPlayerHealTeammate", // Actor
			"OnPlayerLoadGame", // Actor
			"OnPlayerModArmorWeapon", // Actor
			"OnPlayerModRobot", // Actor
			"OnPlayerSwimming", // Actor
			"OnPlayerUseWorkBench", // Actor
			"OnRaceSwitchComplete", // Actor
			"OnSit", // Actor
			"OnSpeechChallengeAvailable", // Actor
			"OnAliasInit", // Alias
			"OnAliasReset", // Alias
			"OnAliasShutdown", // Alias
			"OnLocationCleared", // Location
			"OnLocationLoaded", // Location
			"OnActivate", // ObjectReference
			"OnCellAttach", // ObjectReference
			"OnCellDetach", // ObjectReference
			"OnCellLoad", // ObjectReference
			"OnClose", // ObjectReference
			"OnContainerChanged", // ObjectReference
			"OnDestructionStageChanged", // ObjectReference
			"OnEquipped", // ObjectReference
			"OnExitFurniture", // ObjectReference
			"OnGrab", // ObjectReference
			"OnHolotapeChatter", // ObjectReference
			"OnHolotapePlay", // ObjectReference
			"OnItemAdded", // ObjectReference
			"OnItemRemoved", // ObjectReference
			"OnLoad", // ObjectReference
			"OnLockStateChanged", // ObjectReference
			"OnOpen", // ObjectReference
			"OnPipboyRadioDetection", // ObjectReference
			"OnPlayerDialogueTarget", // ObjectReference
			"OnPowerOff", // ObjectReference
			"OnPowerOn", // ObjectReference
			"OnRead", // ObjectReference
			"OnRelease", // ObjectReference
			"OnReset", // ObjectReference
			"OnSell", // ObjectReference
			"OnSpellCast", // ObjectReference
			"OnTranslationAlmostComplete", // ObjectReference
			"OnTranslationComplete", // ObjectReference
			"OnTranslationFailed", // ObjectReference
			"OnTrapHitStart", // ObjectReference
			"OnTrapHitStop", // ObjectReference
			"OnTriggerEnter", // ObjectReference
			"OnTriggerLeave", // ObjectReference
			"OnUnequipped", // ObjectReference
			"OnUnload", // ObjectReference
			"OnWorkshopMode", // ObjectReference
			"OnWorkshopNPCTransfer", // ObjectReference
			"OnWorkshopObjectDestroyed", // ObjectReference
			"OnWorkshopObjectGrabbed", // ObjectReference
			"OnWorkshopObjectMoved", // ObjectReference
			"OnWorkshopObjectPlaced", // ObjectReference
			"OnWorkshopObjectRepaired", // ObjectReference
			"OnChange", // Package
			"OnEnd", // Package
			"OnStart", // Package
			"OnEntryRun", // Perk
			"OnQuestInit", // Quest
			"OnQuestShutdown", // Quest
			"OnStageSet", // Quest
			"OnStoryIncreaseLevel", // Quest
			"OnAction", // Scene
			"OnBegin", // Scene
			"OnEnd", // Scene
			"OnPhaseBegin", // Scene
			"OnPhaseEnd", // Scene
			"OnAnimationEvent", // ScriptObject
			"OnAnimationEventUnregistered", // ScriptObject
			"OnBeginState", // ScriptObject
			"OnControlDown", // ScriptObject
			"OnControlUp", // ScriptObject
			"OnDistanceGreaterThan", // ScriptObject
			"OnDistanceLessThan", // ScriptObject
			"OnEndState", // ScriptObject
			"OnFurnitureEvent", // ScriptObject
			"OnGainLOS", // ScriptObject
			"OnHit", // ScriptObject
			"OnInit", // ScriptObject
			"OnKeyDown", // ScriptObject
			"OnKeyUp", // ScriptObject
			"OnLooksMenuEvent", // ScriptObject
			"OnLostLOS", // ScriptObject
			"OnMagicEffectApply", // ScriptObject
			"OnMenuOpenCloseEvent", // ScriptObject
			"OnPlayerCameraState", // ScriptObject
			"OnPlayerSleepStart", // ScriptObject
			"OnPlayerSleepStop", // ScriptObject
			"OnPlayerTeleport", // ScriptObject
			"OnPlayerWaitStart", // ScriptObject
			"OnPlayerWaitStop", // ScriptObject
			"OnRadiationDamage", // ScriptObject
			"OnTimer", // ScriptObject
			"OnTimerGameTime", // ScriptObject
			"OnTrackedStatsEvent", // ScriptObject
			"OnTutorialEvent", // ScriptObject
			"OnMenuItemRun", // Terminal
			"OnBegin", // TopicInfo
			"OnEnd", // TopicInfo
        };

		static const std::vector<std::string> EventNamesLowerCase = {
			"oneffectfinish", // ActiveMagicEffect
			"oneffectstart", // ActiveMagicEffect
			"oncombatstatechanged", // Actor
			"oncommandmodecompletecommand", // Actor
			"oncommandmodeenter", // Actor
			"oncommandmodeexit", // Actor
			"oncommandmodegivecommand", // Actor
			"oncompaniondismiss", // Actor
			"onconsciousnessstatechanged", // Actor
			"oncripple", // Actor
			"ondeath", // Actor
			"ondeferredkill", // Actor
			"ondifficultychanged", // Actor
			"ondying", // Actor
			"onenterbleedout", // Actor
			"onentersneaking", // Actor
			"onescortwaitstart", // Actor
			"onescortwaitstop", // Actor
			"ongetup", // Actor
			"onitemequipped", // Actor
			"onitemunequipped", // Actor
			"onkill", // Actor
			"onlocationchange", // Actor
			"onpackagechange", // Actor
			"onpackageend", // Actor
			"onpackagestart", // Actor
			"onpartialcripple", // Actor
			"onpickpocketfailed", // Actor
			"onplayercreaterobot", // Actor
			"onplayerentervertibird", // Actor
			"onplayerfalllongdistance", // Actor
			"onplayerfireweapon", // Actor
			"onplayerhealteammate", // Actor
			"onplayerloadgame", // Actor
			"onplayermodarmorweapon", // Actor
			"onplayermodrobot", // Actor
			"onplayerswimming", // Actor
			"onplayeruseworkbench", // Actor
			"onraceswitchcomplete", // Actor
			"onsit", // Actor
			"onspeechchallengeavailable", // Actor
			"onaliasinit", // Alias
			"onaliasreset", // Alias
			"onaliasshutdown", // Alias
			"onlocationcleared", // Location
			"onlocationloaded", // Location
			"onactivate", // ObjectReference
			"oncellattach", // ObjectReference
			"oncelldetach", // ObjectReference
			"oncellload", // ObjectReference
			"onclose", // ObjectReference
			"oncontainerchanged", // ObjectReference
			"ondestructionstagechanged", // ObjectReference
			"onequipped", // ObjectReference
			"onexitfurniture", // ObjectReference
			"ongrab", // ObjectReference
			"onholotapechatter", // ObjectReference
			"onholotapeplay", // ObjectReference
			"onitemadded", // ObjectReference
			"onitemremoved", // ObjectReference
			"onload", // ObjectReference
			"onlockstatechanged", // ObjectReference
			"onopen", // ObjectReference
			"onpipboyradiodetection", // ObjectReference
			"onplayerdialoguetarget", // ObjectReference
			"onpoweroff", // ObjectReference
			"onpoweron", // ObjectReference
			"onread", // ObjectReference
			"onrelease", // ObjectReference
			"onreset", // ObjectReference
			"onsell", // ObjectReference
			"onspellcast", // ObjectReference
			"ontranslationalmostcomplete", // ObjectReference
			"ontranslationcomplete", // ObjectReference
			"ontranslationfailed", // ObjectReference
			"ontraphitstart", // ObjectReference
			"ontraphitstop", // ObjectReference
			"ontriggerenter", // ObjectReference
			"ontriggerleave", // ObjectReference
			"onunequipped", // ObjectReference
			"onunload", // ObjectReference
			"onworkshopmode", // ObjectReference
			"onworkshopnpctransfer", // ObjectReference
			"onworkshopobjectdestroyed", // ObjectReference
			"onworkshopobjectgrabbed", // ObjectReference
			"onworkshopobjectmoved", // ObjectReference
			"onworkshopobjectplaced", // ObjectReference
			"onworkshopobjectrepaired", // ObjectReference
			"onchange", // Package
			"onend", // Package
			"onstart", // Package
			"onentryrun", // Perk
			"onquestinit", // Quest
			"onquestshutdown", // Quest
			"onstageset", // Quest
			"onstoryincreaselevel", // Quest
			"onaction", // Scene
			"onbegin", // Scene
			"onend", // Scene
			"onphasebegin", // Scene
			"onphaseend", // Scene
			"onanimationevent", // ScriptObject
			"onanimationeventunregistered", // ScriptObject
			"onbeginstate", // ScriptObject
			"oncontroldown", // ScriptObject
			"oncontrolup", // ScriptObject
			"ondistancegreaterthan", // ScriptObject
			"ondistancelessthan", // ScriptObject
			"onendstate", // ScriptObject
			"onfurnitureevent", // ScriptObject
			"ongainlos", // ScriptObject
			"onhit", // ScriptObject
			"oninit", // ScriptObject
			"onkeydown", // ScriptObject
			"onkeyup", // ScriptObject
			"onlooksmenuevent", // ScriptObject
			"onlostlos", // ScriptObject
			"onmagiceffectapply", // ScriptObject
			"onmenuopencloseevent", // ScriptObject
			"onplayercamerastate", // ScriptObject
			"onplayersleepstart", // ScriptObject
			"onplayersleepstop", // ScriptObject
			"onplayerteleport", // ScriptObject
			"onplayerwaitstart", // ScriptObject
			"onplayerwaitstop", // ScriptObject
			"onradiationdamage", // ScriptObject
			"ontimer", // ScriptObject
			"ontimergametime", // ScriptObject
			"ontrackedstatsevent", // ScriptObject
			"ontutorialevent", // ScriptObject
			"onmenuitemrun", // Terminal
			"onbegin", // TopicInfo
			"onend", // TopicInfo
		};
	}
    namespace Starfield {
        static const std::vector<std::string> NativeClasses = {
                //Forms
                "Action",
                "Activator",
                "ActiveMagicEffect",
                "Actor",
                "ActorBase",
                "ActorValue",
                "AffinityEvent",
                "Alias",
                "Ammo",
                "Armor",
                "AssociationType",
                "Book",
                "CameraShot",
                "Cell",
                "Challenge",
                "Class",
                "CombatStyle",
                "ConditionForm",
                "ConstructibleObject",
                "Container",
                "Curve",
                "Door",
                "EffectShader",
                "Enchantment",
                "Explosion",
                "Faction",
                "Flora",
                "Form",
                "FormList",
                "Furniture",
                "GlobalVariable",
                "Hazard",
                "HeadPart",
                "Idle",
                "IdleMarker",
                "ImageSpaceModifier",
                "ImpactDataSet",
                "Ingredient",
                "InputEnableLayer",
                "InstanceNamingRules",
                "Key",
                "Keyword",
                "LegendaryItem",
                "LeveledActor",
                "LeveledItem",
                "LeveledSpaceshipBase",
                "LeveledSpell",
                "Light",
                "Location",
                "LocationAlias",
                "LocationRefType",
                "MagicEffect",
                "Message",
                "MiscObject",
                "MovableStatic",
                "MusicType",
                "Note",
                "ObjectMod",
                "ObjectReference",
                "Outfit",
                "Package",
                "PackIn",
                "Perk",
                "Planet",
                "Potion",
                "Projectile",
                "Quest",
                "Race",
                "RefCollectionAlias",
                "ReferenceAlias",
                "ResearchProject",
                "Resource",
                "Scene",
                "Scroll",
                "ShaderParticleGeometry",
                "Shout",
                "SoulGem",
                "SpaceshipBase",
                "SpaceshipReference",
                "Spell",
                "Static",
                "TalkingActivator",
                "Terminal",
                "TerminalMenu",
                "TextureSet",
                "Topic",
                "TopicInfo",
                "VisualEffect",
                "VoiceType",
                "Weapon",
                "Weather",
                "WordOfPower",
                "WorldSpace",
                "WwiseEvent",
                // Non Form Natives
                "SpeechChallengeObject",
                "Game",
                "Math",
                "ScriptObject",
                "Utility",
        };
        static const std::vector<std::string> EventNames = {
                "OnAction",
                "OnActivate",
                "OnActorActivatedRef",
                "OnActorValueChanged",
                "OnActorValueGreaterThan",
                "OnActorValueLessThan",
                "OnAffinityEvent",
                "OnAffinityEventSent",
                "OnAliasChanged",
                "OnAliasInit",
                "OnAliasReset",
                "OnAliasShutdown",
                "OnAliasStarted",
                "OnAnimationEvent",
                "OnAnimationEventUnregistered",
                "OnBegin",
                "OnBeginState",
                "OnBuilderMenuSelect",
                "OnCellAttach",
                "OnCellDetach",
                "OnCellLoad",
                "OnChallengeCompleted",
                "OnChange",
                "OnClose",
                "OnCombatListAdded",
                "OnCombatListRemoved",
                "OnCombatStateChanged",
                "OnCommandModeCompleteCommand",
                "OnCommandModeEnter",
                "OnCommandModeExit",
                "OnCommandModeGiveCommand",
                "OnCompanionDismiss",
                "OnConsciousnessStateChanged",
                "OnContainerChanged",
                "OnCrewAssigned",
                "OnCrewDismissed",
                "OnCripple",
                "OnDeath",
                "OnDeferredKill",
                "OnDestroyed",
                "OnDestructionStageChanged",
                "OnDifficultyChanged",
                "OnDistanceGreaterThan",
                "OnDistanceLessThan",
                "OnDying",
                "OnEffectFinish",
                "OnEffectStart",
                "OnEnd",
                "OnEndState",
                "OnEnterBleedout",
                "OnEnterFurniture",
                "OnEnterOutpostBeaconMode",
                "OnEnterShipInterior",
                "OnEnterSneaking",
                "OnEntryRun",
                "OnEquipped",
                "OnEscortWaitStart",
                "OnEscortWaitStop",
                "OnExitFurniture",
                "OnExitShipInterior",
                "OnGainLOS",
                "OnGetUp",
                "OnGrab",
                "OnHit",
                "OnHomeShipSet",
                "OnInit",
                "OnItemAdded",
                "OnItemEquipped",
                "OnItemRemoved",
                "OnItemUnequipped",
                "OnKill",
                "OnLoad",
                "OnLocationChange",
                "OnLocationExplored",
                "OnLocationLoaded",
                "OnLockStateChanged",
                "OnLostLOS",
                "OnMagicEffectApply",
                "OnMapMarkerDiscovered",
                "OnMenuOpenCloseEvent",
                "OnMissionAccepted",
                "OnObjectDestroyed",
                "OnObjectRepaired",
                "OnOpen",
                "OnOutpostItemPowerOff",
                "OnOutpostItemPowerOn",
                "OnOutpostPlaced",
                "OnPackageChange",
                "OnPackageEnd",
                "OnPackageStart",
                "OnPartialCripple",
                "OnPhaseBegin",
                "OnPhaseEnd",
                "OnPickLock",
                "OnPickpocketFailed",
                "OnPipboyRadioDetection",
                "OnPlanetSiteSelectEvent",
                "OnPlayerArrested",
                "OnPlayerAssaultActor",
                "OnPlayerBuyShip",
                "OnPlayerCompleteResearch",
                "OnPlayerCraftItem",
                "OnPlayerCreateRobot",
                "OnPlayerCrimeGold",
                "OnPlayerDialogueTarget",
                "OnPlayerEnterVertibird",
                "OnPlayerFailedPlotRoute",
                "OnPlayerFallLongDistance",
                "OnPlayerFireWeapon",
                "OnPlayerFollowerWarp",
                "OnPlayerHealTeammate",
                "OnPlayerItemAdded",
                "OnPlayerJail",
                "OnPlayerLoadGame",
                "OnPlayerLoiteringBegin",
                "OnPlayerLoiteringEnd",
                "OnPlayerModArmorWeapon",
                "OnPlayerModifiedShip",
                "OnPlayerModRobot",
                "OnPlayerMurderActor",
                "OnPlayerPayFine",
                "OnPlayerPlanetSurveyComplete",
                "OnPlayerScannedObject",
                "OnPlayerSellShip",
                "OnPlayerSleepStart",
                "OnPlayerSleepStop",
                "OnPlayerSwimming",
                "OnPlayerTeleport",
                "OnPlayerTrespass",
                "OnPlayerUseWorkBench",
                "OnPlayerWaitStart",
                "OnPlayerWaitStop",
                "OnPowerOff",
                "OnPowerOn",
                "OnQuestInit",
                "OnQuestRejected",
                "OnQuestShutdown",
                "OnQuestStarted",
                "OnQuestTimerEnd",
                "OnQuestTimerMod",
                "OnQuestTimerPause",
                "OnQuestTimerResume",
                "OnQuestTimerStart",
                "OnQuickContainerOpened",
                "OnRaceSwitchComplete",
                "OnRadiationDamage",
                "OnRead",
                "OnRecoverFromBleedout",
                "OnRelease",
                "OnReset",
                "OnScanned",
                "OnSell",
                "OnShipBought",
                "OnShipDock",
                "OnShipFarTravel",
                "OnShipGravJump",
                "OnShipLanding",
                "OnShipRampDown",
                "OnShipRefueled",
                "OnShipScan",
                "OnShipSold",
                "OnShipSystemDamaged",
                "OnShipSystemPowerChange",
                "OnShipSystemRepaired",
                "OnShipTakeOff",
                "OnShipUndock",
                "OnSit",
                "OnSpeechChallengeAvailable",
                "OnSpeechChallengeCompletion",
                "OnSpellCast",
                "OnStageSet",
                "OnStarmapTargetSelectEvent",
                "OnStart",
                "OnStoryActivateActor",
                "OnStoryActorAttach",
                "OnStoryAddToPlayer",
                "OnStoryArrest",
                "OnStoryAssaultActor",
                "OnStoryAttractionObject",
                "OnStoryBribeNPC",
                "OnStoryCastMagic",
                "OnStoryChangeLocation",
                "OnStoryCraftItem",
                "OnStoryCrimeGold",
                "OnStoryCure",
                "OnStoryDialogue",
                "OnStoryDiscoverDeadBody",
                "OnStoryEscapeJail",
                "OnStoryExploredLocation",
                "OnStoryFlatterNPC",
                "OnStoryHackTerminal",
                "OnStoryHello",
                "OnStoryIncreaseLevel",
                "OnStoryInfection",
                "OnStoryIntimidateNPC",
                "OnStoryIronSights",
                "OnStoryJail",
                "OnStoryKillActor",
                "OnStoryLocationLoaded",
                "OnStoryMineExplosion",
                "OnStoryNewVoicePower",
                "OnStoryPayFine",
                "OnStoryPickLock",
                "OnStoryPickPocket",
                "OnStoryPiracyActor",
                "OnStoryPlayerGetsFavor",
                "OnStoryRelationshipChange",
                "OnStoryRemoveFromPlayer",
                "OnStoryScript",
                "OnStoryServedTime",
                "OnStoryShipDock",
                "OnStoryShipLanding",
                "OnStoryTrespass",
                "OnTerminalMenuEnter",
                "OnTerminalMenuItemRun",
                "OnTimer",
                "OnTimerGameTime",
                "OnTrackedStatsEvent",
                "OnTranslationAlmostComplete",
                "OnTranslationComplete",
                "OnTranslationFailed",
                "OnTrapHitStart",
                "OnTrapHitStop",
                "OnTriggerEnter",
                "OnTriggerLeave",
                "OnTutorialEvent",
                "OnUnequipped",
                "OnUnload",
                "OnWorkshopCargoLinkChanged",
                "OnWorkshopFlyCam",
                "OnWorkshopMode",
                "OnWorkshopNPCTransfer",
                "OnWorkshopObjectGrabbed",
                "OnWorkshopObjectMoved",
                "OnWorkshopObjectPlaced",
                "OnWorkshopObjectRemoved",
                "OnWorkshopOutputLink",
        };
        static const std::vector<std::string> EventNamesLowerCase = {
                "onaction",
                "onactivate",
                "onactoractivatedref",
                "onactorvaluechanged",
                "onactorvaluegreaterthan",
                "onactorvaluelessthan",
                "onaffinityevent",
                "onaffinityeventsent",
                "onaliaschanged",
                "onaliasinit",
                "onaliasreset",
                "onaliasshutdown",
                "onaliasstarted",
                "onanimationevent",
                "onanimationeventunregistered",
                "onbegin",
                "onbeginstate",
                "onbuildermenuselect",
                "oncellattach",
                "oncelldetach",
                "oncellload",
                "onchallengecompleted",
                "onchange",
                "onclose",
                "oncombatlistadded",
                "oncombatlistremoved",
                "oncombatstatechanged",
                "oncommandmodecompletecommand",
                "oncommandmodeenter",
                "oncommandmodeexit",
                "oncommandmodegivecommand",
                "oncompaniondismiss",
                "onconsciousnessstatechanged",
                "oncontainerchanged",
                "oncrewassigned",
                "oncrewdismissed",
                "oncripple",
                "ondeath",
                "ondeferredkill",
                "ondestroyed",
                "ondestructionstagechanged",
                "ondifficultychanged",
                "ondistancegreaterthan",
                "ondistancelessthan",
                "ondying",
                "oneffectfinish",
                "oneffectstart",
                "onend",
                "onendstate",
                "onenterbleedout",
                "onenterfurniture",
                "onenteroutpostbeaconmode",
                "onentershipinterior",
                "onentersneaking",
                "onentryrun",
                "onequipped",
                "onescortwaitstart",
                "onescortwaitstop",
                "onexitfurniture",
                "onexitshipinterior",
                "ongainlos",
                "ongetup",
                "ongrab",
                "onhit",
                "onhomeshipset",
                "oninit",
                "onitemadded",
                "onitemequipped",
                "onitemremoved",
                "onitemunequipped",
                "onkill",
                "onload",
                "onlocationchange",
                "onlocationexplored",
                "onlocationloaded",
                "onlockstatechanged",
                "onlostlos",
                "onmagiceffectapply",
                "onmapmarkerdiscovered",
                "onmenuopencloseevent",
                "onmissionaccepted",
                "onobjectdestroyed",
                "onobjectrepaired",
                "onopen",
                "onoutpostitempoweroff",
                "onoutpostitempoweron",
                "onoutpostplaced",
                "onpackagechange",
                "onpackageend",
                "onpackagestart",
                "onpartialcripple",
                "onphasebegin",
                "onphaseend",
                "onpicklock",
                "onpickpocketfailed",
                "onpipboyradiodetection",
                "onplanetsiteselectevent",
                "onplayerarrested",
                "onplayerassaultactor",
                "onplayerbuyship",
                "onplayercompleteresearch",
                "onplayercraftitem",
                "onplayercreaterobot",
                "onplayercrimegold",
                "onplayerdialoguetarget",
                "onplayerentervertibird",
                "onplayerfailedplotroute",
                "onplayerfalllongdistance",
                "onplayerfireweapon",
                "onplayerfollowerwarp",
                "onplayerhealteammate",
                "onplayeritemadded",
                "onplayerjail",
                "onplayerloadgame",
                "onplayerloiteringbegin",
                "onplayerloiteringend",
                "onplayermodarmorweapon",
                "onplayermodifiedship",
                "onplayermodrobot",
                "onplayermurderactor",
                "onplayerpayfine",
                "onplayerplanetsurveycomplete",
                "onplayerscannedobject",
                "onplayersellship",
                "onplayersleepstart",
                "onplayersleepstop",
                "onplayerswimming",
                "onplayerteleport",
                "onplayertrespass",
                "onplayeruseworkbench",
                "onplayerwaitstart",
                "onplayerwaitstop",
                "onpoweroff",
                "onpoweron",
                "onquestinit",
                "onquestrejected",
                "onquestshutdown",
                "onqueststarted",
                "onquesttimerend",
                "onquesttimermod",
                "onquesttimerpause",
                "onquesttimerresume",
                "onquesttimerstart",
                "onquickcontaineropened",
                "onraceswitchcomplete",
                "onradiationdamage",
                "onread",
                "onrecoverfrombleedout",
                "onrelease",
                "onreset",
                "onscanned",
                "onsell",
                "onshipbought",
                "onshipdock",
                "onshipfartravel",
                "onshipgravjump",
                "onshiplanding",
                "onshiprampdown",
                "onshiprefueled",
                "onshipscan",
                "onshipsold",
                "onshipsystemdamaged",
                "onshipsystempowerchange",
                "onshipsystemrepaired",
                "onshiptakeoff",
                "onshipundock",
                "onsit",
                "onspeechchallengeavailable",
                "onspeechchallengecompletion",
                "onspellcast",
                "onstageset",
                "onstarmaptargetselectevent",
                "onstart",
                "onstoryactivateactor",
                "onstoryactorattach",
                "onstoryaddtoplayer",
                "onstoryarrest",
                "onstoryassaultactor",
                "onstoryattractionobject",
                "onstorybribenpc",
                "onstorycastmagic",
                "onstorychangelocation",
                "onstorycraftitem",
                "onstorycrimegold",
                "onstorycure",
                "onstorydialogue",
                "onstorydiscoverdeadbody",
                "onstoryescapejail",
                "onstoryexploredlocation",
                "onstoryflatternpc",
                "onstoryhackterminal",
                "onstoryhello",
                "onstoryincreaselevel",
                "onstoryinfection",
                "onstoryintimidatenpc",
                "onstoryironsights",
                "onstoryjail",
                "onstorykillactor",
                "onstorylocationloaded",
                "onstorymineexplosion",
                "onstorynewvoicepower",
                "onstorypayfine",
                "onstorypicklock",
                "onstorypickpocket",
                "onstorypiracyactor",
                "onstoryplayergetsfavor",
                "onstoryrelationshipchange",
                "onstoryremovefromplayer",
                "onstoryscript",
                "onstoryservedtime",
                "onstoryshipdock",
                "onstoryshiplanding",
                "onstorytrespass",
                "onterminalmenuenter",
                "onterminalmenuitemrun",
                "ontimer",
                "ontimergametime",
                "ontrackedstatsevent",
                "ontranslationalmostcomplete",
                "ontranslationcomplete",
                "ontranslationfailed",
                "ontraphitstart",
                "ontraphitstop",
                "ontriggerenter",
                "ontriggerleave",
                "ontutorialevent",
                "onunequipped",
                "onunload",
                "onworkshopcargolinkchanged",
                "onworkshopflycam",
                "onworkshopmode",
                "onworkshopnpctransfer",
                "onworkshopobjectgrabbed",
                "onworkshopobjectmoved",
                "onworkshopobjectplaced",
                "onworkshopobjectremoved",
                "onworkshopoutputlink",
        };
    }
}
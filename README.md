# The_Explorer

プロジェクト概要
The Explorer は、Unreal Engine 5.8を使用して開発している一人称アクションゲームプロジェクトです。
キャラクター、入力、アニメーション、戦闘システムの基盤をC++で構築し、BlueprintとData Assetを利用して武器やGameplay Abilityを拡張できるように設計しています。
このREADMEでは、主にプロジェクトの Source モジュールについて説明します。ゲームの実行には、別途 Content、Config、Unreal Assetファイルが必要です。
主な担当範囲
- プレイヤーキャラクター構造の設計・実装
- True First Personカメラシステム
- 武装・非武装ロコモーション
- Turn In Placeシステム
- Gameplay Ability Systemを利用した戦闘構造
- 武器の装備・解除および武器別Ability管理
- Enhanced InputとGameplay Tagの連携
- Level Sequenceおよびシネマティック連携
- シネマティック中の入力・キャラクター回転競合の防止
主な機能
True First Person
- 全身キャラクターメッシュと一人称専用メッシュの分離
- Leader Pose Componentによるアニメーション同期
- スケルトンのCameraSocketにカメラピボットを接続
- 一人称メッシュの影とOwner Visibilityを個別に設定
キャラクター移動
- カメラ方向を基準にした移動
- 移動および視点操作
- ジャンプ
- 前進入力中のダッシュ
- 移動中のキャラクター方向補間
- シネマティック中の移動・回転状態制御
アニメーション
- 武装・非武装状態の分離
- 移動方向と速度を利用したロコモーション
- Turn In Place
- 武装中のカメラ方向追従
- 上半身のAim Yaw/Pitch計算
- 武器別Linked Animation Layer
- Foot IK用の足・骨盤オフセット構造
- 一定時間静止した場合のRelax状態遷移
戦闘・武器
- Gameplay Tagを利用した武器登録
- 現在装備中の武器管理
- 武器の装備・解除
- 武器別Input Mapping Context
- 武器別Gameplay Abilityの付与・削除
- 武器別Linked Animation Layerの適用
- 銃器の基本攻撃Abilityを拡張できる構造
Gameplay Ability System
- Ability System ComponentとAttribute Set
- Input TagによるAbilityの発動
- StartUp Data Assetによる初期Ability付与
- 武器装備時の専用Ability付与
- 武器解除時のAbility削除
- OnTriggered、OnGivenの発動ポリシー
システムフロー
Enhanced Input
    ↓
DataAsset_InputConfig
    ↓
Gameplay Tag
    ↓
PlayerInputComponent
    ↓
PlayerAbilitySystemComponent
    ↓
Gameplay Ability
    ↓
HeroCombatComponent / PlayerHeroWeaponBase
武器データの流れ:
PlayerHeroWeaponBase
    ├─ Weapon Input Mapping Context
    ├─ Weapon Linked Animation Layer
    └─ Default Weapon Abilities
            ↓
    Ability System Componentへ登録
Input Tag
実際のキー設定は、Contentフォルダ内のEnhanced Input Assetで管理します。
Gameplay Tag	機能
InputTag.Move	キャラクター移動
InputTag.Look	カメラ操作
InputTag.Jump	ジャンプ
InputTag.Dash	ダッシュ
InputTag.EquipGun	銃器装備
InputTag.UnequipGun	銃器解除
InputTag.LightAttackGun	銃器の基本攻撃


ディレクトリ構成
Source/
├─ game_explore.Target.cs
├─ game_exploreEditor.Target.cs
└─ game_explore/
   ├─ game_explore.Build.cs
   ├─ Public/
   │  ├─ AbilitySystem/
   │  │  ├─ Abilities/
   │  │  ├─ PlayerAbilitySystemComponent.h
   │  │  └─ PlayerAttributeSet.h
   │  ├─ AnimInstances/
   │  │  └─ Hero/
   │  ├─ Characters/
   │  ├─ Components/
   │  │  ├─ Combat/
   │  │  └─ Input/
   │  ├─ Controllers/
   │  ├─ DataAssets/
   │  │  ├─ Input/
   │  │  └─ StartUpData/
   │  ├─ GameModes/
   │  ├─ HeroTypes/
   │  └─ Items/
   │     └─ Weapons/
   └─ Private/
      └─ Publicクラスの実装
主要クラス
クラス	役割
APlayerBaseCharacter	ASCとAttribute Setを所有する基本キャラクター
APlayerHeroCharacter	移動、カメラ、ダッシュ、Turn In Place、シネマティック処理
UPlayerAbilitySystemComponent	入力によるAbility発動と武器Ability管理
UPlayerHeroAnimInstance	ロコモーション、Aim、武装状態、Turn In Placeの値を計算
UPlayerHeroLinkedAnimLayer	武器別Animation Layerの接続
UPawnCombatComponent	武器登録と現在装備中の武器管理
UHeroCombatComponent	プレイヤー専用の武器取得機能
APlayerHeroWeaponBase	武器データと付与済みAbility Handleを管理
UDataAsset_InputConfig	Input ActionとGameplay Tagを接続
UDataAsset_HeroStartUpData	キャラクターの初期Ability設定


開発環境
- Unreal Engine 5.8
- Windows 64-bit
- Visual Studio 2022
- C++
- Blueprint
- Gameplay Ability System
- Gameplay Tags
- Enhanced Input
- Gameplay Tasks
主な有効プラグイン:
- GameplayAbilities
- PCG
- Niagara Fluids
- HDRI Backdrop
- Datasmith Importers
ビルド方法
1. Unreal Engine 5.8をインストールします。
2. Visual Studio 2022でGame development with C++ワークロードをインストールします。
3. .uprojectファイルを右クリックします。
4. Generate Visual Studio project filesを実行します。
5. 生成されたSolutionをVisual Studioで開きます。
6. 以下の構成でビルドします。
Configuration: Development Editor
Platform: Win64
Target: game_exploreEditor
7. ビルド完了後、.uprojectファイルを起動します。
パッケージング
配布用ビルドでは、以下の設定を推奨します。
Build Configuration: Shipping
Full Rebuild: ON
Include app-local prerequisites: ON
パッケージング後は、最上位の実行ファイルだけではなく、生成されたパッケージフォルダ全体を配布してください。
開発上の注意事項
- 実際のキーバインドはEnhanced Input Assetで設定します。
- Blueprint Ability、Animation Blueprint、Input Action、Data AssetはContentフォルダにあります。
- Sourceフォルダだけではプロジェクトを実行できません。
- Ability入力のRelease処理は、今後拡張できる構造になっています。
- Turn In Placeの数値はキャラクターやアニメーションに合わせて調整が必要です。
- Shipping前にデバッグログとDraw Debug機能を確認してください。

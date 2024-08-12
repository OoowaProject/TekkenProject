// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_MH.generated.h"

/**
 * 
 */
// 게임 상태를 나타내는 Enum
UENUM(BlueprintType , Category="GameState")
enum class EGameState : uint8
{
	DefaultState UMETA(DisplayName = "Default State"),
	GameStart UMETA(DisplayName = "Game Start"),
	RoundStart UMETA(DisplayName = "Round Start"),
	InProgress UMETA(DisplayName = "In Progress"),
	RoundEnd UMETA(DisplayName = "Round End"),
	GameOver UMETA(DisplayName = "Game Over")
};

UCLASS()
class JMH_API AGameMode_MH : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameMode_MH();

	virtual void Tick(float DeltaTime) override;

	//CountDown
	UFUNCTION()
	void CountDown(float DeltaTime);
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	float initroundTimer  = 60.f;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	float gameTimer = initroundTimer;

protected:
	virtual void BeginPlay() override;


public: 

	UPROPERTY()
	class UMainGameInstance_MH* MainGameInstance;
	
	//widget
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TSubclassOf<class UinGameUI> inGameWidget;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UPlayerInfoUI* PlayerInfoUI ;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UinGameUI* inGameUI;


	//GM Player
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class ACPP_Tekken8CharacterParent* playerA;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class ACPP_Tekken8CharacterParent* playerB;

	//Player HP

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 player1MaxHP;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 player2MaxHP;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 player1HP = player1MaxHP;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 player2HP = player2MaxHP;

	// ** for make Character **//
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	TSubclassOf<class ACPP_CharacterPaul> Player1Class;

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	TSubclassOf<class ACPP_CharacterPaul> Player2Class;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	class ACPP_Tekken8CharacterParent* Player1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	class ACPP_Tekken8CharacterParent* Player2;

//AI
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	TSubclassOf<class AAICharacter> PlayerAIClass;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	class ACPP_Tekken8CharacterParent* PlayerAI1;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	class ACPP_Tekken8CharacterParent* PlayerAI2;

	//Camera
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	TSubclassOf<class APawn> CameraPawn;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	APawn* SpawnedCameraPawn;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	class APlayerCameraPawn* maincamera;

	//Round
	//player Round Score
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	int32 Player1Score = 0;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	int32 Player2Score = 0;

	//final Winner
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	AActor* Winner;

	//Round Num
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	int32 initRoundNum = 0;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Round")
	int32 CurrentRoundNum = initRoundNum;

	//playernum pual=1,kazuya=2,Jin=3
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 playerANum = 0; // pual=1,kazuya=2,Jin=3
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	int32 playerBNum = 0; // pual=1,kazuya=2,Jin=3
	
public:
	//State//____s____
	// 현재 게임 상태
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category="GameState")
	EGameState CurrentState = EGameState::DefaultState;

	// 게임 상태를 설정하는 함수
	UFUNCTION()
	void SetGameState(EGameState NewState);

	// 새로운 상태에 따라 행동을 처리하는 함수
	UFUNCTION()
	void HandleNewState(EGameState NewState);

	// 게임 시작 처리
	UFUNCTION()
	void StartGame();

	// 게임 시작 라운드 시작 처리
	UFUNCTION()
	void SetGameStart();

	UFUNCTION()
	void RoundStart();

	// 게임 오버 조건 체크
	UFUNCTION()
	void CheckForGameOver();

	// 게임 오버 조건을 만족하는지 체크하는 함수
	UFUNCTION()
	bool IsGameOverConditionMet();
	
	
	//State//____e____
public:	
	//Player 
	UFUNCTION()
	void CheckPlayerHP();

	bool bHasRoundEnded;
	bool bStartRound;

	FTimerHandle TimerHandle;
	
	UFUNCTION()
	int32 GetPlayerHP(ACharacter* Player);
	
	//Round
	//상태 초기화(플레이어 HP, 타이머 시간, 라운드 표시 반영)
	UFUNCTION()
	void ResetRoundState();

	UFUNCTION()
	void InitRoundState();
	
	UFUNCTION()
	void CheckRoundWinner();
	
	UFUNCTION()
	void CheckFinalWinner();

	UFUNCTION()
	void HandleRoundEnd(AActor* RoundWinner);
	
	UFUNCTION()
	void DisablePlayerInput();
	
	UFUNCTION()
	void EnablePlayerInput();

	UPROPERTY()
	APlayerController* PlayerAController;
	UPROPERTY()
	AController* PlayerBController;
	
	//Camera
	UFUNCTION()
	void SetupCameraViewTarget();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void UpdatePlayerHP(ACPP_Tekken8CharacterParent* Player,float NewHP);

	//inGameUi text visible 설정
	UFUNCTION(BlueprintCallable, Category = "Game")
	void UpdateTextVisibility(const FString& TextName, bool bVisible);

	//플레이어 초기위치 저장
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FVector initPlayerALoc;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FRotator initPlayerARot;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FVector initPlayerBLoc ; 

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FRotator initPlayerBRot;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FVector initCameraLoc;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FRotator initCameraRot;


	
	//게임인스턴스에서 가져오기
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 CharacterIndex; //1.paul,2.Kazuya

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 GameModeIndex; //1.PvsP,2.PvsCPU

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	bool IsLeftSide;//true.Left,false.Right
};

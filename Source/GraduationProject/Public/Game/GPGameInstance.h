// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GPGameInstance.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UGPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FORCEINLINE UDataTable* GetItemDataTabe(){return ItemDataTable;}
	FORCEINLINE UDataTable* GetLevelDataTable(){return LevelDataTable;}
	FORCEINLINE UDataTable* GetEnemyDataTable(){return EnemyDataTable;}
private:
	UPROPERTY(EditAnywhere)
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* LevelDataTable;
	UPROPERTY(EditAnywhere)
	UDataTable* EnemyDataTable;
};

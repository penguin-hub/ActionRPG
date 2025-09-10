// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	//HealthBar의 HealthBar 퍼센트 Set
	void SetHealthPercent(float Percent);
private:
	UPROPERTY()
	UHealthBar* HealthBarWidget;
};

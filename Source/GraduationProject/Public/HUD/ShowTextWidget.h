// C135203_GraduationProject
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowTextWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UShowTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextBlock;
};

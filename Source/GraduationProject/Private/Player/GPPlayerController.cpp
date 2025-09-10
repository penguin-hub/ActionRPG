// C135203_GraduationProject

#include "Player/GPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Character/GPCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"
#include "Camera/CameraComponent.h"
#include "Component/GPAttributeComponent.h"
#include "Component/InventorySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AGPPlayerController::AGPPlayerController()
{
	bReplicates = false;
}

void AGPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//컨트롤러가 소유하고 있는 캐릭터 저장
	SetHavingGPCharacter();
	//매핑 컨텍스트 추가, 플레이어 컨트롤러가 데이터 받아오기 위해
	AddMappingContextToController(GPContext);
}

void AGPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CameraToActorToLockOn();
}

//캐릭터를 움직이기 위한 동작
void AGPPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(CanNotMove())
		return;
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	MoveCharacterByInputAxisVector(InputAxisVector);
}

//카메라를 움직이기 위한 동작
void AGPPlayerController::Look_Mouse(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	RotateControllerByInputAxisVector(InputAxisVector);
}

//점프를 위한 동작
void AGPPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if(!HavingGPCharacter->GetIsFocus())
	{
		if(CanJump())
			HavingGPCharacter->Jump();
	}
	//Focus 중일 때는 Dodge Jump 실행
	else
	{
		if(CanJump())
		{
			//Stamina 가 Dodge 가능 할 때
			if(HavingGPCharacter->Attribute->GetStamina()>=5.f)
			{
				HavingGPCharacter->PlayDodgeMontageByAngle();
				HavingGPCharacter->Attribute->UseStamina(5.f);
				HavingGPCharacter->SetActionState(EActionState::Attacking);
			}
		}
	}
}

//달리기를 위한 동작
void AGPPlayerController::RunStart(const FInputActionValue& InputActionValue)
{
	//컨트롤하고 있는 캐릭터의 이동 속도를 조절해준다.
	if(CanRun())
	{
		HavingGPCharacter->GetCharacterMovement()->MaxWalkSpeed *= HavingGPCharacter->RunSpeed;
		HavingGPCharacter->IsRunnig = true;
	}
}

void AGPPlayerController::RunEnd(const FInputActionValue& InputActionValue)
{	
	if(CanStopRun())
	{
		HavingGPCharacter->GetCharacterMovement()->MaxWalkSpeed /= HavingGPCharacter->RunSpeed;
		HavingGPCharacter->IsRunnig = false;
	}
}

void AGPPlayerController::InterAct(const FInputActionValue& InputActionValue)
{
	if(CanInteract())
	{
		//Interact 가 가능한 아이템이면 ItemInteract 인터페이스 함수를 통해 상호작용을 한다.
		IItemInteractInterface* ItemCanInteract = Cast<IItemInteractInterface>(HavingGPCharacter->OverlappingItem);
		if(ItemCanInteract)
		{
			ItemCanInteract->ItemInteract(HavingGPCharacter);
		}
	}
	else
	{
		//무기를 손에 들고 있지 않지만 장비 중인 무기가 있을 때 등의 무기를 손에 든다.
		if(CanArm())
		{
			PlayMontageSection(ArmDisArmMontage, FName("Arm"));
			//무기를 손에 들었으므로 CharacterState 변경
			HavingGPCharacter->SetCharacterState(ECharacterState::EquippedWeapon);
			//ActionState ArmingDisArming 으로, 애니메이션 몽타주에서 ArmingDisArming이 끝날때 다시 바꾸어 줄 것
			HavingGPCharacter->SetActionState(EActionState::ArmingDisArming);
		}
		//무기를 손에 들고 있을 때는 등에 장착
		else if(CanDisArm())
		{
			PlayMontageSection(ArmDisArmMontage, FName("DisArm"));
			//무기를 등에 장착했으므로, CharacterState 변경
			HavingGPCharacter->SetCharacterState(ECharacterState::Unequipped);
			//ActionState ArmingDisArming 으로, 애니메이션 몽타주에서 ArmingDisArming이 끝날때 다시 바꾸어 줄 것
			HavingGPCharacter->SetActionState(EActionState::ArmingDisArming);
		}
	}
}

void AGPPlayerController::Attack(const FInputActionValue& InputActionValue)
{
	if(HavingGPCharacter && HavingGPCharacter->IsCanAttack())
	{
		//점프 중 일때 Attack
		if(HavingGPCharacter->GetCharacterMovement()->IsFalling() && HavingGPCharacter->GetActionState() != EActionState::Attacking)
		{
			HavingGPCharacter->SetActionState(EActionState::Attacking);
		}
		//러닝 중일 때 Attack
		else if(HavingGPCharacter->IsRunnig && HavingGPCharacter->GetActionState() != EActionState::Attacking)
		{
			PlayMontageSection(HavingGPCharacter->RunningAttack, FName("Default"));
			HavingGPCharacter->SetActionState(EActionState::Attacking);
		}
		else
		{
			HavingGPCharacter->Attacking();
		}
	}
}

void AGPPlayerController::Lock_On(const FInputActionValue& InputActionValue)
{
	//Focus 모드로 돌입
	HavingGPCharacter->SetIsFocus(true);
	HavingGPCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	HavingGPCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	if(HavingGPCharacter->GetCombatTarget())
	{
		ActorToLockOn = HavingGPCharacter->GetCombatTarget();
		return;
	}
	FHitResult SphereHitResult;
	DoSphereTraceToFindLockOnTarget(SphereHitResult);

	if(SphereHitResult.GetActor() && SphereHitResult.GetActor()->ActorHasTag(FName("GPEnemy")))
	{
		ActorToLockOn = SphereHitResult.GetActor();
	}
}

void AGPPlayerController::Lock_On_End(const FInputActionValue& InputActionValue)
{
	HavingGPCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	HavingGPCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	
	HavingGPCharacter->SetIsFocus(false);
	if(ActorToLockOn)
		ActorToLockOn = nullptr;
}

void AGPPlayerController::Crouch(const FInputActionValue& InputActionValue)
{
	if(HavingGPCharacter->GetActionState() == EActionState::Unoccupied && UKismetMathLibrary::VSizeXY(HavingGPCharacter->GetCharacterMovement()->Velocity) == 0.f)
	{
		DoCrouch();
	}
	else if(HavingGPCharacter->GetActionState() == EActionState::Crouching && UKismetMathLibrary::VSizeXY(HavingGPCharacter->GetCharacterMovement()->Velocity) == 0.f)
	{
		FHitResult SphereHitResult;
		SphereTraceToCheckCanUncrouch(SphereHitResult);
		//머리 위 무언가 있다면 return
		if(SphereHitResult.GetActor())
			return;
		
		DoUncrouch();
	}
}

void AGPPlayerController::InventoryMenu(const FInputActionValue& InputActionValue)
{
	if(HavingGPCharacter->PlayerHUD)
	{
		HavingGPCharacter->PlayerHUD->DisplayInventoryMenu();
		//HavingGPCharacter->Inventory->DebugShowContent();
	}
		
}

void AGPPlayerController::EquipmentMenu(const FInputActionValue& InputActionValue)
{
	if(HavingGPCharacter->PlayerHUD)
	{
		HavingGPCharacter->PlayerHUD->DisplayEquipmentMenu();
	}
}

void AGPPlayerController::Climbing(const FInputActionValue& InputActionValue)
{
}

void AGPPlayerController::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	if(HavingGPCharacter)
	{
		UAnimInstance* AnimInstance = HavingGPCharacter->GetMesh()->GetAnimInstance();
		if(AnimInstance && Montage)
		{
			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
		}
	}
}

void AGPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//각 인풋액션에 대응하는 함수 바인딩
	BindingFunctionToInputAction();
}

void AGPPlayerController::AddMappingContextToController(UInputMappingContext* InputMappingContext) const
{
	check(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(InputMappingContext, 0);
}

void AGPPlayerController::BindingFunctionToInputAction()
{
	//플레이어컨트롤러의 변수 InputComponent 를 위한 Enhanced Input 을 위한 변수로 변경
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//Move 함수가 MoveAction 에 바인딩 됨
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGPPlayerController::Move);
	//Look_Mouse 함수가 Look_Mouse_Action 에 바인딩 됨
	EnhancedInputComponent->BindAction(Look_Mouse_Action, ETriggerEvent::Triggered, this, &AGPPlayerController::Look_Mouse);
	//Jump 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(Jump_Action, ETriggerEvent::Started, this, &AGPPlayerController::Jump);
	//Run 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(Run_Action, ETriggerEvent::Started, this, &AGPPlayerController::RunStart);
	EnhancedInputComponent->BindAction(Run_Action, ETriggerEvent::Completed, this, &AGPPlayerController::RunEnd);
	//Interact 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(InterAct_Action, ETriggerEvent::Started, this, &AGPPlayerController::InterAct);
	//Attack 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(Attack_Action, ETriggerEvent::Started, this, &AGPPlayerController::Attack);
	//Lock_On 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(LockOn_Action, ETriggerEvent::Started, this, &AGPPlayerController::Lock_On);
	//Lock_On 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(LockOn_Action, ETriggerEvent::Completed, this, &AGPPlayerController::Lock_On_End);
	//Crouch 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(Crouch_Action, ETriggerEvent::Started, this, &AGPPlayerController::Crouch);
	//InventoryMenu 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(InventoryMenu_Action, ETriggerEvent::Started, this, &AGPPlayerController::InventoryMenu);
	//EquipmentMenu 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(EquipmentMenu_Action, ETriggerEvent::Started, this, &AGPPlayerController::EquipmentMenu);
	//Climbing 인풋액션에 대해 함수 바인딩
	EnhancedInputComponent->BindAction(Climbing_Action, ETriggerEvent::Started, this, &AGPPlayerController::Climbing);
}

void AGPPlayerController::MoveCharacterByInputAxisVector(const FVector2d InputAxisVector)
{
	//Controller의 Rotation 정보, Move를 위해 Yaw Rotation 값만을 고려
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	//Forward Vector, 회전 행렬의 X축 단위 벡터
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//Right 방향, 회전 행렬의 Y축 단위 벡터
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//Move가 매 tick 마다 호출되면 Pawn이 Valid 하기 전에 호출될 수도 있으므로 유효성 체크
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AGPPlayerController::RotateControllerByInputAxisVector(const FVector2d InputAxisVector)
{
	if(!ActorToLockOn)
	{
		if(HavingGPCharacter)
		{
			HavingGPCharacter->AddControllerPitchInput(InputAxisVector.Y);
			HavingGPCharacter->AddControllerYawInput(InputAxisVector.X);
		}
	}
}

void AGPPlayerController::SetHavingGPCharacter()
{
	HavingGPCharacter = Cast<AGPCharacter>(GetPawn<APawn>());
}

bool AGPPlayerController::CanInteract()
{
	return HavingGPCharacter->OverlappingItem && HavingGPCharacter->
		GetActionState() == EActionState::Unoccupied;
}

bool AGPPlayerController::CanArm()
{
	return HavingGPCharacter->GetCharacterState() == ECharacterState::Unequipped && HavingGPCharacter->EquippedWeapon && HavingGPCharacter->GetActionState() == EActionState::Unoccupied;
}

bool AGPPlayerController::CanDisArm()
{
	return HavingGPCharacter->GetCharacterState() != ECharacterState::Unequipped && HavingGPCharacter->EquippedWeapon && HavingGPCharacter->GetActionState() == EActionState::Unoccupied;
}

bool AGPPlayerController::CanJump()
{
	if(HavingGPCharacter->GetCharacterMovement()->IsFalling())
		return false;
	return HavingGPCharacter && HavingGPCharacter->GetActionState() == EActionState::Unoccupied;
}

void AGPPlayerController::CameraToActorToLockOn()
{
	//AcotrToLockOn 이 있을 경우 카메라 방향 Target 을 항해
	if(HavingGPCharacter && ActorToLockOn)
	{
		FVector Start = HavingGPCharacter->GetActorLocation();
		FVector Target = ActorToLockOn->GetActorLocation();
		SetControlRotation(UKismetMathLibrary::FindLookAtRotation(Start, Target));
	}
}

bool AGPPlayerController::CanNotMove()
{
	return HavingGPCharacter->GetActionState() != EActionState::Unoccupied && HavingGPCharacter->GetActionState() !=
		EActionState::Crouching;
}

bool AGPPlayerController::CanRun()
{
	return HavingGPCharacter && HavingGPCharacter->Attribute->GetStaminaPercent() > 0 && HavingGPCharacter->
		GetActionState() == EActionState::Unoccupied;
}

bool AGPPlayerController::CanStopRun()
{
	return HavingGPCharacter && HavingGPCharacter->IsRunnig == true;
}

void AGPPlayerController::DoSphereTraceToFindLockOnTarget(FHitResult& SphereHitResult)
{
	const FVector Start = HavingGPCharacter->Camera->GetComponentLocation();
	const FVector Forward = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	const FVector End = Start + Forward*800.f;

	//TArray 동적으로 크기 변하는 배열, unreal engine built-in array type
	TArray<AActor*> ActorsToIgnore;
	//자기 자신과 Hit 하지 않도록 추가
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(HavingGPCharacter);
	
	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		100.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		SphereHitResult,
		true
	);
}

void AGPPlayerController::SphereTraceToCheckCanUncrouch(FHitResult& SphereHitResult)
{
	//머리 위 방향으로
	const FVector Start = HavingGPCharacter->GetActorLocation();
	const FVector Forward = HavingGPCharacter->GetActorUpVector();
	const FVector End = Start + Forward*100.f;

	//TArray 동적으로 크기 변하는 배열, unreal engine built-in array type
	TArray<AActor*> ActorsToIgnore;
	//자기 자신과 Hit 하지 않도록 추가
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(HavingGPCharacter);
		
	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		10.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		SphereHitResult,
		true
	);
}

void AGPPlayerController::DoCrouch()
{
	HavingGPCharacter->SetActionState(EActionState::Crouching);
	HavingGPCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(50.f);
	HavingGPCharacter->GetMesh()->SetRelativeLocation(FVector(0,0,-50.f));
}

void AGPPlayerController::DoUncrouch()
{
	HavingGPCharacter->SetActionState(EActionState::Unoccupied);
	HavingGPCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	HavingGPCharacter->GetMesh()->SetRelativeLocation(FVector(0,0,-90.f));
}
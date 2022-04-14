// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (nullptr == MainCharacter)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (MainCharacter)
	{
		//캐릭터 스피드 가져오기
		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;// 공중 속력은 제거
		Speed = Velocity.Size();

		// 공중에 떠있는지
		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();

		// 움직이고있는지
		if (MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
			bIsAccelerating = true;
		else
			bIsAccelerating = false;

		FRotator AimRotation = MainCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (MainCharacter->GetVelocity().Size() > 0.f) // 이동할때만 저장
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
	

		//FString RotationMessage = FString::Printf(TEXT("Base Aim Rotation : %f"), AimRotation.Yaw);
		//FString MovementRotationMessage = FString::Printf(TEXT("MovementRotation Rotation : %f"), MovementRotation.Yaw);
		FString MovementOffsetYawRotationMessage = FString::Printf(TEXT("MovementOffset Rotation : %f"), MovementOffsetYaw);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, MovementOffsetYawRotationMessage);
		}
	}
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

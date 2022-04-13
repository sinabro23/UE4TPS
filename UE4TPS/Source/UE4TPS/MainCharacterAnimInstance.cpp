// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	}
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

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
		//ĳ���� ���ǵ� ��������
		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;// ���� �ӷ��� ����
		Speed = Velocity.Size();

		// ���߿� ���ִ���
		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();

		// �����̰��ִ���
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

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

		FRotator AimRotation = MainCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (MainCharacter->GetVelocity().Size() > 0.f) // �̵��Ҷ��� ����
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

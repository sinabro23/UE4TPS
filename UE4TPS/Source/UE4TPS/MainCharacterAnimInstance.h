// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4TPS_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	// �������Ʈ �̺�Ʈ�׷������� �Ӽ��� ������Ʈ�ؼ� ���� ��
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* MainCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsInAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw = 0.f; // ���⶧�� �������� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bAiming = false;
};

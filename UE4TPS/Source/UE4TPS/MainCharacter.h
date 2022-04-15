// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class UE4TPS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Jump() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//ī�޶� ȸ�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45.f; // �ʴ� ��������� ī�޶� ���ư���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 45.f;

	// �����Ҷ� ���Ҷ��� Ű�������Ű ���� �ٲٰ��ϱ�����(���콺x)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate = 20.f;
	////////////////////////////////////////////
	// 
	// ���콺 ���� ����
	// meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0")�� �����Ϳ��� �ִ밪 �ּҰ� �Ѱ������, ������â ��ũ���ϴ°��� �Ѱ谪�� ���ϱ����� UIMin = "1.0", UIMax = "1.0"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipTurnRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipLookUpRate = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingTurnRate = 0.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingLookUpRate = 0.2f;
	///////////////////////////////////////////////

	// ũ�ν���� ���� //////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier = 0.f; // ȭ�� �߰������� ũ�ν���� ������������ ���������� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor = 0.f; // ĳ���Ͱ� �̵� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor = 0.f; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor = 0.f; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor = 0.f; // ĳ���Ͱ� �ݹ� �� ���� ũ�ν��� ������ �� ��

	// �ݹ߽� ũ�ν���� ����
	float ShootTimeDuration = 0.05f; // �ݹ��� ���� �ð� ����Ʈ 0.05�� ���� ª������
	bool bFiringBullet = false;
	FTimerHandle CrosshairShootTimer;
	//////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage; // �ѽ�� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	bool bAiming = false;

	float CameraDefaultFOV = 0.f;
	float CameraZoomedFOV = 35.f;
	float CameraCurrentFOV = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 30.f;

	// �ڵ���� ����
	bool bFireButtonPressed = false;
	bool bShouldFire = true; // ���� �������� �Ƚ�������
	float AutomaticFireRate = 0.1; // ����ӵ�
	FTimerHandle AutoFireTimer; // �ݹ� ������ Ÿ�̸�
	//////////////////////////////

	/** True if we should trace every frame for items */
	bool bShouldTraceForItems = false;
	/** Number of overlapped AItems */
	int8 OverlappedItemCount = 0;

	/** The AItem we hit last frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame;

	/** Currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	/** Set this in Blueprints for the default Weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/** The item currently hit by our trace in TraceForItems (could be null) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void FireWeapon();
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	void AimingButtonPressed();
	void AimingButtonReleased();

	void CameraInterpZoom(float DeltaTime);
	void SetLookRates(); // �ܰ��� ī�޶󰨵�

	// ũ�ν���� �������°� ���� �Լ�
	void CalculateCrosshairSpread(float DeltaTime);


	// �ݹ߽� ũ�ν���� ����
	void StartCrosshairBulletFire();
	UFUNCTION()
	void FinishCrosshairBulletFire();
	/////////////////////////

	/// �ڵ���� ���� �Լ�
	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	UFUNCTION()
	void AutoFireReset();
	//////////////////////////

	/** Line trace for items under the crosshairs */
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void TraceForItems();

	AWeapon* SpawnDefaultWeapon();

	/** Takes a weapon and attaches it to the mesh */
	void EquipWeapon(AWeapon* WeaponToEquip);

	/** Detach weapon and let it fall to the ground */
	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();

	void SwapWeapon(AWeapon* WeaponToSwap);

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	/** Adds/subtracts to/from OverlappedItemCount and updates bShouldTraceForItems */
	void IncrementOverlappedItemCount(int8 Amount);
};

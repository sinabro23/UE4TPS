// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;
	CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ� �������� ȸ���Ѵ�
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f); // ī�޶� �޸� ������ ������

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // ī�޶�� ��Ʈ�ѷ����� ȸ���ϸ� �ȵ�.

	// ���콺 ������ ĳ���Ͱ� ȸ������������
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;  // Yaw�� ��Ʈ�ѷ��� ���� ȸ���ϰ�
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; // ��ǲ ������ �������� ĳ���Ͱ� ȸ����������
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // �� �����̼Ƿ���Ʈ�� ȸ����
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	
}

void AMainCharacter::Jump()
{
	Super::Jump();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraInterpZoom(DeltaTime);
	SetLookRates();
	CalculateCrosshairSpread(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn); // ���콺 �̵��� �Լ� ���� ���ϰ� �ٷ�
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AMainCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Released, this, &AMainCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("AimingButton", EInputEvent::IE_Pressed, this, &AMainCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", EInputEvent::IE_Released, this, &AMainCharacter::AimingButtonReleased);
}

void AMainCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		// ��Ʈ�ѷ����� ���� ���ϱ�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X�� ���� �����̴ϱ�
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		// ��Ʈ�ѷ����� ���� ���ϱ�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Y�� ���� �����̴ϱ�
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::Turn(float Value)
{
	float TurnScaleFactor = 0.0f;
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AMainCharacter::LookUp(float Value)
{
	float LookUpScaleFactor = 0.0f;
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AMainCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
		if (bBeamEnd)
		{
			if (ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, BeamEnd);
			}

			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticle, SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
		
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (HipFireMontage)
		{
			AnimInstance->Montage_Play(HipFireMontage);
			AnimInstance->Montage_JumpToSection(FName("StartFire"));
		}
	}

	StartCrosshairBulletFire();
}

bool AMainCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation = FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	//CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start = CrosshairWorldPosition;
		const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f;

		OutBeamLocation = End;

		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		if (ScreenTraceHit.bBlockingHit)
		{
			OutBeamLocation = ScreenTraceHit.Location;

		}

		// Perform Second Trace
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart = MuzzleSocketLocation;
		const FVector WeaponTraceEnd = OutBeamLocation;
		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
		if (WeaponTraceHit.bBlockingHit)
		{
			OutBeamLocation = WeaponTraceHit.Location;
		}

		return true;
	}

	return false;
}

void AMainCharacter::AimingButtonPressed()
{
	bAiming = true;
}

void AMainCharacter::AimingButtonReleased()
{
	bAiming = false;
}

void AMainCharacter::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AMainCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AMainCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange = FVector2D(0.f, 600.f);
	FVector2D VelocityMultiplierRange = FVector2D(0.f, 1.f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;


	// CrosshairVelocityFactor �� ���ϱ�
	// � ���� �������� ������ �������ִ� ��. ���⼭�� �ӵ��� �ۼ�Ʈó�� 0~1�� �������� (�� ���� �ȵ���)
	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

	// CrosshairInAirFactor ���ϱ�
	if (GetCharacterMovement()->IsFalling())
	{
		//																				���������� �ӵ� ������
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		//																			  ���������� �ӵ� ������
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	}

	if (bAiming)
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.f);
	}
	else
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
	}

	if (bFiringBullet) // �ݹ� ��ư������ StartCrosshairBulletFire()�Լ��� ���� 5�ʵ��� True����
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.0f, DeltaTime, 60.f);
	}

	CrosshairSpreadMultiplier =
		0.5f +
		CrosshairVelocityFactor +
		CrosshairInAirFactor -
		CrosshairAimFactor +
		CrosshairShootingFactor;
}

void AMainCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(CrosshairShootTimer, this, &AMainCharacter::FinishCrosshairBulletFire, ShootTimeDuration);
}

void AMainCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void AMainCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	StartFireTimer();
}

void AMainCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AMainCharacter::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AMainCharacter::AutoFireReset, AutomaticFireRate);
	}
}

void AMainCharacter::AutoFireReset()
{
	bShouldFire = true;	
	if (bFireButtonPressed)
	{
		StartFireTimer();
	}
}

float AMainCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"

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
	bUseControllerRotationYaw = false;  // Yaw�� ��Ʈ�ѷ��� ���� ȸ���ϰ�
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // ��ǲ ������ �������� ĳ���Ͱ� ȸ����������
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // �� �����̼Ƿ���Ʈ�� ȸ����
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::Jump()
{
	Super::Jump();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AMainCharacter::FireButton);
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
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::FireButton()
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

		FHitResult FireHit;
		const FVector Start = SocketTransform.GetLocation();
		const FQuat Rotation = SocketTransform.GetRotation();
		const FVector RotationAxis = Rotation.GetAxisX();
		const FVector End = Start + RotationAxis * 50'000.f;

		FVector BeamEndPoint = End;

		GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
		if (FireHit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
			DrawDebugPoint(GetWorld(), FireHit.Location, 5.f, FColor::Red, false, 2.f);
		}

		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FireHit.Location);
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
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Animation/AnimNotify_AttackHitCheck.h"
#include "BasicTest/Animation/Interface/CWAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, const FAnimNotifyEventReference& InEventReference)
{
	Super::Notify(InMeshComp, InAnimation, InEventReference);

	if (InMeshComp)
	{
		if (ICWAnimationAttackInterface* Owner = Cast<ICWAnimationAttackInterface>(InMeshComp->GetOwner()))
		{
			Owner->AttackHitCheck();
		}
	}

}
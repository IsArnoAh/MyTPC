// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadSlotsPanel.h"
ULoadSlotsPanel::ULoadSlotsPanel(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void ULoadSlotsPanel::NativeConstruct()
{
	
	Super::NativeConstruct();
}


void ULoadSlotsPanel::UpdateLoadSlots()
{
	SlotsDetails = Gm->UpDateSlotsDetail();
}



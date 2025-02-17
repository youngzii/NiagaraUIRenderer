// Fill out your copyright notice in the Description page of Project Settings.


#include "NiagaraSystemWidget.h"
#include "SNiagaraUISystemWidget.h"
#include "Materials/MaterialInterface.h"
#include "NiagaraUIActor.h"
#include "NiagaraUIComponent.h"

TSharedRef<SWidget> UNiagaraSystemWidget::RebuildWidget()
{
	niagaraSlateWidget = SNew(SNiagaraUISystemWidget);

	InitializeNiagaraUI();

	return niagaraSlateWidget.ToSharedRef();
}

void UNiagaraSystemWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!niagaraSlateWidget.IsValid())
	{
		return;
	}

	if (!NiagaraActor || !NiagaraComponent)
		InitializeNiagaraUI();
	
}

void UNiagaraSystemWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	
	niagaraSlateWidget.Reset();

	if (NiagaraActor)
		NiagaraActor->Destroy();
}

#if WITH_EDITOR
const FText UNiagaraSystemWidget::GetPaletteCategory()
{
	return NSLOCTEXT("NiagaraUIRenderer", "Palette Category", "Niagara");
}

void UNiagaraSystemWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	InitializeNiagaraUI();
}
#endif

void UNiagaraSystemWidget::InitializeNiagaraUI()
{
	if (UWorld* World = GetWorld())
	{
		if (!World->PersistentLevel)
			return;

			
		if (!NiagaraActor)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			NiagaraActor = World->SpawnActor<ANiagaraUIActor>(FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		}

		NiagaraComponent = NiagaraActor->SpawnNewNiagaraUIComponent(NiagaraSystemReference, AutoActivate, ShowDebugSystemInWorld);

		niagaraSlateWidget->SetNiagaraComponentReference(NiagaraComponent, FNiagaraWidgetProperties(&MaterialRemapList, AutoActivate, ShowDebugSystemInWorld, FakeDepthScale, FakeDepthScaleDistance));
	}
}

void UNiagaraSystemWidget::ActivateSystem(bool Reset)
{
	if (NiagaraComponent)
		NiagaraComponent->Activate(Reset);
}

void UNiagaraSystemWidget::DeactivateSystem()
{
	if (NiagaraComponent)
		NiagaraComponent->Deactivate();
}

UNiagaraUIComponent* UNiagaraSystemWidget::GetNiagaraComponent()
{
	return NiagaraComponent;
}

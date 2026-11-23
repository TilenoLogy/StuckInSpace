// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageOverlayWidget.h"
#include "Components/Image.h"

void UDamageOverlayWidget::SetDamageOpacity(float Opacity)
{
	if (DamageImage)
	{
		DamageImage->SetOpacity(FMath::Clamp(Opacity, 0.0f, 0.6f));
	}
}

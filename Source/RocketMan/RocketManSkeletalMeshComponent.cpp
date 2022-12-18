// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketManSkeletalMeshComponent.h"

#if 0
FMatrix URocketManSkeletalMeshComponent::GetRenderMatrix() const
{
	FMatrix InverseOldViewProjMat;
	FMatrix NewViewProjMat;


	const FMatrix ModelMat = GetComponentTransform().ToMatrixWithScale();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			FSceneViewFamilyContext ViewFamily
			{
				FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true)
			};

			FVector ViewLocation;
			FRotator ViewRotation;
			FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

			if (!SceneView)
			{
				return ModelMat;
			}

			const float GunFov = 54.0f;
			const float DesiredHalfFovRad = GunFov * PI / 360.0f;

			const auto ViewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();
			const float Width = ViewportSize.X;
			const float Height = ViewportSize.Y;
			const float NearClippingPlaneDistance = SceneView->NearClippingDistance;

			const FMatrix NewProjectionMatrix = FReversedZPerspectiveMatrix(DesiredHalfFovRad, Width, Height, NearClippingPlaneDistance);

			const FMatrix ViewMatrix = SceneView->ViewMatrices.GetViewMatrix();
			const FMatrix InverseViewMatrix = SceneView->ViewMatrices.GetInvViewMatrix();

			const FMatrix ProjectionMatrix = SceneView->ViewMatrices.GetProjectionMatrix();
			const FMatrix InverseProjectionMatrix = SceneView->ViewMatrices.GetInvProjectionMatrix();

			NewViewProjMat = ViewMatrix * NewProjectionMatrix;
			InverseOldViewProjMat = InverseProjectionMatrix * InverseViewMatrix;
		}
	}

	return ModelMat * NewViewProjMat * InverseOldViewProjMat;
}
#endif

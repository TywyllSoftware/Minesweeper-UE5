//
//  UtilGridButtonStyle.cpp
//  Minesweeper_Index
//
//  Created by Tomas Mikalauskas on 17/05/2022.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//
#include "UtilGridButtonStyle.h"

//Engine includes
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FUtilGridButtonStyle::StyleInstance = nullptr;

void FUtilGridButtonStyle::Initialize()
{
    if (!StyleInstance.IsValid())
    {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FUtilGridButtonStyle::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FUtilGridButtonStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT("MinesweeperGridButtonStyle"));
    return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )


TSharedRef< FSlateStyleSet > FUtilGridButtonStyle::Create()
{
    const FVector2D Icon10x10(10.0f, 10.0f);
    const FVector2D Icon14x14(14.0f, 14.0f);
    const FVector2D Icon16x16(16.0f, 16.0f);
    const FVector2D Icon20x20(20.0f, 20.0f);
    const FVector2D Icon24x24(24.0f, 24.0f);
    const FVector2D Icon32x32(32.0f, 32.0f);
    const FVector2D Icon40x40(40.0f, 40.0f);
    
    TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MinesweeperGridButtonStyle"));
    Style->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("MinesweeperToolbar/Resources/Button"));

    FButtonStyle GridButtonStyle = FButtonStyle()
        .SetNormal( BOX_BRUSH(TEXT("Button"), Icon32x32, 8.0f / 32.0f))
        .SetDisabled( BOX_BRUSH(TEXT("Button_Disabled"), Icon32x32, 8.0f / 32.0f))
        .SetPressed( BOX_BRUSH(TEXT("Button_Pressed"), Icon32x32, 8.0f / 32.0f))
        .SetHovered( BOX_BRUSH(TEXT("Button_Hovered"), Icon32x32, 8.0f / 32.0f))
        .SetNormalPadding(FMargin(2, 2, 2, 2))
        .SetPressedPadding(FMargin(2, 3, 2, 1));
    
    Style->Set("MinesweeperGridButton.Button", GridButtonStyle);
                    
    //Style->Set("MinesweeperGridButton.Button", new IMAGE_BRUSH(TEXT("Button"), Icon512x512));
    //Style->Set("MinesweeperGridButton.Button_Hovered", new IMAGE_BRUSH(TEXT("Button_Hovered"), Icon512x512));
    //Style->Set("MinesweeperGridButton.Button_Pressed", new IMAGE_BRUSH(TEXT("Button_Pressed"), Icon512x512));
    //Style->Set("MinesweeperGridButton.Button_Disabled", new IMAGE_BRUSH(TEXT("Button_Disabled"), Icon512x512));
    
    return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FUtilGridButtonStyle::ReloadTextures()
{
    FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FUtilGridButtonStyle::Get()
{
    return *StyleInstance;
}

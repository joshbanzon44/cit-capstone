//
// Blueprint Color Picker
// 
// Slate color picker exposed to Blueprints
//
// The MIT License (MIT)
//
// Copyright (c) 2024 Rohan Singh
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include "ColorPicker.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SButton.h"

UColorPicker::UColorPicker(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Default background color is transparent
    BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Default initial color is White
    InitialColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Show full version by default
    bShowInline = false;

    // Transparency is on by default
    bUseAlpha = true;

    // Context menu mode is off by default
    bForContextMenu = false;
}

TSharedRef<SWidget> UColorPicker::RebuildWidget()
{
    // Create the Slate color picker widget
    SlateColorPickerWidget = SNew(SColorPicker)
        .TargetColorAttribute(InitialColor)
        .DisplayInlineVersion(bShowInline)
        .UseAlpha(bUseAlpha)
        .OnColorCommitted_Lambda([this](const FLinearColor& NewColor)
            {
                HandleColorChanged(NewColor);
            });

    // Add the buttons and the color picker widget to a vertical box
    return SNew(SBorder)
        .BorderBackgroundColor(BackgroundColor)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .Padding(10)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SlateColorPickerWidget.ToSharedRef()
            ]
        ];   
}

void UColorPicker::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    // Reset the Slate color picker widget to release it from memory
    SlateColorPickerWidget.Reset();
}

void UColorPicker::HandleColorOkClicked()
{
    if (bForContextMenu)
    {
        TSharedPtr<SWindow> ActiveWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
        if (ActiveWindow.IsValid())
        {
            ActiveWindow->RequestDestroyWindow();
        }
    }
    RemoveFromParent();
}

void UColorPicker::HandleColorCancelClicked(const FLinearColor& Color)
{
    // Return initial color (Discard current color selection)
    OnColorChanged.Broadcast(Color);

    if (bForContextMenu)
    {
        TSharedPtr<SWindow> ActiveWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
        if (ActiveWindow.IsValid())
        {
            ActiveWindow->RequestDestroyWindow();
        }
    }
    RemoveFromParent();
}

void UColorPicker::HandleColorChanged(const FLinearColor& NewColor)
{
    // Send currently selected color
    OnColorChanged.Broadcast(NewColor);
}


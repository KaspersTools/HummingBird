//
// Created by Kasper de Bruin on 01/11/2023.
//

#pragma once

#include <imgui.h>

namespace HummingBirdCore {
  namespace Themes {
    inline void Maya() {
      // Credits: https://gist.github.com/ongamex/4ee36fb23d6c527939d0f4ba72144d29

      ImGuiStyle &style = ImGui::GetStyle();

      style.ChildRounding = 3.f;
      style.GrabRounding = 0.f;
      style.WindowRounding = 0.f;
      style.ScrollbarRounding = 3.f;
      style.FrameRounding = 3.f;
      style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

      style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
      style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
      style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
      style.Colors[ImGuiCol_ChildBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
      style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
      style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
      style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
      style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      //    style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
      style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
      style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
      style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
      style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
      style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
      style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      //    style.Colors[ImGuiCol_Column]                = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
      //    style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      //    style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      //    style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
      //    style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
      //    style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
      style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
      style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
      style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
    }

    inline void Monochrome() {
      // Credits: https://gist.github.com/enemymouse/c8aa24e247a1d7b9fc33d45091cbb8f0

      ImGuiStyle &style = ImGui::GetStyle();
      style.Alpha = 1.0;
      style.ChildRounding = 3;
      style.WindowRounding = 3;
      style.GrabRounding = 1;
      style.GrabMinSize = 20;
      style.FrameRounding = 3;


      style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
      style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
      style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
      style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
      style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
      style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
      style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
      style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
      style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
      style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
      style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
      style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
      style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      //    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
      style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
      style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
      style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
      style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
      style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
      style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
      style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
      style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
      style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
      //    style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
      //    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
      //    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
      style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
      style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
      style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      //    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
      //    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
      //    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
      style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
      style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
      //    style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
      //    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
    }

    inline void The_0n3() {
      // Credits: https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html

      ImGuiStyle *style = &ImGui::GetStyle();

      style->WindowPadding = ImVec2(15, 15);
      style->WindowRounding = 5.0f;
      style->FramePadding = ImVec2(5, 5);
      style->FrameRounding = 4.0f;
      style->ItemSpacing = ImVec2(12, 8);
      style->ItemInnerSpacing = ImVec2(8, 6);
      style->IndentSpacing = 25.0f;
      style->ScrollbarSize = 15.0f;
      style->ScrollbarRounding = 9.0f;
      style->GrabMinSize = 5.0f;
      style->GrabRounding = 3.0f;

      style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
      style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
      style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
      style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
      style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
      style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
      style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
      style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
      style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
      style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
      style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
      style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
      //    style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
      style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
      style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
      style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
      style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
      style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
      style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
      //    style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      //    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
      //    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
      style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
      //    style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
      //    style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
      //    style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
      style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
      style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
      style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
      style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
      style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
      style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    }

    inline void ModernDarkTheme() {
      // Credits: https://github.com/AdamHarris-GamesProgrammer/Dear-ImGui-Themes/blob/master/ModernDarkTheme.txt

      auto &style = ImGui::GetStyle();
      style.ChildRounding = 0;
      style.GrabRounding = 0;
      style.FrameRounding = 2;
      style.PopupRounding = 0;
      style.ScrollbarRounding = 0;
      style.TabRounding = 2;
      style.WindowRounding = 0;
      style.FramePadding = {4, 4};

      style.WindowTitleAlign = {0.0, 0.5};
      style.ColorButtonPosition = ImGuiDir_Left;

      ImVec4 *colors = ImGui::GetStyle().Colors;
      colors[ImGuiCol_Text] = {1.0f, 1.0f, 1.0f, 1.00f};           //
      colors[ImGuiCol_TextDisabled] = {0.25f, 0.25f, 0.25f, 1.00f};//
      colors[ImGuiCol_WindowBg] = {0.09f, 0.09f, 0.09f, 0.94f};    //
      colors[ImGuiCol_ChildBg] = {0.11f, 0.11f, 0.11f, 1.00f};     //
      colors[ImGuiCol_PopupBg] = {0.11f, 0.11f, 0.11f, 0.94f};     //
      colors[ImGuiCol_Border] = {0.07f, 0.08f, 0.08f, 1.00f};
      colors[ImGuiCol_BorderShadow] = {0.00f, 0.00f, 0.00f, 0.00f};
      colors[ImGuiCol_FrameBg] = {0.35f, 0.35f, 0.35f, 0.54f};//
      colors[ImGuiCol_FrameBgHovered] = {0.31f, 0.29f, 0.27f, 1.00f};
      colors[ImGuiCol_FrameBgActive] = {0.40f, 0.36f, 0.33f, 0.67f};
      colors[ImGuiCol_TitleBg] = {0.1f, 0.1f, 0.1f, 1.00f};
      colors[ImGuiCol_TitleBgActive] = {0.3f, 0.3f, 0.3f, 1.00f};//
      colors[ImGuiCol_TitleBgCollapsed] = {0.0f, 0.0f, 0.0f, 0.61f};
      colors[ImGuiCol_MenuBarBg] = {0.18f, 0.18f, 0.18f, 0.94f};//
      colors[ImGuiCol_ScrollbarBg] = {0.00f, 0.00f, 0.00f, 0.16f};
      colors[ImGuiCol_ScrollbarGrab] = {0.24f, 0.22f, 0.21f, 1.00f};
      colors[ImGuiCol_ScrollbarGrabHovered] = {0.31f, 0.29f, 0.27f, 1.00f};
      colors[ImGuiCol_ScrollbarGrabActive] = {0.40f, 0.36f, 0.33f, 1.00f};
      colors[ImGuiCol_CheckMark] = {0.84f, 0.84f, 0.84f, 1.0f};        //
      colors[ImGuiCol_SliderGrab] = {0.8f, 0.8f, 0.8f, 1.0f};          //
      colors[ImGuiCol_SliderGrabActive] = {0.55f, 0.55f, 0.55f, 1.00f};//
      colors[ImGuiCol_Button] = {0.55f, 0.55f, 0.55f, 0.40f};          //
      colors[ImGuiCol_ButtonHovered] = {0.15f, 0.15f, 0.15f, 0.62f};   //
      colors[ImGuiCol_ButtonActive] = {0.60f, 0.60f, 0.60f, 1.00f};    //
      colors[ImGuiCol_Header] = {0.84f, 0.36f, 0.05f, 0.0f};           //
      colors[ImGuiCol_HeaderHovered] = {0.25f, 0.25f, 0.25f, 0.80f};   //
      colors[ImGuiCol_HeaderActive] = {0.42f, 0.42f, 0.42f, 1.00f};
      colors[ImGuiCol_Separator] = {0.35f, 0.35f, 0.35f, 0.50f};//
      colors[ImGuiCol_SeparatorHovered] = {0.31f, 0.29f, 0.27f, 0.78f};
      colors[ImGuiCol_SeparatorActive] = {0.40f, 0.36f, 0.33f, 1.00f};
      colors[ImGuiCol_ResizeGrip] = {1.0f, 1.0f, 1.0f, 0.25f};        //
      colors[ImGuiCol_ResizeGripHovered] = {1.00f, 1.0f, 1.0f, 0.4f}; //
      colors[ImGuiCol_ResizeGripActive] = {1.00f, 1.00f, 1.0f, 0.95f};//
      colors[ImGuiCol_Tab] = {0.18f, 0.18f, 0.18f, 1.0f};             //
      colors[ImGuiCol_TabHovered] = {0.58f, 0.58f, 0.58f, 0.80f};     //
      colors[ImGuiCol_TabActive] = {0.6f, 0.60f, 0.60f, 1.00f};
      colors[ImGuiCol_TabUnfocused] = {0.07f, 0.10f, 0.15f, 0.97f};
      colors[ImGuiCol_TabUnfocusedActive] = {0.14f, 0.26f, 0.42f, 1.00f};
      colors[ImGuiCol_PlotLines] = {0.66f, 0.60f, 0.52f, 1.00f};
      colors[ImGuiCol_PlotLinesHovered] = {0.98f, 0.29f, 0.20f, 1.00f};
      colors[ImGuiCol_PlotHistogram] = {0.60f, 0.59f, 0.10f, 1.00f};
      colors[ImGuiCol_PlotHistogramHovered] = {0.72f, 0.73f, 0.15f, 1.00f};
      colors[ImGuiCol_TextSelectedBg] = {0.27f, 0.52f, 0.53f, 0.35f};
      colors[ImGuiCol_DragDropTarget] = {0.60f, 0.59f, 0.10f, 0.90f};
      colors[ImGuiCol_NavHighlight] = {0.51f, 0.65f, 0.60f, 1.00f};
      colors[ImGuiCol_NavWindowingHighlight] = {1.00f, 1.00f, 1.00f, 0.70f};
      colors[ImGuiCol_NavWindowingDimBg] = {0.80f, 0.80f, 0.80f, 0.20f};
      colors[ImGuiCol_ModalWindowDimBg] = {0.11f, 0.13f, 0.13f, 0.35f};
    }

    inline void EmbraceTheDarkness() {
      // Credits: https://github.com/ocornut/imgui/issues/707#issuecomment-917151020

      ImVec4 *colors = ImGui::GetStyle().Colors;
      colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
      colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
      colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
      colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
      colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
      colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
      colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
      colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
      colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
      colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
      colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
      colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
      colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
      colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
      colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
      colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
      colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      //    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
      //    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
      colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
      colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
      colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
      colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
      colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

      ImGuiStyle &style = ImGui::GetStyle();
      style.WindowPadding = ImVec2(8.00f, 8.00f);
      style.FramePadding = ImVec2(5.00f, 2.00f);
      style.CellPadding = ImVec2(6.00f, 6.00f);
      style.ItemSpacing = ImVec2(6.00f, 6.00f);
      style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
      style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
      style.IndentSpacing = 25;
      style.ScrollbarSize = 15;
      style.GrabMinSize = 10;
      style.WindowBorderSize = 1;
      style.ChildBorderSize = 1;
      style.PopupBorderSize = 1;
      style.FrameBorderSize = 1;
      style.TabBorderSize = 1;
      style.WindowRounding = 7;
      style.ChildRounding = 4;
      style.FrameRounding = 3;
      style.PopupRounding = 4;
      style.ScrollbarRounding = 9;
      style.GrabRounding = 3;
      style.LogSliderDeadzone = 4;
      style.TabRounding = 4;
    }

    inline void PhocosGreen() {
      ImGuiStyle &style = ImGui::GetStyle();
      style.WindowPadding = ImVec2(8, 8);
      style.FramePadding = ImVec2(5, 2);
      style.CellPadding = ImVec2(6, 6);
      style.ItemSpacing = ImVec2(6, 6);
      style.ItemInnerSpacing = ImVec2(6, 6);
      style.TouchExtraPadding = ImVec2(0, 0);
      style.IndentSpacing = 17;
      style.ScrollbarSize = 10;
      style.GrabMinSize = 10;

      style.WindowBorderSize = true;
      style.ChildBorderSize = true;
      style.PopupBorderSize = true;
      style.FrameBorderSize = true;
      style.TabBorderSize = false;

      style.WindowRounding = 3;
      style.ChildRounding = 4;
      style.FrameRounding = 2;
      style.PopupRounding = 4;
      style.ScrollbarRounding = 9;
      style.GrabRounding = 3;

      style.WindowTitleAlign = ImVec2(0.5, 0.5);
      style.WindowMenuButtonPosition = ImGuiDir_None;

      ImVec4 *colors = style.Colors;
      colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
      colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
      colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.667f);
      colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.92f);
      colors[ImGuiCol_Border] = ImVec4(0.50f, 0.40f, 0.96f, 0.11f);
      colors[ImGuiCol_BorderShadow] = ImVec4(0.33f, 0.95f, 0.49f, 0.09f);
      colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.54f);
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.13f, 0.13f, 0.54f);
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
      colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
      colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.37f);
      colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
      colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.78f, 0.38f, 1.00f);
      colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
      colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.23f, 0.21f, 1.00f);
      colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.23f, 0.21f, 0.33f);
      colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.47f, 0.42f, 1.00f);
      colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.47f, 0.42f, 1.00f);
      colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
      colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_PlotLines] = ImVec4(0.38f, 0.84, 0.49f, 1.00f);
      colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.08f, 0.60f, 0.11f, 1.00f);
      colors[ImGuiCol_PlotHistogram] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
      colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.13f, 0.70f, 0.28f, 1.00f);
      colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
      colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
      colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
      colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.23f, 0.21f, 1.00f);
      colors[ImGuiCol_DragDropTarget] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
      colors[ImGuiCol_NavHighlight] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
      colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
      colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
      colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.38f, 0.84f, 0.49f, 1.00f);
    }

    inline void DoughBkins_Default() {

      ImGuiStyle &style = ImGui::GetStyle();

      // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
      style.Alpha = 1.0f;
      style.FrameRounding = 3.0f;
      style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
      style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
      style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
      style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
      style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
      style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
      style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
      style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
      style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
      style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
      style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
      style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
      style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
      style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
      style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);

      style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
      style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
      style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
      style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
      style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
      style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
      style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
      style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
      style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
      style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
      style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    }

    inline void DoughBkins_Black(float alpha_ = .7f) {
      DoughBkins_Default();

      ImGuiStyle &style = ImGui::GetStyle();

      style.Alpha = 1.0f;

      for (int i = 0; i <= ImGuiCol_COUNT; i++) {
        ImVec4 &col = style.Colors[i];
        float H, S, V;
        ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

        if (S < 0.1f) {
          V = 1.0f - V;
        }
        ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
        if (col.w < 1.00f) {
          col.w *= alpha_;
        }
      }
    }

    inline void DoughBkins_White(float alpha_ = 1.0f) {
      DoughBkins_Default();

      ImGuiStyle &style = ImGui::GetStyle();

      for (int i = 0; i <= ImGuiCol_COUNT; i++) {
        ImVec4 &col = style.Colors[i];
        if (col.w < 1.00f) {
          col.x *= alpha_;
          col.y *= alpha_;
          col.z *= alpha_;
          col.w *= alpha_;
        }
      }
    }

    inline void Blender() {
      // 'Blender Dark' theme from v3.0.0 [Improvised]
      // Colors grabbed using X11 Soft/xcolor
      // BY: https://github.com/Raais/ImguiCandy/blob/main/ImCandy/candy.h
      ImGuiStyle *style = &ImGui::GetStyle();
      ImVec4 *colors = style->Colors;
      ImGui::StyleColorsDark(style);//Reset to base/dark theme
      colors[ImGuiCol_Text] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
      colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
      colors[ImGuiCol_ChildBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
      colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
      colors[ImGuiCol_Border] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
      colors[ImGuiCol_BorderShadow] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
      colors[ImGuiCol_FrameBg] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
      colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
      colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_Button] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_Header] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
      colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.19f, 0.39f, 0.69f, 1.00f);
      colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
      colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
      colors[ImGuiCol_PlotHistogram] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.20f, 0.39f, 0.69f, 1.00f);
      colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
      style->WindowPadding = ImVec2(12.00f, 8.00f);
      style->ItemSpacing = ImVec2(7.00f, 3.00f);
      style->GrabMinSize = 20.00f;
      style->WindowRounding = 8.00f;
      style->FrameBorderSize = 0.00f;
      style->FrameRounding = 4.00f;
      style->GrabRounding = 12.00f;
    }

    // BY: https://github.com/Raais/ImguiCandy/blob/main/ImCandy/candy.h
    inline void Nord() {
      // Nord/Nordic GTK [Improvised]
      // https://github.com/EliverLara/Nordic
      ImGuiStyle *style = &ImGui::GetStyle();
      ImVec4 *colors = style->Colors;
      ImGui::StyleColorsDark(style);//Reset to base/dark theme
      colors[ImGuiCol_Text] = ImVec4(0.85f, 0.87f, 0.91f, 0.88f);
      colors[ImGuiCol_TextDisabled] = ImVec4(0.49f, 0.50f, 0.53f, 1.00f);
      colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
      colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
      colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
      colors[ImGuiCol_Border] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
      colors[ImGuiCol_BorderShadow] = ImVec4(0.09f, 0.09f, 0.09f, 0.00f);
      colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
      colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
      colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
      colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
      colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.26f, 0.32f, 0.60f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
      colors[ImGuiCol_CheckMark] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_Button] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_Header] = ImVec4(0.51f, 0.63f, 0.76f, 1.00f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
      colors[ImGuiCol_ResizeGrip] = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.61f, 0.74f, 0.87f, 1.00f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.20f, 0.25f, 1.00f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.24f, 0.31f, 1.00f);
      colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.26f, 0.32f, 1.00f);
      colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
      colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.19f, 0.23f, 1.00f);
      colors[ImGuiCol_PlotHistogram] = ImVec4(0.56f, 0.74f, 0.73f, 1.00f);
      colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.53f, 0.75f, 0.82f, 1.00f);
      colors[ImGuiCol_TextSelectedBg] = ImVec4(0.37f, 0.51f, 0.67f, 1.00f);
      colors[ImGuiCol_NavHighlight] = ImVec4(0.53f, 0.75f, 0.82f, 0.86f);
      style->WindowBorderSize = 1.00f;
      style->ChildBorderSize = 1.00f;
      style->PopupBorderSize = 1.00f;
      style->FrameBorderSize = 1.00f;
    }

    // BY: https://github.com/Raais/ImguiCandy/blob/main/ImCandy/candy.h
    inline void Cyberpunk() {
      // Cyberpunk Neon [Improvised]
      // https://github.com/Roboron3042/Cyberpunk-Neon
      ImGuiStyle *style = &ImGui::GetStyle();
      ImVec4 *colors = style->Colors;
      ImGui::StyleColorsDark(style);//Reset to base/dark theme
      colors[ImGuiCol_Text] = ImVec4(0.00f, 0.82f, 1.00f, 1.00f);
      colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.36f, 0.63f, 1.00f);
      colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.04f, 0.12f, 1.00f);
      colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.04f, 0.22f, 1.00f);
      colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.06f, 0.27f, 1.00f);
      colors[ImGuiCol_Border] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.75f, 1.00f, 0.20f);
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.81f, 0.95f, 1.00f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.00f, 0.54f, 0.81f);
      colors[ImGuiCol_MenuBarBg] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.88f, 1.00f, 1.00f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.95f, 0.19f, 0.67f, 1.00f);
      colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.19f, 0.92f, 1.00f);
      colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 0.95f, 1.00f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.81f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_Button] = ImVec4(0.00f, 0.98f, 1.00f, 0.52f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_Header] = ImVec4(0.00f, 0.95f, 1.00f, 0.40f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.94f, 0.00f, 1.00f, 0.80f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.01f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_Separator] = ImVec4(0.74f, 0.00f, 1.00f, 0.50f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.34f, 0.00f, 1.00f, 0.78f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 1.00f, 0.85f, 1.00f);
      colors[ImGuiCol_ResizeGrip] = ImVec4(0.61f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.89f, 0.26f, 0.98f, 0.67f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.88f, 1.00f, 0.95f);
      colors[ImGuiCol_Tab] = ImVec4(0.36f, 0.00f, 1.00f, 1.00f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.92f, 1.00f, 0.80f);
      colors[ImGuiCol_TabActive] = ImVec4(0.62f, 0.00f, 0.80f, 1.00f);
      colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 0.88f, 1.00f);
      style->FrameBorderSize = 0.00f;
      style->WindowRounding = 0.00f;
      style->FrameRounding = 0.00f;
      style->ScrollbarRounding = 0.00f;
      style->GrabRounding = 0.00f;
    }

    inline void SetWindowBGAlpha(float alpha) {
      ImVec4 *colors = ImGui::GetStyle().Colors;
      colors[ImGuiCol_Text] = ImVec4(
              colors[ImGuiCol_Text].x,
              colors[ImGuiCol_Text].y,
              colors[ImGuiCol_Text].z,
              alpha / 255);
    }
  }// namespace Themes
}// namespace HummingBirdCore

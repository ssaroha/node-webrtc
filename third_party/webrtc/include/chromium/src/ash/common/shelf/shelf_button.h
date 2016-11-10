// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_COMMON_SHELF_SHELF_BUTTON_H_
#define ASH_COMMON_SHELF_SHELF_BUTTON_H_

#include "ash/ash_export.h"
#include "base/macros.h"
#include "ui/gfx/shadow_value.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/image_view.h"

namespace ash {
class InkDropButtonListener;
class ShelfView;

// Button used for items on the launcher, except for the AppList.
class ASH_EXPORT ShelfButton : public views::CustomButton {
 public:
  static const char kViewClassName[];

  // Used to indicate the current state of the button.
  enum State {
    // Nothing special. Usually represents an app shortcut item with no running
    // instance.
    STATE_NORMAL = 0,
    // Button has mouse hovering on it.
    STATE_HOVERED = 1 << 0,
    // Underlying ShelfItem has a running instance.
    STATE_RUNNING = 1 << 1,
    // Underlying ShelfItem is active (i.e. has focus).
    STATE_ACTIVE = 1 << 2,
    // Underlying ShelfItem needs user's attention.
    STATE_ATTENTION = 1 << 3,
    STATE_FOCUSED = 1 << 4,
    // Hide the status (temporarily for some animations).
    STATE_HIDDEN = 1 << 5,
  };

  ShelfButton(InkDropButtonListener* listener, ShelfView* shelf_view);
  ~ShelfButton() override;

  // Sets the image to display for this entry.
  void SetImage(const gfx::ImageSkia& image);

  // Retrieve the image to show proxy operations.
  const gfx::ImageSkia& GetImage() const;

  // |state| is or'd into the current state.
  void AddState(State state);
  void ClearState(State state);
  int state() const { return state_; }

  // Returns the bounds of the icon.
  gfx::Rect GetIconBounds() const;

  // Called when user started dragging the shelf button.
  void OnDragStarted(const ui::LocatedEvent* event);

  // Overrides to views::CustomButton:
  void ShowContextMenu(const gfx::Point& p,
                       ui::MenuSourceType source_type) override;

  // View override - needed by unit test.
  void OnMouseCaptureLost() override;

 protected:
  // View overrides:
  const char* GetClassName() const override;
  bool OnMousePressed(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void GetAccessibleState(ui::AXViewState* state) override;
  void Layout() override;
  void ChildPreferredSizeChanged(views::View* child) override;
  void OnFocus() override;
  void OnBlur() override;
  void OnPaint(gfx::Canvas* canvas) override;

  // ui::EventHandler overrides:
  void OnGestureEvent(ui::GestureEvent* event) override;

  // views::CustomButton overrides:
  std::unique_ptr<views::InkDropRipple> CreateInkDropRipple() const override;
  bool ShouldEnterPushedState(const ui::Event& event) override;
  bool ShouldShowInkDropHighlight() const override;
  void NotifyClick(const ui::Event& event) override;

  // Sets the icon image with a shadow.
  void SetShadowedImage(const gfx::ImageSkia& bitmap);

 private:
  class BarView;

  // Updates the parts of the button to reflect the current |state_| and
  // alignment. This may add or remove views, layout and paint.
  void UpdateState();

  // Updates the status bar (bitmap, orientation, visibility).
  void UpdateBar();

  InkDropButtonListener* listener_;

  // The shelf view hosting this button.
  ShelfView* shelf_view_;

  // The icon part of a button can be animated independently of the rest.
  views::ImageView* icon_view_;

  // Draws a bar underneath the image to represent the state of the application.
  BarView* bar_;

  // The current application state, a bitfield of State enum values.
  int state_;

  gfx::ShadowValues icon_shadows_;

  // If non-null the destuctor sets this to true. This is set while the menu is
  // showing and used to detect if the menu was deleted while running.
  bool* destroyed_flag_;

  DISALLOW_COPY_AND_ASSIGN(ShelfButton);
};

}  // namespace ash

#endif  // ASH_COMMON_SHELF_SHELF_BUTTON_H_

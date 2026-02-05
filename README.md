# OrbitDash Lite 🚀 (Legacy Edition)

**OrbitDash Lite** is a polished arcade game rewritten from the ground up for **Cocos2d-x v2.1.5**. 

While originally developed in modern engines like **Cocos2d-x 4.0** and **Axmol**, this version was created to demonstrate that high-quality gameplay and sophisticated UI can still be achieved using "legacy" frameworks. It serves as both a personal gift and a technical case study for the developer community.

---

## 🌟 Why this version? (The "Cool Stuff")

Many developers assume that older versions of Cocos2d-x are limited. This project proves otherwise by showcasing:

* **Polished UI/UX:** Smooth popups and interactive menus built entirely through code.
* **Modern Logic in Legacy Syntax:** Back-porting features from Axmol/Cocos4 back to the classic 2.1.5 API.
* **Syntax Mastery:** A deep dive into the `menu_selector`, `schedule_selector`, and manual memory management (`retain`/`release`) that defined mobile gaming's golden age.

---

## 📜 Project Evolution

1. **v1.0 (Original):** Built in **Cocos2d-x 4.0** (Modern C++, Metal/Vulkan support).
2. **v1.1 (Update):** Migrated to **Axmol Engine** for modern cross-platform maintenance.
3. **Lite/Legacy (Current):** A complete rewrite in **Cocos2d-x 2.1.5** for educational purposes.

---

## 🛠️ How to Build

To keep the repository clean, the `proj.win32` folder is not included. Follow these steps to build the project from scratch:

### 1. Download the Engine
Get **Cocos2d-x v2.1.5** from this repository made by Chetverockha:  
[Cocos2d-x v2.1.5 Download](https://github.com/chetverochka/Cocos2d-x-2.1.5-modern-build/releases/tag/v1.2)

### 2. Create a New Project
Use the Python script included in the engine folder to generate a new project:

```bash
python create_project.py -project OrbitDash -package com.urafaelx.orbitdash -language cpp
```

### 3. Replace Files
* Replace the `Classes` folder of your new project with the one from this repository.
* Replace the `Resources` folder with the one from this repository.

### 4. Resolution Setup (Crucial ⚠️)
Open your `proj.win32/main.cpp` and set the f1rame size. The UI is designed for a specific aspect ratio; failing to set this will break the UI layout:

```cpp
// Inside main.cpp:
CCEGLView* eglView = CCEGLView::sharedOpenGLView();
eglView->setFrameSize(361, 680); 
```

### 5. Compile
Open the `.sln` file in **Visual Studio 2019** or newer and build in **Release** mode.

---

## 🎓 Educational Highlights

If you are looking to learn from this repository, focus on these areas:

* **Advanced Touch Handling:** See how `TouchDispatcher` is used with negative priorities (e.g., `-500`) to create "impenetrable" modal popups.
* **Scene Transitions:** Learn how to handle navigation between the Main Menu, Settings, and Gameplay scenes using classic Cocos2d-x patterns.
* **Win32 Integration:** Implementation of `ShellExecuteA` to bridge the game with the Windows OS for external links.
* **C++ Memory Management:** A practical look at reference counting without the safety net of modern smart pointers.
* **Custom UI Components:** Study `PopupLayer` and `CCMenuItemSpriteExtra` to understand how to build reusable UI elements.
* **Animation Sequences:** Master the art of combining `CCSpawn`, `CCSequence`, and easing functions for fluid animations.

---

## 📦 Repository Structure

```
OrbitDash-Lite/
├── Classes/          # The "brain" of the game. All .cpp and .h files
│   ├── PopupLayer.h/cpp
│   ├── CCMenuItemSpriteExtra.h/cpp
│   ├── MenuLayer.h/cpp
│   ├── GameLayer.h/cpp
│   └── ...
├── Resources/        # All game assets (Sprites, Bitmap Fonts, Audio)
│   ├── OD_ComingSoonLabel.png
│   ├── OD_OkButton.png
│   └── ...
└── README.md
```

---

## 🐛 Known Issues & Tips

* **Memory Leaks:** If you modify touch handlers, remember to properly remove listeners in `onExit()` or `removeFromParent()`.
* **Menu Positioning:** In Cocos2d-x 2.1.5, `CCMenu` defaults to center positioning. Always call `menu->setPosition(CCPointZero)` when using absolute coordinates.
* **Visual Studio Compatibility:** Works best with VS2019. VS2022 may require additional configuration.
* **Touch Priority:** Lower numbers = higher priority. Use negative values (e.g., `-500`) for modal popups that should block all other touches.

---

## 🔧 Code Examples

### Creating a Modal Popup

```cpp
// In your scene:
PopupLayer* popup = PopupLayer::create();
this->addChild(popup, 1000); // High z-order to appear on top
```

### Custom Menu Item with Callback

```cpp
CCSprite* buttonSprite = CCSprite::create("button.png");
CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(
    buttonSprite,
    this,
    menu_selector(MyClass::onButtonClicked)
);
```

### Smooth Animation Sequence

```cpp
CCFadeIn* fadeIn = CCFadeIn::create(0.3f);
CCScaleTo* scaleUp = CCScaleTo::create(0.3f, 1.2f);
CCSpawn* spawn = CCSpawn::create(fadeIn, scaleUp, NULL);
CCEaseBackOut* ease = CCEaseBackOut::create(spawn);
sprite->runAction(ease);
```

---

## 🚀 Credits

Developed by **Urafael Games**.

This project is shared with the community to keep the history of game development alive and to show that with good logic, you can create "cool stuff" regardless of the engine version.

---

## 📄 License

This project is open-source for educational purposes. Feel free to learn from it, modify it, and share your improvements!

---

## 💬 Community

Have questions or found a bug? Open an issue or reach out!  
Let's keep the legacy of Cocos2d-x alive together. 🎮

---

## 🎯 Future Improvements

- [ ] Add Android build instructions
- [ ] Include iOS project setup guide
- [ ] Create video tutorial series
- [ ] Add more custom UI components examples
- [ ] Performance optimization guide for low-end devices

---

**"Because great games aren't defined by the engine version, but by the passion behind the code."** ✨

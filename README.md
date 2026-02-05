# OrbitDash Lite  (Legacy Edition)

**OrbitDash Lite** is a polished arcade game rewritten from the ground up for **Cocos2d-x v2.1.5**. 

While originally developed in modern engines like **Cocos2d-x 4.0** and **Axmol**, this version was created to demonstrate that high-quality gameplay and sophisticated UI can still be achieved using "legacy" frameworks. It serves as both a personal gift and a technical case study for the developer community.

---

## 🌟 Why this version? (The "Cool Stuff")
Many developers assume that older versions of Cocos2d-x are obsolete. This project proves otherwise by showcasing:
* **Polished UI/UX:** Smooth popups and interactive menus built entirely through code.
* **Modern Logic in Legacy Syntax:** Back-porting features from Axmol/Cocos4 back to the classic 2.1.5 API.
* **Pure Performance:** Blazing fast execution and low overhead, typical of the Win32 era.
* **Syntax Mastery:** A deep dive into the `menu_selector`, `schedule_selector`, and manual memory management (`retain`/`release`) that defined mobile gaming's golden age.

---

## 📜 Project Evolution
1. **v1.0 (Original):** Built in **Cocos2d-x 4.0** (Modern C++, Metal/Vulkan support).
2. **v1.1 (Update):** Migrated to **Axmol Engine** for modern cross-platform maintenance.
3. **Lite/Legacy (Current):** A complete rewrite in **Cocos2d-x 2.1.5** for educational purposes.

---

## 🎓 Educational Highlights
If you are looking to learn from this repository, focus on these areas:

* **Advanced Touch Handling:** See how `TouchDispatcher` is used with negative priorities (e.g., `-500`) to create "impenetrable" modal popups.
* **Scene Transitions:** Learn how to handle navigation between the Main Menu, Settings, and Gameplay scenes using classic Cocos2d-x patterns.
* **Win32 Integration:** Implementation of `ShellExecuteA` to bridge the game with the Windows OS for external links.
* **C++ Memory Management:** A practical look at how the engine's reference counting system works without the safety net of modern smart pointers.

---

## 🛠️ Build & Technical Requirements
* **Environment:** Visual Studio 2019 or higher.
* **Target OS:** Windows (Win32).
* **Dependencies:** Included in the project (Cocos2d-x 2.1.5 core libs).

> **Note:** To run the compiled version, ensure the `.dll` files and the `Resources` content are located in the same directory as the `.exe`.

---

## 📦 Repository Structure
* `/Classes`: The "brain" of the game. All `.cpp` and `.h` files.
* `/Resources`: All game assets (Sprites, Bitmap Fonts, Audio).
* `/proj.win32`: Visual Studio solution and Windows-specific entry points.

---

## 🚀 Credits
Developed by **Urafael**.

This project is shared with the community to keep the history of game development alive and to show that with good logic, you can create "cool stuff" regardless of the engine version.

---
*Created with ❤️ for a special someone and for the global dev community.*

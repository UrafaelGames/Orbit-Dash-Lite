# OrbitDash Lite (Legacy Edition)

**OrbitDash Lite** is an educational rewrite of the original game, specifically developed for the **Cocos2d-x v2.1.5** engine. 

This project serves as a "retro-programming" exercise, where the modern logic from current engine versions was adapted to function within a classic C++ game development environment.

---

## 📜 Project Evolution
The development of OrbitDash has spanned three major technical milestones:
1. **v1.0 (Original):** Launched using **Cocos2d-x 4.0** (Modern rendering pipeline).
2. **v1.1 (Update):** Migrated to **Axmol Engine** for enhanced cross-platform compatibility and maintenance.
3. **Lite Version (Current):** A ground-up reconstruction in **Cocos2d-x 2.1.5** for educational purposes and as a personal gift.

---

## 🎓 Educational Objectives
The source code of this version is designed to demonstrate how to solve technical challenges in "legacy" engines:

* **Layer Management & Touch Priority:** Implementation of popups (Support, Settings, Game Over) that effectively block interaction with underlying layers using the manual `TouchDispatcher` and negative priorities (e.g., `-500`).
* **Object Lifecycle:** Practical use of `onEnter()` and `onExit()` for dynamic input delegate registration.
* **Data Persistence:** Handling high scores and local settings using the `CCUserDefault` system.
* **Native Win32 Integration:** Utilizing `ShellExecuteA` to trigger external system links (social media/support) directly from the Windows executable.
* **Memory Optimization:** An example of transitioning from modern smart pointers back to manual `retain` and `release` memory management.

---

## 🛠️ Build Requirements
To run or modify this project, you will need:
* **IDE:** Visual Studio 2019 or higher.
* **Engine:** Cocos2d-x v2.1.5.
* **Platform:** Windows (Win32).

> **Note:** Ensure that the engine path is correctly linked in the Visual Studio project properties to match your local environment.

---

## 📦 Repository Structure
* `/Classes`: Game logic, scene management, and UI layers.
* `/Resources`: Visual assets (PNG, .fnt fonts) and audio files.
* `/proj.win32`: Visual Studio project files, icons, and the Win32 entry point.

---

## 🚀 Credits & License
Developed with dedication by **Urafael**. 

This code is distributed for educational purposes. Feel free to explore, modify, and learn from the foundations of one of the most iconic game engines of the past decade.

---
*Created with ❤️ as a special gift and for the developer community.*

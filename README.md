# Minecraft (OpenGL C++)

Prosty projekt inspirowany grą Minecraft, napisany w C++ z użyciem OpenGL.
Zawiera podstawowy rendering świata blokowego, system chunków oraz generowanie terenu (noise).

---

## 📦 Tech Stack

* C++
* OpenGL
* GLFW (window/input)
* GLEW (OpenGL extensions)
* GLM (math)
* stb_image (tekstury)

---

## 📁 Struktura projektu

```
Mine/
├── scripts/
│   ├── main.cpp                # entry point
│   ├── player/
│   │   ├── Camera.h
|   |   └── PlayerAction.h           
│   ├── shaders/
│   │   ├── vertexShader.glsl
│   │   ├── fragmentShader.glsl
│   │   └── Shader_H.h
│   └── worldRender/
│       ├── block.cpp / .h
│       ├── chunk.cpp / .h
│       ├── chunkManager.h
│       └── Noise/
│           ├── PerlinNoise.h
│           └── SimpleNoise.h
│
├── graphics/                   # tekstury
├── dependencies/               # biblioteki (GLFW, GLEW, GLM)
└── Mine.vcxproj               # projekt Visual Studio
```

---

## ⚙️ Wymagania

* Visual Studio (zalecane 2022)
* Windows x64
* GPU z obsługą OpenGL
* Zainstalowane:

  * Desktop development with C++

---

## 🚀 Jak uruchomić

### 1. Klonuj repo

```bash
git clone https://github.com/Tssara997/Minecraft.git
cd Minecraft
```

---

### 2. Otwórz projekt

* Otwórz plik:

  ```
  Mine.slnx
  ```
* w Visual Studio

---

### 3. Ustaw konfigurację

* Platforma: **x64**
* Tryb: **Debug** (na start)

---

### 4. Build

* Kliknij:

  ```
  Build → Build Solution (Ctrl + Shift + B)
  ```

---

### 5. Run

* Kliknij:

  ```
  Local Windows Debugger (F5)
  ```

---

## 📌 Alternatywnie (manual run)

Po buildzie exe znajduje się w:

```
Mine/x64/Debug/Mine.exe
```

Uruchom bezpośrednio:

```bash
./Mine.exe
```

---

## 🎮 Funkcjonalności

* renderowanie bloków (voxel)
* system chunków
* generowanie świata (Perlin / Simple noise)
* kamera FPS
* nieszczenie bloków

---

## ⚠️ Typowe problemy

### ❌ Brak DLL (glew32.dll)

Rozwiązanie:

* upewnij się, że plik:

  ```
  glew32.dll
  ```

  jest w tym samym folderze co `.exe`

---

### ❌ Okno się nie otwiera

Sprawdź:

* czy GPU obsługuje OpenGL
* czy sterowniki są aktualne

---

### ❌ Błędy linkera

Projekt używa lokalnych bibliotek w:

```
dependencies/lib/
```

Jeśli coś się wysypie:

* sprawdź ustawienia:

  * **Linker → Input**
  * **VC++ Directories**

---

## 📈 Możliwe ulepszenia

* lighting (Phong / PBR)
* meshing chunków (greedy meshing)
* input system (mysz + keyboard)
* kolizje
* zapis świata

---

## 📜 Licencja

Brak — dodaj jeśli planujesz udostępniać projekt publicznie.

---

## 👤 Autor

* https://github.com/Tssara997

---

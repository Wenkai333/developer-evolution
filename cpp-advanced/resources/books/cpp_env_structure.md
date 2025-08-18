# C++ Development Environment - File Structure & Relationships

## 📁 Project Directory Structure

```
cpp-advanced-study/
├── setup_study_day.sh         # 🔧 Creates day-specific folders
├── verify_setup.sh            # ✅ Tests environment health
├── profile.sh                 # 📊 Performance profiling script
├── .gitignore                  # 🚫 Git exclusion rules
├── day1/                       # 📚 Study Day 1
│   ├── CMakeLists.txt         # 🔨 Main build configuration
│   ├── build.sh               # ⚡ Quick build script
│   ├── run.sh                 # ▶️ Execute programs
│   ├── build/                 # 📦 Compiled outputs (auto-created)
│   ├── examples/              # 💡 Learning examples
│   │   └── CMakeLists.txt    # 🔨 Examples build rules
│   ├── exercises/             # 🏋️ Practice problems  
│   │   └── CMakeLists.txt    # 🔨 Exercises build rules
│   ├── project/               # 🎯 Main project work
│   │   └── CMakeLists.txt    # 🔨 Project build rules
│   └── notes/                 # 📝 Study notes
├── day2/                       # 📚 Study Day 2 (same structure)
└── day3/                       # 📚 Study Day 3 (same structure)
```

## 🔗 VS Code Configuration Files

```
.vscode/
├── settings.json              # ⚙️ Editor & compiler settings
├── launch.json               # 🐛 Debug configurations
└── tasks.json                # 🔨 Build tasks (auto-generated)
```

## 🏠 Home Directory Configuration

```
~/
├── .lldbinit                 # 🐛 LLDB debugger settings
├── .zshrc                    # 🔧 Shell environment (PATH, etc.)
└── .gitconfig                # 📝 Git user settings
```

---

## 🔄 File Relationships & Workflow

### 1. **Setup Phase** 🚀
```
setup_study_day.sh → Creates folder structure
                   → Generates CMakeLists.txt files  
                   → Creates build/run scripts
```

### 2. **Development Cycle** 🔄
```
Write Code (.cpp) → CMakeLists.txt (defines build rules)
                  → build.sh (compiles using CMake + Ninja)
                  → run.sh (executes the program)
                  → VS Code (editing + debugging)
```

### 3. **Configuration Chain** ⚙️
```
.zshrc → Sets LLVM compiler path
       → VS Code settings.json → Uses LLVM compiler
                                → CMakeLists.txt → Uses same compiler
                                                 → Generates compile_commands.json
                                                 → VS Code IntelliSense reads this
```

### 4. **Debugging Flow** 🐛
```
Code with -g flag → Build creates debug symbols
                  → .lldbinit configures debugger
                  → launch.json tells VS Code how to debug
                  → F5 starts debugging session
```

---

## 🎯 Key File Purposes

| File | Purpose | Relationship |
|------|---------|--------------|
| **CMakeLists.txt** | Build configuration | Parent → Child CMake files |
| **settings.json** | VS Code configuration | Uses compiler from .zshrc |
| **build.sh** | Quick build script | Calls CMake with specific options |
| **run.sh** | Execute programs | Runs binaries from build/ folder |
| **.zshrc** | Shell environment | Sets paths for all tools |
| **.lldbinit** | Debugger config | Used by launch.json |
| **setup_study_day.sh** | Folder generator | Creates consistent structure |
| **verify_setup.sh** | Environment test | Checks all dependencies |
| **profile.sh** | Performance analysis | Uses tools installed by brew |

---

## 🚀 Typical Daily Workflow

1. **Start**: `./setup_study_day.sh day1` → Creates structure
2. **Code**: Open VS Code → Write C++ in examples/exercises/project
3. **Build**: `./build.sh` → Compiles everything
4. **Run**: `./run.sh program_name` → Execute specific program
5. **Debug**: F5 in VS Code → Interactive debugging
6. **Profile**: `./profile.sh program_name` → Performance analysis
7. **Verify**: `./verify_setup.sh` → Check environment health

---

## 🎨 Visual Representation

```
🖥️  Terminal                    💻 VS Code                   🔧 System
┌─────────────────┐            ┌──────────────────┐          ┌────────────┐
│ setup_study_day │──────────→ │ Auto-opens with  │          │ .zshrc     │
│ build.sh        │            │ correct settings │←─────────│ .lldbinit  │
│ run.sh          │            │                  │          │ Homebrew   │
│ profile.sh      │            │ IntelliSense ✨  │          │ tools      │
└─────────────────┘            │ Debugging 🐛     │          └────────────┘
                               │ Auto-format 📝   │
📁 Project Structure           └──────────────────┘
┌─────────────────┐                     ↕️
│ CMakeLists.txt  │←───── Build System ──────────────────────┐
│ Source files    │                                          │
│ Build folder    │                                          ▼
└─────────────────┘                               📦 Compiled Programs
```

This structure ensures everything works together seamlessly - from writing code to debugging to performance analysis!
# Mansion Tycoon - AI Management Simulation

A browser-based business simulation game built with Cocos2d-x featuring AI-driven gameplay and procedurally generated content.

## 🎮 Game Overview

Manage a luxury entertainment venue where you make strategic decisions that affect your business, staff, and guests. The AI engine generates dynamic events and characters that adapt to your play style.

## ✨ Key Features

### AI-Powered Gameplay
- **Dynamic Event Generation**: AI creates unique scenarios based on your progress and reputation
- **Learning System**: The game tracks your decisions and generates more relevant events
- **Adaptive Difficulty**: Challenges scale with your success

### Character System
- **Procedural Generation**: Each staff member and guest is uniquely generated
- **Individual Traits**: Characters have personalities, preferences, and satisfaction levels
- **Relationship Management**: Your decisions affect how characters feel about your venue

### Business Management
- **Financial Strategy**: Balance income, expenses, and investments
- **Reputation System**: Build your venue's prestige to attract VIP guests
- **Staff Management**: Hire and maintain a skilled team
- **Guest Satisfaction**: Keep visitors happy for recurring revenue

### Progressive Gameplay
- Day-by-day progression with new challenges
- Expansion opportunities and business partnerships
- Multiple decision paths with different outcomes

## 🛠️ Technical Features

- **Engine**: Cocos2d-x game engine
- **AI Engine**: Custom decision-making and generation system with learning capabilities
- **Save System**: JSON-based persistence inspired by Cocos2d-x UserDefault patterns
- **UI Framework**: Enhanced with scene transitions and particle effects
- **Platform**: Browser-based (via Emscripten/WebAssembly)
- **Architecture**: Modular design with separate AI, game management, and UI layers

## ⭐ NEW FEATURES (Enhanced from GitHub Research)

### 💾 Save/Load System
- **Quick Save/Load**: Instantly save and restore your progress
- **Auto-Save**: Optional automatic saving every 5 minutes
- **Multiple Slots**: Support for different save files
- **JSON Format**: Human-readable save files for debugging
- Inspired by Cocos2d-x's UserDefault and file persistence patterns

### 🎨 Enhanced Visual Effects
- **Particle Systems**: Celebration effects for achievements, smoke for setbacks
- **Scene Transitions**: Smooth fade effects when loading games
- **Animated UI**: Bounce and scale animations for event panels
- **Gradient Backgrounds**: Atmospheric visual depth
- **Button Animations**: Responsive feedback on all interactions

### 🧠 Advanced AI Patterns (from Open Source Research)
- **Weighted Event Generation**: Events adapt to your play style
- **State Machine Logic**: Coherent progression of challenges
- **Dynamic Difficulty**: Adjusts based on your performance
- **Pattern Recognition**: Learns your preferences over time

### 🎯 Improved UX
- **Visual Feedback**: Clear indicators for all actions
- **Animated Notifications**: Floating text for saves and achievements
- **Enhanced Button Design**: Professional UI components
- **Character Panel**: Real-time staff and guest monitoring

## 🏗️ Project Structure

```
Classes/
├── AIEngine.h/cpp          # AI event and character generation
├── GameManager.h/cpp       # Game state and logic management
├── GameScene.h/cpp         # UI and visual presentation
├── AppDelegate.h/cpp       # Application initialization
└── main.cpp                # Entry point
```

## 🎯 Gameplay Loop

1. **Day begins** with current stats displayed (money, reputation, day count)
2. **Event occurs** presenting a scenario with multiple choices
3. **Make decision** from 2-3 options, each with different impacts
4. **View results** showing changes to money, reputation, and satisfaction
5. **Day advances** with daily income/expenses and character updates
6. **Repeat** with AI-generated events tailored to your play style

## 💡 Game Mechanics

### Resources
- **Money**: Used for investments, staff salaries, and expansion
- **Reputation**: Attracts better guests and opportunities
- **Satisfaction**: Affects staff retention and guest loyalty

### Events
- Business decisions (partnerships, expansions)
- Staff management (training, issues)
- Guest requests (VIP demands, special events)
- Reputation events (media attention, reviews)
- Financial challenges
- Competition scenarios

### AI Learning
The AI tracks your decision patterns:
- Choice preferences per event type
- Risk tolerance (conservative vs aggressive)
- Investment priorities
- Management style

Future events adapt to match your play style.

## 🚀 Building for Browser

This game can be compiled to WebAssembly using Emscripten to run in web browsers.

### Prerequisites
- Cocos2d-x v4
- Emscripten SDK
- CMake 3.10+

### Build Commands
```bash
# Setup Emscripten
source /path/to/emsdk/emsdk_env.sh

# Configure with Emscripten
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build-web

# The output will be in build-web/bin/MansionSim.js and .wasm
```

### Deploying
1. Copy `index.html`, `MansionSim.js`, and `MansionSim.wasm` to your web server
2. Serve with proper MIME types for .wasm files
3. Ensure CORS headers if loading from different origin

## 🎨 Design Philosophy

This game focuses on:
- **Strategic thinking** over reflexes
- **Meaningful choices** with visible consequences
- **Emergent gameplay** through AI-generated content
- **Replayability** via adaptive AI learning
- **Appropriate content** suitable for all audiences

## 📊 Stats Tracking

The game tracks:
- Total money earned
- Peak reputation reached
- Days survived
- Staff hired
- Guests served
- Decisions made (by type)
- AI-generated events encountered

## 🔮 Future Enhancements

Potential additions:
- Save/load system
- Multiple venues/locations
- Advanced character interactions
- Seasonal events
- Achievement system
- Leaderboards
- More event types
- Enhanced AI learning algorithms

## 📜 License

This project uses Cocos2d-x which is licensed under the MIT license.

## 🤝 Contributing

This is a demonstration project showing how to build an AI-driven simulation game with appropriate content and engaging mechanics.

---

**Enjoy building your entertainment empire!** 🏛️✨

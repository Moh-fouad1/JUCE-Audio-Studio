# 🎵 Dual Audio Player

A professional dual-channel audio player application built with the JUCE framework. This application features two independent audio players that can be mixed together, with advanced playback controls, waveform visualization, and session management.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![JUCE](https://img.shields.io/badge/JUCE-Framework-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## ✨ Features

### 🎛️ Dual Player System
- **Two Independent Players**: Play different audio files simultaneously in separate channels
- **Audio Mixing**: Real-time mixing between left and right players with a dedicated mix slider
- **Independent Controls**: Each player has its own set of controls and playlist

### 🎧 Playback Features
- **Multiple Format Support**: Supports WAV, MP3, AAC, FLAC, OGG, and M4A formats
- **Playlist Management**: Create and manage playlists with metadata display
- **Shuffle Mode**: Randomize playlist playback order
- **Looping**: Single track looping and A-B loop regions
- **Playback Speed Control**: Adjustable playback speed from 0.5x to 2.0x
- **Seek Controls**: 
  - Seek slider for precise position control
  - Jump to start/end buttons
  - Skip forward/backward by 10 seconds
  - Restart button

### 📊 Visualization
- **Waveform Display**: Real-time waveform visualization with playhead indicator
- **Interactive Waveform**: Click on waveform to jump to any position
- **Played Portion Highlighting**: Visual feedback showing played vs. remaining audio

### 🎯 Advanced Features
- **Markers**: Add, delete, and jump to custom markers in audio tracks
- **A-B Looping**: Set loop points A and B for precise section looping
- **Session Persistence**: Automatically saves and restores playback state
- **Metadata Display**: Shows track title, artist, album, and duration
- **Volume Control**: Independent volume control with mute functionality

### 🎨 User Interface
- **Modern Dark Theme**: Sleek, professional dark interface
- **Scrollable Layout**: Smooth scrolling for dual player interface
- **Responsive Design**: Clean, organized layout with intuitive controls

## 🏗️ Architecture

### Design Patterns

This application demonstrates several important object-oriented design patterns:

#### 1. **Polymorphism**
- Multiple interface inheritance (`PlayerGUI` implements `Component`, `Button::Listener`, `Slider::Listener`, `Timer`, `ListBoxModel`)
- Virtual function overriding for framework integration
- Runtime polymorphism through JUCE's component system

#### 2. **API Pattern (Listener/Callback Pattern)**
- Event-driven architecture using JUCE's listener interfaces
- Eliminates conditional type checking in framework code
- Clean separation between framework and application logic

#### 3. **Composition & Aggregation**
- Strong ownership relationships (composition)
- Component-based architecture
- Modular design for easy extension

For detailed explanations of these patterns, see [readme.md](readme.md).

### Project Structure

```
Source/
├── Main.cpp              # Application entry point
├── MainComponent.h/cpp   # Main application component with dual players
├── PlayerGUI.h/cpp       # Individual player GUI and logic
├── PlayerAudio.h/cpp     # Audio playback engine
└── WaveformDisplay.h/cpp # Waveform visualization component
```

## 🚀 Getting Started

### Prerequisites

- **Visual Studio 2022** (or compatible IDE)
- **JUCE Framework** (included in project)
- **Windows 10/11** (or compatible OS)

### Building the Project

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd "assignment 2 El-Ramly"
   ```

2. **Open the Solution**
   - Open `assignment 2 El-Ramly.sln` in Visual Studio 2022
   - Or open `assignment 2 El-Ramly.jucer` in Projucer (JUCE's project manager)

3. **Build the Project**
   - Select your build configuration (Debug/Release)
   - Build the solution (F7 or Build → Build Solution)
   - The executable will be generated in `x64/Debug/App/` or `x64/Release/App/`

4. **Run the Application**
   - Execute `assignment 2 El-Ramly.exe`
   - Or run directly from Visual Studio (F5)

## 📖 Usage

### Basic Playback

1. **Load Audio Files**
   - Click "Load Files" button in either player
   - Select one or more audio files from the file dialog
   - Files are automatically added to the playlist

2. **Playback Controls**
   - **Play/Pause**: Start or pause playback
   - **Prev/Next**: Navigate between tracks in playlist
   - **Restart**: Jump to beginning of current track
   - **Shuffle**: Randomize playlist order

3. **Navigation**
   - **Go To Start/End**: Jump to beginning or end of track
   - **-10s / +10s**: Skip backward or forward by 10 seconds
   - **Seek Slider**: Drag to jump to any position
   - **Waveform Click**: Click anywhere on waveform to seek

### Advanced Features

1. **Markers**
   - Click "Add Marker" to create a marker at current position
   - Click on marker in list to jump to that position
   - Use "Delete Marker" to remove selected marker

2. **A-B Looping**
   - Click "Set A" to mark loop start point
   - Click "Set B" to mark loop end point
   - Playback will automatically loop between A and B
   - Click "Clear A-B" to remove loop points

3. **Speed Control**
   - Adjust speed slider (0.5x to 2.0x)
   - Useful for slow-motion analysis or fast playback

4. **Mixing**
   - Use the mix slider at the bottom to blend between left and right players
   - Left position: Only left player audible
   - Right position: Only right player audible
   - Center: Equal mix of both players

### Session Management

- Playback state is automatically saved every second
- Position, volume, speed, loop state, and markers are preserved
- Session is restored when application restarts
- Each player maintains its own session (playerA and playerB)

## 🛠️ Technical Details

### Technologies Used

- **C++17**: Modern C++ features
- **JUCE Framework**: Cross-platform C++ application framework
  - Audio processing and playback
  - GUI components and rendering
  - File I/O and format support
- **Visual Studio 2022**: Development environment

### Key Components

- **PlayerAudio**: Handles audio file loading, playback, and format management
- **PlayerGUI**: Manages UI, controls, playlist, and user interactions
- **WaveformDisplay**: Renders audio waveform with interactive seeking
- **MainComponent**: Coordinates dual players and audio mixing

### Audio Processing

- Real-time audio resampling for speed control
- Multi-channel audio mixing
- Low-latency audio processing
- Automatic format detection and decoding

## 📝 Code Quality

- Clean, well-organized code structure
- Object-oriented design principles
- Comprehensive use of design patterns
- JUCE framework best practices
- Memory-safe resource management

## 🤝 Contributing

This is an educational project demonstrating:
- Object-oriented programming principles
- Design pattern implementation
- JUCE framework integration
- Audio application development

## 📄 License

This project is provided for educational purposes.

## 👤 Author

**El-Ramly**

## 🙏 Acknowledgments

- **JUCE Framework**: For providing an excellent audio application framework
- Design patterns and architecture based on software engineering best practices

---

**Note**: This application is designed for educational purposes to demonstrate object-oriented programming concepts, design patterns, and audio application development with the JUCE framework.


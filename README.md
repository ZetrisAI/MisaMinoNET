# MisaMinoNET
[![NuGet - Download](https://img.shields.io/badge/nuget-download-orange)](https://www.nuget.org/packages/MisaMinoNET/)
[![Discord - Join Chat](https://img.shields.io/badge/discord-join%20chat-blue)](https://discord.gg/vfrmzUV)

.NET version of the MisaMino bot (via DLL importing).

## Installation and Usage Example

Use MisaMinoNET from [NuGet](https://www.nuget.org/packages/MisaMinoNET/) in your project.

```cs
using MisaMinoNET;

// Listen for search completion
MisaMino.Finished += ...;

// Start search in the background
MisaMino.FindMove(...);

// Abort search prematurely
MisaMino.Abort();

// Access results of last search
MisaMino.LastSolution;

// Utilize pathfinder
MisaMino.FindPath(...);
```

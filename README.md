GBPP
====

__Graphics Programming Black Book__ by [__Michael Abrash__](http://en.wikipedia.org/wiki/Michael_Abrash).

This repository contains the PDF version of the book as well as the
unmodified source code written by the author himself.


Table of Contents
-----------------

1. [__The Best Optimizer is between Your Ears__][01]
  - The Human Element of Code Optimization
  - Understanding High Performance
    - *When Fast Isn't Fast*
  - Rules for Building High-Performance Code
    - *Know Where You're Going*
    - *Make a Big Map*
    - *Make Lots of Little Maps*
    - *Know the Territory*
    - *Know When It Matters*
    - *Always Consider the Alternatives*
    - *Know How to Turn On the Juice*
2. [__A World Apart__][02]
  - The Unique Nature of Assembly Language Optimization
  - Instructions: The Individual versus the Collective
  - Assembly Is Fundamentally Different
    - *Transformation Inefficiencies*
    - *Self-Reliance*
    - *Knowledge*
  - The Flexible Mind
    - *Where to Begin?*
3. [__Assume Nothing__][03]
  - Understanding and Using the Zen Timer
  - The Costs of Ignorance
  - The Zen Timer
    - *The Zen Timer Is a Means, Not an End*
    - *Starting the Zen Timer*
  - Time and the PC
  - Stopping the Zen Timer
  - Reporting Timing Results
  - Notes on the Zen Timer
  - A Sample Use of the Zen Timer
  - The Long-Period Zen Timer
    - *Stopping the Clock*
  - Example Use of the Long-Period Zen Timer
  - Using the Zen Timer from C
    - *Watch Out for Optimizing Assemblers!*
    - *Further Reading*
    - *Armed with the Zen Timq Onward and Upward*
4. [__In the Lair of the Cycle-Eaters__][04]
  - How the PC Hardware Devours Code Performance
  - Cycle-Eaters
  - The Nature of Cycle-Eaters
    - *The 8088's Ancestral Cycle-Eaters*
  - The 8-Bit Bus Cycle-Eater
    - *The Impact of the 8-Bit Bus Cycle-Eater*
    - *What to Do about the 8-Bit Bus Cycle-Eater?*
  - The Prefetch Queue Cycle-Eater*
    - *Official Execution Times Are Only Part of the Story*
    - *There Is No Such Beast as a True Instruction Execution Time*
    - *Approximating Overall Execution Times*
    - *What to Do about the Prefetch Queue Cycle-Eater?*
    - *Holding Up the 8088*
  - Dynamic RAM Refresh: The Invisible Hand
    - *How DRAM Refresh Works in the PC*
    - *The Impact of DRAM Refresh*
    - *What to Do About the DRAM Refresh Cycle-Eater?*
  - Wait States
  - The Display Adapter Cycle-Eater
    - *The Impact of the Display Adapter Cycle-Eater*
    - *What to Do about the Display Adapter Cycle-Eater?*
    - *Cycle-Eaters: A Summary*
    - *What Does It All Mean?*
5. [__Crossing the Border__][05]
  - Searching Files with Restartable Blocks
    - *Searching for Text*
  - Avoiding the String Trap
  - Brute-Force Techniques
  - Using memchr()
    - *Making a Search Restartable*
  - Interpreting Where the Cycles Go
    - *Knowing When Assembly Is Pointless*
  - Always Look Where Execution Is Going
6. [__Looking Past Face Value__][06]
  - How Machine Instructions May Do More Than You Think
    - *Memory Addressing and Arithmetic*
  - Math via Memory Addressing
    - *The Wonders of LEA on the 386*
  - Multiplication with LEA Using Non-Powers of Two
7. [__Local Optimization__][07]
  - Optimizing Halfway between Algorithms and Cycle Counting
    - *When LOOP Is a Bad Idea*
  -The Lessons of LOOP and JCXZ
    - *Avoiding LOOPS of Any Stripe*
  - Local Optimization
  - Unrolling Loops
    - *Rotating and Shifting with Tables*
    - *NOT Flips Bits -- Not Flags*
    - *Incrementing with and without Carry*
8. [__Speeding Up C with Assembly Language__][08]
  - Jumping Languages When You Know It'll Help
    - *Billy, Don't Be a Compiler*
  - Don't Call Your Functions on Me, Baby
  - Stack Frames Slow So Much
  - Torn Between Two Segments
    - *Why Speeding Up Is Hard to Do*
  - Taking It to the Limit
    - *A C-to-Assembly Case Study*
9. [__Hints My Readers Gave Me__][09]
  - Optimization Odds and Ends from the Field
    - *Another Look at LEA*
    - *The Kennedy Portfolio*
    - *Speeding Up Multiplication*
    - *Optimizing Optimized Searching*
    - *Short Sorts*
    - *Full 32-Bit Division*
    - *Sweet Spot Revisited*
    - *Hard-core Cycle Counting*
    - *Hardwired Far Jumps*
    - *Setting 32-Bit Registers: Time versus Space*
10. [__Patient Coding, Faster Code__][10]
  - How Working Quickly Can Bring Execution to a Crawl
    - *The Case for Delayed Gratification*
  - The Brute-Force Syndrome
    - *Wasted Breakthroughs*
  - Recursion
    - *Patient Optimization*
11. [__Pushing the 286 and 386__][11]
  - New Registers, NewInstructions, New Timings, New Complications
  - Family Matters
  - Crossing the Gulf to the 286 and the 386
  - In the Lair of the Cycle-Eaters, Part II
    - *System Wait States*
    - *Data Alignment*
  - Code Alignment
    - *Alignment and the 386*
    - *Alignment and the Stack*
    - *The DRAM Refresh Cycle-Eater: Still an Act of God*
    - *The Display Adapter Cycle-Eater*
  - New Instructions and Features: The 286
  - New Instructions and Features: The 386
    - *Optimization Rules: The More Things Change...*
    - *Detailed Optimization*
  - popf and the 286
12. [__Pushing the 486__][12]
  - It's Not Just a Bigger 386
    - *Enter the 486*
  - Rules to Optimize By
    - *The Hazards of Indexed Addressing*
    - *Calculate Memory Pointers Ahead of Time*
  - Caveat Programmor
    - *Stack Addressing and Address Pipelining*
    - *Problems with Byte Registers*
    - *More Fun with Byte Registers*
    - *Timing Your Own 486 Code*
  - The Story Continues
13. [__Aiming the 486__][13]
  - Pipelines and Other Hazards of the High End
    - *486 Pipeline Optimization*
  - BSWAP: More Useful Than You Might Think
  - Pushing and Popping Memory
  - Optimal 1-Bit Shifts and Rotates
  - 32-Bit Addressing Modes
14. [__Boyer-Moore String Searching__][14]
  - Optimizing a Pretty Optimum Search Algorithm
  - String Searching Refresher
  - The Boyer-Moore Algorithm
  - Boyer-Moore: The Good and the Bad
  - Further Optimization of Boyer-Moore
    - *Know What You Know*
15. [__Linked Lists and Unintended Challenges__][15]
  - Unfamiliar Problems with Familiar Data Structures
  - Linked Lists
  - Dummies and Sentinels
  - Circular Lists
  - Hi/Lo in 24 Bytes
16. [__There Ain't No Such Thing as the Fastest Code__][16]
  - Lessons Learned in the Pursuit of the Ultimate Word Counter
  - Counting Words in a Hurry
    - *Which Way to Go from Here?*
  - Challenges and Hazards
    - *Blinding Yourself to a Better Approach*
    - *Watch Out for Luggable Assumptions!*
  - The Astonishment of Right-Brain Optimization
  - Levels of Optimization
    - *Optimization Level 1: Good Code*
  - Level 2: A New Perspective
    - *Level 3: Breakthrough*
    - *Enough Word Counting Already!*
17. [__The Game of Life__][17]
  - The Triumph of Algorithmic Optimization in a Cellular Automata Game
  - Conway's Game
    - *The Rules of the Game*
  - Where Does the Time Go?
  - The Hazards and Advantages of Abstraction
  - Heavy-Duty C++ Optimization
  - Bringing In the Right Brain
    - *Re-Examining the Task*
    - *Acting on What We Know*
    - *The Challenge That Ate My Life*
18. [__It's a Wonderful Life__][18]
  - Optimization beyond the Pale
  - Breaking the Rules
  - Table-Driven Magic
  - Keeping Track of Change with a Change List
    - *A Layperson's Overview of QLIFE*
19. [__Pentium: Not the Same Old Song__][19]
  - Learning a Whole Different Set of Optimization Rules
  - The Return of Optimization as Art
  - The Pentium: An Overview
    - *Crossing Cache Lines*
    - *Cache Organization*
  - Faster Addressing and More
  - Branch Prediction
  - Miscellaneous Pentium Topics
    - *486 versus Pentium Optimization*
    - *Going Superscalar*
20. [__Pentium Rules__][20]
  - How Your Carbon-Based Optimizer Can Put the "Super" in Superscalar
  - An Instruction in Every Pipe
  - V-Pipe-Capable Instructions
  - Lockstep Execution
  - Superscalar Notes
    - *Register Starvation*
21. [__Unleashing the Pentium's V-pipe__][21]
  - Focusing on Keeping Both Pentium Pipes Full
  - Address Generation Interlocks
  - Register Contention
    - *Exceptions to Register Contention*
  - Who's in First?
  - Pentium Optimization in Action
    - *A Quick Note on the 386 and 486*
22. [__Zenning and the Flexible Mind__][22]
  - Taking a Spin through What You've Learned
  - Zenning
23. [__Bones and Sinew__][23]
  - At the Very Heart of Standard PC Graphics
  - The VGA
  - An Introduction to VGA Programming
  - At the Core
    - *Linear Planes and True VGA Modes*
    - *Smooth Panning*
    - *Color Plane Manipulation*
    - *Page Flipping*
  - The Hazards of VGA Clones
  - Just the Beginning
  - The Macro Assembler
24. [__Parallel Processing with the VGA__][24]
  - Taking on Graphics Memory Four Bytes at a Time
  - VGA Programming: ALUs and Latches
  - Notes on the ALU/Latch Demo Program
25. [__VGA Data Machinery__][25]
  - The Barrel Shifter, Bit Mask, and Set/Reset Mechanisms
  - VGA Data Rotation
  - The BitMask
  - The VGA's Set/Reset Circuitry
    - *Setting All Planes to a Single Color*
    - *Manipulating Planes Individually*
  - Notes on Set/Reset
  - A Brief Note on Word OUTs
26. [__VGA Write Mode 3__][26]
  - The Write Mode That Grows on You
  - A Mode Born in Strangeness
  - A Note on Preserving Register Bits
27. [__Yet Another VGA Write Mode__][27]
  - Write Mode 2, Chunky Bitmaps, and Text-Graphics Coexistence
  - Write Mode 2 and Set/Reset
    - *A Byte's Progress in Write Mode 2*
    - *Copying Chunky Bitmaps to VGA Memory Using Write Mode 2*
    - *Drawing Color-Patterned Lines Using Write Mode 2*
  - When to Use Write Mode 2 and When to Use Set/Reset
  - Mode 13H--320x200 with 256 Colors
  - Flipping Pages from Text to Graphics and Back
28. [__Reading VGA Memory__][28]
  - Read Modes 0 and 1, and the Color Don't Care Register
  - Read Mode 0
  - Read Mode 1
  - When all Planes "Don't Care"
29. [__Saving Screens and Other VGA Mysteries__][29]
  - Useful Nuggets from the VGA Zen File
  - Saving and Restoring EGA and VGA Screens
  - 16 Colors out of 64
  - Overscan
  - A Bonus Blanker
  - Modifying VGA Registers
30. [__Video Est Omnis Divisa__][30]
  - The Joys and Galling Problems of Using Split Screens on the EGA and VGA
  - How the Split Screen Works
    - *The Split Screen in Action*
    - *VGA and EGA Split-Screen Operation Don't Mix*
  - Setting the Split-Screen-Related Registers
  - The Problem with the EGA Split Screen
  - Split Screen and Panning
    - *The Split Screen and Horizontal Panning: An Example*
  - Notes on Setting and Reading Registers
  - Split Screens in Other Modes
  - How Safe?
31. [__Higher 256-Color Resolution on the VGA__][31]
  - When Is 320x200 Really 320x400?
  - Why 320x200? Only IBM Knows for Sure
  - 320x400 256-Color Mode
    - *Display Memory Organization in 320x400 Mode*
    - *Reading and Writing Pixels*
  - Two 256-Color Pages
  - Something to Think About
32. [__Be It Resolved: 360x480__][32]
  - Taking 256-Color Modes About as Far as the Standard VGA Can Take Them
  - Extended 256-Color Modes: What's Not to Like?
  - 360x480 256-Color Mode
  - How 360x480 256-Color Mode Works
    - *480 Scan Lines per Screen: A Little Slower, But No Big Deal*
    - *360 Pixels per Scan Line: No Mean Feat*
    - *Accessing Display Memory in 360x480 256-Color Mode*
33. [__Yogi Bear and Eurythmics Confront VGA Colors__][33]
  - The Basics of VGA Color Generation
  - VGA Color Basics
    - *The Palette RAM*
    - *The DAC*
    - *Color Paging with the Color Select Register*
    - *256-color Mode*
    - *Setting the Palette RAM*
    - *Setting the DAC*
  - If You Can't Call the BIOS, Who Ya Gonna Call?
  - An Example of Setting the DAC
34. [__Changing Colors without Writing Pixels__][34]
  - Special Effects through Realtime Manipulation of DAC Colors
  - Color Cycling
  - The Heart of the Problem
    - *Loading the DAC via the BIOS*
    - *Loading the DAC Directly*
  - A Test Program for Color Cycling
  - Color Cycling Approaches that Work
  - Odds and Ends
    - *The DAC Mask*
    - *Reading the DAC*
    - *Cycling Down*
35. [__Bresenham Is Fast, and Fast Is Good__][35]
  - Implementing and Optimizing Bresenham's Line-Drawing Algorithm
  - The Task at Hand
  - Bresenham's Line-Drawing Algorithm
    - *Strengths and Weaknesses*
  - An Implementation in C
    - *Looking at EVGALine*
    - *Drawing Each Line*
    - *Drawing Each Pixel*
  - Comments on the C Implementation
  - Bresenham's Algorithm in Assembly
36. [__The Good, the Bad, and the Run-Sliced__][36]
  - Faster Bresenham Lines with Run-Length Slice Line Drawing
  - Run-Length Slice Fundamentals
  - Run-Length Slice Implementation
  - Run-Length Slice Details
37. [__Dead Cats and Lightning Lines__][37]
  - Optimizing Run-Length Slice Line Drawing in a Major Way
  - Fast Run-Length Slice Line Drawing
    - *How Fast Is Fast?*
    - *Further Optimizations*
38. [__The Polygon Primeval__][38]
  - Drawing Polygons Efficiently and Quickly
  - Filled Polygons
    - *Which Side Is Inside?*
  - How Do You Fit Polygons Together?
  - Filling Non-Overlapping Convex Polygons
  - Oddball Cases
39. [__Fast Convex Polygons__][39]
  - Filling Polygons in a Hurry
  - Fast Convex Polygon Filling
    - *Fast Drawing*
    - *Fast Edge Tracing*
  - The Finishing Touch: Assembly Language
    - *Maximizing REP STOS*
  - Faster Edge Tracing
40. [__Of Songs, Taxes, and the Simplicity of Complex Polygons__][40]
  - Dealing with Irregular Polygonal Areas
  - Filling Arbitrary Polygons
    - *Active Edges*
  - Complex Polygon Filling: An Implementation
    - *More on Active Edges*
    - *Performance Considerations*
  - Nonconvex Polygons
    - *Details, Details*
41. [__Those Way-Down Polygon Nomenclature Blues__][41]
  - Names Do Matter when You Conceptualize a Data Structure
  - Nomenclature in Action
42. [__Wu'ed in Haste; Fried, Stewed at Leisure__][42]
  - Fast Antialiased Lines Using Wu's Algorithm
  - Wu Antialiasing
  - Tracing and Intensity in One
  - Sample Wu Antialiasing
    - *Notes on Wu Antialiasing*
43. [__Bit-Plane Animation__][43]
  - A Simple and Extremely Fast Animation Method for Limited Color
  - Bit-Planes: The Basics
    - *Stacking the Palette Regsters*
  - Bit-Plane Animation in Action
  - Limitations of Bit-Plane Animation
  - Shearing and Page Flipping
  - Beating the Odds in the Jaw-Dropping Contest
44. [__Split Screens Save the Page-Flipped Day__][44]
  - 640x480 Page Flipped Animation in 64K...Almost
  - A Plethora of Challenges
  - A Page Flipping Animation Demonstration
    - *Write Mode 3*
    - *Drawing Text*
    - *Page Flipping*
    - *Knowing When to Flip*
  - Enter the Split Screen
45. [__Dog Hair and Dirty Rectangles__][45]
  - Different Angles on Animation
  - Plus Ça Change
  - VGA Access Times
  - Dirty-Rectangle Animation
    - *So Why Not Use Page Flipping?*
  - Dirty Rectangles in Action
  - Hi-Res VGA Page Flipping
  - Another Interesting Twist on Page Flipping
46. [__Who Was that Masked Image?__][46]
  - Optimizing Dirty-Rectangle Animation
  - Dirty-Rectangle Animation, Continued
  - Masked Images
  - Internal Animation
    - *Dirty-Rectangle Management*
  - Drawing Order and Visual Quality
47. [__Mode X: 256-Color VGA Magic__][47]
  - Introducing the VGA's Undocumented "Animation-Optimal" Mode
  - What Makes Mode X Special?
  - Selecting 320x240 256-Color Mode
  - Designing from a Mode X Perspective
  - Hardware Assist from an Unexpected Quarter
48. [__Mode X Marks the Latch__][48]
  - The Internals of Animation's Best Video Display Mode
  - Allocating Memory in Mode X
  - Copying Pixel Blocks within Display Memory
    - *Copying to Display Memory*
  - Who Was that Masked Image Copier?
49. [__Mode X 256-Color Animation__][49]
  - How to Make the VGA Really Get up and Dance
  - Masked Copying
    - *Faster Masked Copying*
    - *Notes on Masked Copying*
  - Animation
  - Mode X Animation in Action
  - Works Fast, Looks Great
50. [__Adding a Dimension__][50]
  - 3-D Animation Using Mode X
  - References on 3-D Drawing
  - The 3-D Drawing Pipeline
    - *Projection*
    - *Translation*
    - *Rotation*
  - A Simple 3-D Example
    - *Notes on the 3-D Animation Example*
  - An Ongoing Journey
51. [__Sneakers in Space__][51]
  - Using Backface Removal to Eliminate Hidden Surfaces
  - One-sided Polygons: Backface Removal
    - *Backface Removal in Action*
  - Incremental Transformation
  - A Note on Rounding Negative Numbers
  - Object Representation
52. [__Fast 3-D Animation: Meet X-Sharp__][52]
  - The First Iteration of a Generalized 3-D Animation Package
  - This Chapter's Demo Program
  - A New Animation Framework: X-Sharp
  - Three Keys to Realtime Animation Performance
    - *Drawbacks*
    - *Where the Time Goes*
53. [__Raw Speed and More__][53]
  - The Naked Truth About Speed in 3-D Animation
  - Raw Speed, Part 1: Assembly Language
  - Raw Speed, Part 11: Look it Up
    - *Hidden Surfaces*
    - *Rounding*
  - Having a Ball
54. [__3-D Shading__][54]
  - Putting Realistic Surfaces on Animated 3-D Objects
  - Support for Older Processors
  - Shading
    - *Ambient Shading*
    - *Diffuse Shading*
  - Shading: Implementation Details
55. [__Color Modeling in 256-Color Mode__][55]
  - Pondering X-Sharp's Color Model in an RGB State of Mind
  - A Color Model
  - A Bonus from the BitMan
56. [__Pooh and the Space Station__][56]
  - Using Fast Texture Mapping to Place Pooh on a Polygon
  - Principles of Quick-and-Dirty Texture Mapping
    - *Mapping Textures Made Easy*
    - *Notes on DDA Texture Mapping*
  - Fast Texture Mapping: An Implementation
57. [__10,000 Freshly Sheared Sheep on the Screen__][57]
  - The Critical Role of Experience in Implementing Fast, Smooth Texture Mapping
  - Visual Quality: A Black Hole ... Er, Art
  - Fixed-Point Arithmetic, Redux
  - Texture Mapping: Orientation Independence
  - Mapping Textures across Multiple Polygons
    - *Fast Texture Mapping*
58. [__Heinlein's Crystal Ball, Spock's Brain, and the 9-Cycle Dare__][58]
  - Using the Whole-Brain Approach to Accelerate Texture Mapping
  - Texture Mapping Redux
    - *Left-Brain Optimization*
    - *A 90-Degree Shift in Perspective*
  - That's Nice--But it Sure as Heck Ain't 9 Cycles
    - *Don't Stop Thinking about Those Cycles*
  - Texture Mapping Notes
59. [__The Idea of BSP Trees__][59]
  - What BSP Trees Are and How to Walk Them
  - BSP Trees
    - *Visibility Determination*
    - *Limitations of BSP Trees*
  - Building a BSP Tree
    - *Visibility Ordering*
  - Inorder Walks of BSP Trees
    - *Know It Cold*
    - *Measure and Learn*
  - Surfing Amidst the Trees
    - *Related Reading*
60. [__Compiling BSP Trees__][60]
  - Taking BSP Trees from Concept to Reality
  - Compiling BSP Trees
    - *Parametric Lines*
    - *Parametric Line Clipping*
    - *The BSP Compiler*
  - Optimizing the BSP Tree
  - BSP Optimization: an Undiscovered Country
61. [__Frames of Reference__][61]
  - The Fundamentals of the Math behind 3-D Graphics
    - *3-D Math*
    - *Foundation Definitions*
  - The Dot Product
    - *Dot Products of Unit Vectors*
  - Cross Products and the Generation of Polygon Normals
  - Using the Sign of the Dot Product
  - Using the Dot Product for Projection
    - *Rotation by Projection*
62. [__One Story, Two Rules, and a BSP Renderer__][62]
  - Taking a Compiled BSP Tree from Logical to Visual Reality
  - BSP-based Rendering
  - The Rendering Pipeline
    - *Moving the Viewer*
    - *Transformation into Viewspace*
    - *Clipping*
    - *Projection to Screenspace*
    - *Walking the Tree, Backface Culling and Drawing*
  - Notes on the BSP Renderer
63. [__Floating-Point for Real-Time 3-D__][63]
  - Knowing When to Hurl Conventional Math Wisdom Out the Window
  - Not Your Father's Floating-point
  - Pentium Floating-point Optimization
    - *Pipelining, Latency, and Throughput*
    - *FXCH*
  - The Dot Product
  - The Cross Product
  - Transformation
  - Projection
  - Rounding Control
  - A Farewell to 3-D Fixed-point
64. [__Quake's Visible-Surface Determination__][64]
  - The Challenge of Separating All Things Seen from All Things Unseen
  - VSD: The Toughest 3-D Challenge of All
  - The Structure of Quake Levels
  - Culling and Visible Surface Determination
    - *Nodes Inside and Outside the View Frustum*
  - Overdraw
  - The Beam Tree
  - 3-D Engine du Jour
    - *Subdividing Raycast*
    - *Vertex-Free Surfaces*
    - *The Draw-Buffer*
    - *Span-Based Drawing*
    - *Portals*
  - Breakthrough!
  - Simplify, and Keep on Trying New Things
  - Learn Now, Pay Forward
  - References
65. [__3-D Clipping and Other Thoughts__][65]
  - Determining What's Inside Your Field of View
  - 3-D Clipping Basics
    - *Intersecting a Line Segment with a Plane*
  - Polygon Clipping
    - *Clipping to the Frustum*
    - *The Lessons of Listing 65.3*
  - Advantages of Viewspace Clipping
  - Further Reading
66. [__Quake's Hidden-Surface Removal__][66]
  - Struggling with Z-Order Solutions to the Hidden Surface Problem
  - Creative Flux and Hidden Surfaces
    - *Drawing Moving Objects*
    - *Performance Impact*
    - *Leveling and Improving Performance*
  - Sorted Spans
    - *Edges versus Spans*
  - Edge-Sorting Keys
    - *Where That l/Z Equation Comes From*
    - *Quake and Z-Sorting*
    - *Decisions Deferred*
67. [__Sorted Spans in Action__][67]
  - Implementing Independent Span Sorting for Rendering without Overdraw
  - Quake and Sorted Spans
  - Types of l/z Span Sorting
    - *Intersecting Span Sorting*
    - *Abutting Span Sorting*
    - *Independent Span Sorting*
  - l/z Span Sorting in Action
    - *Implementation Notes*
68. [__Quake's Lighting Model__][68]
  - A Radically Different Approach to Lighting Polygons
    - *Problems with Gouraud Shading*
    - *Perspective Correctness*
    - *Decoupling Lighting from Rasterization*
    - *Size and Speed*
    - *Mipmapping To The Rescue*
    - *Two Final Notes on Surface Caching*
69. [__Surface Caching and Quake's Triangle Models__][69]
  - []()
    - *Letting the Graphics Card Build the Textures*
    - *The Light Map as Alpha Texture*
    - *Drawing Triangle Models Fast*
    - *Trading Subpixel Precision for Speed*
    - *An Idea that Didn't Work*
    - *An Idea that Did Work*
    - *More Ideas that Might Work*
70. [__Quake: A Post-Mortem and a Glimpse into the Future__][70]
  - []()
    - *Lighting*
    - *Dynamic Lighting*
    - *BSP Models*
    - *Polygon Models and ZBuffering*
    - *The Subdivision Rasterizer*
    - *Sprites*
    - *Particles*
  - How We Spent Our Summer Vacation: After Shipping Quake
    - *Verite Quake*
    - *GLQuake*
    - *WinQuake*
    - *Quakeworld*
    - *Quake 2*
- [__Afterword__][A]<br/>
- [__Index__][B]<br/>


[01]: /book/gpbb.1.01.pdf?raw=true
[02]: /book/gpbb.1.02.pdf?raw=true
[03]: /book/gpbb.1.03.pdf?raw=true
[04]: /book/gpbb.1.04.pdf?raw=true
[05]: /book/gpbb.1.05.pdf?raw=true
[06]: /book/gpbb.1.06.pdf?raw=true
[07]: /book/gpbb.1.07.pdf?raw=true
[08]: /book/gpbb.1.08.pdf?raw=true
[09]: /book/gpbb.1.09.pdf?raw=true
[10]: /book/gpbb.1.10.pdf?raw=true
[11]: /book/gpbb.1.11.pdf?raw=true
[12]: /book/gpbb.1.12.pdf?raw=true
[13]: /book/gpbb.1.13.pdf?raw=true
[14]: /book/gpbb.1.14.pdf?raw=true
[15]: /book/gpbb.1.15.pdf?raw=true
[16]: /book/gpbb.1.16.pdf?raw=true
[17]: /book/gpbb.1.17.pdf?raw=true
[18]: /book/gpbb.1.18.pdf?raw=true
[19]: /book/gpbb.1.19.pdf?raw=true
[20]: /book/gpbb.1.20.pdf?raw=true
[21]: /book/gpbb.1.21.pdf?raw=true
[22]: /book/gpbb.1.22.pdf?raw=true
[23]: /book/gpbb.2.23.pdf?raw=true
[24]: /book/gpbb.2.24.pdf?raw=true
[25]: /book/gpbb.2.25.pdf?raw=true
[26]: /book/gpbb.2.26.pdf?raw=true
[27]: /book/gpbb.2.27.pdf?raw=true
[28]: /book/gpbb.2.28.pdf?raw=true
[29]: /book/gpbb.2.29.pdf?raw=true
[30]: /book/gpbb.2.30.pdf?raw=true
[31]: /book/gpbb.2.31.pdf?raw=true
[32]: /book/gpbb.2.32.pdf?raw=true
[33]: /book/gpbb.2.33.pdf?raw=true
[34]: /book/gpbb.2.34.pdf?raw=true
[35]: /book/gpbb.2.35.pdf?raw=true
[36]: /book/gpbb.2.36.pdf?raw=true
[37]: /book/gpbb.2.37.pdf?raw=true
[38]: /book/gpbb.2.38.pdf?raw=true
[39]: /book/gpbb.2.39.pdf?raw=true
[40]: /book/gpbb.2.40.pdf?raw=true
[41]: /book/gpbb.2.41.pdf?raw=true
[42]: /book/gpbb.2.42.pdf?raw=true
[43]: /book/gpbb.2.43.pdf?raw=true
[44]: /book/gpbb.2.44.pdf?raw=true
[45]: /book/gpbb.2.45.pdf?raw=true
[46]: /book/gpbb.2.46.pdf?raw=true
[47]: /book/gpbb.2.47.pdf?raw=true
[48]: /book/gpbb.2.48.pdf?raw=true
[49]: /book/gpbb.2.49.pdf?raw=true
[50]: /book/gpbb.2.50.pdf?raw=true
[51]: /book/gpbb.2.51.pdf?raw=true
[52]: /book/gpbb.2.52.pdf?raw=true
[53]: /book/gpbb.2.53.pdf?raw=true
[54]: /book/gpbb.2.54.pdf?raw=true
[55]: /book/gpbb.2.55.pdf?raw=true
[56]: /book/gpbb.2.56.pdf?raw=true
[57]: /book/gpbb.2.57.pdf?raw=true
[58]: /book/gpbb.2.58.pdf?raw=true
[59]: /book/gpbb.2.59.pdf?raw=true
[60]: /book/gpbb.2.60.pdf?raw=true
[61]: /book/gpbb.2.61.pdf?raw=true
[62]: /book/gpbb.2.62.pdf?raw=true
[63]: /book/gpbb.2.63.pdf?raw=true
[64]: /book/gpbb.2.64.pdf?raw=true
[65]: /book/gpbb.2.65.pdf?raw=true
[66]: /book/gpbb.2.66.pdf?raw=true
[67]: /book/gpbb.2.67.pdf?raw=true
[68]: /book/gpbb.2.68.pdf?raw=true
[69]: /book/gpbb.2.69.pdf?raw=true
[70]: /book/gpbb.2.70.pdf?raw=true
[A]: /book/gpbb.3.01.pdf?raw=true
[B]: /book/gpbb.3.02.pdf?raw=true

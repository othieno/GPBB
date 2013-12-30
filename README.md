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
24. [__Parallel Processing with the VGA__][24]
25. [__VGA Data Machinery__][25]
26. [__VGA Write Mode 3__][26]
27. [__Yet Another VGA Write Mode__][27]
28. [__Reading VGA Memory__][28]
29. [__Saving Screens and Other VGA Mysteries__][29]
30. [__Video Est Omnis Divisa__][30]
31. [__Higher 256-Color Resolution on the VGA__][31]
32. [__Be It Resolved: 360x480__][32]
33. [__Yogi Bear and Eurythmics Confront VGA Colors__][33]
34. [__Changing Colors without Writing Pixels__][34]
35. [__Bresenham Is Fast, and Fast Is Good__][35]
36. [__The Good, theBad, and the Run-Sliced__][36]
37. [__Dead Cats and Lightning Lines__][37]
38. [__The Polygon Primeval__][38]
39. [__Fast Convex Polygons__][39]
40. [__Of Songs, Taxes, and the Simplicity of Complex Polygons__][40]
41. [__Those Way-Down Polygon Nomenclature Blues__][41]
42. [__Wu'ed in Haste; Fried, Stewed at Leisure__][42]
43. [__Bit-Plane Animation__][43]
44. [__Split Screens Save the Page-Flipped Day__][44]
45. [__Dog Hair and Dirty Rectangles__][45]
46. [__Who Was that Masked Image?__][46]
47. [__Mode X: 256-Color VGA Magic__][47]
48. [__Mode X Marks the Latch__][48]
49. [__Mode X 256-Color Animation__][49]
50. [__Adding a Dimension__][50]
51. [__Sneakers in Space__][51]
52. [__Fast 3-D Animation: Meet X-Sharp__][52]
53. [__Raw Speed and More__][53]
54. [__3-D Shading__][54]
55. [__Color Modeling in 256-Color Mode__][55]
56. [__Pooh and the Space Station__][56]
57. [__10,000 Freshly Sheared Sheep on the Screen__][57]
58. [__Heinlein's Crystal Ball, Spock's Brain, and the 9-Cycle Dare__][58]
59. [__The Idea of BSP Trees__][59]
60. [__Compiling BSP Trees__][60]
61. [__Frames of Reference__][61]
62. [__One Story, Two Rules, and a BSP Renderer__][62]
63. [__Floating-Point for Real-Time 3-D__][63]
64. [__Quake's Visible-Surface Determination__][64]
65. [__3-D Clipping and Other Thoughts__][65]
66. [__Quake's Hidden-Surface Removal__][66]
67. [__Sorted Spans in Action__][67]
68. [__Quake's Lighting Model__][68]
69. [__Surface Caching and Quake's Triangle Models__][69]
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

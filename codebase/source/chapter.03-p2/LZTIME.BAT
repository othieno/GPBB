echo off
rem
rem *** Listing 3-7 ***
rem
rem ***************************************************************
rem * Batch file LZTIME.BAT, which builds and runs the            *
rem * long-period Zen timer program LZTEST.EXE to time the code   *
rem * named as the command-line parameter. Listing 3-5 must be    *
rem * named LZTIMER.ASM, and Listing 3-6 must be named            *
rem * LZTEST.ASM. To time the code in LST3-8, you'd type the      *
rem * DOS command:                                                *
rem *                                                             *
rem * lztime lst3-8                                               *
rem *                                                             *
rem * Note that MASM and LINK must be in the current directory or *
rem * on the current path in order for this batch file to work.   *
rem *                                                             *
rem * This batch file can be speeded up by assembling LZTIMER.ASM *
rem * once, then removing the lines:                              *
rem *                                                             *
rem * masm lztimer;                                               *
rem * if errorlevel 1 goto errorend                               *
rem *                                                             *
rem * from this file.                                             *
rem *                                                             *
rem * By Michael Abrash                                           *
rem ***************************************************************
rem
rem Make sure a file to test was specified.
rem
if not x%1==x goto ckexist
echo ***************************************************************
echo * Please specify a file to test.                              *
echo ***************************************************************
goto end
rem
rem Make sure the file exists.
rem
:ckexist
if exist %1 goto docopy
echo ***************************************************************
echo * The specified file, "%1," doesn't exist.
echo ***************************************************************
goto end
rem
rem copy the file to measure to TESTCODE.
:docopy
copy %1 testcode
masm lztest;
if errorlevel 1 goto errorend
masm lztimer;
if errorlevel 1 goto errorend
link lztest+lztimer;
if errorlevel 1 goto errorend
lztest
goto end
:errorend
echo ***************************************************************
echo * An error occurred while building the long-period Zen timer. *
echo ***************************************************************
:end

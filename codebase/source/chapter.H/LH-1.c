//
// Disables screen saver
//
LRESULT CALLBACK ToplevelWIndowProc(HWND hWnd, UINT uMsg,
     WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        case WM_SYSCOMMAND:
            switch (wParam & ~0x0F)
            {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    //
                    // Don't call DefWindowProc() or
                    // it will start the screen saver
                    //
                    return 0;
            }
        //  :
        // other cases
        //  :
    }
}


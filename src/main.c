#include <main.h>

// Window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static WindowData *windowData = NULL;
    switch (uMsg)
    {
        case WM_SIZE: // When window is resized invalidate current window background so paint is called
        {
            MoveWindow(windowData->hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return 0;
        }
        case WM_CREATE: // When repaint is called execute the following actions tot he window background
        {
            HWND hEdit = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
                0, 0, 0, 0, hWnd, NULL, NULL, NULL);

            HFONT hFont = CreateFontW(
                16,                         // Font height
                0,                          // Font width (0 = default)
                0,                          // Angle of escapement
                0,                          // Orientation angle
                FW_NORMAL,                  // Font weight
                FALSE,                      // Italic
                FALSE,                      // Underline
                FALSE,                      // Strikeout
                DEFAULT_CHARSET,            // Character set
                OUT_DEFAULT_PRECIS,         // Output precision
                CLIP_DEFAULT_PRECIS,        // Clipping precision
                DEFAULT_QUALITY,            // Output quality
                DEFAULT_PITCH | FF_DONTCARE,// Pitch and family
                L"Arial"                    // Font name
            );

            if (windowData == NULL) {
                windowData = malloc(sizeof(WindowData));
                windowData->hEdit = hEdit;
                windowData->hFont = hFont;
            }
            SendMessage(windowData->hEdit, WM_SETFONT, (WPARAM)windowData->hFont, TRUE);
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDM_FILE_NEW_WINDOW: // Execute when file menu option is selected
                    {
                        char modPath[MAX_PATH]; // Get the path to the current executable
                        GetModuleFileName(NULL, modPath, MAX_PATH); // TODO: Add checksum verificaton

                        STARTUPINFO startInfo = {sizeof(STARTUPINFO)}; // Launch a new process from the same executable
                        PROCESS_INFORMATION processInfo;
                        CreateProcess(modPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &processInfo);

                        CloseHandle(processInfo.hProcess); // Close the handles to the new process and thread
                        CloseHandle(processInfo.hThread);
                        break;
                    }
                case IDM_FILE_EXIT:
                    {
                        SendMessage(hWnd, WM_DESTROY, 0, 0);
                        return 0;
                    }
                case IDM_EDIT: // Execute when edit menu option is selected
                break;
            }
        break;

        case WM_DESTROY:
        {
            // Cleanup
            windowData = NULL;
            free(windowData);

            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    const char CLASS_NAME[] = "MyWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hWnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class name
        "Min Pad",                    // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        // Position and size
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hWnd == NULL)
        return 0;

    // Create menu objects
    HMENU hMenu = CreateMenu();
    HMENU fileMenu = CreatePopupMenu();

    // Add options to menu object
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)fileMenu, "File");
    AppendMenu(hMenu, MF_STRING, IDM_EDIT, "Edit");

    // File submenu
    AppendMenu(fileMenu, MF_STRING, IDM_FILE_NEW_WINDOW, "New Window");
    AppendMenu(fileMenu, MF_STRING, IDM_FILE_OPEN, "Open");
    AppendMenu(fileMenu, MF_STRING, IDM_FILE_SAVE, "Save");
    AppendMenu(fileMenu, MF_STRING, IDM_FILE_SAVE_AS, "Save As");

    AppendMenu(fileMenu, MF_SEPARATOR, 0, NULL); // Add horizontal divider
    AppendMenu(fileMenu, MF_STRING, IDM_FILE_EXIT, "Exit");

    // Bind menu to window
    SetMenu(hWnd, hMenu);

    // Display the window
    ShowWindow(hWnd, nCmdShow);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

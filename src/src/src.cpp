#include "afxwin.h"
#include <fstream>

class CMyWindow : public CFrameWnd {

public:
    CMyWindow()
    {
        RECT myRect;
        myRect.left = 0;
        myRect.right = 800;
        myRect.top = 0;
        myRect.bottom = 600;

        Create(NULL, _T("Пример MFC окна"), WS_OVERLAPPEDWINDOW, myRect, nullptr, nullptr, WS_EX_WINDOWEDGE, nullptr);
        RECT buttonRect;
        buttonRect.left = 10;
        buttonRect.right = 100;
        buttonRect.top = 10;
        buttonRect.bottom = 40;
        myButton.Create(_T("Okay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, buttonRect, this, 1);


        RECT staticRect;
        staticRect.left = 10;
        staticRect.right = 110;
        staticRect.top = 50;
        staticRect.bottom = 150;
        imageStatic.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_BITMAP, staticRect, this, 2);

        BlendImages("C:/lessons/image-mixer/assets/chip3.bmp", "C:/lessons/image-mixer/assets/chip2.bmp", 50);

    }
protected:

    void OnButtonClicked()
    {
    }

    void BlendImages(const char* image1Path, const char* image2Path, int blendFactor)
    {
        std::ifstream f1, f2;

        f1.open(image1Path, std::ios::binary);
        f2.open(image2Path, std::ios::binary);

        f1.seekg(0, std::ios::end);
        int fileSize1 = f1.tellg();
        f1.seekg(0, std::ios::beg);

        f2.seekg(0, std::ios::end);
        int fileSize2 = f2.tellg();
        f2.seekg(0, std::ios::beg);

        char* bytes1 = new char[fileSize1];
        char* bytes2 = new char[fileSize2];

        f1.read(bytes1, fileSize1);
        f2.read(bytes2, fileSize2);

        int width = 200;
        int height = 200;

        char* pointerToImageData1 = bytes1 + 54;
        char* pointerToImageData2 = bytes2 + 54;

        char* blendedImageData = new char[fileSize1];

        for (int i = 0; i < width * height * 3; i++)
        {
            blendedImageData[i] = (bytes1[i] * blendFactor + bytes2[i] * (100 - blendFactor)) / 100;
        }

        BITMAPINFO info;

        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = width;
        info.bmiHeader.biHeight = height;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 24;
        info.bmiHeader.biCompression = 0;
        info.bmiHeader.biSizeImage = 0;
        info.bmiHeader.biXPelsPerMeter = 0;
        info.bmiHeader.biYPelsPerMeter = 0;
        info.bmiHeader.biClrUsed = 0;
        info.bmiHeader.biClrImportant = 0;
        info.bmiColors[0].rgbBlue = 0;
        info.bmiColors[0].rgbGreen = 0;
        info.bmiColors[0].rgbRed = 0;
        info.bmiColors[0].rgbReserved = 0;

        HDC hdc = this->GetDC()->m_hDC;

        HBITMAP blendedBitmap = CreateDIBitmap(hdc, &info.bmiHeader, CBM_INIT, blendedImageData, &info, 0);

        imageStatic.SetBitmap(blendedBitmap);

        f1.close();
        f2.close();
        delete[] bytes1;
        delete[] bytes2;
        delete[] blendedImageData;
    }

    CButton myButton;
    CStatic imageStatic;

    DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(CMyWindow, CFrameWnd)
    ON_COMMAND(1, &CMyWindow::OnButtonClicked)
END_MESSAGE_MAP()

class CMyApp : CWinApp
{
    virtual BOOL InitApplication()
    {
        this->m_pMainWnd = new CMyWindow();
        this->m_pMainWnd->ShowWindow(SW_SHOW);
        this->m_pMainWnd->UpdateWindow();

        return TRUE;
    }
};


CMyApp myApplication;

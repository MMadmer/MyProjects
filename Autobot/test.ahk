X := 305
Y := 452

SetFormat, Integer, Hex

WinGet, hWnd, ID, ahk_class WarframePublicEvolutionGfxD3D11

hDC     := DllCall("GetDC", Int, hWnd)
hMemDC  := DllCall("CreateCompatibleDC", UInt, hDC)
WinGetPos,,, Width, Height, ahk_id %hWnd%
hBitmap := DllCall("CreateCompatibleBitmap", UInt, hDC, Int, Width, Int, Height)
DllCall("SelectObject", UInt, hMemDC, UInt, hBitmap)
DllCall("PrintWindow", UInt, hWnd, UInt, hMemDC, UInt, 0)
Color   := DllCall("GetPixel", UInt, hMemDC, UInt, X, UInt, Y)

DllCall("DeleteObject", UInt, hBitmap)
DllCall("DeleteDC", UInt, hMemDC)
DllCall("ReleaseDC", UInt, ahk_id hWnd, UInt, hDC)

Transform, Red,   BitAnd, Color, 0x0000FF

Transform, Green, BitAnd, Color, 0x00FF00
Transform, Green, BitShiftRight, Green, 8

Transform, Blue,  BitAnd, Color, 0xFF0000
Transform, Blue,  BitShiftRight, Blue, 16

SetFormat, Integer, D

MsgBox, Color %A_Tab% %Color%`n`nRed %A_Tab% %Red%`nGreen %A_Tab% %Green%`nBlue %A_Tab% %Blue%

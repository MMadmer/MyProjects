PID := DllCall("GetCurrentProcessId")
IniWrite, %PID%, temp.ini, PID, indexPID

IniRead, loading, conf.ini, settings, loading ;Время загрузки миссии
IniRead, rounds, conf.ini, settings, rounds ;Число раундов
;Антиафк
IniRead, antiAfk, conf.ini, settings, antiAfk
IniRead, antiAfkState, cong.ini, status, antiAfkState
IniRead, afkTimer, conf.ini, timers, afkTimer

indicator := false
welcome := 8000
game := "ahk_class WarframePublicEvolutionGfxD3D11"
baseWeight := 1920
baseHeight := 1200
baseW := 25
baseH := 25
currentW := 0
currentH := 0
prepState := true
waiting := loading + welcome
localRounds := rounds
startX := 0
startY := 0

;main() {
	Voice := ComObjCreate("SAPI.SpVoice")
	Voice.Speak("Script activated")

	WinGetPos,,, Weight, Height, %game% ;Получаем разрешение экрана
	currentW := Floor(Weight / baseWeight * baseW)
	currentH := Floor(Height / baseHeight * baseH)
	;Индикатор
	Gui, -Caption +AlwaysOnTop +Lastfound +ToolWindow HwndGUIhWnd
	Gui, Color, fc0000
	Gui, Show, x0 y0 w%currentW% h%currentH%

	Loop {
		while indicator {
			Gui, Color, 05fc1e ;Зелёный индикатор
			if prepState
				prepare()
		}
		prepState := true

		Gui, Color, fc0000 ;Красный индикатор
	}
;return
;}

;Функции
;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------
;Запуск индекса
prepare() {
	global welcome
	global indicator
	global prepState := false
	global startX := 0
	global startY := 0
	global Voice
	global Weight
	global Height
	global baseWeight
	global baseHeight
	varStart1 := 3
	varStart2 := 3
	varStart3 := 3

	startingX := Floor(Weight / baseWeight * 440)
	startingY := Floor(Height / baseHeight * 400)

	Voice.Speak("Mission starting")
	;Открываем карту
	start("Esc", 1, 700)
	MouseClick, Left, startingX, startingY, 3
	Sleep, 2500
	;Выбираем планету
	start("RButton", 1, 1000)
	startingX := Floor(Weight / baseWeight * 1460)
	startingY := Floor(Height / baseHeight * 935)
	MouseClick, Left, startingX, startingY, 3
	Sleep 1000
	;Запускаем миссию
	startingX := Floor(Weight / baseWeight * 991)
	startingY := Floor(Height / baseHeight * 603)
	MouseClick, Left, startingX, startingY, 3
	Sleep 700
	startingX := Floor(Weight / baseWeight * 1545)
	startingY := Floor(Height / baseHeight * 950)
	MouseClick, Left, startingX, startingY, 3
	Sleep 200
	start("Enter", 1, 5200)
	;Ждём начала
	if indicator {
		while (varStart1 > 0 && varStart2 > 0 && varStart3 > 0) {
			ImageSearch, startX, startY, 905, 175, 1000, 310, *2 *Trans7D7C72 images\starting1.png
			varStart1 := ErrorLevel
			ImageSearch, startX, startY, 905, 165, 1000, 310, *2 *Trans7D7C72 images\starting2.png
			varStart2 := ErrorLevel
			ImageSearch, startX, startY, 905, 165, 1000, 310, *2 *Trans7D7C72 images\starting3.png
			varStart3 := ErrorLevel
			if (varStart1 = 0 || varStart2 = 0 || varStart3 = 0) {
				Voice.Speak("Mission started")
				if indicator
					SetTimer, InGame, %welcome%
			}
		}
	}
}

start(key, loops, delay) {
	global indicator
	global game

	Loop % loops {
		if indicator {
			Send, {%key%}
			Sleep, %delay%
		}
	}
return
}

way(key, delay) {
	global indicator

	if indicator {
		Send, {%key% down}
		Sleep %delay%
		Send, {%key% up}
	}
}
;Метки
;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------
InGame:
	SetTimer, InGame, off
	way("w", 9150)
	way("d", 1720)
	way("w", 2500)
	if indicator
		MouseMove, 10, 0, 3, R
	way("w", 7000)
	if indicator
		MouseMove 1, 0, 3, R
	way("w", 1500)
	way(1, 1000)
	way("d", 500)

	if indicator {
		if	antiAfk
			SetTimer, antiAfkLabel, %afkTimer%
		SetTimer, choice, 1000
	}
return

choice:
	SetTimer, choice, off
	if indicator {
		leaveX := 0
		leaveY := 0
		nextX := 0
		nextY := 0

		ImageSearch, leaveX, leaveY, 300, 365, 820, 735, *2 *TransBlack images\leave.png
		var1 := ErrorLevel
		ImageSearch, nextX, nextY, 1085, 340, 1655, 755, *2 *TransBlack images\next.png
		var2 := ErrorLevel
		ImageSearch, endX, endY, 1712, 1109, 1822, 1144, *2 *TransBlack images\ending.png
		;Провал миссии
		if (ErrorLevel = 0) {
			SetTimer, antiAfkLabel, off
			Voice.Speak("Mission failed")
			prepState := true
			localRounds := rounds
			start("Esc", 1, 3000)
		}
		if (var1 = 0 || var2 = 0) {
			SetTimer, antiAfkLabel, off
			if (localRounds <= 1) {
				Voice.Speak("Mission complete")
				if indicator {
					exitX := Floor(Weight / baseWeight * 540)
					exitY := Floor(Height / baseHeight * 540)
					MouseClick, Left, exitX, exitY, 3
				}
				;Ожидание окна завершения миссии
				while !prepState {
					ImageSearch, endX, endY, 1712, 1109, 1822, 1144, *2 *TransBlack images\ending.png
					if (ErrorLevel = 0) {
						Voice.Speak("The mission is over")
						prepState := true
						localRounds := rounds
						start("Esc", 1, 3000)
					}
				}
			}
			else {
				Voice.Speak("Mission continues")
				localRounds -= 1
				contX := Floor(Weight / baseWeight * 1380)
				contY := Floor(Height / baseHeight * 540)
				MouseClick, Left, contX, contY
				SetTimer, antiAfkLabel, %afkTimer%
				SetTimer, choice, 20000
			}
		}
		else {
			if !prepState
				SetTimer, choice, 500
		}
	}
return

;Антиафк
antiAfkLabel:
	if (!indicator || (!indicator && !antiAfk))
		SetTimer, antiAfkLabel, off
	else {
		if (indicator || (indicator && antiAfk))
			Send, {a down}
		if (indicator || (indicator && antiAfk))
			Sleep 1000
		if (indicator || (indicator && antiAfk))
			Send, {a up}
		if (indicator || (indicator && antiAfk))
			Send, {d down}
		if (indicator || (indicator && antiAfk))
			Sleep 1084
		if (indicator || (indicator && antiAfk))
			Send, {d up}

		if (indicator || (indicator && antiAfk))
			Sleep 250
	}
return

NumpadMult::
	indicator := !indicator
return

Numpad0::
IniWrite, 0, conf.ini, settings, stateIndex
IniWrite, 0, temp.ini, PID, indexPID
ExitApp
return

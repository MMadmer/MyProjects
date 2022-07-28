PID := DllCall("GetCurrentProcessId")
IniWrite, %PID%, temp.ini, PID, spellsPID

game := "ahk_class WarframePublicEvolutionGfxD3D11"
baseWeight := 1920
baseHeight := 1200
baseW := 25
baseH := 25
currentW := 0
currentH := 0
;Параметры статуса таймеров
firstState := false
secondState := false
thirdState := false
fourthState := false
;Статус работы скрипта
indicator := false
;Получение данных о антиафк
IniRead, antiAfk, conf.ini, settings, antiAfk
IniRead, antiAfkState, cong.ini, status, antiAfkState
IniRead, afkTimer, conf.ini, timers, afkTimer

;main() {
	Voice := ComObjCreate("SAPI.SpVoice")
	Voice.Speak("Script activated")
	;====================================================================
	;Выбор и применение билда
	IniRead, build, temp.ini, currentBuild, build
	;Задержка после способностей
	IniRead, firstSpellDelay, builds.ini, % build, firstSpellDelay
	IniRead, secondSpellDelay, builds.ini, % build, secondSpellDelay
	IniRead, thirdSpellDelay, builds.ini, % build, thirdSpellDelay
	IniRead, fourthSpellDelay, builds.ini, % build, fourthSpellDelay
	;Клавиши способностей
	IniRead, firstSpell, builds.ini, % build, firstSpell
	IniRead, secondSpell, builds.ini, % build, secondSpell
	IniRead, thirdSpell, builds.ini, % build, thirdSpell
	IniRead, fourthSpell, builds.ini, % build, fourthSpell
	;====================================================================
	WinGetPos,,, Weight, Height, %game% ;Получаем разрешение экрана
	currentW := Floor(Weight / baseWeight * baseW)
	currentH := Floor(Height / baseHeight * baseH)
	;Индикатор
	Gui, -Caption +AlwaysOnTop +Lastfound +ToolWindow HwndGUIhWnd
	Gui, Color, fc0000
	Gui, Show, x0 y0 w%currentW% h%currentH%

	loop {
		while indicator {
			Gui, Color, 05fc1e ;Зелёный индикатор

			if	(!antiAfkState && antiafk)
				SetTimer, antiAfkLabel, %afkTimer%

			UsingSpell(firstSpellDelay, "first", firstState)
			UsingSpell(secondSpellDelay, "second", secondState)
			UsingSpell(thirdSpellDelay, "third", thirdState)
			UsingSpell(fourthSpellDelay, "fourth", fourthState)

			StatePreset(true, true, true, true, true)
		}
		StatePreset(false, false, false, false, false)
		Gui, Color, fc0000 ;Красный индикатор
	}
;return
;}

;Функции
;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------
;Используем способность и задаём её таймер
UsingSpell(spellDelay, label, state) {
	if (!state && spellDelay > 0) {
		SetTimer, %label%, %spellDelay%
		Sleep 5
	}
return
}

;Устанавливаем статус таймеров
StatePreset(state, state1, state2, state3, state4) {
	global
	antiAfkState := state
	firstState := state1
	secondState := state2
	thirdState := state3
	fourthState := state4
}

;Прожатие способностей
first:
	if (!firstState)
		SetTimer, first, off
	else
		Send, {%firstSpell%}
return

second:
	if (!secondState)
		SetTimer, second, off
	else
	Send, {%secondSpell%}
return

third:
	if (!thirdState)
		SetTimer, third, off
	else
	Send, {%thirdSpell%}
return

fourth:
	if (!fourthState)
		SetTimer, fourth, off
	else
	Send, {%fourthSpell%}
return

;Антиафк
antiAfkLabel:
	if (!AntiAfkState)
		SetTimer, antiAfkLabel, off
	else {
		Send, {a down}
		Sleep 1000
		Send, {a up}
		Send, {d down}
		Sleep 1084
		Send, {d up}

		Sleep 250
	}
return

;Вкл/выкл скрипт
NumpadMult::
	indicator := !indicator
return

Numpad0::
IniWrite, 0, conf.ini, settings, stateSpells
IniWrite, 0, temp.ini, PID, spellsPID
ExitApp
return

﻿;Прогрузка билдов
IniRead, a, builds.ini, build_names, a
IniRead, b, builds.ini, build_names, b
IniRead, c, builds.ini, build_names, c
IniRead, d, builds.ini, build_names, d
IniRead, e, builds.ini, build_names, e
IniRead, f, builds.ini, build_names, f
IniRead, g, builds.ini, build_names, g
IniRead, h, builds.ini, build_names, h
IniRead, i, builds.ini, build_names, i
IniRead, g, builds.ini, build_names, g

;main() {
	Gui, Font, S13 CDefault, consolas
	Gui, Add, ListBox, x22 y19 w150 h150 vList, %a%||%b%|%c%|%d%|%e%|%f%|%g%|%h%|%i%|%g%
	Gui, Font, S11 CDefault, consolas
	Gui, Add, Button, x182 y19 w100 h30 gChangeBuild +Center, Изменить
	Gui, Add, Button, x192 y146 w80 h20 gDelBuild +Center, Удалить
	; Generated using SmartGUI Creator for SciTE
	Gui, Show, w316 h190, Редактор билдов
	Gui, Submit, NoHide
	return
;return
;}

ChangeBuild:
	Run, build_settings.ahk
	ExitApp
return

DelBuild:
	Gui, Submit, NoHide
	MsgBox, 262433, Удаление билда, Удалить выбранный билд?
	ifMsgBox OK
	{

		IniDelete, builds.ini, %List%
		Gui, Submit, NoHide
	}
	else
		return
return

GuiClose:
	Run, launcher.ahk
	ExitApp
return

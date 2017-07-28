; Run for preventing screen saver
RunPreventScreenSaver()
{
	global PreventScreenSaver = true
	Loop
	{
		if global PreventScreenSaver = false
			break
		MouseMove 10,0,50,R
		Sleep 1000
		MouseMove -10,0,50,R
	}
}

; Stop for preventing screen saver
StopPreventScreenSaver()
{
	global PreventScreenSaver = false
}

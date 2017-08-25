#Persistent
Loop {
	IfGreater, A_TimeIdle, 60000, {
		MouseMove 0,0,0,R
	}
}

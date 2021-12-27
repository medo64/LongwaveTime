package main

import (
	"fmt"
	"os"
	"sync"
	"time"
)

func logDebugData(dataTime time.Time, data string, syncSecond string) {
	syncLock.Lock()
	defer syncLock.Unlock()

	var timeText = dataTime.Format("15:04:05")
	if isStdoutTerminal() {
		fmt.Printf("\033[37m%s \033[0m\033[94m%s\033[0m %s\n", timeText, data, syncSecond)
	} else {
		fmt.Println(timeText + " " + data + " " + syncSecond)
	}
}

func logIfPanic(error error) {
	if error != nil { // only do something if it's error
		var text = error.Error()
		if isStdoutTerminal() {
			fmt.Printf("\033[91m%s\033[0m\n", text)
		} else {
			fmt.Println(text)
		}
		os.Exit(1)
	}
}

var syncLock sync.Mutex

var cachedIsStdoutTerminal int

func isStdoutTerminal() bool {
	if cachedIsStdoutTerminal == 0 {
		if fileInfo, _ := os.Stdout.Stat(); (fileInfo.Mode() & os.ModeCharDevice) != 0 {
			cachedIsStdoutTerminal = +1 // cached true
		} else {
			cachedIsStdoutTerminal = -1 // cached false
		}
	}
	return cachedIsStdoutTerminal > 0
}

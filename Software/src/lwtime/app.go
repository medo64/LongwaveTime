package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/tarm/serial"
)

func main() {
	go work()

	exitSignal := make(chan os.Signal, 1)
	signal.Notify(exitSignal, syscall.SIGINT, syscall.SIGTERM)
	<-exitSignal
}

func work() {
	var c = &serial.Config{Name: "COM8", Baud: 115200}
	var s, errOpen = serial.OpenPort(c)
	logIfPanic(errOpen)

	var _, errWriteProtocol = s.Write([]byte("PW\n"))
	logIfPanic(errWriteProtocol)

	waitForNextSecond() // to give it enough time for other calculations taking place

	var waitForNextMinute bool = false
	for {
		if waitForNextMinute {
			waitForAlmostNextMinute()
		} else { // wait for the 59th second every time but the first time
			waitForNextMinute = true
		}

		var outTime = getCurrentTime().Add(1 * time.Second)
		var outData = getAsWWWB(outTime)
		var outSecond = fmt.Sprintf("%02d", outTime.Second())

		var _, errWriteData = s.Write([]byte("R" + outData + "\n"))
		logIfPanic(errWriteData)

		waitForNextSecond()

		var _, errWriteSync = s.Write([]byte("S" + outSecond + "\n"))
		logIfPanic(errWriteSync)

		logDebugData(outTime, outData, outSecond)
	}
}

func getCurrentTime() time.Time {
	var utcTime = time.Now().UTC()
	return time.Date(utcTime.Year(), utcTime.Month(), utcTime.Day(), utcTime.Hour(), utcTime.Minute(), utcTime.Second(), 0, time.UTC)
}

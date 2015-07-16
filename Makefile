all: build upload serial

init:
	platformio init --board=pro16MHzatmega328
	git clone https://github.com/ntruchsess/arduino_uip.git lib/arduino_uip
	wget http://www.airspayce.com/mikem/arduino/RF22/RF22-1.40.zip && unzip RF22-1.40.zip -d lib/ && rm RF22-1.40.zip

build: 
	platformio run

upload:
	platformio run -t upload --upload-port /dev/ttyUSB$(tty)

clean:
	platformio run -t clean

serial:
	platformio serialports monitor -p  /dev/ttyUSB$(tty)

